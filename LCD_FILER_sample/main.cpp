//=====================================================================//
/*!	@file
	@brief	LCD 表示を使ったファイラー・サンプル @n
			ST7565(R)、128 x 64 ピクセルの液晶を接続 @n
			/CS  ---> P53       (36) @n
			A0   ---> P50       (33) @n
			SD   ---> P13/SO20  (43) @n
			SCL  ---> P15/SCK20 (41) @n 
			/RES ---> /RESET    ( 6)
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include <cstdint>
#include "G13/system.hpp"
#include "common/port_utils.hpp"
#include "common/fifo.hpp"
#include "common/uart_io.hpp"
#include "common/itimer.hpp"
#include "common/adc_io.hpp"
#include "common/format.hpp"
#include "common/delay.hpp"
#include "common/monograph.hpp"
#include "common/font6x12.hpp"
#include "common/kfont12.hpp"
#include "common/sdc_io.hpp"
#include "common/filer.hpp"
#include "common/bitset.hpp"
#include "common/switch_man.hpp"

// ターゲットＬＣＤのタイプを選択
#define LCD_ST7565
// #define LCD_SSD1306
#ifdef LCD_ST7565
#include "chip/ST7565.hpp"
#endif
#ifdef LCD_SSD1306
#include "chip/SSD1306.hpp"
#endif

// スイッチ入力を有効にする場合
// ※スイッチの接続は、ADC_SWITCH_sample を参照
#define ADC_SWITCH

namespace {

	// 送信、受信バッファの定義
	typedef utils::fifo<uint8_t, 32> buffer;
	// UART の定義（SAU02、SAU03）
	device::uart_io<device::SAU02, device::SAU03, buffer, buffer> uart_;

	device::itimer<uint8_t> itm_;

#ifdef ADC_SWITCH
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
#endif

	// SDC CSI(SPI) の定義、CSI00 の通信では、「SAU00」を利用、０ユニット、チャネル０
	typedef device::csi_io<device::SAU00> csi;
	csi csi_;

	// FatFS インターフェースの定義
	typedef device::PORT<device::port_no::P0,  device::bitpos::B0> card_select;	///< カード選択信号
	typedef device::PORT<device::port_no::P0,  device::bitpos::B1> card_power;	///< カード電源制御
	typedef device::PORT<device::port_no::P14, device::bitpos::B6> card_detect;	///< カード検出

	typedef utils::sdc_io<csi, card_select, card_power, card_detect> sdc_io;
	sdc_io sdc_(csi_);

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

	typedef device::adc_io<utils::null_task> adc;
	adc adc_;

	graphics::filer<sdc_io, bitmap> filer_(sdc_, bitmap_);
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
#ifdef WITH_RTC
		time_t t = 0;
		if(!rtc_.get_time(t)) {
			utils::format("Stall RTC read (%d)\n") % static_cast<uint32_t>(iica_.get_last_error());
		}
		return utils::str::get_fattime(t);
#else
		return 0;
#endif
	}
};


int main(int argc, char* argv[])
{
	using namespace device;

	utils::port::pullup_all();  ///< 安全の為、全ての入力をプルアップ

	PM4.B3 = 0;  // output

	// インターバル・タイマー開始
	{
		uint8_t intr_level = 2;
		itm_.start(60, intr_level);
	}

	// UART 開始
	{
		uint8_t intr_level = 1;
		uart_.start(115200, intr_level);
	}

	// A/D 開始
	{
		device::PM2.B2 = 1;
		device::PM2.B3 = 1;
		uint8_t intr_level = 0;
		adc_.start(adc::REFP::VDD, adc::REFM::VSS, intr_level);
	}

	// SD カードアクセス開始
	sdc_.initialize();

	// CSI graphics 開始
	{
		uint8_t intr_level = 0;
		if(!csig_.start(8000000, csig::PHASE::TYPE4, intr_level)) {
			uart_.puts("CSI Start fail...\n");
		}
	}

	uart_.puts("Start RL78/G13 SDC Filer sample\n");

	{
		lcd_.start(0x04, true);
		bitmap_.flash(0);
	}

	uint8_t n = 0;
	bool fbf = true;
	bool fbf_back = false;
	bool mount = sdc_.service();
	bool fbcopy = false;
	while(1) {
		itm_.sync();

#ifdef ADC_SWITCH
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
#endif

		fbf_back = fbf;
		if(fbf) {
			bitmap_.flash(0);
			fbf = false;
		}

		bool f = sdc_.service();
		kfont_.set_mount(f);

		fbcopy = filer_.service(f);

		char ch = 0;
		if(sci_length()) {
			ch = sci_getch();
		}

#ifdef ADC_SWITCH
		if(switch_man_.get_positive().get(SWITCH::A)) ch = ' ';
		else if(switch_man_.get_positive().get(SWITCH::UP)) ch = 'a';
		else if(switch_man_.get_positive().get(SWITCH::DOWN)) ch = 'z';
		else if(switch_man_.get_positive().get(SWITCH::RIGHT)) ch = '\r';
		else if(switch_man_.get_positive().get(SWITCH::LEFT)) ch = 0x08;
#endif
		if(ch == ' ') {
			if(filer_.ready()) {
				fbf = filer_.start();
			} else {
				filer_.close();
				fbf = true;
			}
		} else if(ch == 'a') {
			if(!filer_.ready()) {
				fbf = filer_.set_focus(-1);
			}
		} else if(ch == 'z') {
			if(!filer_.ready()) {
				fbf = filer_.set_focus(1);
			}
		} else if(ch == '\r') {
			if(!filer_.ready()) {
				if(filer_.set_directory()) {
					fbf = true;
				} else {
					const char* p = filer_.get_select_path();
					if(p != nullptr) {
						utils::format("Path: '%s'\n") % p;
						filer_.close();
						fbf = true;
					}
				}
			}
		} else if(ch == 0x08) {
			if(!filer_.ready()) {
				if(filer_.set_directory(false)) {
					fbf = true;
				}
			}
		}

		if(!f && mount) {
			fbf = true;
		}
		mount = f;

		++n;
		if(n >= 30) n = 0;
		P4.B3 = n < 10 ? false : true;

		if((fbf_back && !fbf) || fbcopy) {
			lcd_.copy(bitmap_.fb());
		}
	}
}
