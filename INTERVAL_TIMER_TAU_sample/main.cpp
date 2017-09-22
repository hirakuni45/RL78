//=====================================================================//
/*!	@file
	@brief	インターバル。タイマー出力サンプル @n
			TO01(P1-6) に５ＫＨｚの矩形波を出力（１０ＫＨｚインターバル）
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include <cstdint>
#include "common/renesas.hpp"
#include "common/port_utils.hpp"
#include "common/tau_io.hpp"

namespace {

	void wait_()
	{
		asm("nop");
	}

	device::tau_io<device::TAU01> tau_;
}


int main(int argc, char* argv[])
{
	utils::port::pullup_all();  ///< 安全の為、全ての入力をプルアップ

	device::PM4.B3 = 0;  // output

	uint32_t speed = 1000000;
	{
		uint8_t intr_level = 0;
		bool out_enable = true;
		if(!tau_.start_interval(10000, intr_level, out_enable)) {
			speed = 25000;
		}
	}

	bool f = false;
	while(1) {
		for(uint32_t i = 0; i < speed; ++i) {
			wait_();
		}
		device::P4.B3 = f;
		f = !f;
	}
}
