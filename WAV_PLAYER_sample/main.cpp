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

// 128x64 LCD を使い、A/D 入力スイッチを使う場合に有効にする。
#define ENABLE_LCD
// ターゲットＬＣＤのタイプを選択
#define LCD_ST7565
// #define LCD_SSD1306

#ifdef ENABLE_LCD

#ifdef LCD_ST7565
#include "chip/ST7565.hpp"
#endif
#ifdef LCD_SSD1306
#include "chip/SSD1306.hpp"
#endif

#include "common/adc_io.hpp"
#include "common/monograph.hpp"
#include "common/font6x12.hpp"
#include "common/kfont12.hpp"
#include "common/filer.hpp"
#include "common/bitset.hpp"
#include "common/switch_man.hpp"
#endif

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

	typedef utils::sdc_io<csi, card_select, card_power, card_detect> sdc_io;
	sdc_io sdc_(csi_);

	utils::command<64> command_;

	typedef device::tau_io<device::TAU00, pwm::interval_master> master;
	master master_;
	device::tau_io<device::TAU01> pwm1_;
	device::tau_io<device::TAU02> pwm2_;

	// LCD(128x64)、A/D 変換スイッチを使う場合
#ifdef ENABLE_LCD
	// LCD CSI(SPI) の定義、CSI20 の通信では、「SAU10」を利用、１ユニット、チャネル０
	typedef device::csi_io<device::SAU10> csig;
	csig csig_;

	// LCD インターフェースの定義
	typedef device::PORT<device::port_no::P5,  device::bitpos::B5> lcd_sel;	///< LCD 選択信号
	typedef device::PORT<device::port_no::P5,  device::bitpos::B0> lcd_reg;	///< LCD レジスタ選択

#ifdef LCD_ST7565
	chip::ST7565<csig, lcd_sel, lcd_reg> lcd_(csig_);
#endif
#ifdef LCD_SSD1306
	chip::SSD1306<csig, lcd_sel, lcd_reg> lcd_(csig_);
#endif

	typedef graphics::font6x12 afont;
	typedef graphics::kfont12<16> kfont;
	kfont kfont_;
	typedef graphics::monograph<128, 64, afont, kfont> bitmap;
	bitmap bitmap_(kfont_);

	typedef device::adc_io<4, utils::null_task> adc;
	adc adc_;

	graphics::filer<sdc_io, bitmap> filer_(sdc_, bitmap_);

	enum class SWITCH : uint8_t {
		RIGHT,
		UP,
		DOWN,
		LEFT,
		A,
		B
	};

	typedef utils::bitset<uint8_t, SWITCH> switch_bits;
	utils::switch_man<switch_bits> switch_man_;

	int16_t wav_info_x_;
	int16_t wav_info_y_;
#endif

}


const void* ivec_[] __attribute__ ((section (".ivec"))) = {
	/*  0 WDTI            */  nullptr,
	/*  1 LVI             */  nullptr,
	/*  2 P0              */  nullptr,
	/*  3 P1              */  nullptr,
	/*  4 P2              */  nullptr,
	/*  5 P3              */  nullptr,
	/*  6 P4              */  nullptr,
	/*  7 P5              */  nullptr,
	/*  8 ST2/CSI20/IIC20 */  nullptr,
	/*  9 SR2/CSI21/IIC21 */  nullptr,
	/* 10 SRE2/TM11H      */  nullptr,
	/* 11 DMA0            */  nullptr,
	/* 12 DMA1            */  nullptr,
	/* 13 ST0/CSI00/IIC00 */  nullptr,
	/* 14 SR0/CSI01/IIC01 */  nullptr,
	/* 15 SRE0/TM01H      */  nullptr,
	/* 16 ST1/CSI10/IIC10 */  reinterpret_cast<void*>(uart_.send_task),
	/* 17 SR1/CSI11/IIC11 */  reinterpret_cast<void*>(uart_.recv_task),
	/* 18 SRE1/TM03H      */  reinterpret_cast<void*>(uart_.error_task),
	/* 19 IICA0           */  nullptr,
	/* 20 TM00            */  reinterpret_cast<void*>(master_.task),
	/* 21 TM01            */  nullptr,
	/* 22 TM02            */  nullptr,
	/* 23 TM03            */  nullptr,
	/* 24 AD              */  reinterpret_cast<void*>(adc_.task),
	/* 25 RTC             */  nullptr,
	/* 26 IT              */  reinterpret_cast<void*>(itm_.task),
	/* 27 KR              */  nullptr,
	/* 28 ST3/CSI30/IIC30 */  nullptr,
	/* 29 SR3/CSI31/IIC31 */  nullptr,
	/* 30 TM13            */  nullptr,
	/* 31 TM04            */  nullptr,
	/* 32 TM05            */  nullptr,
	/* 33 TM06            */  nullptr,
	/* 34 TM07            */  nullptr,
	/* 35 P6              */  nullptr,
	/* 36 P7              */  nullptr,
	/* 37 P8              */  nullptr,
	/* 38 P9              */  nullptr,
	/* 39 P10             */  nullptr,
	/* 40 P11             */  nullptr,
	/* 41 TM10            */  nullptr,
	/* 42 TM11            */  nullptr,
	/* 43 TM12            */  nullptr,
	/* 44 SRE3/TM13H      */  nullptr,
	/* 45 MD              */  nullptr,
	/* 46 IICA1           */  nullptr,
	/* 47 FL              */  nullptr,
	/* 48 DMA2            */  nullptr,
	/* 49 DMA3            */  nullptr,
	/* 50 TM14            */  nullptr,
	/* 51 TM15            */  nullptr,
	/* 52 TM16            */  nullptr,
	/* 53 TM17            */  nullptr,
};


extern "C" {

#ifdef ENABLE_LCD
	void bmp_putch(char ch)
	{
		if(ch != '\n') {
			wav_info_x_ = bitmap_.draw_font(wav_info_x_, wav_info_y_, ch);
		} else {
			wav_info_x_ = 0;
			wav_info_y_ += bitmap_.get_kfont_height();
		}
	}
#else
	void bmp_putch(char ch)
	{
	}
#endif

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

#ifdef ENABLE_LCD
	void switch_decode_()
	{
		switch_bits lvl;
		// ４つのスイッチ判定（排他的）
		auto val = adc_.get(2);
		val >>= 6;   // 0 to 1023
		val += 128;  // 閾値のオフセット（1024 / 4(SWITCH) / 2）
		val /= 256;  // デコード（1024 / 4(SWITCH）

		if(val < 4) {
			lvl.set(static_cast<SWITCH>(val));
		}

		// ２つのスイッチ判定（同時押し判定）
		val = adc_.get(3);
		val >>= 6;  // 0 to 1023
		if(val < 256) {
			lvl.set(SWITCH::A);
			lvl.set(SWITCH::B);
		} else if(val < 594) {
			lvl.set(SWITCH::A);
		} else if(val < 722) {
			lvl.set(SWITCH::B);
		}

		switch_man_.service(lvl);
	}
#endif

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

#ifdef ENABLE_LCD
		bitmap_.flash(0);
		bool lcd = true;
		wav_info_x_ = 0;
		wav_info_y_ = 0;
#else
		bool lcd = false;
#endif
		if(!wav_.load_header(&fil, lcd)) {
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

#ifdef ENABLE_LCD

		lcd_.copy(bitmap_.fb());
#endif

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
#ifdef ENABLE_LCD
			adc_.start_scan(2);
#endif
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

			char ch = 0;
			if(sci_length() > 0) {
				ch = sci_getch();
			}

#ifdef ENABLE_LCD
			switch_decode_();

			if(switch_man_.get_positive().get(SWITCH::RIGHT)) {
				ch = '>';
			}
			if(switch_man_.get_positive().get(SWITCH::LEFT)) {
				ch = '<';
			}
			if(switch_man_.get_positive().get(SWITCH::A)) {
				ch = ' ';
			}
#endif
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

		master_.at_task().set_param(skip, l_ofs, r_ofs, wofs);

		f_close(&fil);
	}

	void play_loop_(const char*);
	void play_loop_func_(const char* name, const FILINFO* fi, bool dir, void* option)
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

#ifdef ENABLE_LCD
	// A/D 開始
	{
		device::PM2.B2 = 1;
		device::PM2.B3 = 1;
		uint8_t intr_level = 1;
		adc_.start(adc::REFP::VDD, adc::REFM::VSS, intr_level);
	}
#endif

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

#ifdef ENABLE_LCD
	// CSI graphics 開始
	{
		uint8_t intr_level = 0;
		if(!csig_.start(8000000, csig::PHASE::TYPE4, intr_level)) {
			uart_.puts("CSI Start fail...\n");
		}

		lcd_.start(0x04, true);
		bitmap_.flash(0);
	}
#endif

	uart_.puts("Start RL78/G13 WAV file player sample\n");

	command_.set_prompt("# ");

	uint8_t n = 0;
#ifdef ENABLE_LCD
	bool fbf = true;
	bool fbf_back = false;
	bool mount = sdc_.service();
	bool fbcopy = false;
#endif
//	FIL fil;
	char tmp[64];
	while(1) {
		itm_.sync();

#ifdef ENABLE_LCD
		adc_.start_scan(2);

		if((fbf_back && !fbf) || fbcopy) {
			lcd_.copy(bitmap_.fb());
		}

		adc_.sync();

		switch_decode_();

		fbf_back = fbf;
		if(fbf) {
			bitmap_.flash(0);
			fbf = false;
		}

		bool f = sdc_.service();
		kfont_.set_mount(f);

		fbcopy = filer_.service(f, 6);

		if(switch_man_.get_positive().get(SWITCH::A)) {
			if(filer_.ready()) {
				fbf = filer_.start();
			} else {
				filer_.close();
				fbf = true;
			}
		} else if(switch_man_.get_positive().get(SWITCH::UP)) {
			if(!filer_.ready()) {
				fbf = filer_.set_focus(-1);
			}
		} else if(switch_man_.get_positive().get(SWITCH::DOWN)) {
			if(!filer_.ready()) {
				fbf = filer_.set_focus(1);
			}
		} else if(switch_man_.get_negative().get(SWITCH::RIGHT)) {
			if(!filer_.ready()) {
				if(filer_.set_directory()) {
					fbf = true;
				} else {
					const char* p = filer_.get_select_path();
					if(p != nullptr) {
///						utils::format("Path: '%s'\n") % p;
						play_(p);
						filer_.close();
						fbf = true;
					}
				}
			}
		} else if(switch_man_.get_positive().get(SWITCH::LEFT)) {
			if(!filer_.ready()) {
				if(filer_.set_directory(false)) {
					fbf = true;
				}
			}
		} else if(switch_man_.get_negative().get(SWITCH::B)) {
			if(!filer_.ready()) {
				play_loop_("");
				filer_.close();
				fbf = true;
			}
		}

		if(!f && mount) {
			fbf = true;
		}
		mount = f;
#else
		sdc_.service();
#endif

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
