//=====================================================================//
/*!	@file
	@brief	デジタル・マイク制御 RL78/G13 (R5F100ECA) 40ピン @n
			ROM: 32K, RAM: 2K @n
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
#include "chip/TLV320ADC3001.hpp"

#include "sw.hpp"
// #include "serial.hpp"

// #define BETA_VERSION

namespace {

#ifdef BETA_VERSION
	static const uint16_t VERSION = 30;
#else
	static const uint16_t VERSION = 33;
#endif

	typedef device::itimer<uint8_t> ITM;
	ITM		itm_;

	// UART1 の定義（SAU2、SAU3）
	typedef utils::fifo<uint8_t, 32> BUFFER32;
	typedef device::uart_io<device::SAU00, device::SAU01, BUFFER32, BUFFER32> UART0;
	typedef utils::fifo<uint8_t, 64> BUFFER64;
	typedef device::uart_io<device::SAU02, device::SAU03, BUFFER64, BUFFER64> UART1;
	UART0	uart0_;
	UART1	uart1_;

	typedef device::iica_io<device::IICA0> IICA;
	IICA 	iica_;

	typedef chip::TLV320ADC3001<IICA> TI_ADC;
	TI_ADC	ti_adc_(iica_);

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

	// TI ADC RESET
	typedef device::PORT<device::port_no::P6, device::bitpos::B2> TRESET;

	// LED Green
	typedef device::PORT<device::port_no::P1, device::bitpos::B5> LED_G;
	// LED Red
	typedef device::PORT<device::port_no::P1, device::bitpos::B6> LED_R;

	void init_switch_()
	{
		// ボリューム
		VOL_UP::DIR = 0;
		VOL_DN::DIR = 0;

		sw2_.start();
		device::PMC12 = 0b11111110;  // setup P12_0: digital port
		sw5_.start();

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

		TRESET::DIR = 1;
		TRESET::PU = 1;
		TRESET::P = 0;
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

		if(!VOL_UP::P()) lvl |= 1 << static_cast<uint8_t>(SWITCH::VOL_P);
		if(!VOL_DN::P()) lvl |= 1 << static_cast<uint8_t>(SWITCH::VOL_M);
		if(P_OFF::P()) lvl |= 1 << static_cast<uint8_t>(SWITCH::POWER);
		if(S_SEL::P()) lvl |= 1 << static_cast<uint8_t>(SWITCH::SOUND);
		if(TPS::P()) lvl |= 1 << static_cast<uint8_t>(SWITCH::RF_POWER);

		switch_man_.service(lvl);
	}

	utils::command<64> command_;
	
	static const uint8_t volume_limit_ = 100;
	/// setup > cycle の関係が条件
	static const uint8_t repeat_setup_ = 45;  ///< リピート開始までの時間
	static const uint8_t repeat_cycle_ = 10;  ///< リピートサイクル
	uint8_t		volume_ = 0;
	uint8_t		repeat_ = 0;
///	uint8_t		sw5_val_ = 0;
///	uint8_t		sw2_val_ = 0;

	void serial_(bool outreq)
	{
		bool volp = switch_man_.get_level(SWITCH::VOL_P);
		bool volm = switch_man_.get_level(SWITCH::VOL_M);
		if(volp || volm) {
			if(repeat_ >= repeat_setup_) {
				repeat_ = repeat_setup_ - repeat_cycle_;
			} else {
				++repeat_;
				volp = switch_man_.get_positive(SWITCH::VOL_P);
				volm = switch_man_.get_positive(SWITCH::VOL_M);
			}
		} else {
			repeat_ = 0;
		}

		uint8_t vol = volume_;
		if(volp) {
			if(vol < (volume_limit_ - 1)) {
				++vol;
			}
			utils::format("V+: %d\n") % static_cast<uint16_t>(vol);
		}
		if(volm) {
			if(vol > 0) {
				--vol;
			}
			utils::format("V-: %d\n") % static_cast<uint16_t>(vol);
		}
		if(vol != volume_) {
			uart0_.putch('V');
			uart0_.putch((vol / 10) + '0');
			uart0_.putch((vol % 10) + '0');
			uart0_.putch('\n');
			volume_ = vol;
		}

		{
			auto v = sw5_.get();
///			if(outreq || sw5_val_ != v) {
			if(outreq) {
				uart0_.putch('C');
				uart0_.putch((v / 10) + '0');
				uart0_.putch((v % 10) + '0');
				uart0_.putch('\n');
				utils::format("SW5: %d\n") % static_cast<uint16_t>(v);
///				sw5_val_ = v;
			}
		}
		{
			auto v = sw2_.get() & 1;
///			if(outreq || sw2_val_ != v) {
			if(outreq) {
				uart0_.putch('M');
				uart0_.putch(v + '1');
				uart0_.putch('\n');
				utils::format("SW2: %d\n") % static_cast<uint16_t>(v);
///				sw2_val_ = v;
			}
		}
	}

	uint8_t	start_i2c_ = 0;
	uint8_t reset_setup_ = 0;
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

	// UART0 の開始
	{
		uint8_t intr_level = 1;
		uart0_.start(19200, intr_level);
	}

	// UART1 の開始
	{
		uint8_t intr_level = 1;
		// ４０ピン版、セカンドポート選択
		bool sec = true;
		uart1_.start(115200, intr_level, sec);
	}

	ADPC = 0b0010; // A/D AIN0, other digital port
	init_switch_();

	LED_G::DIR = 1;
	LED_G::P = 1;  // LED off
	LED_R::DIR = 1;
	LED_R::P = 1;  // LED off

	// A/D の開始
	{
		device::PM2.B0 = 1;
		uint8_t intr_level = 1;  // 割り込み設定
		adc_.start(ADC::REFP::VDD, ADC::REFM::VSS, intr_level);
	}

#ifdef BETA_VERSION
	utils::format("\nStart Digital MIC Version: %d.%02d / BETA-01\n")
		% (VERSION / 100) % (VERSION % 100);
#else
	utils::format("\nStart Digital MIC Version: %d.%02d\n")
		% (VERSION / 100) % (VERSION % 100);
#endif

	service_switch_();
	service_switch_();
	bool pon_first = false;
	bool poff_first = false;
	if(switch_man_.get_level(SWITCH::POWER)) {
		pon_first = true;
	} else {
		poff_first = true;
	}

	// data flash の開始
	{
	}

	command_.set_prompt("# ");

	bool power = false;
	uint8_t pw_off_cnt = 0;
	uint8_t serial_delay = 0;
	uint8_t mute_delay = 0;
	while(1) {
		itm_.sync();

		adc_.start_scan(0);  // AIN00

		service_switch_();

		if(pon_first || switch_man_.get_positive(SWITCH::POWER)) {
			utils::format("POWER: ON\n");
			TRESET::P = 0;
			P_CONT::P = 1;  // power switch online
			utils::format("P_CONT: online\n");
			start_i2c_ = 20;
			pw_off_cnt = 0;
			serial_delay = 30;  // 500ms
			mute_delay = 42;  // 電源 ON 時は 700ms 後 MUTE
			pon_first = false;
			power = true;
		}
		if(poff_first || switch_man_.get_negative(SWITCH::POWER)) {
			utils::format("POWER: OFF\n");
			pw_off_cnt = 30;
			serial_delay = 0;
			mute_delay = 1;  // 電源 OFF 時は即時 MUTE
			poff_first = false;
			power = false;
		}

		if(serial_delay > 0) {
			--serial_delay;
			if(serial_delay == 0) {
				serial_(true);
			}
		}

		if(pw_off_cnt > 0) {
			--pw_off_cnt;
			if(pw_off_cnt == 0) {
				utils::format("P_CONT: offline\n");
				P_CONT::P = 0;  // power switch offline
			}
		}

		if(mute_delay > 0) {
			--mute_delay;
			if(mute_delay == 0) {
				MUTE::P = power;
			}
		}

		if(power) {
			if(switch_man_.get_turn(SWITCH::SOUND)) {
				utils::format("SOUND: %s\n")
					% (switch_man_.get_level(SWITCH::SOUND) ? "Sharp" : "Mild");
			}
			auto f = switch_man_.get_level(SWITCH::SOUND);
			S_CONT::P = f;
			if(f) {  // H: 緑 LED、L: 緑、赤 LED
				LED_R::P = 1;
				LED_G::P = 0;
			} else {
				LED_R::P = 0;
				LED_G::P = 0;
			}
		} else {  // 電源 OFF 時は即時消灯
			LED_R::P = 1;
			LED_G::P = 1;
		}

		if(power && switch_man_.get_turn(SWITCH::RF_POWER)) {
			utils::format("RF: %s\n")
				% (switch_man_.get_level(SWITCH::RF_POWER) ? "10mW" : "1mW");
		}

		if(start_i2c_ > 0) {
			--start_i2c_;
			if(start_i2c_ == 0) {
				utils::format("Reset TI (H) OK\n");
				TRESET::P = 1;  // TI Reset open
				reset_setup_ = 10;
			}
		}
		if(reset_setup_ > 0) {
			--reset_setup_;
			if(reset_setup_ == 0) {
				// IICA(I2C) の開始
				uint8_t intr_level = 0;
///				if(!iica_.start(IICA::speed::fast, intr_level)) {
				if(!iica_.start(IICA::speed::standard, intr_level)) {
					utils::format("I2C start error (%d)\n") % static_cast<uint32_t>(iica_.get_last_error());
				} else {
					utils::format("I2C start: OK\n");
					auto f = ti_adc_.start(TI_ADC::INSEL::IN2R_3R, TI_ADC::INSEL::IN2R_3R,
								TI_ADC::INF::LJF_16_SLAVE);
					utils::format("TLV320ADC3001 LJF/16Bits/Slave: %s\n") % (f ? "OK" : "NG");
				}
			}
		}

		adc_.sync();  // スキャン終了待ち
		auto adi = adc_.get(0) >> 6;

		// コマンド入力と、コマンド解析
		if(command_.service()) {
			auto n = command_.get_words();
			bool error = false;
			if(command_.cmp_word(0, "sw2")) {
				utils::format("SW2: %02b\n") % static_cast<uint16_t>(sw2_.get());
			} else if(command_.cmp_word(0, "sw5")) {
				utils::format("SW5: %05b\n") % static_cast<uint16_t>(sw5_.get());
			} else if(command_.cmp_word(0, "vol")) {
				utils::format("volume: %d\n") % static_cast<uint16_t>(volume_);
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
			} else if(command_.cmp_word(0, "volt")) {
				// Vref: 310: 3.3V とした場合の電圧
//				uint32_t vol = static_cast<uint32_t>(adi) * 1024 / 310;
				uint32_t vol = static_cast<uint32_t>(adi) * 1024 / 155;
				utils::format("VOLTAGE: %4.2:10y [V]\n") % vol;
			} else if(command_.cmp_word(0, "help") || command_.cmp_word(0, "?")) {
				utils::format("sw2            list SW2\n");
				utils::format("sw5            list SW5\n");
				utils::format("vol            list Volume value\n");
				utils::format("rf             list RF SW\n");
				utils::format("sound          list Sound SW\n");
				utils::format("power          list Power SW\n");
				utils::format("mute on, off   Mute ctrl\n");
				utils::format("chage on, off  Chage ctrl\n");
				utils::format("volt           list volatage\n");
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
	}
}
