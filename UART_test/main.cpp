//=====================================================================//
/*!	@file
	@brief	UART の簡単なテスト
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include <cstdint>
#include "G13/system.hpp"
#include "G13/port.hpp"
#include "common/uart_io.hpp"
#include "common/format.hpp"

namespace {
	void wait_()
	{
		asm("nop");
	}

	device::uart_io<device::SAU00, device::SAU01, 128, 128> uart_io_;
}

extern "C" {
	void sci_putch(char ch)
	{
		uart_io_.putch(ch);
	}

	void sci_puts(const char* str)
	{
		uart_io_.puts(str);
	}
};


int main(int argc, char* argv[])
{
	device::PM4.B3 = 0;  // output

	bool polling = true;
	uart_io_.start(115200, polling);

	uart_io_.puts("Start RL78/G13 uart test...\n");

	bool f = false;
///	uint32_t n = 0;
	while(1) {
		if(uart_io_.recv_length()) {
			auto ch = uart_io_.getch();
			uart_io_.putch(ch);

			device::P4.B3 = f;
			f = !f;
		}

///		utils::format("%d\n") % n;
///		++n;
	}
}
