#pragma once
//=========================================================================//
/*!	@file
	@brief	RL78/G13 グループ・Ａ／Ｄコンバーター定義（８／１０ビット）
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016, 2021 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=========================================================================//
#include "common/io_utils.hpp"
#include "G13/peripheral.hpp"

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	A/D コンバータ・クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class _>
	struct adc_t {

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
		typedef adm0_t< rw8_t<0xFFF30> > ADM0_;
		static ADM0_ ADM0;


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
		typedef adm1_t< rw8_t<0xFFF32> > ADM1_;
		static ADM1_ ADM1;


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
		typedef adm2_t< rw8_t<0xF0010> > ADM2_;
		static ADM2_ ADM2;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief	12ビットA/D変換結果レジスタ（ADCR）@n
					B0 to B11
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef ro16_t<0xFFF1E> ADCR_;
		static ADCR_ ADCR;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief	8ビットA/D変換結果レジスタ（ADCRH）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef ro8_t<0xFFF1F> ADCRH_;
		static ADCRH_ ADCRH;


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
		typedef ads_t< rw8_t<0xFFF31> > ADS_;
		static ADS_ ADS;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief	変換結果比較上限値設定レジスタ（ADUL）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef rw8_t<0xF0011> ADUL_;
		static ADUL_ ADUL;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief	変換結果比較下限値設定レジスタ（ADLL）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef rw8_t<0xF0012> ADLL_;
		static ADLL_ ADLL;


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
		typedef adtes_t< rw8_t<0xF0013> > ADTES_;
		static ADTES_ ADTES;


		//-------------------------------------------------------------//
		/*!
			@brief  ペリフェラル種別を取得
			@return ペリフェラル種別
		*/
		//-------------------------------------------------------------//
		static peripheral get_peripheral() { return peripheral::ADC; }
	};
	// テンプレート内、スタティック定義、実態：
	template<class _> typename adc_t<_>::ADM0_ adc_t<_>::ADM0;
	template<class _> typename adc_t<_>::ADM1_ adc_t<_>::ADM1;
	template<class _> typename adc_t<_>::ADM2_ adc_t<_>::ADM2;
	template<class _> typename adc_t<_>::ADCR_ adc_t<_>::ADCR;
	template<class _> typename adc_t<_>::ADCRH_ adc_t<_>::ADCRH;
	template<class _> typename adc_t<_>::ADS_ adc_t<_>::ADS;
	template<class _> typename adc_t<_>::ADUL_ adc_t<_>::ADUL;
	template<class _> typename adc_t<_>::ADLL_ adc_t<_>::ADLL;
	template<class _> typename adc_t<_>::ADTES_ adc_t<_>::ADTES;

	typedef adc_t<void> adc;
}
