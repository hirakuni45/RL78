//=====================================================================//
/*!	@file
	@brief	サウンド生成サンプル @n
			P16/TO01(40) から、左チャネル @n
			P17/TO02(39) から、右チャネル
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include <cstdint>
#include "common/port_utils.hpp"
#include "common/fifo.hpp"
#include "common/uart_io.hpp"
#include "common/itimer.hpp"
#include "common/format.hpp"
#include "common/command.hpp"

namespace {

	// 送信、受信バッファの定義
	typedef utils::fifo<uint32_t, 32> buffer;
	// UART の定義（SAU02、SAU03）
	device::uart_io<device::SAU02, device::SAU03, buffer, buffer> uart_;

	// インターバル・タイマー
	device::itimer<uint8_t> itm_;

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


	void UART1_TX_intr(void)
	{
		uart_.send_task();
	}


	void UART1_RX_intr(void)
	{
		uart_.recv_task();
	}


	void UART1_ER_intr(void)
	{
		uart_.error_task();
	}


//	void ADC_intr(void)
//	{
//		adc_.task();
//	}


	void ITM_intr(void)
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
