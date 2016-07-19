#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/G13 グループ・システム・レジスター定義 @n
			Copyright 2016 Kunihito Hiramatsu
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include "common/io_utils.hpp"

namespace device {

	/// @brief PER0 レジスタ定義 
	typedef rw8_t<0xF00F0> per0_rw;
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	周辺イネーブル・レジスタ０（PER0） @n
				リセット時：00H
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	struct per0_rw_t : public per0_rw {
		using per0_rw::operator =;
		using per0_rw::operator ();
		using per0_rw::operator |=;
		using per0_rw::operator &=;

		bit_rw_t<per0_rw, 7> RTCEN;		///< リアルタイム・クロック、12ビット・インターバル・タイマの入力クロック供給の制御
		bit_rw_t<per0_rw, 6> IICA1EN;	///< シリアル・インタフェース１の入力クロック供給の制御
		bit_rw_t<per0_rw, 5> ADCEN;		///< A/Dコンバータの入力クロックの制御
		bit_rw_t<per0_rw, 4> IICA0EN;	///< シリアル・インタフェース０の入力クロック供給の制御
		bit_rw_t<per0_rw, 3> SAU1EN;	///< シリアル・アレイ・ユニット１の入力クロック供給の制御
		bit_rw_t<per0_rw, 2> SAU0EN;	///< シリアル・アレイ・ユニット０の入力クロック供給の制御
		bit_rw_t<per0_rw, 1> TAU1EN;	///< タイマ・アレイ・ユニット１の入力クロック供給の制御
		bit_rw_t<per0_rw, 0> TAU0EN;	///< タイマ・アレイ・ユニット０の入力クロック供給の制御
	};
	static per0_rw_t PER0;

}
