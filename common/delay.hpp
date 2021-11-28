#pragma once
//=====================================================================//
/*!	@file
	@brief	delay ユーティリティー (RL78/G13 32MHz)
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016, 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include <cstdint>

/// F_CLK が３２、２４以外はエラーにする
#if (F_CLK != 32000000) && (F_CLK != 24000000)
#  error "delay.hpp requires F_CLK to be defined"
#endif

namespace utils {


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  基準クロック３２／２４ＭＨｚの待ち @n
				※-O2、-Os などの最適化が条件、最適化無しの場合などは、@n
				アセンブルリストを参照の上、マシンサイクルを確認 @n
				※基準クロックが32MHz以外の場合、@n
				ループのデクリメントは８クロック必要なので、NOP を調整して @n
				クロックを合わせる。 @n
				※8MHzまでしか対応できない。
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	class delay {
	public:

		//-----------------------------------------------------------------//
		/*!
			@brief  ナノ秒単位の待ち
			@param[in]	ns	待ち時間（ナノ秒）
		*/
		//-----------------------------------------------------------------//
		static void nano_second(uint16_t ns) {
#if (F_CLK == 32000000)
			ns /= 250;  // 31.25ns x 8 (250ns)
#elif (F_CLK == 24000000)
			ns /= 333;  // 41.67ns x 8 (333ns)
#endif
			while(ns > 0) {
				--ns;
			}
			// (2) decw	0xffef0
			// (1) movw	ax, 0xffef0
    		// (1) cmpw	ax, #0
    		// (1) skz
    		// (3) br	!!4922 <.L629>
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  マイクロ秒単位の待ち
			@param[in]	us	待ち時間（マイクロ秒）
		*/
		//-----------------------------------------------------------------//
		static void micro_second(uint16_t us) {
			while(us > 0) {
#if (F_CLK == 32000000)
				asm("nop"); asm("nop"); asm("nop"); asm("nop");
				asm("nop"); asm("nop"); asm("nop"); asm("nop");
				asm("nop"); asm("nop"); asm("nop"); asm("nop");
				asm("nop"); asm("nop"); asm("nop"); asm("nop");
				asm("nop"); asm("nop"); asm("nop"); asm("nop");
				asm("nop"); asm("nop"); asm("nop"); asm("nop");
#elif (F_CLK == 24000000)
				asm("nop"); asm("nop"); asm("nop"); asm("nop");
				asm("nop"); asm("nop"); asm("nop"); asm("nop");
				asm("nop"); asm("nop"); asm("nop"); asm("nop");
				asm("nop"); asm("nop"); asm("nop"); asm("nop");
#endif
				--us;
			}
			// (2) decw	0xffef0
			// (1) movw	ax, 0xffef0
    		// (1) cmpw	ax, #0
    		// (1) skz
    		// (3) br	!!4922 <.L629>
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  ミリ秒単位の待ち
			@param[in]	ms	待ち時間（ミリ秒）
		*/
		//-----------------------------------------------------------------//
		static void milli_second(uint16_t ms) {
			while(ms > 0) {
				micro_second(1000);
				--ms;
			}
		}
	};
}
