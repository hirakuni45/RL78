//=====================================================================//
/*!	@file
	@brief	RL78/G13 グループ・オプション・バイト @n
			Copyright 2016 Kunihito Hiramatsu
	@author	平松邦仁 (hira@rvf-rc45.net)
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
	0x60,		///< ウオッチドッグ関係
	0xff,		///< LVD 設定
	START_FREQ,	///< オンチップオシレーター
	0x04,		///< デバッグ設定
};

// EOF
