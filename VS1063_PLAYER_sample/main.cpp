//=====================================================================//
/*!	@file
	@brief	VS1063a を使った、オーディオプレイヤー @n
			・P73/SO01 (26) ---> VS1063:SI     (29) @n
			・P74/SI01 (25) ---> VS1063:SO     (30) @n
			・P75/SCK01(24) ---> VS1063:SCLK   (28) @n
			・P52      (35) ---> VS1063:/xCS   (23) @n
			・P53      (36) ---> VS1063:/xDCS  (13) @n 
			・P54      (37) ---> VS1063:/DREQ  ( 8) @n 
			・/RESET   ( 6) ---> VS1063:/xRESET( 3)
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include <cstdint>
#include "common/port_utils.hpp"
#include "common/tau_io.hpp"
#include "common/fifo.hpp"
#include "common/uart_io.hpp"
#include "common/itimer.hpp"
#include "common/format.hpp"
#include "common/delay.hpp"
#include "common/csi_io.hpp"
#include "common/sdc_io.hpp"
#include "common/command.hpp"
#include "chip/VS1063.hpp"

namespace {

	// 送信、受信バッファの定義
	typedef utils::fifo<uint8_t, 32> buffer;
	// UART の定義（SAU02、SAU03）
	device::uart_io<device::SAU02, device::SAU03, buffer, buffer> uart_;

	// インターバル・タイマー
	device::itimer<uint8_t> itm_;

	// SDC CSI(SPI) の定義、CSI00 の通信では、「SAU00」を利用、０ユニット、チャネル０
	typedef device::csi_io<device::SAU00> csi0;
	csi0 csi0_;

	// FatFS インターフェースの定義
	typedef device::PORT<device::port_no::P0,  device::bitpos::B0> card_select;	///< カード選択信号
	typedef device::PORT<device::port_no::P0,  device::bitpos::B1> card_power;	///< カード電源制御
	typedef device::PORT<device::port_no::P14, device::bitpos::B6> card_detect;	///< カード検出
	utils::sdc_io<csi0, card_select, card_power, card_detect> sdc_(csi0_);

	utils::command<64> command_;

	// VS1053 SPI の定義 CSI01「SAU01」
	typedef device::csi_io<device::SAU01> csi1;
	csi1 csi1_;
	typedef device::PORT<device::port_no::P5,  device::bitpos::B2> vs1063_sel;	///< VS1063 /CS
	typedef device::PORT<device::port_no::P5,  device::bitpos::B3> vs1063_dcs;	///< VS1063 /DCS
	typedef device::PORT<device::port_no::P5,  device::bitpos::B4> vs1063_req;	///< VS1063 DREQ
	chip::VS1063<csi1, vs1063_sel, vs1063_dcs, vs1063_req> vs1063_(csi1_);
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

	DSTATUS disk_initialize(BYTE drv) {
		return sdc_.at_mmc().disk_initialize(drv);
	}

	DSTATUS disk_status(BYTE drv) {
		return sdc_.at_mmc().disk_status(drv);
	}

	DRESULT disk_read(BYTE drv, BYTE* buff, DWORD sector, UINT count) {
		return sdc_.at_mmc().disk_read(drv, buff, sector, count);
	}

	DRESULT disk_write(BYTE drv, const BYTE* buff, DWORD sector, UINT count) {
		return sdc_.at_mmc().disk_write(drv, buff, sector, count);
	}

	DRESULT disk_ioctl(BYTE drv, BYTE ctrl, void* buff) {
		return sdc_.at_mmc().disk_ioctl(drv, ctrl, buff);
	}

	DWORD get_fattime(void) {
		return 0;
	}
};


namespace {

	void play_(const char* fname)
	{
		if(!sdc_.get_mount()) {
			utils::format("SD Card unmount.\n");
			return;
		}

		utils::format("Play: '%s'\n") % fname;

		FIL fil;
		if(sdc_.open(&fil, fname, FA_READ) != FR_OK) {
			utils::format("Can't open input file: '%s'\n") % fname;
			return;
		}

		vs1063_.play(&fil);
	}

	void play_loop_(const char*);
	void play_loop_func_(const char* name, uint32_t size, bool dir)
	{
		if(dir) {
			play_loop_(name);
		} else {
			play_(name);
		}
	}

	void play_loop_(const char* root)
	{
		sdc_.dir_loop(root, play_loop_func_);
	}
}


int main(int argc, char* argv[])
{
	utils::port::pullup_all();  ///< 安全の為、全ての入力をプルアップ

	device::PM4.B3 = 0;  // output

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

	// SD カード・サービス開始
	sdc_.initialize();

	// VS1063 初期化
	{
		vs1063_.start();
	}	

	uart_.puts("Start RL78/G13 VS1053 player sample\n");

	command_.set_prompt("# ");

	uint8_t n = 0;
	FIL fil;
	char tmp[64];
	while(1) {
		itm_.sync();

		sdc_.service();

		// コマンド入力と、コマンド解析
		if(command_.service()) {
			auto cmdn = command_.get_words();
			if(cmdn >= 1) {
				if(command_.cmp_word(0, "dir")) {
					if(!sdc_.get_mount()) {
						utils::format("SD Card unmount.\n");
					} else {
						if(cmdn >= 2) {
							command_.get_word(1, sizeof(tmp), tmp);
							sdc_.dir(tmp);
						} else {
							sdc_.dir("");
						}
					}
				} else if(command_.cmp_word(0, "cd")) {  // cd [xxx]
					if(cmdn >= 2) {
						command_.get_word(1, sizeof(tmp), tmp);
						sdc_.cd(tmp);						
					} else {
						sdc_.cd("/");
					}
				} else if(command_.cmp_word(0, "pwd")) { // pwd
					utils::format("%s\n") % sdc_.get_current();
				} else if(command_.cmp_word(0, "play")) {  // play [xxx]
					if(cmdn >= 2) {
						command_.get_word(1, sizeof(tmp), tmp);
						if(std::strcmp(tmp, "*") == 0) {
							play_loop_("");
						} else {
							play_(tmp);
						}
					} else {
						play_loop_("");
					}
				} else {
					utils::format("dir ---> directory file\n");
					utils::format("play file-name ---> play file\n");
					utils::format("play * ---> play file all\n");
				}
			}
		}

		++n;
		if(n >= 30) n = 0;
		device::P4.B3 = n < 10 ? false : true; 	
	}
}
