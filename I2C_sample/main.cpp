//=====================================================================//
/*!	@file
	@brief	IICA(I2C) のサンプル
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include "common/renesas.hpp"
#include "common/port_utils.hpp"
#include "common/itimer.hpp"
#include "common/uart_io.hpp"
#include "common/format.hpp"
#include "common/iica_io.hpp"
#include "common/fifo.hpp"
#include "common/command.hpp"

namespace {

	typedef device::itimer<uint8_t> ITM;
	ITM		itm_;

	typedef utils::fifo<uint8_t, 32> BUFFER;
	typedef device::uart_io<device::SAU02, device::SAU03, BUFFER, BUFFER> UART;
	UART	uart_;

	typedef device::iica_io<device::IICA0> I2C;
	I2C		i2c_;

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


	void ITM_intr(void)
	{
		itm_.task();
	}
};


int main(int argc, char* argv[])
{
	using namespace device;

	utils::port::pullup_all();  ///< 安全の為、全ての入力をプルアップ

	PM4.B3 = 0;  // output

	// itimer の開始
	{
		uint8_t intr_level = 1;
		itm_.start(60, intr_level);
	}

	// UART の開始
	{
		uint8_t intr_level = 1;
		uart_.start(115200, intr_level);
	}

	// I2C の開始
	{
		uint8_t intr_level = 0;
		if(!i2c_.start(I2C::speed::fast, intr_level)) {
//		if(!i2c_.start(I2C::speed::standard, intr_level)) {
			utils::format("IICA start error (%d)\n") % static_cast<uint32_t>(i2c_.get_last_error());
		}
	}

	uart_.puts("Start RL78/G13 I2C sample\n");

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
