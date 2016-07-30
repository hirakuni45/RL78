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
#include "G13/port.hpp"
#include "common/fifo.hpp"
#include "common/uart_io.hpp"
#include "common/itimer.hpp"
#include "common/format.hpp"
#include "common/delay.hpp"
#include "chip/ST7565.hpp"

namespace {
	void wait_()
	{
		asm("nop");
	}

	// 送信、受信バッファの定義
	typedef utils::fifo<128> buffer;
	// UART の定義（SAU0、SAU1）
	device::uart_io<device::SAU00, device::SAU01, buffer, buffer> uart0_;

	device::itimer<uint8_t> itm_;

	// CSI(SPI) の定義、CSI10 の通信では、「SAU02」を利用、０ユニット、チャネル２
	typedef device::csi_io<device::SAU02> csi10;
	csi10 csi10_;
	chip::ST7565<csi10> lcd_(csi10_);
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
	/* 13 */  reinterpret_cast<void*>(uart0_.send_task),
	/* 14 */  reinterpret_cast<void*>(uart0_.recv_task),
	/* 15 */  reinterpret_cast<void*>(uart0_.error_task),
	/* 16 */  nullptr,
	/* 17 */  nullptr,
	/* 18 */  nullptr,
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
		uart0_.putch(ch);
	}

	void sci_puts(const char* str)
	{
		uart0_.puts(str);
	}
};


int main(int argc, char* argv[])
{
	using namespace device;

	// インターバル・タイマー開始
	{
		uint8_t intr_level = 1;
		itm_.start(60, intr_level);
	}

	// UART0 開始
	{
		uint8_t intr_level = 1;
		uart0_.start(115200, intr_level);
	}

	// CSI10 開始
	{
		uint8_t intr_level = 0;
		if(!csi10_.start(8000000, intr_level)) {
			uart0_.puts("CSI Start fail...\n");
		}
	}

	uart0_.puts("Start RL78/G13 LCD-Dotmatix sample\n");

	PM4.B3 = 0;  // output

#if 0
	while(1) {
		csi10_.write(0x55);
		utils::delay::micro_second(10);
		P4.B3 = !P4.B3();
	}
#endif

	{
		lcd_.start(0x04);
	}

	bool f = false;
	uint8_t n = 0;
	while(1) {
		itm_.sync();
		++n;
		if(n >= 30) n = 0;
		P4.B3 = n < 10 ? false : true; 	
	}
}
