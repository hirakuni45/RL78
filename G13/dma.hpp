#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/G13 DMA コントローラー定義 @n
			Copyright 2016 Kunihito Hiramatsu
	@author	平松邦仁 (hira@rvf-rc45.net)
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
	template <uint32_t BASE, uint32_t OFS>
	struct dma_t {

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  SFR アドレス・レジスタ（DSA）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static rw8_t<BASE + OFS> DSA;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  RAM アドレス・レジスタ（DRA）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static rw16_t<BASE + 0x2 + (OFS * 2)> DRA;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief	バイト・カウント・レジスタ（DBC）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static rw16_t<BASE + 0x6 + (OFS * 2)> DBC;


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
		static dmc_t< rw8_t<BASE + 0xa + OFS> > DMC;


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
		static drc_t< rw8_t<BASE + 0xc + OFS> > DRC;
	};

	typedef dma_t<0xFFFB0, 0> DMA0;
	typedef dma_t<0xFFFB0, 1> DMA1;
	typedef dma_t<0xF0200, 0> DMA2;
	typedef dma_t<0xF0200, 1> DMA3;
}
