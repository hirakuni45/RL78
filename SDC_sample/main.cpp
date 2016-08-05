//=====================================================================//
/*!	@file
	@brief	SD カードの読み書き、サンプル
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include <cstdint>
#include "G13/system.hpp"
#include "common/port_utils.hpp"
#include "common/fifo.hpp"
#include "common/uart_io.hpp"
#include "common/itimer.hpp"
#include "common/format.hpp"
#include "common/delay.hpp"
#include "common/csi_io.hpp"
#include "ff12a/mmc_io.hpp"
#include "ff12a/src/ff.h"
#include "common/command.hpp"

namespace {
	void wait_()
	{
		asm("nop");
	}

	// 送信、受信バッファの定義
	typedef utils::fifo<128> buffer;
	// UART の定義（SAU2、SAU3）
	device::uart_io<device::SAU02, device::SAU03, buffer, buffer> uart_;

	// インターバル・タイマー
	device::itimer<uint8_t> itm_;

	// CSI(SPI) の定義、CSI00 の通信では、「SAU00」を利用、０ユニット、チャネル０
	typedef device::csi_io<device::SAU00> csi;
	csi csi_;

	// FatFS インターフェースの定義
	typedef device::PORT<device::port_no::P0,  device::bitpos::B0> card_select;	///< カード選択信号
	typedef device::PORT<device::port_no::P0,  device::bitpos::B1> card_power;	///< カード電源制御
	typedef device::PORT<device::port_no::P14, device::bitpos::B6> card_detect;	///< カード検出
	fatfs::mmc_io<csi, card_select> mmc_(csi_);

	// FatFS コンテキスト
	FATFS	fatfs_;

	utils::command<64> command_;
}

const void* ivec_[] __attribute__ ((section (".ivec"))) = {
	/*  0 */  nullptr,
	/*  1 */  nullptr,
	/*  2 */  nullptr,
	/*  3 */  nullptr,
	/*  4 */  nullptr,
	/*  5 */  nullptr,
	/*  6 */  nullptr,
	/*  7 */  nullptr,
	/*  8 */  nullptr,
	/*  9 */  nullptr,
	/* 10 */  nullptr,
	/* 11 */  nullptr,
	/* 12 */  nullptr,
	/* 13 */  nullptr,
	/* 14 */  nullptr,
	/* 15 */  nullptr,
	/* 16 */  reinterpret_cast<void*>(uart_.send_task),
	/* 17 */  reinterpret_cast<void*>(uart_.recv_task),
	/* 18 */  reinterpret_cast<void*>(uart_.error_task),
	/* 19 */  nullptr,
	/* 20 */  nullptr,
	/* 21 */  nullptr,
	/* 22 */  nullptr,
	/* 23 */  nullptr,
	/* 24 */  nullptr,
	/* 25 */  nullptr,
	/* 26 */  reinterpret_cast<void*>(itm_.task),
};


extern "C" {
	void sci_putch(char ch)
	{
		uart_.putch(ch);
	}

	void sci_puts(const char* str)
	{
		uart_.puts(str);
	}

	char sci_getch(void)
	{
		return uart_.getch();
	}

	uint16_t sci_length()
	{
		return uart_.recv_length();
	}

	DSTATUS disk_initialize (BYTE drv) {
		return mmc_.disk_initialize(drv);
	}

	DSTATUS disk_status(BYTE drv) {
		return mmc_.disk_status(drv);
	}

	DRESULT disk_read(BYTE drv, BYTE* buff, DWORD sector, UINT count) {
		return mmc_.disk_read(drv, buff, sector, count);
	}

	DRESULT disk_write(BYTE drv, const BYTE* buff, DWORD sector, UINT count) {
		return mmc_.disk_write(drv, buff, sector, count);
	}

	DRESULT disk_ioctl(BYTE drv, BYTE ctrl, void* buff) {
		return mmc_.disk_ioctl(drv, ctrl, buff);
	}

	DWORD get_fattime(void) {
		return 0;
	}
};

namespace {
	void list_dir_()
	{
		DIR dir;
		auto st = f_opendir(&dir, "");
		if(st != FR_OK) {
			utils::format("Can't open dir: %d\n") % static_cast<uint32_t>(st);
		} else {
			for(;;) {
				FILINFO fno;
				// Read a directory item
				if(f_readdir(&dir, &fno) != FR_OK) {
					sci_puts("Can't read dir\n");
					break;
				}
				if(!fno.fname[0]) break;
					if(fno.fattrib & AM_DIR) {
					utils::format("          /%s\n") % fno.fname;
				} else {
					utils::format("%8d  %s\n") % static_cast<uint32_t>(fno.fsize) % fno.fname;
				}
			}
		}
	}
}


int main(int argc, char* argv[])
{
	using namespace device;

	utils::port::pullup_all();  ///< 安全の為、全ての入力をプルアップ

	// インターバル・タイマー開始
	{
		uint8_t intr_level = 1;
		itm_.start(60, intr_level);
	}

	// UART 開始
	{
		uint8_t intr_level = 1;
		uart_.start(115200, intr_level);
	}

	// CSI 開始
	// ※SD カードのアクセスでは、「PHASE::TYPE4」を選択する。
	{
		uint8_t intr_level = 0;
		if(!csi_.start(16000000, csi::PHASE::TYPE4, intr_level)) {
			uart_.puts("CSI Start fail ! (Clock spped over range)\n");
		}
	}

	uart_.puts("Start RL78/G13 SD-CARD Access sample\n");

	command_.set_prompt("# ");

	PM4.B3 = 0;  // output

	card_select::PMC = 0;
	card_select::PM = 0;
	card_select::P = 0;  // 電源ＯＦＦ時、「０」にしておかないと電流が回り込む

	card_power::PMC = 0;
	card_power::PM = 0;
	card_power::P = 1; 

	card_detect::PM = 1;  // input
	card_detect::POM = 0;
	card_detect::PU = 1;  // pull-up

#if 0
	card_power::P = 0;  // power ON!
	utils::delay::milli_second(100);

	{  // カードをマウント
		auto st = f_mount(&fatfs_, "", 0);
		if(st != FR_OK) {
			utils::format("Mount fail: %d\n") % static_cast<uint32_t>(st);
		} else {
			list_dir_();
		}
	}
#endif

	uint8_t n = 0;
	bool cd = false;
	uint8_t mount_delay = 0;
	while(1) {
		itm_.sync();

		auto st = !card_detect::P();
		if(!cd && st) {
//			mount_delay = 10;
//			card_power::P = 0;
//			card_select::P = 1;
			utils::format("Card ditect\n");
		} else if(cd && !st) {
//			card_power::P = 1;
//			card_select::P = 0;
//			f_mount(&fatfs_, "", 0);
			utils::format("Card unditect\n");
		}
		cd = st;
#if 0
		if(mount_delay) {
			--mount_delay;
			if(mount_delay == 0) {
				auto st = f_mount(&fatfs_, "", 1);
				if(st != FR_OK) {
					utils::format("Mount fail: %d\n") % static_cast<uint32_t>(st);
					card_power::P = 1;
					card_select::P = 0;
				}
			}
		}
#endif

		// コマンド入力と、コマンド解析
		if(command_.service()) {
			auto cmdn = command_.get_words();
			if(cmdn >= 1) {
			}
		}

		++n;
		if(n >= 30) n = 0;
		P4.B3 = n < 10 ? false : true; 	
	}
}
