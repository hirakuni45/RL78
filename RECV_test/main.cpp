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
#include "common/csi_io.hpp"
#include "common/format.hpp"
#include "common/command.hpp"
#include "common/switch_man.hpp"

#include "chip/PCM1772.hpp"
#include "chip/SEGMENT.hpp"

namespace {

	static const uint16_t VERSION = 11;

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

	// 出力ポート定義
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

	typedef device::PORT<device::port_no::P2,device::bitpos::B1> MUTE;

	// 入力ポート定義
	typedef device::PORT<device::port_no::P1, device::bitpos::B0> LED_M1;
	typedef device::PORT<device::port_no::P2, device::bitpos::B7> LED_M2;
	typedef device::PORT<device::port_no::P2, device::bitpos::B4> LED_M3;
	typedef device::PORT<device::port_no::P2, device::bitpos::B3> LED_M4;

	typedef device::PORT<device::port_no::P4, device::bitpos::B6> CH_SEL;
	typedef device::PORT<device::port_no::P4, device::bitpos::B5> IR_TEST;
	typedef device::PORT<device::port_no::P4, device::bitpos::B4> CH_SCAN;
	typedef device::PORT<device::port_no::P4, device::bitpos::B3> L_M;

	typedef device::PORT<device::port_no::P12, device::bitpos::B4> S_M1;
	typedef device::PORT<device::port_no::P12, device::bitpos::B3> S_M2;
	typedef device::PORT<device::port_no::P13, device::bitpos::B7> S_M3;
	typedef device::PORT<device::port_no::P12, device::bitpos::B2> S_M4;

	typedef device::PORT<device::port_no::P12, device::bitpos::B1> MSEL;

	enum class INPUT_TYPE : uint8_t {
		CH_SEL,
		IR_TEST,
		CH_SCAN,

		L_M,	///< MIC/LINE

		S_M1,
		S_M2,
		S_M3,
		S_M4,

		MSEL,	///< モジュール選択、L:1個、H:2個
	};

	typedef utils::switch_man<uint16_t, INPUT_TYPE> INPUT;
	INPUT	input_;

	void service_input_()
	{
		uint16_t lvl = 0;

		if(CH_SEL::P() ) lvl |= 1 << static_cast<uint8_t>(INPUT_TYPE::CH_SEL);
		if(IR_TEST::P()) lvl |= 1 << static_cast<uint8_t>(INPUT_TYPE::IR_TEST);
		if(CH_SCAN::P()) lvl |= 1 << static_cast<uint8_t>(INPUT_TYPE::CH_SCAN);

		if(L_M::P()) lvl |= 1 << static_cast<uint8_t>(INPUT_TYPE::L_M);

		if(S_M1::P() ) lvl |= 1 << static_cast<uint8_t>(INPUT_TYPE::S_M1);
		if(S_M2::P() ) lvl |= 1 << static_cast<uint8_t>(INPUT_TYPE::S_M2);
		if(S_M3::P() ) lvl |= 1 << static_cast<uint8_t>(INPUT_TYPE::S_M3);
		if(S_M4::P() ) lvl |= 1 << static_cast<uint8_t>(INPUT_TYPE::S_M4);

		if(MSEL::P()) lvl |= 1 << static_cast<uint8_t>(INPUT_TYPE::MSEL);

		input_.service(lvl);
	}

	// CSI(SPI) の定義、CSI30 の通信では、「SAU12」を利用、１ユニット、チャネル２
	typedef device::csi_io<device::SAU12> CSI;
	CSI		csi_;

	// PCM1772 デバイス選択（P34 ---> PCM1772:/MS(15)）
	typedef device::PORT<device::port_no::P3, device::bitpos::B4> PCM_SEL;
	// PCM1772 パワー・ダウン（P30 ---> PCM1772:/PD(?))
	typedef device::PORT<device::port_no::P3, device::bitpos::B0> PCM_PD;

	typedef chip::PCM1772<CSI, PCM_SEL, PCM_PD> PCM;
	PCM		pcm_(csi_);

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
		uart1_.start(9600, intr_level);
	}
	// UART2 の開始
	{
		uint8_t intr_level = 1;
		uart2_.start(9600, intr_level);
	}

	device::PFSEG0 = 0x00;
	device::PFSEG1 = 0x00;
	device::PFSEG2 = 0x00;
	device::PFSEG3 = 0x00;
	device::PFSEG4 = 0x00;
	device::PFSEG5 = 0x00;
	device::PFSEG6 = 0x00;

	SEG1::start();
	SEG2::start();

	LED_M1::DIR = 1;
	LED_M2::DIR = 1;
	LED_M3::DIR = 1;
	LED_M4::DIR = 1;

	MUTE::DIR = 1;
	MUTE::P = 1;

	utils::format("\nStart Digital MIC Reciver Version: %d.%02d\n") % (VERSION / 100) % (VERSION % 100);

	// CSI 開始
	{
		uint8_t intr_level = 0;
		if(!csi_.start(4000000, CSI::PHASE::TYPE4, intr_level)) {
			utils::format("CSI Start fail...\n");
		}
	}

	// PCM1772 初期化
	pcm_.start();

	command_.set_prompt("# ");

	MUTE::P = 0;  // MUTE 0ff

	uint8_t	n = 0;
	uint8_t t = 0;
	while(1) {
		itm_.sync();

		service_input_();

		if(input_.get_positive(INPUT_TYPE::CH_SEL)) {
			utils::format("CH_SEL\n");
		}
		if(input_.get_positive(INPUT_TYPE::IR_TEST)) {
			utils::format("IR_TEST\n");
		}
		if(input_.get_positive(INPUT_TYPE::CH_SCAN)) {
			utils::format("CH_SCAN\n");
		}

		if(input_.get_turn(INPUT_TYPE::L_M)) {
			utils::format("L_M: %d\n") % input_.get_level(INPUT_TYPE::L_M);
		}
		if(input_.get_turn(INPUT_TYPE::S_M1)) {
			utils::format("S_M1: %d\n") % input_.get_level(INPUT_TYPE::S_M1);
		}
		if(input_.get_turn(INPUT_TYPE::S_M2)) {
			utils::format("S_M2: %d\n") % input_.get_level(INPUT_TYPE::S_M2);
		}
		if(input_.get_turn(INPUT_TYPE::S_M3)) {
			utils::format("S_M3: %d\n") % input_.get_level(INPUT_TYPE::S_M3);
		}
		if(input_.get_turn(INPUT_TYPE::S_M4)) {
			utils::format("S_M4: %d\n") % input_.get_level(INPUT_TYPE::S_M4);
		}
		if(input_.get_turn(INPUT_TYPE::MSEL)) {
			utils::format("MSEL: %d\n") % input_.get_level(INPUT_TYPE::MSEL);
		}

		SEG1::decimal(0);
		SEG2::decimal(0);

		// コマンド入力と、コマンド解析
		if(command_.service()) {
			uint8_t cmdn = command_.get_words();
			if(cmdn >= 1) {
				if(command_.cmp_word(0, "help")) {
					utils::format("---\n");
				}
			}
		}

		while(uart1_.recv_length() > 0) {
			uint8_t d = uart1_.getch();
			utils::format("UART1: 0x%02X\n") % static_cast<uint16_t>(d);
		}
		while(uart2_.recv_length() > 0) {
			uint8_t d = uart2_.getch();
			utils::format("UART2: 0x%02X\n") % static_cast<uint16_t>(d);
		}


		++n;
		if(n >= 60) {
			n = 0;
			++t;
			if(t >= 100) t = 0;

			LED_M1::P = 1;
			LED_M2::P = 1;
			LED_M3::P = 1;
			LED_M4::P = 1;
		}
	}
}
