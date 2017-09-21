//=====================================================================//
/*!	@file
	@brief	LED 点滅サンプル @n
			LED は、P43(2) に接続 @n
			ex: Vcc = LED(A), P43 = LED(K) 吸い込み @n
			    ※必ず直列に適等な抵抗を入れて電流を調整する。@n
				・3.3V: 赤色 LED(VF: 1.7V)、1mA、1.5K @n
				・5.0V: 赤色 LED(VF: 1.7V)、1mA、3.3K @n
				・5.0V: 赤色 LED(VF: 1.7V)、3mA、1.1K
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include <cstdint>
#include "common/port_utils.hpp"

namespace {
	void wait_()
	{
		asm("nop");
	}
}

int main(int argc, char* argv[])
{
	utils::port::pullup_all();  ///< 安全の為、全ての入力をプルアップ

	device::PM4.B3 = 0;  // output

	bool f = false;
	while(1) {
		for(uint32_t i = 0; i < 100000; ++i) {
			wait_();
		}
		device::P4.B3 = f;
		f = !f;
	}
}
