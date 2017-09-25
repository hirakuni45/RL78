//=====================================================================//
/*!	@file
	@brief	ソフト・ディレイ・サンプル @n
			P4_3 を 10us（マイクロ秒）毎に反転する。(50KHz)
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include <cstdint>
#include "common/port_utils.hpp"
#include "common/delay.hpp"

int main(int argc, char* argv[])
{
	using namespace device;

	utils::port::pullup_all();  ///< 安全の為、全ての入力をプルアップ

	PM4.B3 = 0;  // output

	while(1) {
		utils::delay::micro_second(10);
//		utils::delay::milli_second(500);
		P4.B3 = !P4.B3();
	}
}
