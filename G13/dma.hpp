#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/G13 DMA コントローラー定義
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016, 2021 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include "common/io_utils.hpp"

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  DMA コントローラー・テンプレート
		@param[in]	BASE	ベース
		@param[in]	OFS		オフセット
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <peripheral PER, uint32_t BASE, uint32_t OFS>
	struct dma_t {

		static constexpr auto PERIPHERAL = PER;

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  SFR アドレス・レジスタ（DSA）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef rw8_t<BASE + OFS> DSA_;
		static DSA_ DSA;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  RAM アドレス・レジスタ（DRA）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef rw16_t<BASE + 0x2 + (OFS * 2)> DRA_;
		static DRA_ DRA;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief	バイト・カウント・レジスタ（DBC）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef rw16_t<BASE + 0x6 + (OFS * 2)> DBC_;
		static DBC_ DBC;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  DMAモード・コントロール・レジスタ（DMC）
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct dmc_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B7>	    STG;    ///< DMA転送開始ソフトウエア・トリガ
			bit_rw_t<T, bitpos::B6>	    DRS;    ///< DMA転送方向の選択
			bit_rw_t<T, bitpos::B5>	    DS;     ///< DMA転送での転送データ・サイズの指定
			bit_rw_t<T, bitpos::B4>	    DWAIT;  ///< DMA転送の保留
			bits_rw_t<T, bitpos::B0, 4>	IFC;    ///< DMA起動要因の選択
		};
		typedef dmc_t< rw8_t<BASE + 0xa + OFS> > DMC_;
		static DMC_ DMC;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  DMA動作コントロール・レジスタ（DRC）
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct drc_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B7>	DEN;  ///< DMA動作許可フラグ
			bit_rw_t<T, bitpos::B0>	DST;  ///< DMA転送モード・フラグ
		};
		typedef drc_t< rw8_t<BASE + 0xc + OFS> > DRC_;
		static DRC_ DRC;
	};
	// テンプレート内、スタティック定義、実態：
	template <peripheral PER, uint32_t BASE, uint32_t OFS>
		typename dma_t<PER, BASE, OFS>::DSA_ dma_t<PER, BASE, OFS>::DSA;
	template <peripheral PER, uint32_t BASE, uint32_t OFS>
		typename dma_t<PER, BASE, OFS>::DRA_ dma_t<PER, BASE, OFS>::DRA;
	template <peripheral PER, uint32_t BASE, uint32_t OFS>
		typename dma_t<PER, BASE, OFS>::DBC_ dma_t<PER, BASE, OFS>::DBC;
	template <peripheral PER, uint32_t BASE, uint32_t OFS>
		typename dma_t<PER, BASE, OFS>::DMC_ dma_t<PER, BASE, OFS>::DMC;
	template <peripheral PER, uint32_t BASE, uint32_t OFS>
		typename dma_t<PER, BASE, OFS>::DRC_ dma_t<PER, BASE, OFS>::DRC;

	typedef dma_t<peripheral::DMA0, 0xFFFB0, 0> DMA0;
	typedef dma_t<peripheral::DMA1, 0xFFFB0, 1> DMA1;
	typedef dma_t<peripheral::DMA2, 0xF0200, 0> DMA2;
	typedef dma_t<peripheral::DMA3, 0xF0200, 1> DMA3;
}
