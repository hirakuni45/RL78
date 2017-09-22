//=====================================================================//
/*!	@file
	@brief	R78 ハードウェアー初期化
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include <common/vect.h>

extern void rl78_run_preinit_array(void);
extern void rl78_run_init_array(void);
extern void rl78_run_fini_array(void);
extern void rl78_run_ctor(void);

void init()
{
	rl78_run_ctor();

	rl78_run_preinit_array();
	rl78_run_init_array();
	rl78_run_fini_array();

	ei();
}
