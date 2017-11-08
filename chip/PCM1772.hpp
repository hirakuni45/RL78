#pragma once
//=====================================================================//
/*!	@file
	@brief	PCM1772 ドライバー・クラス @n
			LOW-VOLTAGE AND LOW-POWER STEREO AUDIO @n
			DIGITAL-TO-ANALOG CONVERTER WITH LINEOUT AMPLIFIER
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include <cstdint>
#include "common/delay.hpp"

namespace chip {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  PCM1772 テンプレートクラス
		@param[in]	CSI_IO	CSI(SPI) 制御クラス
		@param[in]	SEL	デバイス選択
		@param[in]	PD	パワー・ダウン選択
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class CSI_IO, class SEL, class PD>
	class PCM1772 {

		CSI_IO&	csi_;

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief  コンストラクター
		*/
		//-----------------------------------------------------------------//
		PCM1772(CSI_IO& csi) : csi_(csi) { }

		//-----------------------------------------------------------------//
		/*!
			@brief  開始
		*/
		//-----------------------------------------------------------------//
		void start()
		{
			PD::DIR = 1;    // output /PD (power down)
			PD::P = 0;      // power down (assert reset)
			SEL::DIR = 1;	// output /MS (device select)
			SEL::P = 1;		// device disable

			utils::delay::milli_second(100);

			PD::P = 1;		// disable power down




		}
	};
}
