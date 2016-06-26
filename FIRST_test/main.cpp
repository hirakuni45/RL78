#include <stdint.h>


static void wait_()
{
	asm("nop");
}


int main(int argc, char* argv[])
{

	for(int i = 0; i < 100; ++i) {
		wait_();
	}


}
