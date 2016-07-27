//=====================================================================//
/*!	@file
	@brief	DS3231 RTC のテスト
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include <cstdint>
#include<cstring>
#include "G13/system.hpp"
#include "G13/port.hpp"
#include "common/uart_io.hpp"
#include "common/fifo.hpp"
#include "common/format.hpp"
#include "common/iica_io.hpp"
#include "common/delay.hpp"
#include "common/itimer.hpp"
#include "common/command.hpp"
#include "common/time.h"
#include "common/ds3231_io.hpp"

namespace {

	typedef device::iica_io<device::IICA0> IICA;
	IICA iica0_;

	device::ds3231_io<IICA> rtc_(iica0_);

	typedef utils::fifo<128> buffer;

	device::uart_io<device::SAU00, device::SAU01, buffer, buffer> uart0_;

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
	/* 13 */  reinterpret_cast<void*>(uart0_.send_task),
	/* 14 */  reinterpret_cast<void*>(uart0_.recv_task),
	/* 15 */  reinterpret_cast<void*>(uart0_.error_task),
	/* 16 */  nullptr,
	/* 17 */  nullptr,
	/* 18 */  nullptr,
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
		uart0_.putch(ch);
	}

	void sci_puts(const char* str)
	{
		uart0_.puts(str);
	}

	char sci_getch(void)
	{
		return uart0_.getch();
	}

	uint16_t sci_length()
	{
		return uart0_.recv_length();
	}
};


static const char* wday_[] = {
	"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" 
};


static const char* mon_[] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};


namespace {

	time_t get_time_()
	{
		time_t t = 0;
		if(!rtc_.get_time(t)) {
			utils::format("Stall RTC read (%d)\n") % static_cast<uint32_t>(iica0_.get_last_error());
		}
		return t;
	}


	void disp_time_(time_t t) {
		struct tm *m = gmtime(&t);
		utils::format("%s %s %d %02d:%02d:%02d  %4d\n")
			% wday_[m->tm_wday]
			% mon_[m->tm_mon]
			% static_cast<uint32_t>(m->tm_mday)
			% static_cast<uint32_t>(m->tm_hour)
			% static_cast<uint32_t>(m->tm_min)
			% static_cast<uint32_t>(m->tm_sec)
			% static_cast<uint32_t>(m->tm_year + 1900);
	}


	bool check_key_word_(uint8_t idx, const char* key)
	{
		char buff[12];
		if(command_.get_word(idx, sizeof(buff), buff)) {
			if(std::strcmp(buff, key) == 0) {
				return true;
			}				
		}
		return false;
	}


	const char* get_dec_(const char* p, char tmch, int& value) {
		int v = 0;
		char ch;
		while((ch = *p) != 0) {
			++p;
			if(ch == tmch) {
				break;
			} else if(ch >= '0' && ch <= '9') {
				v *= 10;
				v += ch - '0';
			} else {
				return nullptr;
			}
		}
		value = v;
		return p;
	}


	void set_time_date_()
	{
		time_t t = get_time_();
		if(t == 0) return;

		struct tm *m = gmtime(&t);
		bool err = false;
		if(command_.get_words() == 3) {
			char buff[12];
			if(command_.get_word(1, sizeof(buff), buff)) {
				const char* p = buff;
				int vs[3];
				uint8_t i;
				for(i = 0; i < 3; ++i) {
					p = get_dec_(p, '/', vs[i]);
					if(p == nullptr) {
						break;
					}
				}
				if(p != nullptr && p[0] == 0 && i == 3) {
					if(vs[0] >= 1900 && vs[0] < 2100) m->tm_year = vs[0] - 1900;
					if(vs[1] >= 1 && vs[1] <= 12) m->tm_mon = vs[1] - 1;
					if(vs[2] >= 1 && vs[2] <= 31) m->tm_mday = vs[2];		
				} else {
					err = true;
				}
			}

			if(command_.get_word(2, sizeof(buff), buff)) {
				const char* p = buff;
				int vs[3];
				uint8_t i;
				for(i = 0; i < 3; ++i) {
					p = get_dec_(p, ':', vs[i]);
					if(p == nullptr) {
						break;
					}
				}
				if(p != nullptr && p[0] == 0 && (i == 2 || i == 3)) {
					if(vs[0] >= 0 && vs[0] < 24) m->tm_hour = vs[0];
					if(vs[1] >= 0 && vs[1] < 60) m->tm_min = vs[1];
					if(i == 3 && vs[2] >= 0 && vs[2] < 60) m->tm_sec = vs[2];
					else m->tm_sec = 0;
				} else {
					err = true;
				}
			}
		}

		if(err) {
			sci_puts("Can't analize Time/Date input.\n");
			return;
		}

		time_t tt = mktime(m);
		if(!rtc_.set_time(tt)) {
			sci_puts("Stall RTC write...\n");
		}
	}
}


int main(int argc, char* argv[])
{
	using namespace device;

	PM4.B3 = 0;  // output

	// itimer の開始
	{
		uint8_t intr_level = 1;
		itm_.start(60, intr_level);
	}

	// UART0 の開始
	{
		uint8_t intr_level = 1;
		uart0_.start(115200, intr_level);
	}

	// IICA(I2C) の開始
	{
		uint8_t intr_level = 0;
//		if(!iica0_.start(IICA::speed::fast, intr_level)) {
		if(!iica0_.start(IICA::speed::standard, intr_level)) {
			sci_puts("IICA start error\n");
		}
	}

	sci_puts("Start RL78/G13 DS3231(I2C-RTC) sample\n");

	// DS3231(RTC) の開始
	if(!rtc_.start()) {
		utils::format("Stall RTC start (%d)\n") % static_cast<uint32_t>(iica0_.get_last_error());
	}

	command_.set_prompt("# ");

	uint8_t cnt = 0;
	while(1) {
		itm_.sync();

		if(cnt >= 20) {
			cnt = 0;
		}
		if(cnt < 10) P4.B3 = 1;
		else P4.B3 = 0;
		++cnt;

		// コマンド入力と、コマンド解析
		if(command_.service()) {
			uint8_t cmdn = command_.get_words();
			if(cmdn >= 1) {
				if(check_key_word_(0, "date")) {
					if(cmdn == 1) {
						time_t t = get_time_();
						if(t != 0) {
							disp_time_(t);
						}
					} else {
						set_time_date_();
					}
				} else if(check_key_word_(0, "help")) {
					sci_puts("date\n");
					sci_puts("date yyyy/mm/dd hh:mm[:ss]\n");
				} else {
					char buff[12];
					if(command_.get_word(0, sizeof(buff), buff)) {
						sci_puts("Command error: ");
						sci_puts(buff);
						sci_putch('\n');
					}
				}
			}
		}
	}
}
