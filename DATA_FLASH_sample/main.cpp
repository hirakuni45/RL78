//=====================================================================//
/*!	@file
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include <cstdint>
#include "G13/system.hpp"
#include "G13/port.hpp"
#include "common/fifo.hpp"
#include "common/uart_io.hpp"
#include "common/itimer.hpp"
#include "common/format.hpp"

#include "data_flash_lib/data_flash_util.h"

namespace {

	void wait_()
	{
		asm("nop");
	}

	// 送信、受信バッファの定義
	typedef utils::fifo<uint8_t, 32> buffer;
	// UART の定義（SAU0、SAU1）
	device::uart_io<device::SAU00, device::SAU01, buffer, buffer> uart0_io_;

	device::itimer<uint8_t> itm_;
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
	using namespace device;

	{
		uint8_t intr_level = 1;
		itm_.start(60, intr_level);
	}

	{
		uint8_t intr_level = 1;
		uart0_io_.start(115200, intr_level);
	}

	{
		pfdl_open();


		pfdl_close();
	}

	uart0_io_.puts("Start RL78/G13 Data-Flash sample\n");

	PM4.B3 = 0;  // output

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
		P4.B3 = f;
		f = !f;
	}
}
