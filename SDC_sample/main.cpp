//=====================================================================//
/*!	@file
	@brief	SD カードの読み書き、サンプル
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include <cstdint>
#include <cstring>
#include "G13/system.hpp"
#include "common/port_utils.hpp"
#include "common/fifo.hpp"
#include "common/uart_io.hpp"
#include "common/itimer.hpp"
#include "common/format.hpp"
#include "common/delay.hpp"
#include "common/csi_io.hpp"
#include "common/sdc_io.hpp"
#include "common/command.hpp"

namespace {
	void wait_()
	{
		asm("nop");
	}

	// 送信、受信バッファの定義
	typedef utils::fifo<64> buffer;
	// UART の定義（SAU2、SAU3）
	device::uart_io<device::SAU02, device::SAU03, buffer, buffer> uart_;

	// インターバル・タイマー
	device::itimer<uint16_t> itm_;

	// CSI(SPI) の定義、CSI00 の通信では、「SAU00」を利用、０ユニット、チャネル０
	typedef device::csi_io<device::SAU00> csi;
	csi csi_;

	// FatFS インターフェースの定義
	typedef device::PORT<device::port_no::P0,  device::bitpos::B0> card_select;	///< カード選択信号
	typedef device::PORT<device::port_no::P0,  device::bitpos::B1> card_power;	///< カード電源制御
	typedef device::PORT<device::port_no::P14, device::bitpos::B6> card_detect;	///< カード検出

	utils::sdc_io<csi, card_select, card_power, card_detect> sdc_(csi_);

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

	uint8_t v_ = 91;
	uint8_t m_ = 123;
	uint8_t rand_()
	{
		v_ += v_ << 2;
		++v_;
		uint8_t n = 0;
		if(m_ & 0x02) n = 1;
		if(m_ & 0x40) n ^= 1;
		m_ += m_;
		if(n == 0) ++m_;
		return v_ ^ m_;
	}

	bool create_test_file_(const char* fname, uint32_t size)
	{
		utils::format("SD Write test...\n");

		uint8_t buff[512];
		for(uint16_t i = 0; i < sizeof(buff); ++i) {
			buff[i] = rand_();
		}

		auto st = itm_.get_counter();
		FIL fp;
		if(f_open(&fp, fname, FA_WRITE | FA_CREATE_ALWAYS) != FR_OK) {
			utils::format("Can't create file: '%s'\n") % fname;
			return false;
		}
		auto rs = size;
		while(rs > 0) {
			UINT sz = sizeof(buff);
			if(sz > rs) sz = rs;
			UINT bw;
			f_write(&fp, buff, sz, &bw);
			rs -= bw;
		}
		f_close(&fp);

		auto ed = itm_.get_counter();
		uint32_t len;
		if(ed > st) len = ed - st;
		else len = 65536 + ed - st;
		utils::format("Write frame: %d\n") % len;
		auto pbyte = size * 60 / len;
		utils::format("Write: %d Bytes/Sec\n") % pbyte;
		utils::format("Write: %d KBytes/Sec\n") % (pbyte / 1024);

		return true;
	}

	void test_all_()
	{
		const char* test_file = { "TEST.BIN" };
		uint32_t size = 1024L * 1024L;
		if(!create_test_file_(test_file, size)) {
			return;
		}
		utils::format("SD Speed test start...\n");

		auto st = itm_.get_counter();

		utils::format("SD Read test...\n");
		FIL fp;
		if(f_open(&fp, test_file, FA_READ) != FR_OK) {
			utils::format("Can't read file: '%s'\n") % test_file;
		}
		auto rs = size;
		while(rs > 0) {
			uint8_t buff[512];
			UINT rb;
			UINT sz = sizeof(buff);
			if(sz > rs) sz = rs;
			f_read(&fp, buff, sz, &rb);
			rs -= rb;
		}
		f_close(&fp);

		auto ed = itm_.get_counter();
		uint32_t len;
		if(ed > st) len = ed - st;
		else len = 65536 + ed - st;
		utils::format("Read frame: %d\n") % len;
		auto pbyte = size * 60 / len;
		utils::format("Read: %d Bytes/Sec\n") % pbyte;
		utils::format("Read: %d KBytes/Sec\n") % (pbyte / 1024);
	}
}

int main(int argc, char* argv[])
{
	using namespace device;

	utils::port::pullup_all();  ///< 安全の為、全ての入力をプルアップ

	PM4.B3 = 0;  // output

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

	sdc_.initialize();

	uart_.puts("Start RL78/G13 SD-CARD Access sample\n");

	command_.set_prompt("# ");

	uint8_t n = 0;
	while(1) {
		itm_.sync();

		sdc_.service();

		// コマンド入力と、コマンド解析
		if(command_.service()) {
			auto cmdn = command_.get_words();
			if(cmdn >= 1) {
				bool f = false;
				if(command_.cmp_word(0, "dir")) {
					sdc_.dir("");
					f = true;
				} else if(command_.cmp_word(0, "speed")) {
					test_all_();
					f = true;
				}
				if(!f) {
					char tmp[16];
					command_.get_word(0, sizeof(tmp), tmp);
					utils::format("Command error: '%s'\n") % tmp;
				}
			}
		}

		++n;
		if(n >= 30) n = 0;
		P4.B3 = n < 10 ? false : true; 	
	}
}
