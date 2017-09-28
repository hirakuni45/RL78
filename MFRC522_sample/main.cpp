//=====================================================================//
/*!	@file
	@brief	MFRC522・メイン @n
																@n
			＊＊＊＊＊　電源は必ず３．３Ｖで使う事！ ＊＊＊＊＊ @n
																@n
			MFRC522(SDA)  ---> MFRC_CS (P1_0:20) @n
			MFRC522(SCK)  ---> SPI_SCK (P1_1:19) @n
			MFRC522(MOSI) ---> SPI_MOSI(P1_2:18) @n
			MFRC522(MISO) ---> SPI_MISO(P1_3:17) @n
			MFRC522(IRQ)       N.C @n
			MFRC522(GND)  ---> GND @n
			MFRC522(RES)  ---> MFRC_RES(P4_2: 1) @n
			MFRC522(3.3V) ---> 3.3V
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
#include "chip/MFRC522.hpp"

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

	// MFRC522 インターフェースの定義
	typedef device::PORT<device::port_no::P5, device::bitpos::B5> MFRC_CS;	///< 選択信号
	typedef device::PORT<device::port_no::P5, device::bitpos::B4> MFRC_RES;	///< リセット信号
	typedef chip::MFRC522<SPI, MFRC_CS, MFRC_RES> MFRC;
	MFRC	mfrc_(spi_);
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


	// MFRC522 開始
	mfrc_.start();

	// バージョンの表示
	char tmp[64];
	mfrc_.list_version(tmp, sizeof(tmp));
	utils::format("%s") % tmp;


#if 0
	// セルフ・テスト
	if(mfrc_.self_test()) {
		utils::format("MFRC522 Self test: OK\n");
	} else {
		utils::format("MFRC522 Self test: NG\n");
	}
#endif

	PM4.B3 = 0;  // output

	uint8_t n = 0;
	uint8_t idx = 0;
	while(1) {
		itm_.sync();

		// Look for new cards
		if(mfrc_.detect_card()) {
			utils::format("Card Detect !\n");
#if 0
	// Select one of the cards
	if ( ! mfrc.PICC_ReadCardSerial()) {
		return;
	}

	// Dump debug info about the card; PICC_HaltA() is automatically called
	mfrc.PICC_DumpToSerial(&(mfrc.uid));
#endif
		}

		++n;
		if(n >= 30) n = 0;
		P4.B3 = n < 10 ? false : true; 	
	}
}
