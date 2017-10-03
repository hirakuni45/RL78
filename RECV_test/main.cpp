//=====================================================================//
/*!	@file
	@brief	デジタル・マイク・レシーバー
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include "common/renesas.hpp"
#include "common/port_utils.hpp"
#include "common/itimer.hpp"
#include "common/fifo.hpp"
#include "common/uart_io.hpp"
#include "common/tau_io.hpp"
#include "common/iica_io.hpp"
#include "common/format.hpp"
#include "common/command.hpp"
#include "chip/SEGMENT.hpp"

namespace {

	typedef device::itimer<uint8_t> ITM;
	ITM		itm_;

	// UART の定義
	typedef utils::fifo<uint8_t, 64> BUFFER;
	typedef device::uart_io<device::SAU00, device::SAU01, BUFFER, BUFFER> UART0;
	typedef device::uart_io<device::SAU02, device::SAU03, BUFFER, BUFFER> UART1;
	typedef device::uart_io<device::SAU10, device::SAU11, BUFFER, BUFFER> UART2;
	UART0	uart0_;
	UART1	uart1_;
	UART2	uart2_;

	typedef device::PORT<device::port_no::P7, device::bitpos::B3> SG1_A;
	typedef device::PORT<device::port_no::P7, device::bitpos::B2> SG1_B;
	typedef device::PORT<device::port_no::P7, device::bitpos::B1> SG1_C;
	typedef device::PORT<device::port_no::P7, device::bitpos::B0> SG1_D;
	typedef device::PORT<device::port_no::P5, device::bitpos::B2> SG1_E;
	typedef device::PORT<device::port_no::P5, device::bitpos::B1> SG1_F;
	typedef device::PORT<device::port_no::P5, device::bitpos::B0> SG1_G;
	typedef chip::SEGMENT<SG1_A, SG1_B, SG1_C, SG1_D, SG1_E, SG1_F, SG1_G, device::NULL_PORT> SEG1;

	typedef device::PORT<device::port_no::P12,device::bitpos::B6> SG2_A;
	typedef device::PORT<device::port_no::P12,device::bitpos::B5> SG2_B;
	typedef device::PORT<device::port_no::P3, device::bitpos::B1> SG2_C;
	typedef device::PORT<device::port_no::P7, device::bitpos::B7> SG2_D;
	typedef device::PORT<device::port_no::P7, device::bitpos::B6> SG2_E;
	typedef device::PORT<device::port_no::P7, device::bitpos::B5> SG2_F;
	typedef device::PORT<device::port_no::P7, device::bitpos::B4> SG2_G;
	typedef chip::SEGMENT<SG2_A, SG2_B, SG2_C, SG2_D, SG2_E, SG2_F, SG2_G, device::NULL_PORT> SEG2;

	utils::command<64> command_;
}


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


	INTERRUPT_FUNC void UART0_TX_intr(void)
	{
		uart0_.send_task();
	}


	INTERRUPT_FUNC void UART0_RX_intr(void)
	{
		uart0_.recv_task();
	}


	INTERRUPT_FUNC void UART0_ER_intr(void)
	{
		uart0_.error_task();
	}


	INTERRUPT_FUNC void UART1_TX_intr(void)
	{
		uart1_.send_task();
	}


	INTERRUPT_FUNC void UART1_RX_intr(void)
	{
		uart1_.recv_task();
	}


	INTERRUPT_FUNC void UART1_ER_intr(void)
	{
		uart1_.error_task();
	}


	INTERRUPT_FUNC void UART2_TX_intr(void)
	{
		uart2_.send_task();
	}


	INTERRUPT_FUNC void UART2_RX_intr(void)
	{
		uart2_.recv_task();
	}


	INTERRUPT_FUNC void UART2_ER_intr(void)
	{
		uart2_.error_task();
	}


	INTERRUPT_FUNC void ITM_intr(void)
	{
		itm_.task();
	}
};


int main(int argc, char* argv[])
{
//	utils::port::pullup_all();  ///< 安全の為、全ての入力をプルアップ

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

	// UART1 の開始
	{
		uint8_t intr_level = 1;
		uart1_.start(19200, intr_level);
	}
	// UART2 の開始
	{
		uint8_t intr_level = 1;
		uart2_.start(19200, intr_level);
	}


	utils::format("Start Digital MIC Reciver\n");


	device::PFSEG0 = 0x00;
	device::PFSEG1 = 0x00;
	device::PFSEG2 = 0x00;
	device::PFSEG3 = 0x00;
	device::PFSEG4 = 0x00;
	device::PFSEG5 = 0x00;
	device::PFSEG6 = 0x00;

	SEG1::start();
	SEG2::start();

	command_.set_prompt("# ");

	uint8_t	n = 0;
	uint8_t t = 0;
	while(1) {
		itm_.sync();

		SEG1::decimal(t / 10);
		SEG2::decimal(t % 10);

		// コマンド入力と、コマンド解析
		if(command_.service()) {
			uint8_t cmdn = command_.get_words();
			if(cmdn >= 1) {
				if(command_.cmp_word(0, "help")) {
					utils::format("---\n");
				}
			}
		}

		++n;
		if(n >= 60) {
			n = 0;
			++t;
			if(t >= 100) t = 0;

//			utils::format("%d\n") % static_cast<uint16_t>(t);

		}
		if(n < 20) {
//			SEG1::decimal(4);
		} else {
//			SEG1::decimal(7);
		}
	}
}
