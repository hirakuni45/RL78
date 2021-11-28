#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/G13 グループ・シリアル・アレイ・ユニット定義
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
		@brief  シリアル・アレイ・ユニット・テンプレート
		@param[in]	PER		ペリフェラル型
		@param[in]	UOFS	ユニット・オフセット（0x00、0x40）
		@param[in]	CHOFS	チャネル・オフセット（0x00, 0x02, 0x04, 0x06)
		@param[in]	SDR_O	SDR レジスターオフセット
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t SDR_O>
	struct sau_t {

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  シリアル・クロック選択レジスタ m（SPS）テンプレート @n
					※ユニット単位
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct sps_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bits_rw_t<T, bitpos::B0, 4>	PRS0;
			bits_rw_t<T, bitpos::B4, 4>	PRS1;
		};
		typedef sps_t< rw8_t<0xF0126 + UOFS> > SPS_;
		static SPS_ SPS;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  シリアル・モード・レジスタ mn（SMR）テンプレート
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct smr_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B15>	CKS;	///< チャネルnの動作クロック（f MCK ）の選択
			bit_rw_t<T, bitpos::B14>	CCS;	///< チャネルnの転送クロック（f TCLK ）の選択
			bit_rw_t<T, bitpos::B8 >	STS;	///< スタート・トリガ要因の選択
			bit_rw_t<T, bitpos::B6 >	SIS;	///< UARTモードでのチャネルnの受信データのレベル反転の制御
			bits_rw_t<T, bitpos::B1, 2>	MD;		///< チャネルnの動作モードの設定
			bit_rw_t<T, bitpos::B0>		MD0;	///< チャネルnの割り込み要因の選択
		};
		typedef smr_t< rw16_t<0xF0110 + UOFS + CHOFS> > SMR_;
		static SMR_ SMR;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  シリアル通信動作設定レジスタ mn（SCR）テンプレート
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct scr_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B15>		TXE;	///< 送信モードの設定
			bit_rw_t<T, bitpos::B14>		RXE;	///< 受信モードの設定
			bit_rw_t<T, bitpos::B13>		DAP;	///< CSIモードでのデータとクロックの位相選択
			bit_rw_t<T, bitpos::B12>		CKP;	///< CSIモードでのデータとクロックの位相選択

			bit_rw_t<T, bitpos::B10>		EOC;	///< エラー割り込み信号（INTSREx（x = 0-3））のマスク制御
			bits_rw_t<T, bitpos::B8, 2>	PTC;	///< UARTモードでのパリティ・ビットの設定
			bit_rw_t<T, bitpos::B7>		DIR;	///< CSI, UARTモードでのデータ転送順序の選択

			bits_rw_t<T, bitpos::B4, 2>	SLC;	///< UARTモードでのストップ・ビットの設定

			bits_rw_t<T, bitpos::B0, 2>	DLS;	///< CSI, UARTモードでのデータ長の設定
		};
		typedef scr_t< rw16_t<0xF0118 + UOFS + CHOFS> > SCR_;
		static SCR_ SCR;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  シリアル・データ・レジスタ（SDR）@n
					１６ビットアクセス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef rw16_t<0xFFF10 + SDR_O> SDR_;
		static SDR_ SDR;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  シリアル・データ・レジスタ（SDR_L）@n
					下位８ビットアクセス (B0 - B7)
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef rw8_t<0xFFF10 + SDR_O> SDR_L_;
		static SDR_L_ SDR_L;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  シリアル・フラグ・クリア・トリガ・レジスタ mn（SIR）テンプレート
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct sir_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B2>	FEC;	///< チャネルnのフレーミング・エラー・フラグのクリア・トリガ
			bit_rw_t<T, bitpos::B1>	PEC;	///< チャネルnのパリティ・エラー・フラグのクリア・トリガ
			bit_rw_t<T, bitpos::B0>	OVC;	///< チャネルnのオーバラン・エラー・フラグのクリア・トリガ
		};
		typedef sir_t< rw8_t<0xF0108 + UOFS + CHOFS> > SIR_;
		static SIR_ SIR;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  シリアル・ステータス・レジスタmn（SSR）テンプレート
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct ssr_t : public T {
			using T::operator ();

			bit_ro_t<T, bitpos::B6>	TSF;	///< チャネルnの通信状態表示フラグ
			bit_ro_t<T, bitpos::B5>	BFF;	///< チャネルnのバッファ・レジスタ状態表示フラグ

			bit_ro_t<T, bitpos::B2>	FEF;	///< チャネルnのフレーミング・エラー検出フラグ
			bit_ro_t<T, bitpos::B1>	PEF;	///< チャネルnのパリティ／ACKエラー検出フラグ
			bit_ro_t<T, bitpos::B0>	OVF;	///< チャネルnのオーバラン・エラー検出フラグ
		};
		typedef ssr_t< ro8_t<0xF0100 + UOFS + CHOFS> > SSR_;
		static SSR_ SSR;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  シリアル・チャネル開始レジスタ m（SS）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef bit_rw_t<rw8_t<0xF0122 + UOFS>, static_cast<bitpos>(CHOFS / 2)> SS_;
		static SS_ SS;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  シリアル・チャネル停止レジスタ m（ST）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef bit_rw_t<rw8_t<0xF0124 + UOFS>, static_cast<bitpos>(CHOFS / 2)> ST_;
		static ST_ ST;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  シリアル・チャネル許可ステータス・レジスタ m（SE）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef bit_ro_t<ro8_t<0xF0120 + UOFS>, static_cast<bitpos>(CHOFS / 2)> SE_;
		static SE_ SE;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  シリアル出力許可レジスタ m（SOE）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef bit_rw_t<rw8_t<0xF012A + UOFS>, static_cast<bitpos>(CHOFS / 2)> SOE_;
		static SOE_ SOE;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  シリアル出力レジスタ m（SO）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef bit_rw_t<rw16_t<0xF0128 + UOFS>, static_cast<bitpos>(CHOFS / 2)> SO_;
		static SO_ SO;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  シリアル・クロック・レジスタ m（CKO）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef bit_rw_t<rw16_t<0xF0128 + UOFS>, static_cast<bitpos>(8 + (CHOFS / 2))> CKO_;
		static CKO_ CKO;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  シリアル出力レベル・レジスタ m（SOL）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef bit_rw_t<rw8_t<0xF0134 + UOFS>, static_cast<bitpos>(CHOFS / 2)> SOL_;
		static SOL_ SOL;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  シリアル・スタンバイ・コントロール・レジスタ m（SSC）テンプレート @n
					※ユニット単位
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct ssc_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B1>	SSEC;
			bit_rw_t<T, bitpos::B0>	SWC;
		};
		typedef ssc_t<rw8_t<0xF0138 + UOFS> > SSC_;
		static SSC_ SSC;


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
			@return 「０～３」
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
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t SDR_O>
		typename sau_t<PER, UOFS, CHOFS, SDR_O>::SPS_ sau_t<PER, UOFS, CHOFS, SDR_O>::SPS;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t SDR_O>
		typename sau_t<PER, UOFS, CHOFS, SDR_O>::SMR_ sau_t<PER, UOFS, CHOFS, SDR_O>::SMR;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t SDR_O>
		typename sau_t<PER, UOFS, CHOFS, SDR_O>::SCR_ sau_t<PER, UOFS, CHOFS, SDR_O>::SCR;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t SDR_O>
		typename sau_t<PER, UOFS, CHOFS, SDR_O>::SDR_ sau_t<PER, UOFS, CHOFS, SDR_O>::SDR;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t SDR_O>
		typename sau_t<PER, UOFS, CHOFS, SDR_O>::SDR_L_ sau_t<PER, UOFS, CHOFS, SDR_O>::SDR_L;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t SDR_O>
		typename sau_t<PER, UOFS, CHOFS, SDR_O>::SIR_ sau_t<PER, UOFS, CHOFS, SDR_O>::SIR;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t SDR_O>
		typename sau_t<PER, UOFS, CHOFS, SDR_O>::SSR_ sau_t<PER, UOFS, CHOFS, SDR_O>::SSR;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t SDR_O>
		typename sau_t<PER, UOFS, CHOFS, SDR_O>::SS_ sau_t<PER, UOFS, CHOFS, SDR_O>::SS;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t SDR_O>
		typename sau_t<PER, UOFS, CHOFS, SDR_O>::ST_ sau_t<PER, UOFS, CHOFS, SDR_O>::ST;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t SDR_O>
		typename sau_t<PER, UOFS, CHOFS, SDR_O>::SE_ sau_t<PER, UOFS, CHOFS, SDR_O>::SE;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t SDR_O>
		typename sau_t<PER, UOFS, CHOFS, SDR_O>::SOE_ sau_t<PER, UOFS, CHOFS, SDR_O>::SOE;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t SDR_O>
		typename sau_t<PER, UOFS, CHOFS, SDR_O>::SO_ sau_t<PER, UOFS, CHOFS, SDR_O>::SO;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t SDR_O>
		typename sau_t<PER, UOFS, CHOFS, SDR_O>::CKO_ sau_t<PER, UOFS, CHOFS, SDR_O>::CKO;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t SDR_O>
		typename sau_t<PER, UOFS, CHOFS, SDR_O>::SOL_ sau_t<PER, UOFS, CHOFS, SDR_O>::SOL;
	template <peripheral PER, uint32_t UOFS, uint32_t CHOFS, uint32_t SDR_O>
		typename sau_t<PER, UOFS, CHOFS, SDR_O>::SSC_ sau_t<PER, UOFS, CHOFS, SDR_O>::SSC;


	typedef sau_t<peripheral::SAU00, 0x00, 0x00, 0x00> SAU00;
	typedef sau_t<peripheral::SAU01, 0x00, 0x02, 0x02> SAU01;
	typedef sau_t<peripheral::SAU02, 0x00, 0x04, 0x34> SAU02;
	typedef sau_t<peripheral::SAU03, 0x00, 0x06, 0x36> SAU03;

	typedef sau_t<peripheral::SAU10, 0x40, 0x00, 0x38> SAU10;
	typedef sau_t<peripheral::SAU11, 0x40, 0x02, 0x3A> SAU11;
	typedef sau_t<peripheral::SAU12, 0x40, 0x04, 0x04> SAU12;
	typedef sau_t<peripheral::SAU13, 0x40, 0x06, 0x06> SAU13;


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  シリアル・アレイ・ユニット・テンプレート（共有レジスタ）
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class _>
	struct sau_share_t {

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

			bit_rw_t<T, bitpos::B1>	ISC1;	///< タイマ・アレイ・ユニットのチャネル7の入力切り替え
			bit_rw_t<T, bitpos::B0>	ISC0;	///< 外部割り込み（INTP0）の入力切り替え
		};
		typedef isc_t<rw8_t<0xF0073> > ISC_;
		static ISC_ ISC;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  ノイズ・フィルタ許可レジスタ0（NFEN0）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef rw8_t<0xF0070> NFEN0_;
		static NFEN0_ NFEN0;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  ノイズ・フィルタ許可レジスタ0（SNFEN30）
					RxD3端子のノイズ・フィルタ使用可否
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef bit_rw_t<rw8_t<0xF0070>, bitpos::B6> SNFEN30_;
		static SNFEN30_ SNFEN30;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  ノイズ・フィルタ許可レジスタ0（SNFEN20）
					RxD2端子のノイズ・フィルタ使用可否
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef bit_rw_t<rw8_t<0xF0070>, bitpos::B4> SNFEN20_;
		static SNFEN20_ SNFEN20;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  ノイズ・フィルタ許可レジスタ0（SNFEN10）
					RxD1端子のノイズ・フィルタ使用可否
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef bit_rw_t<rw8_t<0xF0070>, bitpos::B2> SNFEN10_;
		static SNFEN10_ SNFEN10;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  ノイズ・フィルタ許可レジスタ0（SNFEN00）
					RxD0端子のノイズ・フィルタ使用可否
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef bit_rw_t<rw8_t<0xF0070>, bitpos::B0> SNFEN00_;
		static SNFEN00_ SNFEN00;
	};
	// テンプレート内、スタティック定義、実態：
	template<class _> typename sau_share_t<_>::ISC_ sau_share_t<_>::ISC;
	template<class _> typename sau_share_t<_>::NFEN0_ sau_share_t<_>::NFEN0;
	template<class _> typename sau_share_t<_>::SNFEN30_ sau_share_t<_>::SNFEN30;
	template<class _> typename sau_share_t<_>::SNFEN20_ sau_share_t<_>::SNFEN20;
	template<class _> typename sau_share_t<_>::SNFEN10_ sau_share_t<_>::SNFEN10;
	template<class _> typename sau_share_t<_>::SNFEN00_ sau_share_t<_>::SNFEN00;

	typedef sau_share_t<void> SAU;
}
