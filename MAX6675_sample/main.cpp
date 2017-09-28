//=====================================================================//
/*!	@file
	@brief	MAX6675・メイン
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
#include "common/itimer.hpp"
#include "common/format.hpp"
#include "common/delay.hpp"
#include "common/csi_io.hpp"
#include "chip/MAX6675.hpp"

namespace {

	// 送信、受信バッファの定義
	typedef utils::fifo<uint8_t, 32> BUFFER;
	// UART の定義（SAU02、SAU03）
	typedef device::uart_io<device::SAU02, device::SAU03, BUFFER, BUFFER> UART;
	UART	uart_;

	typedef device::itimer<uint8_t> ITM;
	ITM		itm_;

	// CSI(SPI) の定義、CSI20 の通信では、「SAU10」を利用、１ユニット、チャネル０
	typedef device::csi_io<device::SAU10> SPI;
	SPI		spi_;

	// MAX6675 インターフェースの定義
	typedef device::PORT<device::port_no::P5, device::bitpos::B5> MAX_CS;	///< MAX7765 選択信号

	typedef chip::MAX6675<SPI, MAX_CS> MAX6675;
	MAX6675	max6675_(spi_);
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

	// インターバル・タイマー開始
	{
		uint8_t intr_level = 1;
		itm_.start(60, intr_level);
	}

	// UART 開始
	{
		uint8_t intr_level = 1;
		uart_.start(115200, intr_level);
	}

	utils::format("Start RL78/G13 MAX6675 sample\n");

	// SPI 開始
	{
		uint8_t intr_level = 0;
		if(!spi_.start(8000000, SPI::PHASE::TYPE4, intr_level)) {
			uart_.puts("SPI Start fail...\n");
		}
	}

	// MAX6675 開始
	max6675_.start();

	PM4.B3 = 0;  // output

	uint8_t n = 0;
	uint8_t cnt = 0;
	while(1) {
		itm_.sync();

		++cnt;
		if(cnt >= 30) {
			cnt = 0;

			auto v = max6675_.get_temp();
			utils::format("%6.3f\n") % v;
		}

		++n;
		if(n >= 30) n = 0;
		P4.B3 = n < 10 ? false : true; 	
	}
}
