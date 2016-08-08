//=====================================================================//
/*!	@file
	@brief	SD カードの WAV 形式のファイルを再生するサンプル @n
			TO01(P1-6) から、左チャネル @n
			TO02(P1-7) から、右チャネル
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
#include "wav_in.hpp"

namespace pwm {

	class interval_master {
		uint8_t buff_[1024];
		uint16_t inc_;
		uint16_t rate_;
		uint8_t skip_;
		uint8_t	l_ofs_;
		uint8_t	r_ofs_;
		uint8_t wofs_;
		volatile uint16_t pos_;

	public:
		interval_master() : inc_(0), rate_(0), skip_(0), l_ofs_(0), r_ofs_(0), wofs_(0), pos_(0) { }

		void init() {
			wofs_ = 0x00;
			for(uint16_t i = 0; i < 1024; ++i) {
				buff_[i] = 0x80;
			}
		}

		uint8_t* get_buff() { return buff_; }

		uint16_t get_pos() const { return pos_; }

		void set_rate(uint16_t rate) { rate_ = rate / 10; }

		void set_param(uint8_t skip, uint8_t l_ofs, uint8_t r_ofs, uint8_t wofs) {
			skip_ = skip;
			l_ofs_ = l_ofs;
			r_ofs_ = r_ofs;
			wofs_ = wofs;
		} 

		void operator() () {
			device::TAU01::TDRL = buff_[pos_ + l_ofs_] + wofs_;
			device::TAU02::TDRL = buff_[pos_ + r_ofs_] + wofs_;
			inc_ += rate_;
			if(inc_ >= 6250) {
				inc_ -= 6250;
				pos_ += skip_;
				pos_ &= 1024 - 1;
			}
		}
	};
}

namespace {

	// 送信、受信バッファの定義
	typedef utils::fifo<64> buffer;
	// UART の定義（SAU02、SAU03）
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

	utils::sdc_io<csi, card_select, card_power, card_detect> sdc_(csi_);

	utils::command<64> command_;

	typedef device::tau_io<device::TAU00, pwm::interval_master> master;
	master master_;
	device::tau_io<device::TAU01> pwm1_;
	device::tau_io<device::TAU02> pwm2_;
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
	/* 20 */  reinterpret_cast<void*>(master_.task),
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

	bool init_pwm_()
	{
		// 62.5 KHz (16MHz / 256)
		uint8_t intr_level = 3;
		if(!master_.start_interval(1, 256 - 1, intr_level)) {
			return false;
		}
		intr_level = 0;
		if(!pwm1_.start_pwm<master::tau_type>(0, intr_level)) {
			return false;
		}
		if(!pwm2_.start_pwm<master::tau_type>(0, intr_level)) {
			return false;
		}
		return true;
	}

	audio::wav_in wav_;

	void play_(const char* fname)
	{
		FIL fil;
		if(f_open(&fil, fname, FA_READ) != FR_OK) {
			utils::format("Can't open input file: %s\n") % fname;
			return;
		}

		if(!wav_.load_header(&fil)) {
			f_close(&fil);
			utils::format("WAV file header load fail: %s\n") % fname;
			return;
		}

		auto fsize = wav_.get_size();
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
			utils::format("Fail bits\n");
			return;
		}
		master_.at_task().set_param(skip, l_ofs, r_ofs, wofs);

		uint32_t fpos = 0;
		uint16_t wpos = master_.at_task().get_pos();
		uint16_t pos = wpos;
		while(fpos < fsize) {
			while(((wpos ^ pos) & 512) == 0) {
				pos = master_.at_task().get_pos();
			}
			uint8_t* buff = master_.at_task().get_buff();
			UINT br;
			if(f_read(&fil, &buff[wpos & 512], 512, &br) != FR_OK) {
				break;
			}
			fpos += 512;
			wpos = pos;
		}
		f_close(&fil);
		master_.at_task().init();
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

	// PWM 開始
	master_.at_task().init();
	if(!init_pwm_()) {
		uart_.puts("PWM initialization fail\n");
	}

	uart_.puts("Start RL78/G13 WAV file player sample\n");

	command_.set_prompt("# ");

//	auto val = master_.get_value();  // マスターチャネルのカウント最大値
//	pwm1_.set_value(val / 4);  // PWM Duty 25%
//	pwm2_.set_value(val * 3 / 4);  // PWM Duty 75%

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
				} else if(cmdn >= 2 && command_.cmp_word(0, "play")) {
					char fname[16];
					command_.get_word(1, sizeof(fname), fname);
					play_(fname);
				} else {
//					utils::format("\nCommand error: '%s'\n") % 
				}
			}
		}

		++n;
		if(n >= 30) n = 0;
		device::P4.B3 = n < 10 ? false : true; 	
	}
}
