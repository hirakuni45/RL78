//=====================================================================//
/*!	@file
	@brief	デジタル・マイク・レシーバー
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include <cstdint>
#include "common/renesas.hpp"
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
