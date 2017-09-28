//=====================================================================//
/*!	@file
	@brief	A/D 変換のサンプル @n
			P22/ANI2(54)、P23/ANI3(53) を変換して表示
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
#include "common/format.hpp"
#include "common/itimer.hpp"
#include "common/adc_io.hpp"
#include "common/task.hpp"

namespace {

	typedef utils::fifo<uint8_t, 32> BUFFER;
	typedef device::uart_io<device::SAU02, device::SAU03, BUFFER, BUFFER> UART1;
	UART1	uart_;

	typedef device::itimer<uint8_t> ITM;
	ITM		itm_;

	// 最終チャネル番号＋１を設定
	typedef device::adc_io<4, utils::null_task> ADC;
	ADC 	adc_;
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
	utils::port::pullup_all();  ///< 安全の為、全ての入力をプルアップ

	device::PM4.B3 = 0;  // output

	{
		uint8_t intr_level = 1;
		uart_.start(115200, intr_level);
	}

	{
		uint8_t intr_level = 1;
		itm_.start(60, intr_level);
	}

	{
		device::PM2.B2 = 1;
		device::PM2.B3 = 1;
		uint8_t intr_level = 1;  // 割り込み設定
		adc_.start(ADC::REFP::VDD, ADC::REFM::VSS, intr_level);
	}

	uart_.puts("Start RL78/G13 A/D Convert sample\n");

	uint8_t n = 0;
	uint8_t t = 0;
	while(1) {
		itm_.sync();
		adc_.start_scan(2, true);  // スキャン開始チャネル、温度取得

		adc_.sync();  // スキャン終了待ち

		if(t >= 30) {
			auto val = adc_.get(2);
			val >>= 6;
#if 1
			uint32_t vol = static_cast<uint32_t>(val) * 1024 / 310;  // Vref: 3.3V とした場合の電圧
			utils::format("A/D CH2: %4.2:10y [V] (%d)\n") % vol % val;
#else
			float vol = static_cast<float>(val) * 3.3f / 1023.0f;
			utils::format("A/D CH2: %4.2f [V] (%d)\n") % vol % val;
#endif
			val = adc_.get(3);
			val >>= 6;
#if 1
			vol = static_cast<uint32_t>(val) * 1024 / 310;
			utils::format("A/D CH3: %4.2:10y [V] (%d)\n") % vol % val;
#else
			vol = static_cast<float>(val) * 3.3f / 1023.0f;
			utils::format("A/D CH3: %4.2f [V] (%d)\n") % vol % val;
#endif
			// 温度表示
			val = adc_.get_temp();
			val >>= 6;
			// 1.05V: 25.0度, 3.6mV/度
			float tv = static_cast<float>(val) * 3.3f / 1023.0f;
			float temp = (1.05f - tv) / 0.0036f + 25.0f;
			utils::format("A/D TEMP: %4.2f [K], %4.3f [V]\n") % temp % tv;

			t = 0;
		} else {
			++t;
		}

		++n;
		if(n >= 30) n = 0;
		device::P4.B3 = n < 10 ? false : true; 	
	}
}
