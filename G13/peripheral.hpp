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
#include "G13/system.hpp"

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  ペリフェラル種別
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	enum class peripheral : uint8_t {
		ITM,		///< １２ビットインターバルタイマー

		UART0,		///< UART0 シリアル
		UART1,		///< UART1 シリアル
		UART2,		///< UART2 シリアル
		UART3,		///< UART3 シリアル

		SPI0,		///< SPI0 シリアル
		SPI1,		///< SPI1 シリアル
		SPI2,		///< SPI2 シリアル
		SPI3,		///< SPI3 シリアル

		ADC,		///< A/D コンバーター

		I2C,		///< I2C インターフェース

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
			PER0.RTCEN = ena;
			break;
		default:
			break;
		}
	}
}
