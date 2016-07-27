//=====================================================================//
/*!	@file
	@brief	ソフト・ディレイ・サンプル @n
			P4_3 を 10us（マイクロ秒）毎に反転する。(50KHz)
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include <cstdint>
#include "G13/port.hpp"
#include "common/delay.hpp"

namespace {

}

int main(int argc, char* argv[])
{
	using namespace device;

	PM4.B3 = 0;  // output

	while(1) {
		utils::delay::micro_second(10);
		P4.B3 = !P4.B3();
	}
}
