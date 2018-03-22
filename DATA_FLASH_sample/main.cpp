//=====================================================================//
/*!	@file
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include <cstdint>
#include "common/renesas.hpp"
#include "common/port_utils.hpp"
#include "common/fifo.hpp"
#include "common/uart_io.hpp"
#include "common/itimer.hpp"
#include "common/format.hpp"
#include "common/input.hpp"
#include "common/flash_io.hpp"
#include "common/command.hpp"

namespace {

	// 送信、受信バッファの定義
	typedef utils::fifo<uint8_t, 32> BUFFER;
	// UART の定義（SAU2、SAU3）
	typedef device::uart_io<device::SAU02, device::SAU03, BUFFER, BUFFER> UART1;
	UART1	uart_;

	typedef device::itimer<uint8_t> ITM;
	ITM		itm_;

	typedef device::flash_io FLASH;
	FLASH	flash_;

	utils::command<64> command_;	
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


	INTERRUPT_FUNC void ITM_intr(void)
	{
		itm_.task();
	}
};

namespace {

	uint16_t get_hexadecimal_(const char* str)
	{
		uint16_t v = 0;
		char ch;
		while((ch = *str++) != 0) {
			v <<= 4;
			if(ch >= '0' && ch <= '9') v |= ch - '0';
			else if(ch >= 'A' && ch <= 'F') v |= ch - 'A' + 10;
			else if(ch >= 'a' && ch <= 'f') v |= ch - 'a' + 10;
			else return 0;
		}
		return v;
	}


	void dump_(uint16_t org, uint16_t len)
	{
		bool adr = true;
		for(uint16_t i = 0; i < len; ++i) {
			if(adr) {
				utils::format("0x%04X:") % static_cast<uint32_t>(org);
				adr = false;
			}
			uint8_t dat = flash_.read(org);
			utils::format(" %02X%") % static_cast<uint32_t>(dat);
			uint16_t a = org;
			++org;
			if(((a ^ org) & 16) != 0) {
				utils::format("\n");
				adr = true;
			}
		}
		if(!adr) {
			utils::format("\n");
		}
	}
}


int main(int argc, char* argv[])
{
	using namespace device;

	uint8_t resf = RESF();

	utils::port::pullup_all();  ///< 安全の為、全ての入力をプルアップ

	utils::delay::milli_second(500);

	{
		uint8_t intr_level = 1;
		itm_.start(60, intr_level);
	}

	{
		uint8_t intr_level = 1;
		uart_.start(115200, intr_level);
	}

	utils::format("Start RL78/G13 Data-Flash sample\n");

	PM4.B3 = 0;  // output
	P4.B3 = 0;

	if(resf != 0) {
		utils::format("RESF: %02X\n") % static_cast<uint16_t>(resf);
	}

	{
		if(flash_.start()) {
			utils::format("Data Flash Start (FDL T4): OK\n");
			utils::format("Data Flash Size: 0x%04X\n") % flash_.size();
		} else {
			utils::format("Data Flash Start (FDL T4): NG\n");
		}
	}

	command_.set_prompt("# ");

	uint8_t cnt = 0;
	while(1) {
		itm_.sync();

		// コマンド入力と、コマンド解析
		if(command_.service()) {
			auto n = command_.get_words();
			if(command_.cmp_word(0, "erase") && n >= 2) {
				bool f = false;
				char buff[16];
				if(command_.get_word(1, sizeof(buff), buff)) {
					int bank = 0;
					if((utils::input("%d", buff) % bank).status()) {
						if(static_cast<uint32_t>(bank) < flash_.bank()) {
							f = flash_.erase(bank);
							if(!f) {
								utils::format("Erase error: bank %d\n") % bank;
								f = true;
							} else {
								f = flash_.erase_check(bank);
								if(!f) {
									utils::format("Erase Check error: bank %d\n") % bank;
									f = true;
								}
							}
						}
					}
				}
				if(!f) {
					utils::format("Erase param error: '%s'\n") % buff;
				}
			} else if(command_.cmp_word(0, "r") && n >= 2) {
				bool f = true;
				uint16_t org = 0;
				char buff[8];
				if(command_.get_word(1, sizeof(buff), buff)) {
					org = get_hexadecimal_(buff);
				} else {
					f = false;
				}
				uint16_t len = 1;
				if(n >= 3) {
					if(command_.get_word(2, sizeof(buff), buff)) {
						len = get_hexadecimal_(buff);
					} else {
						f = false;
					}
				}
				if(f) {
					dump_(org, len);
				} else {
					utils::format("Read param error: %s\n") % command_.get_command();
				}
			} else if(command_.cmp_word(0, "write") && n >= 3) {
				char buff[8];
				if(command_.get_word(1, sizeof(buff), buff)) {
					uint16_t org = get_hexadecimal_(buff);
					for(uint8_t i = 2; i < n; ++i) {
						if(command_.get_word(i, sizeof(buff), buff)) {
							uint16_t data = get_hexadecimal_(buff);
							if(!flash_.write(org, data)) {
								utils::format("Write error: %04X: %02X\n")
									% static_cast<uint32_t>(org) % static_cast<uint32_t>(data);
							}
							++org;
						}
					}
				}
			} else if(command_.cmp_word(0, "?") || command_.cmp_word(0, "help")) {
				utils::format("erase [bank] (erase 0 to %d)\n") % flash_.bank();
				utils::format("r org [end] (read)\n");
				utils::format("write org data... (write)\n");
			} else {
				const char* p = command_.get_command();
				if(p[0]) {
					utils::format("command error: '%s'\n") % p;
				}
			}
		}

		++cnt;
		if(cnt >= 20) {
			P4.B3 = !P4.B3();
			cnt = 0;
		}
	}
}
