#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/G13 グループ・システム・レジスター定義
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include "common/io_utils.hpp"

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	クロック動作モード制御レジスタ（CMC）
		@param[in]	T	アクセス・クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class T>
	struct cmc_t : public T {
		using T::operator =;
		using T::operator ();
		using T::operator |=;
		using T::operator &=;

		bit_rw_t<T, bitpos::B7> EXCLK;      ///< 高速システム・クロック端子の動作モード
		bit_rw_t<T, bitpos::B6> OSCSEL;     ///< 高速システム・クロック端子の動作モード
		bit_rw_t<T, bitpos::B5> EXCLKS;     ///< サブシステム・クロック端子の動作モード
		bit_rw_t<T, bitpos::B4> OSCSELS;    ///< サブシステム・クロック端子の動作モード

		bit_rw_t<T, bitpos::B2>     AMPHS1; ///< XT1発振回路の発振モード選択 1
		bit_rw_t<T, bitpos::B1>     AMPHS0; ///< XT1発振回路の発振モード選択 0
		bits_rw_t<T, bitpos::B1, 2> AMPHS;  ///< XT1発振回路の発振モード選択

		bit_rw_t<T, bitpos::B0> AMPH;   ///< X1クロック発振周波数の制御
	};
	static cmc_t< rw8_t<0xFFFA0> > CMC;


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	クロック動作ステータス制御レジスタ（CKC）
		@param[in]	T	アクセス・クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class T>
	struct ckc_t : public T {
		using T::operator =;
		using T::operator ();
		using T::operator |=;
		using T::operator &=;

		bit_rw_t<T, bitpos::B7> CLS;   ///< CPU／周辺ハードウエア・クロック（f CLK ）のステータス 
		bit_rw_t<T, bitpos::B6> CSS;   ///< CPU／周辺ハードウエア・クロック（f CLK ）の選択
		bit_rw_t<T, bitpos::B5> MCS;   ///< メイン・システム・クロック（f MAIN ）のステータス
		bit_rw_t<T, bitpos::B4> MCM0;  ///< メイン・システム・クロック（f MAIN ）の動作制御
	};
	static ckc_t< rw8_t<0xFFFA4> > CKC;


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	クロック動作ステータス制御レジスタ（CSC）
		@param[in]	T	アクセス・クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class T>
	struct csc_t : public T {
		using T::operator =;
		using T::operator ();
		using T::operator |=;
		using T::operator &=;

		bit_rw_t<T, bitpos::B7> MSTOP;    ///< 高速システム・クロックの動作制御
		bit_rw_t<T, bitpos::B6> XTSTOP;   ///< サブシステム・クロックの動作制御
		bit_rw_t<T, bitpos::B0> MIOSTOP;  ///< 高速オンチップ・オシレータ・クロックの動作制御
	};
	static csc_t< rw8_t<0xFFFA1> > CSC;


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	発振安定時間カウンタ状態レジスタ（OSTC）
		@param[in]	T	アクセス・クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class T>
	struct ostc_t : public T {
		using T::operator ();

		/// ro8_t<T::address>  MOST;  ///< 発振安定時間のステータス

		bit_ro_t<T, bitpos::B7> MOST8;
		bit_ro_t<T, bitpos::B6> MOST9;
		bit_ro_t<T, bitpos::B5> MOST10;
		bit_ro_t<T, bitpos::B4> MOST11;
		bit_ro_t<T, bitpos::B3> MOST13;
		bit_ro_t<T, bitpos::B2> MOST15;
		bit_ro_t<T, bitpos::B1> MOST17;
		bit_ro_t<T, bitpos::B0> MOST18;
	};
	static ostc_t< ro8_t<0xFFFA2> > OSTC;


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	発振安定時間選択レジスタ（OSTS）
		@param[in]	T	アクセス・クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class T>
	struct osts_t : public T {
		using T::operator =;
		using T::operator ();
		using T::operator |=;
		using T::operator &=;

		bits_rw_t<T, bitpos::B0, 3>  OSTS;  ///< 発振安定時間の選択

		bit_rw_t<T, bitpos::B2>  OSTS2;
		bit_rw_t<T, bitpos::B1>  OSTS1;
		bit_rw_t<T, bitpos::B0>  OSTS0;
	};
	static osts_t< rw8_t<0xFFFA3> > OSTS;


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

		bit_rw_t<T, bitpos::B7> RTCEN;   ///< リアルタイム・クロック、インターバル・タイマの入力クロック供給の制御
		bit_rw_t<T, bitpos::B6> IICA1EN; ///< シリアル・インタフェース１の入力クロック供給の制御
		bit_rw_t<T, bitpos::B5> ADCEN;   ///< A/Dコンバータの入力クロックの制御
		bit_rw_t<T, bitpos::B4> IICA0EN; ///< シリアル・インタフェース０の入力クロック供給の制御
		bit_rw_t<T, bitpos::B3> SAU1EN;  ///< シリアル・アレイ・ユニット１の入力クロック供給の制御
		bit_rw_t<T, bitpos::B2> SAU0EN;  ///< シリアル・アレイ・ユニット０の入力クロック供給の制御
		bit_rw_t<T, bitpos::B1> TAU1EN;  ///< タイマ・アレイ・ユニット１の入力クロック供給の制御
		bit_rw_t<T, bitpos::B0> TAU0EN;  ///< タイマ・アレイ・ユニット０の入力クロック供給の制御
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

		bit_rw_t<T, bitpos::B7>  RTCLPC;    ///< STOPモード時およびサブシステム・クロックでCPU動作中のHALTモード時の設定
		bit_rw_t<T, bitpos::B4>  WUTMMCK0;  ///< リアルタイム・クロック，インターバル・タイマの動作クロックの選択
	};
	static osmc_t< rw8_t<0xF00F3> > OSMC;


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	高速オンチップ・オシレータ周波数選択レジスタ（HOCODIV）
		@param[in]	T	アクセス・クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class T>
	struct hocodiv_t : public T {
		using T::operator =;
		using T::operator ();
		using T::operator |=;
		using T::operator &=;

		bits_rw_t<T, bitpos::B0, 3>  HOCODIV;

		bit_rw_t<T, bitpos::B2>  HOCODIV2;
		bit_rw_t<T, bitpos::B1>  HOCODIV1;
		bit_rw_t<T, bitpos::B0>  HOCODIV0;
	};
	static hocodiv_t< rw8_t<0xF00A8> > HOCODIV;  ///< 高速オンチップ・オシレータ・クロック周波数の選択


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	高速オンチップ・オシレータ・トリミング・レジスタ（HIOTRM）
		@param[in]	T	アクセス・クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class T>
	struct hiotrm_t : public T {
		using T::operator =;
		using T::operator ();
		using T::operator |=;
		using T::operator &=;

		bits_rw_t<T, bitpos::B0, 6>  HIOTRM;  ///< 高速オンチップ・オシレータ

		bit_rw_t<T, bitpos::B5>  HIOTRM5;
		bit_rw_t<T, bitpos::B4>  HIOTRM4;
		bit_rw_t<T, bitpos::B3>  HIOTRM3;
		bit_rw_t<T, bitpos::B2>  HIOTRM2;
		bit_rw_t<T, bitpos::B1>  HIOTRM1;
		bit_rw_t<T, bitpos::B0>  HIOTRM0;
	};
	static hiotrm_t< rw8_t<0xF00A0> > HIOTRM;


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	データ・フラッシュ・コントロール・レジスタ（DFLCTL）
		@param[in]	T	アクセス・クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class T>
	struct dflctl_t : public T {
		using T::operator =;
		using T::operator ();
		using T::operator |=;
		using T::operator &=;

		bit_rw_t<T, bitpos::B0>  DFLLEN;
	};
	static dflctl_t< rw8_t<0xF0090> > DFLCTL;


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	リセット・コントロール・フラグ・レジスタ（RESF）
		@param[in]	T	アクセス・クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class T>
	struct resf_t : public T {
		using T::operator ();

		bit_ro_t<T, bitpos::B0>  LVIRF;
		bit_ro_t<T, bitpos::B1>  IAWRF;
		bit_ro_t<T, bitpos::B2>  RPERF;

		bit_ro_t<T, bitpos::B4>  WDTRF;

		bit_ro_t<T, bitpos::B7>  TRAP;
	};
	static resf_t< ro8_t<0xFFFA8> > RESF;

}
