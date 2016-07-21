#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/G13 グループ IICA 制御 @n
			Copyright 2016 Kunihito Hiramatsu
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include "G13/system.hpp"
#include "G13/iica.hpp"

/// F_CLK はボーレートパラメーター計算で必要、設定が無いとエラーにします。
#ifndef F_CLK
#  error "uart_io.hpp requires F_CLK to be defined"
#endif

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  IICA 制御クラス
		@param[in]	IICA	IICA クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class IICA>
	class iica_io {


	public:



	};
}
