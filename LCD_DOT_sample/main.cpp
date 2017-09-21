//=====================================================================//
/*!	@file
	@brief	LCD ドットマトリックスの簡単なサンプル @n
			ST7565(R)、128 x 64 ピクセルの液晶を接続 @n
			/CS  ---> P53       (36) @n
			A0   ---> P50       (33) @n
			SD   ---> P13/SO20  (43) @n
			SCL  ---> P15/SCK20 (41) @n 
			/RES ---> /RESET    ( 6)
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
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
#include "common/monograph.hpp"

// ターゲットＬＣＤのタイプを選択
#define LCD_ST7565
// #define LCD_SSD1306
#ifdef LCD_ST7565
#include "chip/ST7565.hpp"
#endif
#ifdef LCD_SSD1306
#include "chip/SSD1306.hpp"
#endif

namespace {
	// 送信、受信バッファの定義
	typedef utils::fifo<uint8_t, 32> buffer;
	// UART の定義（SAU02、SAU03）
	device::uart_io<device::SAU02, device::SAU03, buffer, buffer> uart_;

	device::itimer<uint8_t> itm_;

	// CSI(SPI) の定義、CSI20 の通信では、「SAU10」を利用、１ユニット、チャネル０
	typedef device::csi_io<device::SAU10> csi;
	csi csi_;

	// LCD インターフェースの定義
	typedef device::PORT<device::port_no::P5,  device::bitpos::B5> lcd_sel;	///< LCD 選択信号
	typedef device::PORT<device::port_no::P5,  device::bitpos::B0> lcd_reg;	///< LCD レジスタ選択

#ifdef LCD_ST7565
	chip::ST7565<csi, lcd_sel, lcd_reg> lcd_(csi_);
#endif
#ifdef LCD_SSD1306
	chip::SSD1306<csi, lcd_sel, lcd_reg> lcd_(csi_);
#endif

	graphics::kfont_null kfont_;
	graphics::monograph<128, 64> bitmap_(kfont_);

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
};



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
	{
		uint8_t intr_level = 0;
		if(!csi_.start(8000000, csi::PHASE::TYPE4, intr_level)) {
			uart_.puts("CSI Start fail...\n");
		}
	}

	uart_.puts("Start RL78/G13 ST7565(R) sample\n");

	PM4.B3 = 0;  // output

	{
		lcd_.start(0x04, true);
		bitmap_.clear(0);
	}

	uint16_t x = rand_() & 127;
	uint16_t y = rand_() & 63;
	uint16_t xx;
	uint16_t yy;
	uint8_t loop = 0;
	uint8_t	nn = 0;

	uint8_t n = 0;
	while(1) {
		itm_.sync();

		if(nn >= 4) {
			lcd_.copy(bitmap_.fb(), bitmap_.page_num());
			nn = 0;
		}
		++nn;

		if(loop >= 20) {
			loop = 0;
			bitmap_.clear(0);
			bitmap_.frame(0, 0, 128, 64, 1);
		}
		xx = rand_() & 127;
		yy = rand_() & 63;
		bitmap_.line(x, y, xx, yy, 1);
		x = xx;
		y = yy;
		++loop;

		++n;
		if(n >= 30) n = 0;
		P4.B3 = n < 10 ? false : true; 	
	}
}
