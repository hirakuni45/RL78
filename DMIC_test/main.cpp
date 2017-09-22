//=====================================================================//
/*!	@file
	@brief	デジタル・マイク制御 (R5F100ECA) 40ピン @n
			ROM: 32K, RAM: @n
			P15: green LED @n
			P16: red LED @n
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include <cstdint>
#include<cstring>
#include "common/renesas.hpp"
#include "common/port_utils.hpp"
#include "common/uart_io.hpp"
#include "common/fifo.hpp"
#include "common/format.hpp"
#include "common/iica_io.hpp"
#include "common/itimer.hpp"
#include "common/command.hpp"

namespace {

	typedef device::iica_io<device::IICA0> IICA;
	IICA 	iica_;

	// UART1 の定義（SAU2、SAU3）
	typedef utils::fifo<uint8_t, 64> BUFFER;
	typedef device::uart_io<device::SAU00, device::SAU01, BUFFER, BUFFER> UART0;
	typedef device::uart_io<device::SAU02, device::SAU03, BUFFER, BUFFER> UART1;
	UART1	uart1_;

	typedef device::itimer<uint8_t> ITM;
	ITM		itm_;

	utils::command<64> command_;
}


extern "C" {

	void sci_putch(char ch)
	{
		uart1_.putch(ch);
	}


	void sci_puts(const char* str)
	{
		uart1_.puts(str);
	}


	char sci_getch(void)
	{
		return uart1_.getch();
	}


	uint16_t sci_length()
	{
		return uart1_.recv_length();
	}


	void UART1_TX_intr(void)
	{
		uart1_.send_task();
	}


	void UART1_RX_intr(void)
	{
		uart1_.recv_task();
	}


	void UART1_ER_intr(void)
	{
		uart1_.error_task();
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


int main(int argc, char* argv[])
{
	using namespace device;

//	utils::port::pullup_all();  ///< 安全の為、全ての入力をプルアップ

	// itimer の開始
	{
		uint8_t intr_level = 1;
		itm_.start(60, intr_level);
	}

	// UART1 の開始
	{
		uint8_t intr_level = 1;
		uart1_.start(115200, intr_level);
	}

	// IICA(I2C) の開始
	{
		uint8_t intr_level = 0;
//		if(!iica_.start(IICA::speed::fast, intr_level)) {
		if(!iica_.start(IICA::speed::standard, intr_level)) {
			utils::format("IICA start error (%d)\n") % static_cast<uint32_t>(iica_.get_last_error());
		}
	}

	ADPC = 0x01; // A/D input All digital port
	PM2.B3 = 0;  // POWER CTRL (OUTPUT)
	P2.B3  = 1;  // Active high (ON)

	PM1.B5 = 0;
	PM1.B6 = 0;

	uint8_t cnt = 0;
	while(1) {
		itm_.sync();

		if(cnt >= 20) {
			cnt = 0;
		}
		if(cnt < 10) {
			P1.B5 = 1;
			P1.B6 = 0;
		} else {
			P1.B5 = 0;
			P1.B6 = 1;
		}
		++cnt;
	}

#if 0
	utils::format("Start RL78/G13 DMIC ctrl\n");

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
			uint8_t cmdn = command_.get_words();
			if(cmdn >= 1) {
#if 0
				if(command_.cmp_word(0, "date")) {
					if(cmdn == 1) {
						time_t t = get_time_();
						if(t != 0) {
							disp_time_(t);
						}
					} else {
						set_time_date_();
					}
				} else if(command_.cmp_word(0, "help")) {
					sci_puts("date\n");
					sci_puts("date yyyy/mm/dd hh:mm[:ss]\n");
				} else {
					char buff[16];
					if(command_.get_word(0, sizeof(buff), buff)) {
						sci_puts("Command error: ");
						sci_puts(buff);
						sci_putch('\n');
					}
				}
#endif
			}
		}
	}
#endif
}
