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
/// #include "common/uart_io.hpp"

#include "chip/SEGMENT.hpp"

namespace {

	typedef device::itimer<uint8_t> ITM;
	ITM		itm_;

#if 0
	// UART1 の定義（SAU2、SAU3）
	typedef utils::fifo<uint8_t, 64> BUFFER;
	typedef device::uart_io<device::SAU00, device::SAU01, BUFFER, BUFFER> UART0;
	typedef device::uart_io<device::SAU02, device::SAU03, BUFFER, BUFFER> UART1;
	UART0	uart0_;
//	UART1	uart1_;
#endif

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
}


extern "C" {

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

	device::PFSEG0 = 0x00;
	device::PFSEG1 = 0x00;
	device::PFSEG2 = 0x00;
	device::PFSEG3 = 0x00;
	device::PFSEG4 = 0x00;
	device::PFSEG5 = 0x00;
	device::PFSEG6 = 0x00;

	SEG1::start();
	SEG2::start();

	uint8_t	n = 0;
	uint8_t t = 0;
	while(1) {
		itm_.sync();

		SEG1::decimal(t / 10);
		SEG2::decimal(t % 10);

		++n;
		if(n >= 60) {
			n = 0;
			++t;
			if(t >= 100) t = 0;

		}
		if(n < 20) {
//			SEG1::decimal(4);
		} else {
//			SEG1::decimal(7);
		}
	}
}
