//=====================================================================//
/*!	@file
	@brief	LED 点滅 @n
			LED は、P43(2) に接続 @n
			ex: Vcc = LED(A), P43 = LED(K) 吸い込み @n
			    ※必ず直列に適等な抵抗を入れて電流を 3mA 程度に調整する。
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include <stdint.h>
#include "common/io_utils.hpp"

namespace device {

	typedef rw8_t<0xfff04> p4_rw;
	struct p4_t : public p4_rw {
		using p4_rw::operator =;
		using p4_rw::operator ();
		using p4_rw::operator |=;
		using p4_rw::operator &=;

		bit_rw_t<p4_rw, 3> B3;
	};
	static p4_t P4;


	typedef rw8_t<0xfff24> pm4_rw;
	struct pm4_t : public pm4_rw {
		using pm4_rw::operator =;
		using pm4_rw::operator ();
		using pm4_rw::operator |=;
		using pm4_rw::operator &=;

		bit_rw_t<pm4_rw, 3> B3;
	};
	static pm4_t PM4;

}


namespace {
	void wait_()
	{
		asm("nop");
	}
}


int main(int argc, char* argv[])
{
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
