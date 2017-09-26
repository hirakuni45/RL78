//=====================================================================//
/*!	@file
	@brief	RL78/G13 グループ・セキリティ・バイト
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include <stdint.h>

//-----------------------------------------------------------------//
/*!
	@brief  セキリティーＩＤテーブル
*/
//-----------------------------------------------------------------//
const unsigned char sec_id[]  __attribute__ ((section (".sec_id"))) = {
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00
};
