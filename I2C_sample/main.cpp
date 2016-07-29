//=====================================================================//
/*!	@file
	@brief	IICA(I2C) のサンプル
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include <cstdint>
#include "G13/system.hpp"
#include "G13/port.hpp"
#include "common/itimer.hpp"
#include "common/uart_io.hpp"
#include "common/format.hpp"
#include "common/iica_io.hpp"
#include "common/fifo.hpp"
#include "common/command.hpp"

namespace {
	void wait_()
	{
		asm("nop");
	}
	device::itimer<uint8_t> itm_;

	typedef utils::fifo<128> buffer;
	device::uart_io<device::SAU00, device::SAU01, buffer, buffer> uart0_;

	typedef device::iica_io<device::IICA0> IICA;
	IICA iica0_;

	utils::command<64> command_;
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

	char sci_getch(void)
	{
		return uart0_.getch();
	}

	uint16_t sci_length()
	{
		return uart0_.recv_length();
	}
};


int main(int argc, char* argv[])
{
	using namespace device;

	PM4.B3 = 0;  // output

	// itimer の開始
	{
		uint8_t intr_level = 1;
		itm_.start(60, intr_level);
	}

	// UART0 の開始
	{
		uint8_t intr_level = 1;
		uart0_.start(115200, intr_level);
	}

	// IICA(I2C) の開始
	{
		uint8_t intr_level = 0;
		if(!iica0_.start(IICA::speed::fast, intr_level)) {
//		if(!iica0_.start(IICA::speed::standard, intr_level)) {
			utils::format("IICA start error (%d)\n") % static_cast<uint32_t>(iica0_.get_last_error());
		}
	}

	uart0_.puts("Start RL78/G13 I2C sample\n");

	command_.set_prompt("# ");

	uint8_t cnt = 0;
	while(1) {
		itm_.sync();

		if(cnt >= 20) {
			cnt = 0;
		}
		if(cnt < 10) P4.B3 = 1;
		else P4.B3 = 0;
		++cnt;

		// コマンド入力と、コマンド解析
		if(command_.service()) {

		}
	}
}
