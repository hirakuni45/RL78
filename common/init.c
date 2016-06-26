#include <stdint.h>

extern void rl78_run_preinit_array(void);
extern void rl78_run_init_array(void);
extern void rl78_run_fini_array(void);

void init()
{
	rl78_run_preinit_array();
	rl78_run_init_array();
	rl78_run_fini_array();
}
