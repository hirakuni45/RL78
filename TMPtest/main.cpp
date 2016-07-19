//=====================================================================//
/*!	@file
	@brief	簡単なテスト・テンプレート
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include <cstdint>
#include "G13/port.hpp"

namespace {
	void wait_()
	{
		asm("nop");
	}

	void (*task_)();
}

namespace device {
	static basic_rw_t<rw8_t<0xFFFA5> > CKS0;
	static basic_rw_t<rw8_t<0xFFFA6> > CKS1;
}


int main(int argc, char* argv[])
{
	task_ = wait_;

	using namespace device;

	CKS0 = 0x87;
	CKS1 = 0x86;

	PM14.B0 = 0;
	PM14.B1 = 0;

	PM4.B3 = 0;  // output

	bool f = false;
	uint32_t n = 0;
	while(1) {
		for(uint32_t i = 0; i < 1000000; ++i) {
			(*task_)();
		}
		P4.B3 = f;
		f = !f;
	}
}
