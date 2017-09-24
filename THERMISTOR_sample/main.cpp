//=====================================================================//
/*!	@file
	@brief	RL78 サーミスター・メイン
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include <cstdint>
#include "common/renesas.hpp"
#include "common/port_utils.hpp"
#include "common/fifo.hpp"
#include "common/uart_io.hpp"
#include "common/format.hpp"
#include "common/itimer.hpp"
#include "common/adc_io.hpp"
#include "common/task.hpp"
#include "chip/NTCTH.hpp"

namespace {

	typedef utils::fifo<uint8_t, 32> BUFFER;
	typedef device::uart_io<device::SAU02, device::SAU03, BUFFER, BUFFER> UART1;
	UART1	uart_;

	typedef device::itimer<uint8_t> ITM;
	ITM		itm_;

	// 最終チャネル番号＋１を設定
	typedef device::adc_io<4, utils::null_task> ADC;
	ADC 	adc_;

	// サーミスタ定義
	// A/D: 10 bits, NT103_41G, 分圧抵抗: 10K オーム、サーミスタ: ＶＣＣ側
	typedef chip::NTCTH<1023, chip::thermistor::HX103_3380, 10000, true> THMISTER;
	THMISTER thmister_;
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


	void ADC_intr(void)
	{
		adc_.task();
	}


	void ITM_intr(void)
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



	utils::format("Start RL78/G13 THERMISTOR sample\n");

	uint8_t n = 0;
	uint8_t t = 0;
	while(1) {
		itm_.sync();

		++t;
		if(t >= 60) {
			adc_.start_scan(2); // スキャン開始チャネル
			adc_.sync();  		// スキャン終了待ち

			auto v = adc_.get(2);
			v >>= 6;
			utils::format("温度： %5.2f [度]\n") % thmister_(v);
			t = 0;
		}

		++n;
		if(n >= 30) n = 0;
		device::P4.B3 = n < 10 ? false : true; 	
	}
}
