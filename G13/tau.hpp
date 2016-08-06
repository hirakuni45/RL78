#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/G13 グループ・タイマ・アレイ・ユニット定義 @n
			Copyright 2016 Kunihito Hiramatsu
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include "common/io_utils.hpp"

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  タイマ・アレイ・ユニット・テンプレート
		@param[in]	UOFS	ユニット・オフセット（0x00、0x40）
		@param[in]	CHOFS	チャネル・オフセット（0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E)
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <uint32_t UOFS, uint32_t CHOFS>
	struct tau_t {

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
		static tps_t< rw16_t<0xF01B6 + UOFS> > TPS;


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

			bits_rw_t<T, bitpos::B0, 4>	 MD;
			bits_rw_t<T, bitpos::B6, 2>	 CIS;
			bits_rw_t<T, bitpos::B8, 3>	 STS;
#if (CHOFS == 4 || CHOFS == 8 || CHOFS == 12)
			bit_rw_t<T, bitpos::B11>	 MAS;
#elif (CHOFS == 2 || CHOFS == 6)
			bit_rw_t<T, bitpos::B11>	 SPLIT;
#endif
			bit_rw_t<T, bitpos::B12>	 CCS;
			bits_rw_t<T, bitpos::B14, 3> CKS;
		};
		static tmr_t< rw16_t<0xF0190 + UOFS + CHOFS> > TMR;


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
		static tsr_t< rw8_t<0xF01A0 + UOFS + CHOFS> > TSR;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ・チャネル許可ステータス・レジスタ（TE）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static bit_rw_t<rw8_t<0xF01B0 + UOFS>, static_cast<bitpos>(CHOFS)> TE;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ・チャネル許可ステータス・レジスタ１（TEH1）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static bit_rw_t<rw16_t<0xF01B0 + UOFS>, bitpos::B9> TEH1;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ・チャネル許可ステータス・レジスタ３（TEH3）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static bit_rw_t<rw16_t<0xF01B0 + UOFS>, bitpos::B11> TEH3;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ・チャネル開始レジスタ（TS）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static bit_rw_t<rw8_t<0xF01B2 + UOFS>, static_cast<bitpos>(CHOFS)> TS;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ・チャネル開始レジスタ１（TS1）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static bit_rw_t<rw16_t<0xF01B2 + UOFS>, bitpos::B9> TS1;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ・チャネル開始レジスタ３（TS3）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static bit_rw_t<rw16_t<0xF01B2 + UOFS>, bitpos::B11> TS3;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ・チャネル停止レジスタ（TT）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static bit_rw_t<rw8_t<0xF01B4 + UOFS>, static_cast<bitpos>(CHOFS)> TT;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ・チャネル停止レジスタ１（TT1）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static bit_rw_t<rw16_t<0xF01B4 + UOFS>, bitpos::B9> TT1;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ・チャネル停止レジスタ３（TT3）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static bit_rw_t<rw16_t<0xF01B4 + UOFS>, bitpos::B11> TT3;


#if (CHOFS == 0)
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ入力選択レジスタ（TIS）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static bits_rw_t<rw8_t<0xF0074>, bitpos::B0, 3> TIS;
#endif


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ出力許可レジスタ（TOE）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static bit_rw_t<rw8_t<0xF01BA + UOFS>, static_cast<bitpos>(CHOFS)> TOE;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ出力レジスタ（TO）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static bit_rw_t<rw8_t<0xF01B8 + UOFS>, static_cast<bitpos>(CHOFS)> TO;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ出力レベル・レジスタ（TOL）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static bit_rw_t<rw8_t<0xF01BC + UOFS>, static_cast<bitpos>(CHOFS)> TOL;


#if (CHOFS != 0)
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイマ出力モード・レジスタ（TOM）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static bit_rw_t<rw8_t<0xF01BE + UOFS>, static_cast<bitpos>(CHOFS)> TOM;
#endif


#if (CHOFS != 14)
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
		static isc_t< rw8_t<0xF0073> > ISC;
#endif



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
		static nfen_t< rw8_t<0xF0071 + (UOFS / 0x40)> > NFEN;


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
	};
	typedef tau_t<0x00, 0x00> TAU00;
	typedef tau_t<0x00, 0x02> TAU01;
	typedef tau_t<0x00, 0x04> TAU02;
	typedef tau_t<0x00, 0x06> TAU03;
	typedef tau_t<0x00, 0x08> TAU04;
	typedef tau_t<0x00, 0x0A> TAU05;
	typedef tau_t<0x00, 0x0C> TAU06;
	typedef tau_t<0x00, 0x0E> TAU07;

	typedef tau_t<0x40, 0x00> TAU10;
	typedef tau_t<0x40, 0x02> TAU11;
	typedef tau_t<0x40, 0x04> TAU12;
	typedef tau_t<0x40, 0x06> TAU13;
	typedef tau_t<0x40, 0x08> TAU14;
	typedef tau_t<0x40, 0x0A> TAU15;
	typedef tau_t<0x40, 0x0C> TAU16;
	typedef tau_t<0x40, 0x0E> TAU17;





}
