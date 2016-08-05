//=====================================================================//
/*!	@file
	@brief	A/D の簡単なサンプル
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include <cstdint>
#include "G13/system.hpp"
#include "common/port_utils.hpp"
#include "common/fifo.hpp"
#include "common/uart_io.hpp"
#include "common/format.hpp"
#include "common/itimer.hpp"
#include "G13/adc.hpp"

namespace {
	void wait_()
	{
		asm("nop");
	}

	typedef utils::fifo<128> buffer;
	device::uart_io<device::SAU00, device::SAU01, buffer, buffer> uart0_io_;

	device::itimer<uint8_t> itm_;
}

/// 割り込みベクターの定義
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
	/* 13 */  reinterpret_cast<void*>(uart0_io_.send_task),
	/* 14 */  reinterpret_cast<void*>(uart0_io_.recv_task),
	/* 15 */  reinterpret_cast<void*>(uart0_io_.error_task),
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
		uart0_io_.putch(ch);
	}

	void sci_puts(const char* str)
	{
		uart0_io_.puts(str);
	}
};


int main(int argc, char* argv[])
{
	utils::port::pullup_all();  ///< 安全の為、全ての入力をプルアップ

	device::PM4.B3 = 0;  // output

	{
		uint8_t intr_level = 0;
		uart0_io_.start(115200, intr_level);
	}

	{
		uint8_t intr_level = 1;
		itm_.start(60, intr_level);
	}

	uart0_io_.puts("Start RL78/G13 A/D sample\n");

	bool f = false;
	uint32_t n = 0;
	while(1) {
		itm_.sync();
		if(uart0_io_.recv_length()) {
			auto ch = uart0_io_.getch();
			if(ch == '\r') {
				utils::format("%d\n") % n;
				++n;
			} else {
				uart0_io_.putch(ch);
			}
		}
		device::P4.B3 = f;
		f = !f;
	}
}
