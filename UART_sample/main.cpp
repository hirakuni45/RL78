//=====================================================================//
/*!	@file
	@brief	UART の簡単なサンプル @n
			「P11/RxD0 (45)」から入力した値を「P12/TxD0 (44)」@n
			「P03/RxD1 (59)」から入力した値を「P02/TxD1 (60)」@n
			ボーレートは、「115200 b.p.s.」を設定
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include <cstdint>
#include "G13/system.hpp"
#include "common/port_utils.hpp"
#include "common/uart_io.hpp"
#include "common/format.hpp"
#include "common/fifo.hpp"

#include <cstdio>

// どれか一つだけ有効にする。
// #define UART0
#define UART1

namespace {
	void wait_()
	{
		asm("nop");
	}

	// 送信、受信バッファの定義
	typedef utils::fifo<uint8_t, 64> buffer;
#ifdef UART0
	// UART0 の定義（SAU0、SAU1）
	device::uart_io<device::SAU00, device::SAU01, buffer, buffer> uart_;
#endif
#ifdef UART1
	// UART1 の定義（SAU2、SAU3）
	device::uart_io<device::SAU02, device::SAU03, buffer, buffer> uart_;
#endif
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
	/* 13 */  reinterpret_cast<void*>(uart_.send_task),  // UART0-TX
	/* 14 */  reinterpret_cast<void*>(uart_.recv_task),  // UART0-RX
	/* 15 */  reinterpret_cast<void*>(uart_.error_task), // UART0-ER
	/* 16 */  reinterpret_cast<void*>(uart_.send_task),  // UART1-TX
	/* 17 */  reinterpret_cast<void*>(uart_.recv_task),  // UART1-RX
	/* 18 */  reinterpret_cast<void*>(uart_.error_task), // UART1-ER
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

	char sci_getch(void)
	{
		return uart_.getch();
	}
};


int main(int argc, char* argv[])
{
	utils::port::pullup_all();  ///< 安全の為、全ての入力をプルアップ

	device::PM4.B3 = 0;  // output

	{
		// 割り込みを使う場合、intr_level を設定
		// ポーリングなら「０」
		uint8_t intr_level = 0;
		uart_.start(115200, intr_level);
	}

	utils::format("Start RL78/G13 UART%d sample\n") % static_cast<uint32_t>(uart_.get_chanel_no());

	float a = 1000.0005f;
	utils::format("Float: %4.4f\n") % a;
	int b = 1234;
	printf("Float: %d\n", b);

	bool f = false;
	uint32_t n = 0;
	while(1) {
		for(uint32_t i = 0; i < 100000; ++i) {
			if(uart_.recv_length()) {
				auto ch = uart_.getch();
				if(ch == '\r') {
					utils::format("%d\n") % n;
					++n;
				} else {
					uart_.putch(ch);
				}
			}
		}
		device::P4.B3 = f;
		f = !f;
	}
}
