//=====================================================================//
/*!	@file
	@brief	RL78/G13 グループ・オプション・バイト
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016, 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include <stdint.h>

// 起動時の動作周波数を指定出来る。
// どれかを１つだけ有効にする事。
#define START_FREQ	0xE8	///< 32MHz
// #define START_FREQ	0xE0	///< 24MHz
// #define START_FREQ	0xE9	///< 16MHz
// #define START_FREQ	0xE1	///< 12MHz
// #define START_FREQ	0xEA	///<  8MHz
// #define START_FREQ	0xE2	///<  6MHz
// #define START_FREQ	0xEB	///<  4MHz
// #define START_FREQ	0xE3	///<  3MHz
// #define START_FREQ	0xEC	///<  2MHz
// #define START_FREQ	0xED	///<  1MHz

//-----------------------------------------------------------------//
/*!
	@brief  オプション・バイト設定
*/
//-----------------------------------------------------------------//
const uint8_t opt_[] __attribute__ ((section (".opt"))) = {
	0xef,		///< ウオッチドッグ関係
	0xff,		///< LVD 設定
	START_FREQ,	///< オンチップオシレーター
	0x85,		///< デバッグ設定
};

//	0xef, 0xff, 0xe8, 0x85

#if 0
	0x60,		///< ウオッチドッグ関係
	0xff,		///< LVD 設定
	START_FREQ,	///< オンチップオシレーター
	0x04,		///< デバッグ設定
#endif

// EOF
