#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/G13 グループ・１２ビット・インターバル・タイマー定義
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016, 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include "common/io_utils.hpp"
#include "G13/peripheral.hpp"

namespace device {

	struct itm {

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  １２ビット・インターバル・タイマ・コントロール・レジスタ（ITMC）
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct itmc_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B15>	 RINTE;  ///< 12ビット・インターバル・タイマの動作制御
			bits_rw_t<T, bitpos::B0, 12> ITCMP;  ///< 12ビット・インターバル・タイマのコンペア値設定


		};
		static itmc_t< rw16_t<0xFFF90> > ITMC;


		//-----------------------------------------------------------------//
		/*!
			@brief  ペリフェラル種別を取得
			@return ペリフェラル種別
		*/
		//-----------------------------------------------------------------//
		static peripheral get_peripheral() { return peripheral::ITM; }
	};
}
