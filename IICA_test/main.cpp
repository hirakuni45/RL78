//=====================================================================//
/*!	@file
	@brief	IICA(I2C) の簡単なテスト
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include <cstdint>
#include "G13/system.hpp"
#include "G13/port.hpp"
#include "common/uart_io.hpp"
#include "common/format.hpp"
#include "common/iica_io.hpp"

namespace {
	void wait_()
	{
		asm("nop");
	}
	typedef device::iica_io<device::IICA0> IICA;
	IICA iica0_io_;
	device::uart_io<device::SAU00, device::SAU01, 128, 128> uart0_io_;
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
	device::PM4.B3 = 0;  // output

	{
		uint8_t intr_level = 0;
		iica0_io_.start(IICA::speed::fast, intr_level);
	}

	{
		uint8_t intr_level = 1;
		uart0_io_.start(115200, intr_level);
	}
	uart0_io_.puts("Start RL78/G13 I2C test...\n");

	// I2C 送信
	{
		uint8_t tmp[4];
		tmp[0] = 0x00;
		tmp[1] = 0x01;
		tmp[2] = 0x02;
		tmp[3] = 0x03;
		iica0_io_.send(0x71, tmp, 4);
	}

	// I2C 受信
	{
		uint8_t tmp[4];
		iica0_io_.recv(0x71, tmp, 4);
	}

	bool f = false;
	uint32_t n = 0;
	while(1) {
		for(uint32_t i = 0; i < 100000; ++i) {
			if(uart0_io_.recv_length()) {
				auto ch = uart0_io_.getch();
				if(ch == '\r') {
					utils::format("%d\n") % n;
					++n;
				} else {
					uart0_io_.putch(ch);
				}
			}
		}
		device::P4.B3 = f;
		f = !f;
	}
}
