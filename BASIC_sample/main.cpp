//=====================================================================//
/*!	@file
	@brief	BASIC サンプル
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include "common/renesas.hpp"
#include "common/port_utils.hpp"
#include "common/fifo.hpp"
#include "common/uart_io.hpp"
#include "common/itimer.hpp"
#include "common/format.hpp"
#include "common/command.hpp"

namespace {

	// 送信、受信バッファの定義
	typedef utils::fifo<uint32_t, 32> BUFFER;
	// UART の定義（SAU02、SAU03）
	typedef device::uart_io<device::SAU02, device::SAU03, BUFFER, BUFFER> UART;
	UART	uart_;

	// インターバル・タイマー
	typedef device::itimer<uint8_t> ITM;
	ITM		itm_;

	utils::command<64> command_;
}


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


	uint16_t sci_length()
	{
		return uart_.recv_length();
	}


	INTERRUPT_FUNC void UART1_TX_intr(void)
	{
		uart_.send_task();
	}


	INTERRUPT_FUNC void UART1_RX_intr(void)
	{
		uart_.recv_task();
	}


	INTERRUPT_FUNC void UART1_ER_intr(void)
	{
		uart_.error_task();
	}


	INTERRUPT_FUNC void ITM_intr(void)
	{
		itm_.task();
	}
};


namespace {


}


int main(int argc, char* argv[])
{
	utils::port::pullup_all();  ///< 安全の為、全ての入力をプルアップ

	device::PM4.B3 = 0;  // output

	// インターバル・タイマー開始
	{
		uint8_t intr_level = 1;
		itm_.start(60, intr_level);
	}

	// UART 開始
	{
		uint8_t intr_level = 1;
		uart_.start(115200, intr_level);
	}

	uart_.puts("Start RL78/G13 BASIC sample\n");

	command_.set_prompt("# ");

	uint8_t n = 0;
	while(1) {
		itm_.sync();

		// コマンド入力と、コマンド解析
		if(command_.service()) {
		}

		++n;
		if(n >= 30) {
			n = 0;
		}
		device::P4.B3 = n < 10 ? false : true;
	}
}
