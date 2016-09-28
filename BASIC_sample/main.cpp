//=====================================================================//
/*!	@file
	@brief	サウンド生成サンプル @n
			P16/TO01(40) から、左チャネル @n
			P17/TO02(39) から、右チャネル
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include <cstdint>
#include "common/port_utils.hpp"
#include "common/fifo.hpp"
#include "common/uart_io.hpp"
#include "common/itimer.hpp"
#include "common/format.hpp"
#include "common/command.hpp"

namespace {

	// 送信、受信バッファの定義
	typedef utils::fifo<uint32_t, 32> buffer;
	// UART の定義（SAU02、SAU03）
	device::uart_io<device::SAU02, device::SAU03, buffer, buffer> uart_;

	// インターバル・タイマー
	device::itimer<uint8_t> itm_;

	utils::command<64> command_;
}


const void* ivec_[] __attribute__ ((section (".ivec"))) = {
	/*  0 WDTI            */  nullptr,
	/*  1 LVI             */  nullptr,
	/*  2 P0              */  nullptr,
	/*  3 P1              */  nullptr,
	/*  4 P2              */  nullptr,
	/*  5 P3              */  nullptr,
	/*  6 P4              */  nullptr,
	/*  7 P5              */  nullptr,
	/*  8 ST2/CSI20/IIC20 */  nullptr,
	/*  9 SR2/CSI21/IIC21 */  nullptr,
	/* 10 SRE2/TM11H      */  nullptr,
	/* 11 DMA0            */  nullptr,
	/* 12 DMA1            */  nullptr,
	/* 13 ST0/CSI00/IIC00 */  nullptr,
	/* 14 SR0/CSI01/IIC01 */  nullptr,
	/* 15 SRE0/TM01H      */  nullptr,
	/* 16 ST1/CSI10/IIC10 */  reinterpret_cast<void*>(uart_.send_task),
	/* 17 SR1/CSI11/IIC11 */  reinterpret_cast<void*>(uart_.recv_task),
	/* 18 SRE1/TM03H      */  reinterpret_cast<void*>(uart_.error_task),
	/* 19 IICA0           */  nullptr,
	/* 20 TM00            */  nullptr,
	/* 21 TM01            */  nullptr,
	/* 22 TM02            */  nullptr,
	/* 23 TM03            */  nullptr,
	/* 24 AD              */  nullptr,
	/* 25 RTC             */  nullptr,
	/* 26 IT              */  reinterpret_cast<void*>(itm_.task),
	/* 27 KR              */  nullptr,
	/* 28 ST3/CSI30/IIC30 */  nullptr,
	/* 29 SR3/CSI31/IIC31 */  nullptr,
	/* 30 TM13            */  nullptr,
	/* 31 TM04            */  nullptr,
	/* 32 TM05            */  nullptr,
	/* 33 TM06            */  nullptr,
	/* 34 TM07            */  nullptr,
	/* 35 P6              */  nullptr,
	/* 36 P7              */  nullptr,
	/* 37 P8              */  nullptr,
	/* 38 P9              */  nullptr,
	/* 39 P10             */  nullptr,
	/* 40 P11             */  nullptr,
	/* 41 TM10            */  nullptr,
	/* 42 TM11            */  nullptr,
	/* 43 TM12            */  nullptr,
	/* 44 SRE3/TM13H      */  nullptr,
	/* 45 MD              */  nullptr,
	/* 46 IICA1           */  nullptr,
	/* 47 FL              */  nullptr,
	/* 48 DMA2            */  nullptr,
	/* 49 DMA3            */  nullptr,
	/* 50 TM14            */  nullptr,
	/* 51 TM15            */  nullptr,
	/* 52 TM16            */  nullptr,
	/* 53 TM17            */  nullptr,
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
	utils::port::pullup_all();  ///< 安全の為、全ての入力をプルアップ

	device::PM4.B3 = 0;  // output

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

	uart_.puts("Start RL78/G13 BASIC sample\n");

	command_.set_prompt("# ");

	uint8_t n = 0;
	while(1) {
		itm_.sync();

		// コマンド入力と、コマンド解析
		if(command_.service()) {
		}

		++n;
		if(n >= 30) {
			n = 0;
		}
		device::P4.B3 = n < 10 ? false : true;
	}
}
