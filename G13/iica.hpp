#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/G13 グループ・シリアル・インターフェース IICA 定義 @n
			Copyright 2016 Kunihito Hiramatsu
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include "common/io_utils.hpp"

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  シリアル・インターフェース IICA クラス
		@param[in]	UOFS	ユニット・オフセット（0x00、0x08）
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <uint32_t UOFS>
	struct iica_t {

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  コントロール・レジスタ０（IICCTL0）
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct iicctl0_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, 7> IICE;  ///< I2C の動作許可
			bit_rw_t<T, 6> LREL;  ///< 通信退避
			bit_rw_t<T, 5> WREL;  ///< ウエイト解除
			bit_rw_t<T, 4> SPIE;  ///< ストップ・コンディション検出による割り込み要求発生の許可／禁止
			bit_rw_t<T, 3> WTIM;  ///< ウエイトおよび割り込み要求発生の制御
			bit_rw_t<T, 2> ACKE;  ///< アクノリッジ制御
			bit_rw_t<T, 1> STT;   ///< スタート・コンディション・トリガ
			bit_rw_t<T, 0> SPT;   ///< ストップ・コンディション・トリガ
		};
		static iicctl0_t< rw8_t<0xF0230 + UOFS> > IICCTL0;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  ステータス・レジスタ（IICS）
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct iics_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, 7> MSTS;  ///< マスタ状態確認フラグ
			bit_rw_t<T, 6> ALD;   ///< アービトレーション負け検出
			bit_rw_t<T, 5> EXC;   ///< 拡張コード受信検出
			bit_rw_t<T, 4> COI;   ///< アドレス一致検出
			bit_rw_t<T, 3> TRC;   ///< 送信／受信状態検出
			bit_rw_t<T, 2> ACKD;  ///< アクノリッジ（ACK）検出
			bit_rw_t<T, 1> STD;   ///< スタート・コンディション検出
			bit_rw_t<T, 0> SPD;   ///< ストップ・コンディション検出
		};
		static iics_t< rw8_t<0xFFF51 + (UOFS / 2)> > IICS;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  フラグ・レジスタ（IICF）
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct iicf_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, 7> STCF;    ///< STTnクリア・フラグ
			bit_rw_t<T, 6> IICBY;   ///< I2C バス状態フラグ
			bit_rw_t<T, 1> STCEN;   ///< 初期スタート許可トリガ
			bit_rw_t<T, 0> IICRSV;  ///< 通信予約機能禁止ビット
		};
		static iicf_t< rw8_t<0xFFF52 + (UOFS / 2)> > IICF;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  コントロール・レジスタ１（IICCTL1）
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct iicctl1_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, 7> WUP;  ///< アドレス一致ウエイク・アップの制御
			bit_rw_t<T, 5> CLD;  ///< SCLAn端子のレベル検出（IICEn = 1のときのみ有効）
			bit_rw_t<T, 4> DAD;  ///< SDAAn端子のレベル検出（IICEn = 1のときのみ有効）
			bit_rw_t<T, 3> SMC;  ///< 動作モードの切り替え
			bit_rw_t<T, 2> DFC;  ///< デジタル・フィルタの動作の制御
			bit_rw_t<T, 0> PRS;  ///< IICA動作クロック（f MCK ）の制御
		};
		static iicctl1_t< rw8_t<0xF0231 + UOFS> > IICCTL1;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  ロウ・レベル幅設定レジスタ（IICWL）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static rw8_t<0xF0232 - (UOFS * 64) + UOFS> IICWL;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  ハイ・レベル幅設定レジスタ（IICWH）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static rw8_t<0xF0233 - (UOFS * 64) + UOFS> IICWH;


        //-----------------------------------------------------------------//
        /*!
            @brief  ユニット番号を取得
			@return 「０」又は「１」
        */
        //-----------------------------------------------------------------//
		uint8_t get_unit_no() const { return UOFS / 0x08; }
	};

	typedef iica_t<0x00> IICA0;
	typedef iica_t<0x08> IICA1;
}
