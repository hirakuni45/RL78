#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/L1C 割り込み機能を制御するレジスタ定義
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include "common/io_utils.hpp"
#include "L1C/peripheral.hpp"

namespace device {

	struct intr {

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  割り込み要求フラグ・レジスタ(IF0L)
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct if0l_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B7>  PIF5;   ///< ( 7) 端子入力エッジ検出５
			bit_rw_t<T, bitpos::B6>  PIF4;   ///< ( 6) 端子入力エッジ検出４
			bit_rw_t<T, bitpos::B5>  PIF3;   ///< ( 5) 端子入力エッジ検出３
			bit_rw_t<T, bitpos::B4>  PIF2;   ///< ( 4) 端子入力エッジ検出２
			bit_rw_t<T, bitpos::B3>  PIF1;   ///< ( 3) 端子入力エッジ検出１
			bit_rw_t<T, bitpos::B2>  PIF0;   ///< ( 2) 端子入力エッジ検出０
			bit_rw_t<T, bitpos::B1>  LVIIF;  ///< ( 1) 電圧検出 
			bit_rw_t<T, bitpos::B0>  WDTIIF; ///< ( 0) ウォッチドッグ・タイマのインターバル
		};
		static if0l_t< rw8_t<0xFFFE0> > IF0L;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  割り込み要求フラグ・レジスタ(IF0H)
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct if0h_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B7>  SRIF0;

			bit_rw_t<T, bitpos::B6>  TMIF00;

			bit_rw_t<T, bitpos::B5>  STIF0;
			bit_rw_t<T, bitpos::B5>  CSIIF00;
			bit_rw_t<T, bitpos::B5>  IICIF00;

			bit_rw_t<T, bitpos::B2>  SREIF2;

			bit_rw_t<T, bitpos::B1>  SRIF2;

			bit_rw_t<T, bitpos::B1>  IICIF21;

			bit_rw_t<T, bitpos::B0>  STIF2;
			bit_rw_t<T, bitpos::B0>  CSIIF20;
			bit_rw_t<T, bitpos::B0>  IICIF20;
		};
		static if0h_t< rw8_t<0xFFFE1> > IF0H;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  割り込み要求フラグ・レジスタ(IF1L)
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct if1l_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B7>  TMIF01;

			bit_rw_t<T, bitpos::B5>  RTITIF;
			bit_rw_t<T, bitpos::B4>  IICAIF0;

			bit_rw_t<T, bitpos::B3>  SREIF1;
			bit_rw_t<T, bitpos::B3>  TMIF03H;

			bit_rw_t<T, bitpos::B2>  SRIF1;

			bit_rw_t<T, bitpos::B1>  CSIIF10;
			bit_rw_t<T, bitpos::B1>  IICIF10;
			bit_rw_t<T, bitpos::B1>  STIF1;

			bit_rw_t<T, bitpos::B0>  SREIF0;
			bit_rw_t<T, bitpos::B0>  TMMK01H;
		};
		static if1l_t< rw8_t<0xFFFE2> > IF1L;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  割り込み要求フラグ・レジスタ(IF1H)
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct if1h_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B7>  SRIF3;

			bit_rw_t<T, bitpos::B6>  CSIIF30;
			bit_rw_t<T, bitpos::B6>  STIF3;
			bit_rw_t<T, bitpos::B6>  IICIF30;

			bit_rw_t<T, bitpos::B5>  KRIF;
			bit_rw_t<T, bitpos::B4>  TMKAIF;
			bit_rw_t<T, bitpos::B3>  RTCIF;
			bit_rw_t<T, bitpos::B2>  ADIF;
			bit_rw_t<T, bitpos::B1>  TMIF03;
			bit_rw_t<T, bitpos::B0>  TMIF02;
		};
		static if1h_t< rw8_t<0xFFFE3> > IF1H;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  割り込み要求フラグ・レジスタ(IF2L)
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct if2l_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B7>  CMPIF1;
			bit_rw_t<T, bitpos::B6>  CMPIF0;

			bit_rw_t<T, bitpos::B4>  PIF7;
			bit_rw_t<T, bitpos::B3>  PIF6;
			bit_rw_t<T, bitpos::B2>  TMIF05;
			bit_rw_t<T, bitpos::B1>  TMIF04;
		};
		static if2l_t< rw8_t<0xFFFD0> > IF2L;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  割り込み要求フラグ・レジスタ(IF2H)
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct if2h_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B7>  FLIF;
			bit_rw_t<T, bitpos::B6>  TKB2IF1;
			bit_rw_t<T, bitpos::B5>  TKB2IF0;
			bit_rw_t<T, bitpos::B4>  SREIF3;
			bit_rw_t<T, bitpos::B3>  RSUIF;
			bit_rw_t<T, bitpos::B2>  USBIF;
			bit_rw_t<T, bitpos::B1>  TMIF07;
			bit_rw_t<T, bitpos::B0>  TMIF06;
		};
		static if2h_t< rw8_t<0xFFFD1> > IF2H;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  割り込み要求フラグ・レジスタ(IF3L)
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct if3l_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B2>  FIFOIF1;
			bit_rw_t<T, bitpos::B1>  FIFOIF0;
			bit_rw_t<T, bitpos::B0>  TKB2IF2;
		};
		static if3l_t< rw8_t<0xFFFD2> > IF3L;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  割り込みマスク・フラグ・レジスタ(MK0L)
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct mk0l_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B7>  PMK5;
			bit_rw_t<T, bitpos::B6>  PMK4;
			bit_rw_t<T, bitpos::B5>  PMK3;
			bit_rw_t<T, bitpos::B4>  PMK2;
			bit_rw_t<T, bitpos::B3>  PMK1;
			bit_rw_t<T, bitpos::B2>  PMK0;
			bit_rw_t<T, bitpos::B1>  LVIMK;
			bit_rw_t<T, bitpos::B0>  WDTIMK;
		};
		static mk0l_t< rw8_t<0xFFFE4> > MK0L;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  割り込みマスク・フラグ・レジスタ(MK0H)
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct mk0h_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B7>  SRMK0;
			bit_rw_t<T, bitpos::B6>  TMMK00;

			bit_rw_t<T, bitpos::B5>  STMK0;
			bit_rw_t<T, bitpos::B5>  CSIMK00;
			bit_rw_t<T, bitpos::B5>  IICMK00;

			bit_rw_t<T, bitpos::B2>  SREMK2;
			bit_rw_t<T, bitpos::B1>  SRMK2;

			bit_rw_t<T, bitpos::B0>  STMK2;
			bit_rw_t<T, bitpos::B0>  CSIMK20;
			bit_rw_t<T, bitpos::B0>  IICMK20;
		};
		static mk0h_t< rw8_t<0xFFFE5> > MK0H;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  割り込み要求フラグ・レジスタ(MK1L)
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct mk1l_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B7>  TMMK01;

			bit_rw_t<T, bitpos::B5>  RTITMK;
			bit_rw_t<T, bitpos::B4>  IICAMK0;

			bit_rw_t<T, bitpos::B3>  SREMK1;
			bit_rw_t<T, bitpos::B3>  TMMK03H;

			bit_rw_t<T, bitpos::B2>  SRMK1;

			bit_rw_t<T, bitpos::B1>  CSIMK10;
			bit_rw_t<T, bitpos::B1>  IICMK10;
			bit_rw_t<T, bitpos::B1>  STMK1;

			bit_rw_t<T, bitpos::B0>  SREMK0;
			bit_rw_t<T, bitpos::B0>  TMMK01H;
		};
		static mk1l_t< rw8_t<0xFFFE6> > MK1L;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  割り込みマスク・フラグ・レジスタ(MK1H)
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct mk1h_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B7>  SRMK3;

			bit_rw_t<T, bitpos::B6>  CSIMK30;
			bit_rw_t<T, bitpos::B6>  IICMK30;
			bit_rw_t<T, bitpos::B6>  STMK3;

			bit_rw_t<T, bitpos::B5>  KRMK;
			bit_rw_t<T, bitpos::B4>  TMKAMK;
			bit_rw_t<T, bitpos::B3>  RTCMK;
			bit_rw_t<T, bitpos::B2>  ADMK;
			bit_rw_t<T, bitpos::B1>  TMMK03;
			bit_rw_t<T, bitpos::B0>  TMMK02;
		};
		static mk1h_t< rw8_t<0xFFFE7> > MK1H;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  割り込みマスク・フラグ・レジスタ(MK2L)
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct mk2l_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B7>  CMPMK1;
			bit_rw_t<T, bitpos::B6>  CMPMK0;

			bit_rw_t<T, bitpos::B4>  PMK7;
			bit_rw_t<T, bitpos::B3>  PMK6;
			bit_rw_t<T, bitpos::B2>  TMMK05;
			bit_rw_t<T, bitpos::B1>  TMMK04;
		};
		static mk2l_t< rw8_t<0xFFFD4> > MK2L;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  割り込みマスク・フラグ・レジスタ(MK2H)
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct mk2h_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B7>  FLMK;
			bit_rw_t<T, bitpos::B6>  TKB2MK1;
			bit_rw_t<T, bitpos::B5>  TKB2MK0;
			bit_rw_t<T, bitpos::B4>  SREMK3;
			bit_rw_t<T, bitpos::B3>  RSUMK;
			bit_rw_t<T, bitpos::B2>  USBMK;
			bit_rw_t<T, bitpos::B1>  TMMK07;
			bit_rw_t<T, bitpos::B0>  TMMK06;
		};
		static mk2h_t< rw8_t<0xFFFD5> > MK2H;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  割り込みマスク・フラグ・レジスタ(MK3L)
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct mk3l_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B2>  FIFOMK1;
			bit_rw_t<T, bitpos::B1>  FIFOMK0;
			bit_rw_t<T, bitpos::B0>  TKB2MK2;
		};
		static mk3l_t< rw8_t<0xFFFD6> > MK3L;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  優先順位指定フラグ・レジスタ(PRx0L)
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct prx0l_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B7>  PPR5;
			bit_rw_t<T, bitpos::B6>  PPR4;
			bit_rw_t<T, bitpos::B5>  PPR3;
			bit_rw_t<T, bitpos::B4>  PPR2;
			bit_rw_t<T, bitpos::B3>  PPR1;
			bit_rw_t<T, bitpos::B2>  PPR0;
			bit_rw_t<T, bitpos::B1>  LVIPR;
			bit_rw_t<T, bitpos::B0>  WDTIPR;
		};
		static prx0l_t< rw8_t<0xFFFE8> > PR00L;
		static prx0l_t< rw8_t<0xFFFEC> > PR10L;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief	優先順位指定フラグ・レジスタ(PRx0H)
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct prx0h_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B7>  SRPR0;
			bit_rw_t<T, bitpos::B6>  TMPR00;

			bit_rw_t<T, bitpos::B5>  STPR0;
			bit_rw_t<T, bitpos::B5>  CSIPR00;
			bit_rw_t<T, bitpos::B5>  IICPR00;

			bit_rw_t<T, bitpos::B2>  SREPR2;
			bit_rw_t<T, bitpos::B1>  SRPR2;

			bit_rw_t<T, bitpos::B0>  STPR2;
			bit_rw_t<T, bitpos::B0>  CSIPR20;
			bit_rw_t<T, bitpos::B0>  IICPR20;
		};
		static prx0h_t< rw8_t<0xFFFE9> > PR00H;
		static prx0h_t< rw8_t<0xFFFED> > PR10H;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  優先順位指定フラグ・レジスタ(PRx1L)
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct prx1l_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B7>  TMPR01;

			bit_rw_t<T, bitpos::B5>  RTITPR;
			bit_rw_t<T, bitpos::B4>  IICAPR0;

			bit_rw_t<T, bitpos::B3>  SREPR1;
			bit_rw_t<T, bitpos::B3>  TMPR03H;

			bit_rw_t<T, bitpos::B2>  SRPR1;

			bit_rw_t<T, bitpos::B1>  CSIPR10;
			bit_rw_t<T, bitpos::B1>  IICPR10;
			bit_rw_t<T, bitpos::B1>  STPR1;

			bit_rw_t<T, bitpos::B0>  SREPR0;
			bit_rw_t<T, bitpos::B0>  TMPR01H;
		};
		static prx1l_t< rw8_t<0xFFFEA> > PR01L;
		static prx1l_t< rw8_t<0xFFFEE> > PR11L;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  優先順位指定フラグ・レジスタ(PRx1H)
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct prx1h_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B7>  SRPR3;

			bit_rw_t<T, bitpos::B6>  CSIPR30;
			bit_rw_t<T, bitpos::B6>  IICPR30;
			bit_rw_t<T, bitpos::B6>  STPR3;

			bit_rw_t<T, bitpos::B5>  KRPR;
			bit_rw_t<T, bitpos::B4>  TMKAPR;
			bit_rw_t<T, bitpos::B3>  RTCPR;
			bit_rw_t<T, bitpos::B2>  ADPR;
			bit_rw_t<T, bitpos::B1>  TMPR03;
			bit_rw_t<T, bitpos::B0>  TMPR02;
		};
		static prx1h_t< rw8_t<0xFFFEB> > PR01H;
		static prx1h_t< rw8_t<0xFFFEF> > PR11H;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  優先順位指定フラグ・レジスタ(PRx2L)
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct prx2l_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B7>  CMPPR1;
			bit_rw_t<T, bitpos::B6>  CMPPR0;

			bit_rw_t<T, bitpos::B4>  PPR7;
			bit_rw_t<T, bitpos::B3>  PPR6;
			bit_rw_t<T, bitpos::B2>  TMPR05;
			bit_rw_t<T, bitpos::B1>  TMPR04;
		};
		static prx2l_t< rw8_t<0xFFFD8> > PR02L;
		static prx2l_t< rw8_t<0xFFFDC> > PR12L;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  優先順位指定フラグ・レジスタ(PRx2H)
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct prx2h_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B7>  FLPR;
			bit_rw_t<T, bitpos::B6>  TKB2PR1;
			bit_rw_t<T, bitpos::B5>  TKB2PR0;
			bit_rw_t<T, bitpos::B4>  SREPR3;
			bit_rw_t<T, bitpos::B3>  RSUPR;
			bit_rw_t<T, bitpos::B2>  USBPR;
			bit_rw_t<T, bitpos::B1>  TMPR07;
			bit_rw_t<T, bitpos::B0>  TMPR06;
		};
		static prx2h_t< rw8_t<0xFFFD9> > PR02H;
		static prx2h_t< rw8_t<0xFFFDD> > PR12H;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  優先順位指定フラグ・レジスタ(PRx3L)
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct prx3l_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, bitpos::B2>  FIFOPR1;
			bit_rw_t<T, bitpos::B1>  FIFOPR0;
			bit_rw_t<T, bitpos::B0>  TKB2PR2;
		};
		static prx3l_t< rw8_t<0xFFFDA> > PR03L;
		static prx3l_t< rw8_t<0xFFFDE> > PR13L;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  外部割り込み立ち上がりエッジ許可レジスタ（EGP0）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static basic_rw_t< rw8_t<0xFFF38> > EGP0;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  外部割り込み立ち下がりエッジ許可レジスタ（EGN0）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static basic_rw_t< rw8_t<0xFFF39> > EGN0;


		//-------------------------------------------------------------//
		/*!
			@brief  割り込み要求フラグの取得
			@param[in]	per	ペリフェラル型
			@return 割り込み発生なら「true」
		*/
		//-------------------------------------------------------------//
		static bool get_request(peripheral per)
		{
			switch(per) {
			case peripheral::ITM:
				return IF1H.TMKAIF();

			case peripheral::ADC:
				return IF1H.ADIF();

			case peripheral::SAU00:  // UART0-TX
				return IF0H.STIF0();
				break;
			case peripheral::SAU01:  // UART0-RX
				return IF0H.SRIF0();
				break;
			case peripheral::SAU02:  // UART1-TX
				return IF1L.STIF1();
				break;
			case peripheral::SAU03:  // UART1-RX
				return IF1L.SRIF1();
				break;
			case peripheral::SAU10:  // UART2-TX
				return IF0H.STIF2();
				break;
			case peripheral::SAU11:  // UART2-RX
				return IF0H.SRIF2();
				break;
			case peripheral::SAU12:  // UART3-TX
				return IF1H.STIF3();
				break;
			case peripheral::SAU13:  // UART3-RX
				return IF1H.SRIF3();
				break;

			default:
				return false;
			}
		}


		//-------------------------------------------------------------//
		/*!
			@brief  割り込み要求フラグの設定
			@param[in]	per	ペリフェラル型
			@return 割り込み発生なら「true」
		*/
		//-------------------------------------------------------------//
		static void set_request(peripheral per, bool ena)
		{
			switch(per) {
			case peripheral::ITM:
				IF1H.TMKAIF = ena;
				break;

			case peripheral::ADC:
				IF1H.ADIF = ena;
				break;

			case peripheral::SAU00:  // UART0-TX
				IF0H.STIF0 = ena;
				break;
			case peripheral::SAU01:  // UART0-RX
				IF0H.SRIF0 = ena;
				break;
			case peripheral::SAU02:  // UART1-TX
				IF1L.STIF1 = ena;
				break;
			case peripheral::SAU03:  // UART1-RX
				IF1L.SRIF1 = ena;
				break;
			case peripheral::SAU10:  // UART2-TX
				IF0H.STIF2 = ena;
				break;
			case peripheral::SAU11:  // UART2-RX
				IF0H.SRIF2 = ena;
				break;
			case peripheral::SAU12:  // UART3-TX
				IF1H.STIF3 = ena;
				break;
			case peripheral::SAU13:  // UART3-RX
				IF1H.SRIF3 = ena;
				break;

			default:
				break;
			}
		}


		//-------------------------------------------------------------//
		/*!
			@brief  割り込み許可
			@param[in]	per	ペリフェラル型
			@param[in]	ena	不許可なら「false」
		*/
		//-------------------------------------------------------------//
		static void enable(peripheral per, bool ena = true)
		{
			switch(per) {

			case peripheral::ITM:
				MK1H.TMKAMK = !ena;
				break;

			case peripheral::ADC:
				MK1H.ADMK = !ena;
				break;

			case peripheral::SAU00:  // UART0-TX
				MK0H.STMK0 = !ena;
				break;
			case peripheral::SAU01:  // UART0-RX
				MK0H.SRMK0 = !ena;
				break;
			case peripheral::SAU02:  // UART1-TX
				MK1L.STMK1 = !ena;
				break;
			case peripheral::SAU03:  // UART1-RX
				MK1L.SRMK1 = !ena;
				break;
			case peripheral::SAU10:  // UART2-TX
				MK0H.STMK2 = !ena;
				break;
			case peripheral::SAU11:  // UART2-RX
				MK0H.SRMK2 = !ena;
				break;
			case peripheral::SAU12:  // UART3-TX
				MK1H.STMK3 = !ena;
				break;
			case peripheral::SAU13:  // UART3-RX
				MK1H.SRMK3 = !ena;
				break;

			default:
				break;
			}
		}


		//-------------------------------------------------------------//
		/*!
			@brief  割り込みレベル設定
			@param[in]	per		ペリフェラル型
			@param[in]	level	割り込みレベル（０、１、２）
		*/
		//-------------------------------------------------------------//
		static void set_level(peripheral per, uint8_t level)
		{
			switch(per) {

			case peripheral::ITM:
				PR01H.TMKAPR = (level) & 1;
				PR11H.TMKAPR = (level & 2) >> 1;
				break;

			case peripheral::ADC:
				PR01H.ADPR = (level) & 1;
				PR11H.ADPR = (level & 2) >> 1;
				break;

			case peripheral::SAU00:  // UART0-TX
				PR00H.STPR0 = (level) & 1;
				PR10H.STPR0 = (level & 2) >> 1;
				break;
			case peripheral::SAU01:  // UART0-RX
				PR00H.SRPR0 = (level) & 1;
				PR10H.SRPR0 = (level & 2) >> 1;
				break;
			case peripheral::SAU02:  // UART1-TX
				PR01L.STPR1 = (level) & 1;
				PR11L.STPR1 = (level & 2) >> 1;
				break;
			case peripheral::SAU03:  // UART1-RX
				PR01L.SRPR1 = (level) & 1;
				PR11L.SRPR1 = (level & 2) >> 1;
				break;
			case peripheral::SAU10:  // UART2-TX
				PR00H.STPR2 = (level) & 1;
				PR10H.STPR2 = (level & 2) >> 1;
				break;
			case peripheral::SAU11:  // UART2-RX
				PR00H.SRPR2 = (level) & 1;
				PR10H.SRPR2 = (level & 2) >> 1;
				break;
			case peripheral::SAU12:  // UART3-TX
				PR01H.STPR3 = (level) & 1;
				PR11H.STPR3 = (level & 2) >> 1;
				break;
			case peripheral::SAU13:  // UART3-RX
				PR01H.SRPR3 = (level) & 1;
				PR11H.SRPR3 = (level & 2) >> 1;
				break;

			default:
				break;
			}
		}
	};
}
