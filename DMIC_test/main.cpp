//=====================================================================//
/*!	@file
	@brief	デジタル・マイク制御 (R5F100ECA) 40ピン @n
			ROM: 32K, RAM: @n
			P15: green LED @n
			P16: red LED @n
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include "common/renesas.hpp"
#include "common/port_utils.hpp"
#include "common/fifo.hpp"
#include "common/uart_io.hpp"
#include "common/adc_io.hpp"
#include "common/format.hpp"
#include "common/iica_io.hpp"
#include "common/itimer.hpp"
#include "common/flash_io.hpp"
#include "common/command.hpp"

#include "sw.hpp"

namespace {

	typedef device::itimer<uint8_t> ITM;
	ITM		itm_;

	// UART1 の定義（SAU2、SAU3）
	typedef utils::fifo<uint8_t, 64> BUFFER;
	typedef device::uart_io<device::SAU00, device::SAU01, BUFFER, BUFFER> UART0;
	typedef device::uart_io<device::SAU02, device::SAU03, BUFFER, BUFFER> UART1;
	UART0	uart0_;
//	UART1	uart1_;

	typedef device::iica_io<device::IICA0> IICA;
	IICA 	iica_;

	// 最終チャネル番号＋１を設定
	typedef device::adc_io<1, utils::null_task> ADC;
	ADC 	adc_;

	typedef device::flash_io FLASH;
	FLASH	flash_;

//	utils::command<64> command_;

	// MIC 切り替え、入力定義
	typedef device::PORT<device::port_no::P12, device::bitpos::B2> MIC_SW1;
	typedef device::PORT<device::port_no::P12, device::bitpos::B1> MIC_SW2;
	utils::sw2<MIC_SW1, MIC_SW2> sw2_;

	// CH 設定、入力定義
	typedef device::PORT<device::port_no::P2,  device::bitpos::B1> CH_SW1;
	typedef device::PORT<device::port_no::P2,  device::bitpos::B0> CH_SW2;
	typedef device::PORT<device::port_no::P0,  device::bitpos::B1> CH_SW3;
	typedef device::PORT<device::port_no::P0,  device::bitpos::B0> CH_SW4;
	typedef device::PORT<device::port_no::P12, device::bitpos::B0> CH_SW5;
	utils::sw5<CH_SW1, CH_SW2, CH_SW3, CH_SW4, CH_SW5> sw5_;

	// Volume +/-
	typedef device::PORT<device::port_no::P3,  device::bitpos::B1> VOL_UP;
	typedef device::PORT<device::port_no::P7,  device::bitpos::B3> VOL_DN;
	utils::sw2<VOL_UP, VOL_DN> vol_;
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


	INTERRUPT_FUNC void ADC_intr(void)
	{
		adc_.task();
	}


	INTERRUPT_FUNC void ITM_intr(void)
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

	// UART0 の開始
	{
		uint8_t intr_level = 1;
		uart0_.start(19200, intr_level);
	}

	// IICA(I2C) の開始
	{
		uint8_t intr_level = 0;
//		if(!iica_.start(IICA::speed::fast, intr_level)) {
		if(!iica_.start(IICA::speed::standard, intr_level)) {
			utils::format("IICA start error (%d)\n") % static_cast<uint32_t>(iica_.get_last_error());
		}
	}

	// A/D の開始
	{
		device::PM2.B0 = 1;
		uint8_t intr_level = 1;  // 割り込み設定
		adc_.start(ADC::REFP::VDD, ADC::REFM::VSS, intr_level);
	}

	// data flash の開始
	{
	}

	ADPC = 0x01; // A/D input All digital port

	PM2.B3 = 0;  // POWER CTRL (OUTPUT)
	P2.B3  = 1;  // Active high (ON)

	PM1.B5 = 0;  // LED G output
	PM1.B6 = 0;  // LED R output

	sw2_.start();
	PMC12 = 0b11111110;  // setup P12_0: digital port
	sw5_.start();

	vol_.start();

///	utils::format("Start Digital MIC\n");

	uint8_t cnt = 0;
	uint8_t vol = 0;
	while(1) {
		itm_.sync();

		vol_.service();
		if(vol_.positive() & 1) {
			if(vol < 8) {
				++vol;
			}
		}
		if(vol_.positive() & 2) {
			if(vol > 0) {
				--vol;
			}
		}

		if(uart0_.recv_length() > 0) {
			char ch = uart0_.getch();

			if(ch == 'C') {  // CH-SW (0 to 31)
				auto n = sw5_.get();
				uart0_.putch((n / 10) + '0');
				uart0_.putch((n % 10) + '0');
				uart0_.putch('\n');
			} else if(ch == 'M') {  // MIC-SW (0 to 3)
				auto n = sw2_.get();
				uart0_.putch((n % 10) + '0');
				uart0_.putch('\n');
			} else if(ch == 'F') {  // 混信フラグ (0 to 1)
				uart0_.putch('0');
				uart0_.putch('\n');
			} else if(ch == 'V') {  // ボリューム値
				uart0_.putch('0' + vol);
				uart0_.putch('\n');
			}
		}

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
}
