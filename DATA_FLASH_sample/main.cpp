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
#include "common/flash_io.hpp"

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

	device::flash_io flash_;
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

	char sci_getch(void)
	{
		return uart0_io_.getch();
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

	uart0_io_.puts("Start RL78/G13 Data-Flash sample\n");

	{
		if(!flash_.start()) {
			utils::format("Data Flash Start: NG\n");
		} else {
			utils::format("Data Flash Start: OK\n");
		}
	}

	PM4.B3 = 0;  // output

	uint8_t cnt = 0;
	while(1) {
		itm_.sync();

#if 0
		if(command_.service()) {
			if(command_.cmp_word(0, "erase")) {
				bool f = false;
				if(command_.cmp_word(1, "bank0")) {
					f = flash_.erase(flash_io::data_area::bank0);
				} else if(command_.cmp_word(1, "bank1")) {
					f = flash_.erase(flash_io::data_area::bank1);
				} else {
					sci_puts("Erase bank error...\n");
					f = true;
				}
				if(!f) {
					sci_puts("Erase error...\n");
				}
			} else if(command_.cmp_word(0, "r")) {
				char buff[5];
				if(command_.get_word(1, sizeof(buff), buff)) {
					uint16_t ofs = get_hexadecimal_(buff);
					uint8_t v = flash_.read(ofs);
					put_hexadecimal_byte_(v);
					sci_putch('\n');
				}
			} else if(command_.cmp_word(0, "write")) {
				char buff[5];
				if(command_.get_word(1, sizeof(buff), buff)) {
					uint16_t ofs = get_hexadecimal_(buff);
					if(command_.get_word(2, sizeof(buff), buff)) {
						uint16_t val = get_hexadecimal_(buff);
						if(!flash_.write(ofs, val)) {
							sci_puts("Write error...\n");
						}
					}
				}
			} else if(command_.cmp_word(0, "?")) {
				sci_puts("erase bank[01]\n");
				sci_puts("r xxxx\n");
				sci_puts("write xxxx yy\n");
			} else {
				const char* p = command_.get_command();
				if(p[0]) {
					sci_puts("command error: ");
					sci_puts(p);
					sci_puts("\n");
				}
			}
		}
#endif

		++cnt;
		if(cnt >= 20) {
			P4.B3 = !P4.B3();
			cnt = 0;
		}
	}
}
