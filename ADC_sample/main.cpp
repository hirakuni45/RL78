//=====================================================================//
/*!	@file
	@brief	A/D 変換のサンプル @n
			P22/ANI2(54)、P23/ANI3(53) を変換して表示
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include <cstdint>
#include "common/port_utils.hpp"
#include "common/fifo.hpp"
#include "common/uart_io.hpp"
#include "common/format.hpp"
#include "common/itimer.hpp"
#include "common/adc_io.hpp"
#include "common/task.hpp"

namespace {
	void wait_()
	{
		asm("nop");
	}

	typedef utils::fifo<uint8_t, 32> buffer;
	device::uart_io<device::SAU02, device::SAU03, buffer, buffer> uart_;

	device::itimer<uint8_t> itm_;

	typedef device::adc_io<utils::null_task> adc;
	adc adc_;
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
	/* 13 */  nullptr,
	/* 14 */  nullptr,
	/* 15 */  nullptr,
	/* 16 */  reinterpret_cast<void*>(uart_.send_task),  // UART1-TX
	/* 17 */  reinterpret_cast<void*>(uart_.recv_task),  // UART1-RX
	/* 18 */  reinterpret_cast<void*>(uart_.error_task), // UART1-ER
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
	utils::port::pullup_all();  ///< 安全の為、全ての入力をプルアップ

	device::PM4.B3 = 0;  // output

	{
		uint8_t intr_level = 1;
		uart_.start(115200, intr_level);
	}

	{
		uint8_t intr_level = 1;
		itm_.start(60, intr_level);
	}

	{
		device::PM2.B2 = 1;
		device::PM2.B3 = 1;
		uint8_t intr_level = 0;
		adc_.start(adc::REFP::VDD, adc::REFM::VSS, intr_level);
	}

	uart_.puts("Start RL78/G13 A/D Convert sample\n");

	uint8_t n = 0;
	uint8_t t = 0;
	while(1) {
		itm_.sync();

		if(t >= 60) {
			auto val = adc_.get(2);
			utils::format("A/D CH2: %d\n") % (val >> 6);
			val = adc_.get(3);
			utils::format("A/D CH3: %d\n") % (val >> 6);
			t = 0;
		} else {
			++t;
		}

		++n;
		if(n >= 30) n = 0;
		device::P4.B3 = n < 10 ? false : true; 	
	}
}
