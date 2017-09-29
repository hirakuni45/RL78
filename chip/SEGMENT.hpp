#pragma once
//=====================================================================//
/*!	@file
	@brief	Segment クラス
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include <cstdint>

namespace chip {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  SEGMENT テンプレートクラス
		@param[in]	A	セグメントＡ
		@param[in]	B	セグメントＢ
		@param[in]	C	セグメントＣ
		@param[in]	D	セグメントＤ
		@param[in]	E	セグメントＥ
		@param[in]	F	セグメントＦ
		@param[in]	G	セグメントＧ
		@param[in]	DP	セグメントＤＰ
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class A, class B, class C, class D, class E, class F, class G, class DP>
	class SEGMENT {

		static const uint8_t seg_a  = 0b00000001;
		static const uint8_t seg_b  = 0b00000010;
		static const uint8_t seg_c  = 0b00000100;
		static const uint8_t seg_d  = 0b00001000;
		static const uint8_t seg_e  = 0b00010000;
		static const uint8_t seg_f  = 0b00100000;
		static const uint8_t seg_g  = 0b01000000;
		static const uint8_t seg_dp = 0b10000000;

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief	開始
		 */
		//-----------------------------------------------------------------//
		static void start()
		{
			A::DIR = 1;
			B::DIR = 1;
			C::DIR = 1;
			D::DIR = 1;
			E::DIR = 1;
			F::DIR = 1;
			G::DIR = 1;
			DP::DIR = 1;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	ダイレクト設定
			@param[in]	val	値
		 */
		//-----------------------------------------------------------------//
		static void direct(uint8_t val) {
			A::P = (val & 0x01) == 0;
			B::P = (val & 0x02) == 0;
			C::P = (val & 0x04) == 0;
			D::P = (val & 0x08) == 0;
			E::P = (val & 0x10) == 0;
			F::P = (val & 0x20) == 0;
			G::P = (val & 0x40) == 0;
			DP::P = (val & 0x80) == 0;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	数字設定
			@param[in]	n	数字（0 to 9)
		 */
		//-----------------------------------------------------------------//
		static void decimal(uint8_t n)
		{
			static const uint8_t tbl[10] = {
				seg_a | seg_b | seg_c | seg_d | seg_e | seg_f,
				seg_b | seg_c,
				seg_a | seg_b | seg_g | seg_e | seg_d,
				seg_a | seg_b | seg_g | seg_c | seg_d,
				seg_f | seg_g | seg_b | seg_c,
				seg_a | seg_f | seg_g | seg_c | seg_d,
				seg_a | seg_f | seg_g | seg_c | seg_d | seg_e,
				seg_a | seg_b | seg_c,
				seg_a | seg_b | seg_c | seg_d | seg_e | seg_f | seg_g,
				seg_a | seg_b | seg_c | seg_d | seg_f | seg_g,
			};

			if(n < 10) {
				direct(tbl[n]);
			} else {
				direct(seg_g);
			}
		}
	};
}
