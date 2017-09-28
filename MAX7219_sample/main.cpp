//=====================================================================//
/*!	@file
	@brief	MAX7219・メイン
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
#include "chip/MAX7219.hpp"

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

	// MAX7219 インターフェースの定義
	typedef device::PORT<device::port_no::P5, device::bitpos::B5> MAX_CS;	///< MAX7219 選択信号
	chip::MAX7219<SPI, MAX_CS> max7219_(spi_);
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

	utils::format("Start RL78/G13 MAX7219 sample\n");

	// SPI 開始
	{
		uint8_t intr_level = 0;
		if(!spi_.start(8000000, SPI::PHASE::TYPE4, intr_level)) {
			uart_.puts("SPI Start fail...\n");
		}
	}

	// MAX7219 開始
	max7219_.start();

	for(uint8_t i = 0; i < 8; ++i) {
		max7219_.set_cha(i, '-');
	}

	PM4.B3 = 0;  // output

	uint8_t n = 0;
	uint8_t idx = 0;
	while(1) {
		itm_.sync();

		max7219_.service();
		max7219_.set_intensity(0);

		if(sci_length()) {
			if(idx > 7) {
				max7219_.shift_top();
				idx = 7;
			}
			char ch = sci_getch();
			sci_putch(ch);
			max7219_.set_cha(idx ^ 7, ch);
			++idx;
		}

		++n;
		if(n >= 30) n = 0;
		P4.B3 = n < 10 ? false : true; 	
	}
}
