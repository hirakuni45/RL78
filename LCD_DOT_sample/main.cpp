//=====================================================================//
/*!	@file
	@brief	LCD ドットマトリックスの簡単なサンプル @n
			ST7565、128 x 64 ピクセルの液晶を接続 @n
			/RES ---> /RESET    ( 6) @n
			A0   ---> P00       (62) @n
			/CS  ---> P01       (61) @n
			SCL  ---> P04/SCL10 (58) @n
			SI   ---> P02/SO10  (60)  
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
#include "chip/ST7565.hpp"
#include "common/monograph.hpp"

namespace {
	void wait_()
	{
		asm("nop");
	}

	// 送信、受信バッファの定義
	typedef utils::fifo<128> buffer;
	// UART の定義（SAU2、SAU3）
	device::uart_io<device::SAU02, device::SAU03, buffer, buffer> uart_;

	device::itimer<uint8_t> itm_;

	// CSI(SPI) の定義、CSI10 の通信では、「SAU00」を利用、０ユニット、チャネル０
	typedef device::csi_io<device::SAU00> csi;
	csi csi_;
	chip::ST7565<csi> lcd_(csi_);

	graphics::monograph bitmap_;

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

	// UART0 開始
	{
		uint8_t intr_level = 1;
		uart_.start(115200, intr_level);
	}

	// CSI10 開始
	{
		uint8_t intr_level = 0;
		if(!csi_.start(8000000, csi::PHASE::TYPE4, intr_level)) {
			uart_.puts("CSI Start fail...\n");
		}
	}

	uart_.puts("Start RL78/G13 LCD-Dotmatix sample\n");

	PM4.B3 = 0;  // output

	{
		lcd_.start(0x04);
		bitmap_.start();
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
			lcd_.copy(bitmap_.fb());
			nn = 0;
		}
		++nn;

		if(loop >= 20) {
			loop = 0;
//			bitmap_.clear(0);
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
