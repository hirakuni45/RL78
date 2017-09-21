//=====================================================================//
/*!	@file
	@brief	BMP180/BMP280 温度、圧力センサのサンプル
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include <cstdint>
#include<cstring>
#include "G13/system.hpp"
#include "common/port_utils.hpp"
#include "common/uart_io.hpp"
#include "common/fifo.hpp"
#include "common/format.hpp"
#include "common/iica_io.hpp"
#include "common/delay.hpp"
#include "common/itimer.hpp"
#include "common/command.hpp"
#include "common/time.h"
#include "chip/BMP180.hpp"
#include "chip/BMP280.hpp"

namespace {

	typedef device::iica_io<device::IICA0> IICA;
	IICA iica_;

// どちらかを有効にする
// ※I2C アドレスは、通常同一なので、両方を同時に使う事は出来ない。
// ※BMP280 は、ピン設定で、I2C アドレスを変更可能
//	chip::BMP180<IICA> bmpx_(iica_);
	chip::BMP280<IICA> bmpx_(iica_);

	typedef utils::fifo<uint8_t, 64> buffer;

	// UART1 の定義（SAU2、SAU3）
	device::uart_io<device::SAU02, device::SAU03, buffer, buffer> uart_;

	device::itimer<uint8_t> itm_;

	utils::command<64> command_;
}


const void* ivec_[] __attribute__ ((section (".ivec"))) = {
	/*  0 */  nullptr,
	/*  1 */  nullptr,
	/*  2 */  nullptr,
	/*  3 */  nullptr,
	/*  4 */  nullptr,
	/*  5 */  nullptr,
	/*  6 */  nullptr,
	/*  7 */  nullptr,
	/*  8 */  nullptr,
	/*  9 */  nullptr,
	/* 10 */  nullptr,
	/* 11 */  nullptr,
	/* 12 */  nullptr,
	/* 13 */  nullptr,
	/* 14 */  nullptr,
	/* 15 */  nullptr,
	/* 16 */  reinterpret_cast<void*>(uart_.send_task),  // UART1-TX
	/* 17 */  reinterpret_cast<void*>(uart_.recv_task),  // UART1-RX
	/* 18 */  reinterpret_cast<void*>(uart_.error_task), // UART1-ER
	/* 19 */  nullptr,
	/* 20 */  nullptr,
	/* 21 */  nullptr,
	/* 22 */  nullptr,
	/* 23 */  nullptr,
	/* 24 */  nullptr,
	/* 25 */  nullptr,
	/* 26 */  reinterpret_cast<void*>(itm_.task),
};


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
};


namespace {

}


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
		if(!iica_.start(IICA::speed::fast, intr_level)) {
//		if(!iica_.start(IICA::speed::standard, intr_level)) {
			utils::format("IICA start error (%d)\n") % static_cast<uint32_t>(iica_.get_last_error());
		}
	}

	sci_puts("Start RL78/G13 BMP180/BMP280(I2C) sample\n");

	// BMP180/BMP280 の開始
	if(!bmpx_.start()) {
		utils::format("Stall BMP180/BMP280 start (%d)\n") % static_cast<uint32_t>(iica_.get_last_error());
	}

	command_.set_prompt("# ");

	uint8_t n = 0;
	uint8_t cnt = 0;
	while(1) {
		itm_.sync();

		if(cnt >= 20) {
			cnt = 0;
		}
		if(cnt < 10) P4.B3 = 1;
		else P4.B3 = 0;
		++cnt;

		++n;
		if(n >= 60) {
			n = 0;
			auto t = bmpx_.get_temperature();
			utils::format("Temperature: %d.%02d C\n") % (t / 100) % (t % 100);

			auto p = bmpx_.get_pressure();
			utils::format("Pressure: %d.%02d hPa\n") % (p / 100) % (p % 100);

			auto a = bmpx_.get_altitude();
			utils::format("Altitude: %3.2f m\n") % a;
		}

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
}
