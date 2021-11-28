#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/G13 グループ・タイマ・アレイ・ユニット定義
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016, 2021 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include "common/io_utils.hpp"
#include "G13/peripheral.hpp"

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  タイマ・アレイ・ユニット・テンプレート
		@param[in]	PER		ペリフェラル型
		@param[in]	UOFS	ユニット・オフセット（0x00、0x40）
		@param[in]	CHOFS	チャネル・オフセット（0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E)
		@param[in]	DRADR	データレジスタ・アドレス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t DRADR>
	struct tau_t {

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ・カウンタ・レジスタ（TCR）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef ro16_t<0xF0180 + UOFS + CHOFS> TCR_;
		static TCR_ TCR;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ・データ・レジスタ（TDR）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef rw16_t<DRADR> TDR_;
		static TDR_ TDR;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ・データ・レジスタ・下位バイト（TDRL）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef rw8_t<DRADR> TDRL_;
		static TDRL_ TDRL;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ・データ・レジスタ・上位バイト（TDRH）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef rw8_t<DRADR + 1> TDRH_;
		static TDRH_ TDRH;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ・クロック選択レジスタ（TPS）テンプレート @n
					※ユニット単位
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct tps_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bits_rw_t<T, bitpos::B0,  4>	PRS0;
			bits_rw_t<T, bitpos::B4,  4>	PRS1;
			bits_rw_t<T, bitpos::B8,  2>	PRS2;
			bits_rw_t<T, bitpos::B12, 2>	PRS3;
		};
		typedef tps_t< rw16_t<0xF01B6 + UOFS> > TPS_;
		static TPS_ TPS;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ・モード・レジスタ（TMR）テンプレート
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct tmr_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B0>	 	 MD0;
			bits_rw_t<T, bitpos::B1, 3>	 MD;
			bits_rw_t<T, bitpos::B6, 2>	 CIS;
			bits_rw_t<T, bitpos::B8, 3>	 STS;

			bit_rw_t<T, bitpos::B11>	 MAS;  ///< Master chanel: 2, 4, 6

			bit_rw_t<T, bitpos::B11>	 SPLIT;  ///< Split: 1, 3

			bit_rw_t<T, bitpos::B12>	 CCS;
			bits_rw_t<T, bitpos::B14, 2> CKS;
		};
		typedef tmr_t< rw16_t<0xF0190 + UOFS + CHOFS> > TMR_;
		static TMR_ TMR;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ・ステータス・レジスタ（TSR）テンプレート
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct tsr_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B0>	 OVF;
		};
		typedef tsr_t< rw8_t<0xF01A0 + UOFS + CHOFS> > TSR_;
		static TSR_ TSR;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ・チャネル許可ステータス・レジスタ（TE）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef bit_ro_t<rw8_t<0xF01B0 + UOFS>, static_cast<bitpos>(CHOFS / 2)> TE_;
		static TE_ TE;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ・チャネル許可ステータス・レジスタ１（TEH1）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef bit_ro_t<rw16_t<0xF01B0 + UOFS>, bitpos::B9> TEH1_;
		static TEH1_ TEH1;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ・チャネル許可ステータス・レジスタ３（TEH3）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef bit_ro_t<rw16_t<0xF01B0 + UOFS>, bitpos::B11> TEH3_;
		static TEH3_ TEH3;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ・チャネル開始レジスタ（TS）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef bit_rw_t<rw8_t<0xF01B2 + UOFS>, static_cast<bitpos>(CHOFS / 2)> TS_;
		static TS_ TS;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ・チャネル開始レジスタ１（TS1）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef bit_rw_t<rw16_t<0xF01B2 + UOFS>, bitpos::B9> TS1_;
		static TS1_ TS1;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ・チャネル開始レジスタ３（TS3）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef bit_rw_t<rw16_t<0xF01B2 + UOFS>, bitpos::B11> TS3_;
		static TS3_ TS3;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ・チャネル停止レジスタ（TT）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef bit_rw_t<rw8_t<0xF01B4 + UOFS>, static_cast<bitpos>(CHOFS / 2)> TT_;
		static TT_ TT;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ・チャネル停止レジスタ１（TT1）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef bit_rw_t<rw16_t<0xF01B4 + UOFS>, bitpos::B9> TT1_;
		static TT1_ TT1;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ・チャネル停止レジスタ３（TT3）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef bit_rw_t<rw16_t<0xF01B4 + UOFS>, bitpos::B11> TT3_;
		static TT3_ TT3;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ入力選択レジスタ（TIS）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef bits_rw_t<rw8_t<0xF0074>, bitpos::B0, 3> TIS_;
		static TIS_ TIS;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ出力許可レジスタ（TOE）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef bit_rw_t<rw8_t<0xF01BA + UOFS>, static_cast<bitpos>(CHOFS / 2)> TOE_;
		static TOE_ TOE;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ出力レジスタ（TO）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef bit_rw_t<rw8_t<0xF01B8 + UOFS>, static_cast<bitpos>(CHOFS / 2)> TO_;
		static TO_ TO;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ出力レベル・レジスタ（TOL）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef bit_rw_t<rw8_t<0xF01BC + UOFS>, static_cast<bitpos>(CHOFS / 2)> TOL_;
		static TOL_ TOL;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ出力モード・レジスタ（TOM）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef bit_rw_t<rw8_t<0xF01BE + UOFS>, static_cast<bitpos>(CHOFS / 2)> TOM_;
		static TOM_ TOM;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  入力切り替え制御レジスタ（ISC）テンプレート
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct isc_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B0>	 ISC0;
			bit_rw_t<T, bitpos::B1>	 ISC1;
		};
		typedef isc_t< rw8_t<0xF0073> > ISC_;
		static ISC_ ISC;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  ノイズ・フィルタ許可レジスタ（NFEN）テンプレート
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct nfen_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, static_cast<bitpos>(CHOFS)> TNFEN;
		};
		typedef nfen_t< rw8_t<0xF0071 + (UOFS / 0x40)> > NFEN_;
		static NFEN_ NFEN;


        //-----------------------------------------------------------------//
        /*!
            @brief  ユニット番号を取得
			@return 「０」又は「１」
        */
        //-----------------------------------------------------------------//
		static uint8_t get_unit_no() { return UOFS / 0x40; }


        //-----------------------------------------------------------------//
        /*!
            @brief  チャネル番号を取得
			@return 「０～７」
        */
        //-----------------------------------------------------------------//
		static uint8_t get_chanel_no() { return CHOFS / 0x02; }


		//-------------------------------------------------------------//
		/*!
			@brief  ペリフェラル種別を取得
			@return ペリフェラル種別
		*/
		//-------------------------------------------------------------//
		static peripheral get_peripheral() { return PER; }
	};
	// テンプレート内、スタティック定義、実態：
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t DRADR>
		typename tau_t<PER, UOFS, CHOFS, DRADR>::TCR_ tau_t<PER, UOFS, CHOFS, DRADR>::TCR;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t DRADR>
		typename tau_t<PER, UOFS, CHOFS, DRADR>::TDR_ tau_t<PER, UOFS, CHOFS, DRADR>::TDR;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t DRADR>
		typename tau_t<PER, UOFS, CHOFS, DRADR>::TDRL_ tau_t<PER, UOFS, CHOFS, DRADR>::TDRL;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t DRADR>
		typename tau_t<PER, UOFS, CHOFS, DRADR>::TDRH_ tau_t<PER, UOFS, CHOFS, DRADR>::TDRH;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t DRADR>
		typename tau_t<PER, UOFS, CHOFS, DRADR>::TPS_ tau_t<PER, UOFS, CHOFS, DRADR>::TPS;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t DRADR>
		typename tau_t<PER, UOFS, CHOFS, DRADR>::TMR_ tau_t<PER, UOFS, CHOFS, DRADR>::TMR;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t DRADR>
		typename tau_t<PER, UOFS, CHOFS, DRADR>::TSR_ tau_t<PER, UOFS, CHOFS, DRADR>::TSR;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t DRADR>
		typename tau_t<PER, UOFS, CHOFS, DRADR>::TE_ tau_t<PER, UOFS, CHOFS, DRADR>::TE;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t DRADR>
		typename tau_t<PER, UOFS, CHOFS, DRADR>::TEH1_ tau_t<PER, UOFS, CHOFS, DRADR>::TEH1;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t DRADR>
		typename tau_t<PER, UOFS, CHOFS, DRADR>::TEH3_ tau_t<PER, UOFS, CHOFS, DRADR>::TEH3;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t DRADR>
		typename tau_t<PER, UOFS, CHOFS, DRADR>::TS_ tau_t<PER, UOFS, CHOFS, DRADR>::TS;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t DRADR>
		typename tau_t<PER, UOFS, CHOFS, DRADR>::TS1_ tau_t<PER, UOFS, CHOFS, DRADR>::TS1;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t DRADR>
		typename tau_t<PER, UOFS, CHOFS, DRADR>::TS3_ tau_t<PER, UOFS, CHOFS, DRADR>::TS3;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t DRADR>
		typename tau_t<PER, UOFS, CHOFS, DRADR>::TT_ tau_t<PER, UOFS, CHOFS, DRADR>::TT;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t DRADR>
		typename tau_t<PER, UOFS, CHOFS, DRADR>::TT1_ tau_t<PER, UOFS, CHOFS, DRADR>::TT1;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t DRADR>
		typename tau_t<PER, UOFS, CHOFS, DRADR>::TT3_ tau_t<PER, UOFS, CHOFS, DRADR>::TT3;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t DRADR>
		typename tau_t<PER, UOFS, CHOFS, DRADR>::TIS_ tau_t<PER, UOFS, CHOFS, DRADR>::TIS;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t DRADR>
		typename tau_t<PER, UOFS, CHOFS, DRADR>::TOE_ tau_t<PER, UOFS, CHOFS, DRADR>::TOE;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t DRADR>
		typename tau_t<PER, UOFS, CHOFS, DRADR>::TO_ tau_t<PER, UOFS, CHOFS, DRADR>::TO;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t DRADR>
		typename tau_t<PER, UOFS, CHOFS, DRADR>::TOL_ tau_t<PER, UOFS, CHOFS, DRADR>::TOL;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t DRADR>
		typename tau_t<PER, UOFS, CHOFS, DRADR>::TOM_ tau_t<PER, UOFS, CHOFS, DRADR>::TOM;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t DRADR>
		typename tau_t<PER, UOFS, CHOFS, DRADR>::ISC_ tau_t<PER, UOFS, CHOFS, DRADR>::ISC;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t DRADR>
		typename tau_t<PER, UOFS, CHOFS, DRADR>::NFEN_ tau_t<PER, UOFS, CHOFS, DRADR>::NFEN;

	typedef tau_t<peripheral::TAU00, 0x00, 0x00, 0xFFF18> TAU00;
	typedef tau_t<peripheral::TAU01, 0x00, 0x02, 0xFFF1A> TAU01;
	typedef tau_t<peripheral::TAU02, 0x00, 0x04, 0xFFF64> TAU02;
	typedef tau_t<peripheral::TAU03, 0x00, 0x06, 0xFFF66> TAU03;
	typedef tau_t<peripheral::TAU04, 0x00, 0x08, 0xFFF68> TAU04;
	typedef tau_t<peripheral::TAU05, 0x00, 0x0A, 0xFFF6A> TAU05;
	typedef tau_t<peripheral::TAU06, 0x00, 0x0C, 0xFFF6C> TAU06;
	typedef tau_t<peripheral::TAU07, 0x00, 0x0E, 0xFFF6E> TAU07;

	typedef tau_t<peripheral::TAU10, 0x40, 0x00, 0xFFF70> TAU10;
	typedef tau_t<peripheral::TAU11, 0x40, 0x02, 0xFFF72> TAU11;
	typedef tau_t<peripheral::TAU12, 0x40, 0x04, 0xFFF74> TAU12;
	typedef tau_t<peripheral::TAU13, 0x40, 0x06, 0xFFF76> TAU13;
	typedef tau_t<peripheral::TAU14, 0x40, 0x08, 0xFFF78> TAU14;
	typedef tau_t<peripheral::TAU15, 0x40, 0x0A, 0xFFF7A> TAU15;
	typedef tau_t<peripheral::TAU16, 0x40, 0x0C, 0xFFF7C> TAU16;
	typedef tau_t<peripheral::TAU17, 0x40, 0x0E, 0xFFF7E> TAU17;
}
