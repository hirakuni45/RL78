#pragma once
//=====================================================================//
/*!	@file
	@brief	ポート・ユーティリティー
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2017, 2021 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include "common/renesas.hpp"

namespace utils {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  ポート・ユーティリティー・クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	struct port {

		//-----------------------------------------------------------------//
		/*!
			@brief  全てのポートをプルアップ
		*/
		//-----------------------------------------------------------------//
		static void pullup_all()
		{
#if defined(SIG_G13)
			device::PU0  = 0b1111'1111;
			device::PU1  = 0b1111'1111;
			device::PU3  = 0b1111'1111;
			device::PU4  = 0b1111'1111;
			device::PU5  = 0b1111'1111;
			device::PU6  = 0b1111'0000;
			device::PU7  = 0b1111'1111;
			device::PU8  = 0b1111'1111;
			device::PU9  = 0b1111'1111;
			device::PU10 = 0b0111'1111;
			device::PU11 = 0b1111'1111;
			device::PU12 = 0b1110'0001;
			device::PU14 = 0b1111'1111;
#elif defined(SIG_L1C)
			device::PU0  = 0b1111'1111;
			device::PU1  = 0b1111'1111;
			device::PU3  = 0b1111'1111;
			device::PU4  = 0b1111'1111;
			device::PU5  = 0b1111'1111;
			device::PU6  = 0b1111'0000;
			device::PU7  = 0b1111'1111;
			device::PU8  = 0b1111'1111;
			device::PU12 = 0b1110'0001;
			device::PU14 = 0b1111'1111;
#endif
		}
	};
}
