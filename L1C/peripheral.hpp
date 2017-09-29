#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/L1C グループ・ペリフェラル
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include <cstdint>
#include "L1C/system.hpp"

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  ペリフェラル種別
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	enum class peripheral : uint8_t {
		ITM,		///< １２ビットインターバルタイマー

		ADC,		///< A/D コンバーター

		I2C,		///< I2C インターフェース

		SAU00,		///< シリアル・アレイ・ユニット００
		SAU01,		///< シリアル・アレイ・ユニット０１
		SAU02,		///< シリアル・アレイ・ユニット０２
		SAU03,		///< シリアル・アレイ・ユニット０３

		SAU10,		///< シリアル・アレイ・ユニット１０
		SAU11,		///< シリアル・アレイ・ユニット１１
		SAU12,		///< シリアル・アレイ・ユニット１２
		SAU13,		///< シリアル・アレイ・ユニット１３

		TAU0,		///< タイマー・アレイ・ユニット０
		TAU1,		///< タイマー・アレイ・ユニット１
		TAU2,		///< タイマー・アレイ・ユニット２
		TAU3,		///< タイマー・アレイ・ユニット３
		TAU4,		///< タイマー・アレイ・ユニット４
		TAU5,		///< タイマー・アレイ・ユニット５
		TAU6,		///< タイマー・アレイ・ユニット６
		TAU7,		///< タイマー・アレイ・ユニット７
	};


	//-----------------------------------------------------------------//
	/*!
		@brief  ペリフェラル許可
		@param[in]	per	ペリフェラル型
		@param[in]	ena	不許可の場合「false」
	*/
	//-----------------------------------------------------------------//
	static void enable(peripheral per, bool ena = true)
	{
		switch(per) {
		case peripheral::ITM:
			PER1.TMKAEN = ena;
			break;

		case peripheral::SAU00:
		case peripheral::SAU01:
		case peripheral::SAU02:
		case peripheral::SAU03:
			PER0.SAU0EN = ena;
			break;

		case peripheral::SAU10:
		case peripheral::SAU11:
		case peripheral::SAU12:
		case peripheral::SAU13:
			PER0.SAU1EN = ena;
			break;

		default:
			break;
		}
	}
}
