//=====================================================================//
/*!	@file
	@brief	ＰＷＭ出力サンプル
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include <cstdint>
#include "common/port_utils.hpp"
#include "common/tau_io.hpp"

namespace {
	void wait_()
	{
		asm("nop");
	}

	typedef device::tau_io<device::TAU00> master;
	master master_;
	device::tau_io<device::TAU01> pwm_;

	bool init_pwm_()
	{
		uint8_t intr_level = 0;
		if(!master_.start_interval(100000, intr_level)) {
			return false;
		}
		if(!pwm_.start_pwm<master::tau_type>(0, intr_level)) {
			return false;
		}
		return true;
	}
}

int main(int argc, char* argv[])
{
	utils::port::pullup_all();  ///< 安全の為、全ての入力をプルアップ

	device::PM4.B3 = 0;  // output

	uint32_t speed = 1000000;
	{
		if(!init_pwm_()) {
			speed /= 15;
		}
	}

	auto val = master_.get_value();  // マスターチャネルのカウント最大値
	pwm_.set_value(val / 4);  // PWM Duty 25%

	bool f = false;
	while(1) {
		for(uint32_t i = 0; i < speed; ++i) {
			wait_();
		}
		device::P4.B3 = f;
		f = !f;
	}
}
