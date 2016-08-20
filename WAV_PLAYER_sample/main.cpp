//=====================================================================//
/*!	@file
	@brief	SD カードの WAV 形式のファイルを再生するサンプル @n
			P16/TO01(40) から、左チャネル @n
			P17/TO02(39) から、右チャネル
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

	// インターバル・タイマー割り込み制御クラス
	// ※PWMコンペアレジスターに、直接書き込んでいるので、PWMチャネルを変更する場合は注意
	class interval_master {
		uint8_t buff_[1024];
		volatile uint16_t	inc_;
		volatile uint16_t	rate_;
		volatile uint8_t	skip_;
		volatile uint8_t	l_ofs_;
		volatile uint8_t	r_ofs_;
		volatile uint8_t	wofs_;
		volatile uint16_t	pos_;

	public:
		interval_master() : inc_(0), rate_(4410), skip_(0), l_ofs_(0), r_ofs_(2), wofs_(0x80), pos_(0) { }

		// リセット後初期化
		void init()
		{
			for(uint16_t i = 0; i < sizeof(buff_); ++i) {
				buff_[i] = 0x00;
			}
		}

		// バッファを取得
		uint8_t* get_buff() { return buff_; }

		// 波形位置を取得
		uint16_t get_pos() const { return pos_; }

		// サンプルレートを設定
		// ex:
		// 48KHz    ---> 4800
		// 38KHz    ---> 3800
		// 44.1KHz  ---> 4410
		// 22.05KHz ---> 2205
		void set_rate(uint16_t rate) { rate_ = rate / 10; }

		// 波形位置更新タイミングで同期
		void sync() {
			if(skip_ == 0) return;
			volatile auto n = pos_;
			while(n == pos_) ;
		}

		// ポーズ（無音）
		// skip: 波形の移動量
		void pause(uint8_t skip) {
			if(skip == 0) {
				sync();
				skip_ = skip;
				buff_[pos_ + l_ofs_] = wofs_ ^ 0x80;
				buff_[pos_ + r_ofs_] = wofs_ ^ 0x80;
			} else {
				sync();
				skip_ = skip;
			}
		}

		// 波形バッファに直接「値」を書き込む
		void set_level(uint8_t val) {
			buff_[pos_ + l_ofs_] = val;
			buff_[pos_ + r_ofs_] = val;
		}

		// 再生パラメーターの設定
		void set_param(uint8_t skip, uint8_t l_ofs, uint8_t r_ofs, uint8_t wofs) {
			sync();
			inc_ = 0;
			skip_ = 0;
			pos_ = 0;
			l_ofs_ = 0;
			r_ofs_ = 0;
			buff_[0] = wofs_ ^ 0x80;
			for(uint16_t i = 1; i < sizeof(buff_); ++i) {
				buff_[i] = wofs ^ 0x80;
			}
			wofs_ = wofs;
			buff_[0] = wofs_ ^ 0x80;
			l_ofs_ = l_ofs;
			r_ofs_ = r_ofs;
			inc_ = 0;
			skip_ = skip;
		} 

		// 割り込み、functor
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
	typedef utils::fifo<uint32_t, 32> buffer;
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
		if(!sdc_.get_mount()) {
			master_.at_task().set_param(4, 0, 2, 0x80);
			utils::format("SD Card unmount.\n");
			return;
		}

		FIL fil;
		if(!sdc_.open(&fil, fname, FA_READ)) {
			master_.at_task().set_param(4, 0, 2, 0x80);
			utils::format("Can't open input file: '%s'\n") % fname;
			return;
		}

		if(!wav_.load_header(&fil)) {
			master_.at_task().set_param(4, 0, 2, 0x80);
			f_close(&fil);
			utils::format("WAV file load fail: '%s'\n") % fname;
			return;
		}

		auto fsize = wav_.get_size();
		utils::format("File:   '%s'\n") % fname;
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
		bool pause = false;
		while(fpos < fsize) {
			if(!pause) {
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

				// LED モニターの点滅
				if(n >= 20) {  // play 時
					n = 0;
					device::P4.B3 = !device::P4.B3();
				}
				++n;
			} else {  // pause 時
				if(n < 192) {
					device::P4.B3 = (n >> 5) & 1;
				} else {
					device::P4.B3 = 1;
				}
				utils::delay::milli_second(2);
				++n;
			}

			if(sci_length() > 0) {
				auto ch = sci_getch();
				if(ch == '>') {  // '>'
					break;
				} else if(ch == '<') {  // '<'
					fpos = 0;
					f_lseek(&fil, wav_.get_top());
				} else if(ch == ' ') {  // [space]
					if(pause) {
						master_.at_task().pause(skip);
					} else {
						master_.at_task().pause(0);
					}
					pause = !pause;
				}
			}			
		}

		master_.at_task().set_param(skip, l_ofs, r_ofs, wofs);

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

	// PWM 開始
	master_.at_task().init();
	if(!init_pwm_()) {
		uart_.puts("PWM initialization fail\n");
	}
	// ポップノイズ低減
//	master_.at_task().pause(0);
//	for(uint8_t i =  0; i < 128; ++i) {
//		master_.at_task().set_level(0x80 + i);
//		utils::delay::milli_second(3);
//	}
//	master_.at_task().set_param(4, 0, 2, 0x80);

	uart_.puts("Start RL78/G13 WAV file player sample\n");

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
					utils::format("dir [xxx] : list directory\n");
					utils::format("pwd : list current directory\n");
					utils::format("cd : change directory\n");
					utils::format("play [xxx] : play file ('*' wildcard)\n");
				}
			}
		}

		++n;
		if(n >= 30) n = 0;
		device::P4.B3 = n < 10 ? false : true; 	
	}
}
