#pragma once
//=====================================================================//
/*!	@file
	@brief	delay ユーティリティー (RL78/G13 32MHz) @n
			Copyright 2016 Kunihito Hiramatsu
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include <cstdint>

namespace utils {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  基準クロック３２ＭＨｚの待ち @n
				※-O2、-Os などの最適化が条件、最適化無しの場合などは、@n
				アセンブルリストを参照の上、マシンサイクルを確認
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	struct delay {

		//-----------------------------------------------------------------//
		/*!
			@brief  マイクロ秒単位の待ち
			@param[in]	us	待ち時間（マイクロ秒）
		*/
		//-----------------------------------------------------------------//
		static void micro_second(uint16_t us) {
			while(us > 0) {
				asm("nop"); asm("nop"); asm("nop"); asm("nop");
				asm("nop"); asm("nop"); asm("nop"); asm("nop");
				asm("nop"); asm("nop"); asm("nop"); asm("nop");
				asm("nop"); asm("nop"); asm("nop"); asm("nop");
				asm("nop"); asm("nop"); asm("nop"); asm("nop");
				asm("nop"); asm("nop"); asm("nop"); asm("nop");
				--us;
			}
			// (2) decw	0xffef0
			// (1) movw	ax, 0xffef0
    		// (1) cmpw	ax, #0
    		// (1) skz
    		// (3) br	!!4922 <.L629>
		}
	};
}
