#include <stdint.h>
#include "common/io_utils.hpp"

// P43(2) に LED を繋ぐ

namespace device {

	typedef io8<0xfff04> p4_io;
	struct p4_t : public p4_io {
		using p4_io::operator =;
		using p4_io::operator ();
		using p4_io::operator |=;
		using p4_io::operator &=;

		bit_t<p4_io, 3> B3;
	};
	static p4_t P4;


	typedef io8<0xfff24> pm4_io;
	struct pm4_t : public pm4_io {
		using pm4_io::operator =;
		using pm4_io::operator ();
		using pm4_io::operator |=;
		using pm4_io::operator &=;

		bit_t<pm4_io, 3> B3;
	};
	static pm4_t PM4;

}

static void wait_()
{
	asm("nop");
}


int main(int argc, char* argv[])
{
	device::PM4.B3 = 0;  // output

	bool f = false;
	while(1) {
		for(long i = 0; i < 100000; ++i) {
			wait_();
		}
		device::P4.B3 = f;
		f = !f;
	}
}
