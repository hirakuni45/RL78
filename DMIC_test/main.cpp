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
#include "common/switch_man.hpp"

#include "sw.hpp"
#include "serial.hpp"

namespace {

	static const uint16_t VERSION = 20;

	typedef device::itimer<uint8_t> ITM;
	ITM		itm_;

	// UART1 の定義（SAU2、SAU3）
	typedef utils::fifo<uint8_t, 64> BUFFER;
	typedef device::uart_io<device::SAU00, device::SAU01, BUFFER, BUFFER> UART0;
	typedef device::uart_io<device::SAU02, device::SAU03, BUFFER, BUFFER> UART1;
	UART0	uart0_;
	UART1	uart1_;

	typedef device::iica_io<device::IICA0> IICA;
	IICA 	iica_;

	// 最終チャネル番号＋１を設定
	typedef device::adc_io<1, utils::null_task> ADC;
	ADC 	adc_;

	typedef device::flash_io FLASH;
	FLASH	flash_;

	// MIC 切り替え、入力定義
	typedef device::PORT<device::port_no::P12, device::bitpos::B2> MIC_SW1;
	typedef device::PORT<device::port_no::P12, device::bitpos::B1> MIC_SW2;
	typedef utils::sw2<MIC_SW1, MIC_SW2> SW2;
	SW2		sw2_;

	// CH 設定、入力定義
	typedef device::PORT<device::port_no::P2,  device::bitpos::B1> CH_SW1;
	typedef device::PORT<device::port_no::P2,  device::bitpos::B6> CH_SW2;  // P20/AIN0 --> P26 
	typedef device::PORT<device::port_no::P2,  device::bitpos::B5> CH_SW3;
	typedef device::PORT<device::port_no::P2,  device::bitpos::B4> CH_SW4;
	typedef device::PORT<device::port_no::P12, device::bitpos::B0> CH_SW5;
	typedef utils::sw5<CH_SW1, CH_SW2, CH_SW3, CH_SW4, CH_SW5> SW5;
	SW5		sw5_;

	// ボリューム +/-
	typedef device::PORT<device::port_no::P3,  device::bitpos::B1> VOL_UP;
	typedef device::PORT<device::port_no::P7,  device::bitpos::B3> VOL_DN;

	// 電源スイッチ入力 (0: OFF, 1: ON)
	typedef device::PORT<device::port_no::P2, device::bitpos::B2> P_OFF;
	// 電源制御 (0: OFF, 1: ON)
	typedef device::PORT<device::port_no::P2, device::bitpos::B3> P_CONT;

	// MUTE 制御 (0: ON, 1: OFF)
	typedef device::PORT<device::port_no::P1, device::bitpos::B7> MUTE;

	// 音質制御出力 (0: Sharp, 1: Mild)
	typedef device::PORT<device::port_no::P7, device::bitpos::B2> S_CONT;
	// 音質制御入力 (0: Sharp, 1: Mild) 
	typedef device::PORT<device::port_no::P7, device::bitpos::B1> S_SEL;

	// 10mW/1mW (0: 10mW, 1: 1mW) 
	typedef device::PORT<device::port_no::P7, device::bitpos::B0> TPS;

	// 充電制御 (0: 非充電、1: 充電)
	typedef device::PORT<device::port_no::P1, device::bitpos::B0> B_CONT;


	void init_switch_()
	{
		// ボリューム
		VOL_UP::DIR = 0;
		VOL_DN::DIR = 0;

		// 電源オフ・スイッチ (0: OFF, 1: ON)
		P_OFF::DIR = 0;
		// 電源制御 (0: OFF, 1: ON)
		P_CONT::DIR = 1;
		P_CONT::P = 0;

		// MUTE 制御 (0: ON, 1: OFF)
		MUTE::DIR = 1;
		MUTE::P = 0;

		// 音質制御出力 (0: Sharp, 1: Mild)
		S_CONT::DIR = 1;
		S_CONT::P = 0;

		// 音質制御入力 (0: Sharp, 1: Mild) 
		S_SEL::DIR = 0;

		// 10mW/1mW 切り替え (0: 10mW, 1: 1mW) 
		TPS::DIR = 0;

		// 充電制御 (0: 非充電、1: 充電)
		B_CONT::DIR = 1;
		B_CONT::P = 0;
	}


	enum class SWITCH : uint8_t {
		VOL_P,		///< ボリューム＋
		VOL_M,		///< ボリューム－
		POWER,		///< 電源センス
		SOUND,		///< 音質入力
		RF_POWER,	///< 10mW/1mW 切り替え
	};

	utils::switch_man<uint8_t, SWITCH> switch_man_;

	void service_switch_()
	{
		uint8_t lvl = 0;
		if(VOL_UP::P()) lvl |= 1 << static_cast<uint8_t>(SWITCH::VOL_P);
		if(VOL_DN::P()) lvl |= 1 << static_cast<uint8_t>(SWITCH::VOL_M);
		if(P_OFF::P()) lvl |= 1 << static_cast<uint8_t>(SWITCH::POWER);
		if(S_SEL::P()) lvl |= 1 << static_cast<uint8_t>(SWITCH::SOUND);
		if(TPS::P()) lvl |= 1 << static_cast<uint8_t>(SWITCH::RF_POWER);

		switch_man_.service(lvl);
	}

	typedef dmic::serial<UART0, SW5, SW2> SERIAL;
	SERIAL	serial_(uart0_, sw5_, sw2_);

	utils::command<64> command_;
	
}


extern "C" {

	void sci_putch(char ch)
	{
		uart1_.putch(ch);
	}


	void sci_puts(const char* str)
	{
		uart1_.puts(str);
	}


	char sci_getch(void)
	{
		return uart1_.getch();
	}


	uint16_t sci_length()
	{
		return uart1_.recv_length();
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


	INTERRUPT_FUNC void UART1_TX_intr(void)
	{
		uart1_.send_task();
	}


	INTERRUPT_FUNC void UART1_RX_intr(void)
	{
		uart1_.recv_task();
	}


	INTERRUPT_FUNC void UART1_ER_intr(void)
	{
		uart1_.error_task();
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
		uint8_t intr_level = 2;
		itm_.start(60, intr_level);
	}

	// UART1 の開始
	{
		uint8_t intr_level = 1;
		// ４０ピン版、セカンドポート選択
		bool sec = true;
		uart1_.start(115200, intr_level, sec);
	}

	ADPC = 0x01; // A/D input All digital port
	init_switch_();

	// IICA(I2C) の開始
	{
		uint8_t intr_level = 0;
///		if(!iica_.start(IICA::speed::fast, intr_level)) {
		if(!iica_.start(IICA::speed::standard, intr_level)) {
			utils::format("IICA start error (%d)\n") % static_cast<uint32_t>(iica_.get_last_error());
		}
	}

	// A/D の開始
	if(0) {
		device::PM2.B0 = 1;
		uint8_t intr_level = 1;  // 割り込み設定
		adc_.start(ADC::REFP::VDD, ADC::REFM::VSS, intr_level);
	}

	// data flash の開始
	{
	}

	PM1.B5 = 0;  // LED G output
	PM1.B6 = 0;  // LED R output

	serial_.start();

	utils::format("Start Digital MIC Version: %d.%02d\n") % (VERSION / 100) % (VERSION % 100);

	command_.set_prompt("# ");

	uint8_t cnt = 0;
	uint8_t pw_cnt = 0;
	while(1) {
		itm_.sync();

		service_switch_();
		bool volp = switch_man_.get_positive(SWITCH::VOL_P);
		bool volm = switch_man_.get_positive(SWITCH::VOL_M);
		if(volp) {
			utils::format("V+: %d\n") % serial_.get_volume();
		}
		if(volm) {
			utils::format("V-: %d\n") % serial_.get_volume();
		}

		if(switch_man_.get_turn(SWITCH::POWER)) {
			utils::format("POWER: %s\n")
				% (switch_man_.get_level(SWITCH::POWER) ? "ON" : "OFF");
			if(switch_man_.get_level(SWITCH::POWER)) {
				pw_cnt = 30;
			} else {
				P_CONT::P = 0;
			}
		}
		if(pw_cnt) {
			--pw_cnt;
			if(pw_cnt == 0) {
///				utils::format("POWER enable\n");
///				P_CONT::P = 1;
			}
		}

		if(switch_man_.get_turn(SWITCH::SOUND)) {
			utils::format("SOUND: %s\n")
				% (switch_man_.get_level(SWITCH::SOUND) ? "Sharp" : "Mild");
			S_CONT::P = switch_man_.get_level(SWITCH::SOUND);
		}

		if(switch_man_.get_turn(SWITCH::RF_POWER)) {
			utils::format("RF: %s\n")
				% (switch_man_.get_level(SWITCH::RF_POWER) ? "10mW" : "1mW");
		}

		// コマンド入力と、コマンド解析
		if(command_.service()) {
			auto n = command_.get_words();
			bool error = false;
			if(command_.cmp_word(0, "sw2")) {
				utils::format("SW2: %02b\n") % static_cast<uint16_t>(sw2_.get());
			} else if(command_.cmp_word(0, "sw5")) {
				utils::format("SW5: %05b\n") % static_cast<uint16_t>(sw5_.get());
			} else if(command_.cmp_word(0, "vol")) {
				utils::format("volume: %d\n") % serial_.get_volume();
			} else if(command_.cmp_word(0, "rf")) {
				utils::format("RF: %s\n")
					% (switch_man_.get_level(SWITCH::RF_POWER) ? "10mW" : "1mW");
			} else if(command_.cmp_word(0, "sound")) {
				utils::format("SOUND: %s\n")
					% (switch_man_.get_level(SWITCH::SOUND) ? "Sharp" : "Mild");
			} else if(command_.cmp_word(0, "power")) {
				utils::format("POWER: %s\n")
					% (switch_man_.get_level(SWITCH::POWER) ? "ON" : "OFF");
			} else if(command_.cmp_word(0, "mute")) {
				if(n >= 2) {
					if(command_.cmp_word(1, "on")) {
						MUTE::P = 0;
					} else if(command_.cmp_word(1, "off")) {
						MUTE::P = 1;
					} else {
						error = true;
					}
				} else {
					error = true;
				}
			} else if(command_.cmp_word(0, "chage")) {
				if(n >= 2) {
					if(command_.cmp_word(1, "on")) {
						B_CONT::P = 1;
					} else if(command_.cmp_word(1, "off")) {
						B_CONT::P = 0;
					} else {
						error = true;
					}
				} else {
					error = true;
				}
			} else if(command_.cmp_word(0, "help") || command_.cmp_word(0, "?")) {
				utils::format("sw2              list SW2\n");
				utils::format("sw5              list SW5\n");
				utils::format("vol              list Volume value\n");
				utils::format("rf               list RF SW\n");
				utils::format("sound            list Sound SW\n");
				utils::format("power [on, off]  list/ctrl Power SW\n");
				utils::format("mute on, off     Mute ctrl\n");
				utils::format("chage on, off    Chage ctrl\n");
			} else {
				error = true;
			}

			if(error) {
				const char* p = command_.get_command();
				if(p[0]) {
					utils::format("command error: '%s'\n") % p;
				}
			}
		}

		serial_.service(volp, volm);

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
