//=====================================================================//
/*!	@file
	@brief	インターバル・タイマー・テスト
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include <cstdint>
#include "common/renesas.hpp"
#include "common/port_utils.hpp"
#include "common/itimer.hpp"

namespace {

	device::itimer<uint8_t> itm_;

}


extern "C" {

	void ITM_intr(void)
	{
		itm_.task();
	}
};


int main(int argc, char* argv[])
{
	using namespace device;

	utils::port::pullup_all();  ///< 安全の為、全ての入力をプルアップ

	PM4.B3 = 0;  // output

	uint8_t intr_level = 1;
	itm_.start(60, intr_level);

	uint16_t n = 0;
	while(1) {
///		utils::delay::micro_second(10);
		itm_.sync();
		++n;
		if(n >= 30) n = 0;
		P4.B3 = n < 10 ? false : true; 	
	}
}
