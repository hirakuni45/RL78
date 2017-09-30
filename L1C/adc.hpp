#pragma once
//=========================================================================//
/*!	@file
	@brief	RL78/L1C グループ・Ａ／Ｄコンバーター定義（８／１２ビット）
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=========================================================================//
#include "common/io_utils.hpp"
#include "L1C/peripheral.hpp"

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	A/D コンバータ・クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	struct adc {

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief	A/Dコンバータ・モード・レジスタ0（ADM0）
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct adm0_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t <T, bitpos::B7>    ADCS;  ///< A/D変換動作の制御
			bit_rw_t <T, bitpos::B6>    ADMD;  ///< A/D変換チャネル選択モードを設定
			bit_rw_t <T, bitpos::B5>    FR2;
			bit_rw_t <T, bitpos::B4>    FR1;
			bit_rw_t <T, bitpos::B3>    FR0;
			bits_rw_t<T, bitpos::B3, 3> FR;
			bit_rw_t <T, bitpos::B2>    LV1;
			bit_rw_t <T, bitpos::B1>    LV0;
			bits_rw_t<T, bitpos::B1, 2> LV;
			bit_rw_t <T, bitpos::B0>    ADCE;  ///< A/D電圧コンパレータの動作制御
		};
		static adm0_t< rw8_t<0xFFF30> > ADM0;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief	A/Dコンバータ・モード・レジスタ1（ADM1）
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct adm1_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t <T, bitpos::B7>    ADTMD1;
			bit_rw_t <T, bitpos::B6>    ADTMD0;
			bits_rw_t<T, bitpos::B6, 2> ADTMD;   ///< A/D変換トリガ・モードの選択

			bit_rw_t <T, bitpos::B5>    ADSCM;   ///< A/D変換動作モードの設定

			bit_rw_t <T, bitpos::B1>    ADTRS1; 
			bit_rw_t <T, bitpos::B0>    ADTRS0;
			bits_rw_t<T, bitpos::B0, 2> ADTRS;   ///< ハードウエア・トリガ信号の選択
		};
		static adm1_t< rw8_t<0xFFF32> > ADM1;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief	A/Dコンバータ・モード・レジスタ2（ADM2）
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct adm2_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t <T, bitpos::B7>    ADREFP1;
			bit_rw_t <T, bitpos::B6>    ADREFP0;
			bits_rw_t<T, bitpos::B6, 2> ADREFP;   ///< A/Dコンバータの＋側の基準電圧の選択
			bit_rw_t <T, bitpos::B5>    ADREFM;   ///< A/Dコンバータの－側の基準電圧の選択

			bit_rw_t <T, bitpos::B3>    ADRCK;    ///< 変換結果上限／下限値チェック
			bit_rw_t <T, bitpos::B2>    AWC;      ///< SNOOZEモードの設定

			bit_rw_t <T, bitpos::B0>    ADTYP;    ///< A/D変換分解能の選択
		};
		static adm2_t< rw8_t<0xF0010> > ADM2;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief	12ビットA/D変換結果レジスタ（ADCR）@n
					B0 to B11
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static ro16_t<0xFFF1E> ADCR;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief	8ビットA/D変換結果レジスタ（ADCRH）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static ro8_t<0xFFF1F> ADCRH;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief	アナログ入力チャネル指定レジスタ（ADS）
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct ads_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t <T, bitpos::B7>    ADISS;

			bit_rw_t <T, bitpos::B4>    ADS4;
			bit_rw_t <T, bitpos::B3>    ADS3;
			bit_rw_t <T, bitpos::B2>    ADS2;
			bit_rw_t <T, bitpos::B1>    ADS1;
			bit_rw_t <T, bitpos::B0>    ADS0;
			bits_rw_t<T, bitpos::B0, 5> ADS;
		};
		static ads_t< rw8_t<0xFFF31> > ADS;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief	変換結果比較上限値設定レジスタ（ADUL）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static rw8_t<0xF0011> ADUL;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief	変換結果比較下限値設定レジスタ（ADLL）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static rw8_t<0xF0012> ADLL;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief	A/Dテスト・レジスタ（ADTES）
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct adtes_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t <T, bitpos::B1>    ADTES1;
			bit_rw_t <T, bitpos::B0>    ADTES0;
			bits_rw_t<T, bitpos::B0, 2> ADTES;  ///< A/D変換対象
		};
		static adtes_t< rw8_t<0xF0013> > ADTES;


		//-------------------------------------------------------------//
		/*!
			@brief  ペリフェラル種別を取得
			@return ペリフェラル種別
		*/
		//-------------------------------------------------------------//
		static peripheral get_peripheral() { return peripheral::ADC; }
	};
}
