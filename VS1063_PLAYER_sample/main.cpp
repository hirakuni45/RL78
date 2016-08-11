//=====================================================================//
/*!	@file
	@brief	VS1063a を使った、オーディオプレイヤー @n
			・P73/SO01 (26) ---> VS1053:SI     (29) @n
			・P74/SI01 (25) ---> VS1053:SO     (30) @n
			・P75/SCK01(24) ---> VS1053:SCLK   (28) @n
			・P62      (19) ---> VS1053:/xCS   (23) @n
			・P63      (20) ---> VS1053:/xDCS  (13) @n 
			・/RESET   ( 6) ---> VS1053:/xRESET( 3)
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
	typedef utils::fifo<64> buffer;
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
	typedef device::PORT<device::port_no::P6,  device::bitpos::B2> vs1053_select;	///< VS1053 選択信号
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
		FIL fil;
		if(f_open(&fil, fname, FA_READ) != FR_OK) {
			utils::format("Can't open input file: '%s'\n") % fname;
			return;
		}

#if 0
		if(!wav_.load_header(&fil)) {
			f_close(&fil);
			utils::format("WAV file load fail: '%s'\n") % fname;
			return;
		}

		auto fsize = wav_.get_size();
		utils::format("File:   %s\n") % fname;
		utils::format("Size:   %d\n") % fsize;
		utils::format("Rate:   %d\n") % wav_.get_rate();
		utils::format("Chanel: %d\n") % static_cast<uint32_t>(wav_.get_chanel());
		utils::format("Bits:   %d\n") % static_cast<uint32_t>(wav_.get_bits());

		master_.at_task().set_rate(wav_.get_rate());
		uint8_t skip = 0;
		uint8_t l_ofs = 0;
		uint8_t r_ofs = 0;
		uint8_t wofs = 0;
		if(wav_.get_bits() == 8) {
			skip = wav_.get_chanel();
			if(wav_.get_chanel() == 1) {
				l_ofs = 0;
				r_ofs = 0;
			} else {
				l_ofs = 0;
				r_ofs = 1;
			}
		} else if(wav_.get_bits() == 16) {
			skip = wav_.get_chanel() * 2;
			if(wav_.get_chanel() == 1) {
				l_ofs = 1;
				r_ofs = 1;
			} else {
				l_ofs = 1;
				r_ofs = 3;
			}
			wofs = 0x80;
		} else {
			f_close(&fil);
			utils::format("Fail bits: '%s'\n") % fname;
			return;
		}
		master_.at_task().set_param(skip, l_ofs, r_ofs, wofs);

		uint32_t fpos = 0;
		uint16_t wpos = master_.at_task().get_pos();
		uint16_t pos = wpos;
		uint8_t n = 0;
		while(fpos < fsize) {
			while(((wpos ^ pos) & 512) == 0) {
				pos = master_.at_task().get_pos();
			}
			uint8_t* buff = master_.at_task().get_buff();
			UINT br;
			if(f_read(&fil, &buff[wpos & 512], 512, &br) != FR_OK) {
				utils::format("Abort: '%s'\n") % fname;
				break;
			}
			fpos += 512;
			wpos = pos;

			++n;
			if(n >= 20) {
				n = 0;
				device::P4.B3 = !device::P4.B3();  // LED モニターの点滅
			}

			if(sci_length() > 0) {
				auto ch = sci_getch();
				if(ch == '>') {
					break;
				} else if(ch == '<') {
					fpos = 0;
					f_lseek(&fil, wav_.get_top());
				}
			}			
		}
		for(uint8_t i = 0; i < 2; ++i) {
			while(((wpos ^ pos) & 512) == 0) {
				pos = master_.at_task().get_pos();
			}
			uint8_t* buff = master_.at_task().get_buff();
			UINT br;
			for(uint16_t j = 0; j < 512; ++j) {
				buff[(wpos & 512) + j] = wofs ^ 0x80;
			}
			fpos += 512;
			wpos = pos;
		}
#endif
		f_close(&fil);
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

	uart_.puts("Start RL78/G13 VS1053 player sample\n");

	command_.set_prompt("# ");

	uint8_t n = 0;
	FIL fil;
	while(1) {
		itm_.sync();

		sdc_.service();

		// コマンド入力と、コマンド解析
		if(command_.service()) {
			auto cmdn = command_.get_words();
			if(cmdn >= 1) {
				if(command_.cmp_word(0, "dir")) {
					sdc_.dir("");
				} else if(command_.cmp_word(0, "play")) {
					if(cmdn >= 2) {
						char fname[16];
						command_.get_word(1, sizeof(fname), fname);
						if(std::strcmp(fname, "*") == 0) {
							play_loop_("");
						} else {
							play_(fname);
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
