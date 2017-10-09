//=====================================================================//
/*!	@file
	@brief	A/D 変換による複数スイッチ検出サンプル @n
			P22/ANI2(54) ４つのスイッチ（排他）@n
                      P22                           @n
                       |                            @n
			VDD - 10K -+- 3.3K -+- 6.8K -+- 20K -+  @n
                       |        |        |       |  @n
			         RIGHT     UP      DOWN    LEFT @n
                       |        |        |       |  @n
                      VSS      VSS      VSS     VSS @n
			P23/ANI3(55)：２つのスイッチ（同時押し対応）@n
            VDD - 5K -+- 10K -+- 5K -+- VSS @n
                      |       |      |      @n
                      +-  A  -+-  B -+
	@author	平松邦仁 (hira@rvf-rc45.net)
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
#include "common/switch_man.hpp"

namespace {

	typedef utils::fifo<uint8_t, 32> BUFFER;
	typedef device::uart_io<device::SAU02, device::SAU03, BUFFER, BUFFER> UART;
	UART	uart_;

	typedef device::itimer<uint8_t> ITM;
	ITM		itm_;

	// A/D 変換の終了チャネル数を設定
	typedef device::adc_io<4, utils::null_task> ADC;
	ADC		adc_;

	enum class SWITCH : uint8_t {
		RIGHT,
		UP,
		DOWN,
		LEFT,
		A,
		B
	};

	utils::switch_man<uint8_t, SWITCH> switch_man_;
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
		uint8_t intr_level = 1;
		adc_.start(ADC::REFP::VDD, ADC::REFM::VSS, intr_level);
	}

	uart_.puts("Start RL78/G13 A/D Switch sample\n");

	uint8_t n = 0;
	while(1) {
		itm_.sync();
		adc_.start_scan(2);  // チャネル２から開始

		adc_.sync();

		// ４つのスイッチ判定（排他的）
		auto val = adc_.get(2);
		val >>= 6;   // 0 to 1023
		val += 128;  // 閾値のオフセット（1024 / 4(SWITCH) / 2）
		val /= 256;  // デコード（1024 / 4(SWITCH）

		uint8_t tmp = 0;
		if(val < 4) {
			tmp |= 1 << val;
		}

		// ２つのスイッチ判定（同時押し判定）
		val = adc_.get(3);
		val >>= 6;  // 0 to 1023
		if(val < 256) {
			tmp |= 1 << static_cast<uint8_t>(SWITCH::A);
			tmp |= 1 << static_cast<uint8_t>(SWITCH::B);
		} else if(val < 594) {
			tmp |= 1 << static_cast<uint8_t>(SWITCH::A);
		} else if(val < 722) {
			tmp |= 1 << static_cast<uint8_t>(SWITCH::B);
		}

		switch_man_.service(tmp);

		if(switch_man_.get_positive(SWITCH::UP)) {
			utils::format("UP   : on\n");
		}
		if(switch_man_.get_positive(SWITCH::DOWN)) {
			utils::format("DOWN : on\n");
		}
		if(switch_man_.get_positive(SWITCH::LEFT)) {
			utils::format("LEFT : on\n");
		}
		if(switch_man_.get_positive(SWITCH::RIGHT)) {
			utils::format("RIGHT: on\n");
		}

		if(switch_man_.get_positive(SWITCH::A)) {
			utils::format("A    : on\n");
		}
		if(switch_man_.get_positive(SWITCH::B)) {
			utils::format("B    : on\n");
		}

		if(switch_man_.get_negative(SWITCH::UP)) {
			utils::format("UP   : off\n");
		}
		if(switch_man_.get_negative(SWITCH::DOWN)) {
			utils::format("DOWN : off\n");
		}
		if(switch_man_.get_negative(SWITCH::LEFT)) {
			utils::format("LEFT : off\n");
		}
		if(switch_man_.get_negative(SWITCH::RIGHT)) {
			utils::format("RIGHT: off\n");
		}

		if(switch_man_.get_negative(SWITCH::A)) {
			utils::format("A    : off\n");
		}
		if(switch_man_.get_negative(SWITCH::B)) {
			utils::format("B    : off\n");
		}

		++n;
		if(n >= 30) {
			n = 0;
		}
		device::P4.B3 = n < 10 ? false : true; 	
	}
}
