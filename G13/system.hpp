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

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	周辺イネーブル・レジスタ０（PER0） @n
				リセット時：00H
		@param[in]	T	アクセス・クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class T>
	struct per0_t : public T {
		using T::operator =;
		using T::operator ();
		using T::operator |=;
		using T::operator &=;

		bit_rw_t<T, 7> RTCEN;   ///< リアルタイム・クロック、インターバル・タイマの入力クロック供給の制御
		bit_rw_t<T, 6> IICA1EN; ///< シリアル・インタフェース１の入力クロック供給の制御
		bit_rw_t<T, 5> ADCEN;   ///< A/Dコンバータの入力クロックの制御
		bit_rw_t<T, 4> IICA0EN; ///< シリアル・インタフェース０の入力クロック供給の制御
		bit_rw_t<T, 3> SAU1EN;  ///< シリアル・アレイ・ユニット１の入力クロック供給の制御
		bit_rw_t<T, 2> SAU0EN;  ///< シリアル・アレイ・ユニット０の入力クロック供給の制御
		bit_rw_t<T, 1> TAU1EN;  ///< タイマ・アレイ・ユニット１の入力クロック供給の制御
		bit_rw_t<T, 0> TAU0EN;  ///< タイマ・アレイ・ユニット０の入力クロック供給の制御
	};
	static per0_t< rw8_t<0xF00F0> > PER0;


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	サブシステム・クロック供給モード制御レジスタ（OSMC）
		@param[in]	T	アクセス・クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class T>
	struct osmc_t : public T {
		using T::operator =;
		using T::operator ();
		using T::operator |=;
		using T::operator &=;

		bit_rw_t<T, 7>  RTCLPC;    ///< STOPモード時およびサブシステム・クロックでCPU動作中のHALTモード時の設定
		bit_rw_t<T, 4>  WUTMMCK0;  ///< リアルタイム・クロック，インターバル・タイマの動作クロックの選択
	};
	static osmc_t< rw8_t<0xF00F3> > OSMC;
}
