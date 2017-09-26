//=====================================================================//
/*!	@file
	@brief	VL53L0X 距離センサー・サンプル
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2017 Kunihito Hiramatsu @n
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
#include "chip/VL53L0X.hpp"

namespace {

	device::itimer<uint8_t> itm_;

	typedef utils::fifo<uint8_t, 32> BUFFER;
	typedef device::uart_io<device::SAU02, device::SAU03, BUFFER, BUFFER> UART;
	UART	uart_;

	typedef device::iica_io<device::IICA0> IICA;
	IICA 	iica_;

	typedef chip::VL53L0X<IICA> VLX;
	VLX		vlx_(iica_);
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

	// IICA(I2C) の開始
	{
		uint8_t intr_level = 0;
		if(!iica_.start(IICA::speed::standard, intr_level)) {
			utils::format("IICA start error (%d)\n") % static_cast<uint32_t>(iica_.get_last_error());
		}
	}

	utils::format("Start RL78/G13 VL53L0X sample\n");

	// VL53L0X を開始
	if(!vlx_.start()) {
		utils::format("VL53L0X start fail\n");
	} else {
		// 20ms
		vlx_.set_measurement_timing_budget(200000);
	}

	uint8_t cnt = 0;
	uint8_t	itv = 0;
	while(1) {
		itm_.sync();

		++itv;
		if(itv >= 50) {
			auto len = vlx_.read_range_single_millimeters();
			if(vlx_.timeout_occurred()) {
				utils::format("Length: timeout\n");
			} else {
				utils::format("Length: %d [mm]\n") % (len - 50);
			}
			itv = 0;
		}

		if(cnt >= 20) {
			cnt = 0;
		}
		if(cnt < 10) P4.B3 = 1;
		else P4.B3 = 0;
		++cnt;
	}
}
