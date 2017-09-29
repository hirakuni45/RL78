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
#if defined(SIG_G13)
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
#elif defined(SIG_L1C)
// #define START_FREQ	0xF0	///< 48MHz for USB
#define START_FREQ	0xE0	///< 24MHz
// #define START_FREQ	0xE9	///< 16MHz
// #define START_FREQ	0xE1	///< 12MHz
// #define START_FREQ	0xEA	///<  8MHz
// #define START_FREQ	0xE2	///<  6MHz
// #define START_FREQ	0xEB	///<  4MHz
// #define START_FREQ	0xE3	///<  3MHz
// #define START_FREQ	0xEC	///<  2MHz
// #define START_FREQ	0xED	///<  1MHz
#endif

/*
  オンチップ・デバッグ・オプション・バイトのフォーマット（0x000C3/0x010C3）
  B7:OCDENSET, B6:0, B5:0, B4:0, B3:0, B2:1, B1:0, B0:OCDERSD
  OCDENSET: 0, OCDERSD: 0 ---> オンチップ・デバッグ動作禁止
  OCDENSET: 0, OCDERSD: 1 ---> 設定禁止
  OCDENSET: 1, OCDERSD: 0 ---> オンチップ・デバッグ動作許可。
                               オンチップ・デバッグ・セキュリティID認証失敗時にフラッシュ・メモリのデータを
                               消去する
  OCDENSET: 1, OCDERSD: 1 ---> オンチップ・デバッグ動作許可。
                               オンチップ・デバッグ・セキュリティID認証失敗時にフラッシュ・メモリのデータを
                               消去しない
*/

//-----------------------------------------------------------------//
/*!
	@brief  オプション・バイト設定
*/
//-----------------------------------------------------------------//
const uint8_t opt_[] __attribute__ ((section (".opt"))) = {
	0x60,		///< 0x000C0/0x010C0 ウオッチドッグ関係
	0xff,		///< 0x000C1/0x010C1 LVD 設定
	START_FREQ,	///< 0x000C2/0x010C2 オンチップオシレーター
	0x04,		///< 0x000C3/0x010C3 デバッグ設定
};

//	0xef, 0xff, 0xe8, 0x85

// EOF
