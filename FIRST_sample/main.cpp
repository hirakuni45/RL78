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
	@copyright	Copyright (C) 2016, 2021 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include "common/renesas.hpp"

namespace {

	// 吸い込みなので、三番目のパラメーターを「false」とする。
	// LED::P = 1 で、実際のポートは、０になり、LED が点灯する。
	typedef device::PORT<device::port_no::P4, device::bitpos::B3, false> LED;

}

int main(int argc, char* argv[])
{
	utils::port::pullup_all();  ///< 安全の為、全ての入力をプルアップ

	LED::DIR = 1;  // 出力設定

	bool f = true;
	while(1) {
		utils::delay::milli_second(250);  ///< 0.25 秒毎の点滅
		LED::P = f;
		f = !f;
	}
}
