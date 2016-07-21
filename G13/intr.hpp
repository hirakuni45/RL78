#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/G13 割り込み機能を制御するレジスタ定義 @n
			Copyright 2016 Kunihito Hiramatsu
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include "common/io_utils.hpp"

namespace device {

	namespace intr {

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

			bit_rw_t<T, 7>  PIF5;   ///< ( 7) 端子入力エッジ検出５
			bit_rw_t<T, 6>  PIF4;   ///< ( 6) 端子入力エッジ検出４
			bit_rw_t<T, 5>  PIF3;   ///< ( 5) 端子入力エッジ検出３
			bit_rw_t<T, 4>  PIF2;   ///< ( 4) 端子入力エッジ検出２
			bit_rw_t<T, 3>  PIF1;   ///< ( 3) 端子入力エッジ検出１
			bit_rw_t<T, 2>  PIF0;   ///< ( 2) 端子入力エッジ検出０
			bit_rw_t<T, 1>  LVIIF;  ///< ( 1) 電圧検出 
			bit_rw_t<T, 0>  WDTIIF;	///< ( 0) ウォッチドッグ・タイマのインターバル
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

			bit_rw_t<T, 7>  SREIF0;   ///< UART0受信の通信エラー発生
			bit_rw_t<T, 7>  TMIF01H;  ///< タイマ・チャネル01のカウント完了またはキャプチャ完了（上位8ビット・タイマ動作時）

			bit_rw_t<T, 6>  SRIF0;    ///< (14) UART0受信の転送完了
			bit_rw_t<T, 6>  CSIIF01;  ///< (14) CSI01の転送完了，バッファ空き割り込み
			bit_rw_t<T, 6>  IICIF01;  ///< (14) IIC01の転送完了

			bit_rw_t<T, 5>  STIF0;    ///< (13) UART0送信の転送完了，バッファ空き割り込み
			bit_rw_t<T, 5>  CSIIF00;  ///< (13) CSI00の転送完了，バッファ空き割り込み
			bit_rw_t<T, 5>  IICIF00;  ///< (13) IIC00の転送完了

			bit_rw_t<T, 4>  DMAIF1;   ///< (12) DMA1の転送完了
			bit_rw_t<T, 3>  DMAIF0;   ///< (11) DMA0の転送完了

			bit_rw_t<T, 2>  SREIF2;   ///< (10) UART2受信の通信エラー発生
			bit_rw_t<T, 2>  TMIF11H;  ///< (10) タイマ・チャネル11のカウント完了またはキャプチャ完了（上位8ビット・タイマ動作時）

			bit_rw_t<T, 1>  SRIF2;    ///< ( 9) UART2受信の転送完了
			bit_rw_t<T, 1>  CSIIF21;  ///< ( 9) CSI21の転送完了，バッファ空き割り込み
			bit_rw_t<T, 1>  IICIF21;  ///< ( 9) IIC21の転送完了

			bit_rw_t<T, 0>  STIF2;    ///< ( 8) UART2送信の転送完了，バッファ空き割り込み
			bit_rw_t<T, 0>  CSIIF20;  ///< ( 8) CSI20の転送完了，バッファ空き割り込み
			bit_rw_t<T, 0>  IICIF20;  ///< ( 8) IIC20の転送完了
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

			bit_rw_t<T, 7>  TMIF03;   ///< (23) タイマ・チャネル03のカウント完了またはキャプチャ完了（16ビット／下位8ビット・タイマ動作時）
			bit_rw_t<T, 6>  TMIF02;   ///< (22) タイマ・チャネル02のカウント完了またはキャプチャ完了
			bit_rw_t<T, 5>  TMIF01;   ///< (21) タイマ・チャネル01のカウント完了またはキャプチャ完了（16ビット／下位8ビット・タイマ動作時）
			bit_rw_t<T, 4>  TMIF00;   ///< (20) タイマ・チャネル00のカウント完了またはキャプチャ完了
			bit_rw_t<T, 3>  IICAIF0;  ///< (19) IICA0通信完了

			bit_rw_t<T, 2>  SREIF1;   ///< (18) UART1受信の通信エラー発生
			bit_rw_t<T, 2>  TMIF03H;  ///< (18) タイマ・チャネル03のカウント完了またはキャプチャ完了（上位8ビット・タイマ動作時）

			bit_rw_t<T, 1>  SRIF1;    ///< (17) UART1受信の転送完了
			bit_rw_t<T, 1>  CSIIF11;  ///< (17) CSI11の転送完了，バッファ空き割り込み
			bit_rw_t<T, 1>  IICIF11;  ///< (17) IIC11の転送完了

			bit_rw_t<T, 0>  STIF1;    ///< (16) UART1送信の転送完了，バッファ空き割り込み
			bit_rw_t<T, 0>  CSIIF10;  ///< (16) CSI10の転送完了，バッファ空き割り込み
			bit_rw_t<T, 0>  IICIF10;  ///< (16) IIC10の転送完了
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

			bit_rw_t<T, 7>  TMIF04;
			bit_rw_t<T, 6>  TMIF13;

			bit_rw_t<T, 5>  SRIF3;
			bit_rw_t<T, 5>  CSIIF31;
			bit_rw_t<T, 5>  IICIF31;

			bit_rw_t<T, 4>  STIF3;
			bit_rw_t<T, 4>  CSIIF30;
			bit_rw_t<T, 4>  IICIF30;

			bit_rw_t<T, 3>  KRIF;     ///< (27)
			bit_rw_t<T, 2>  ITIF;     ///< (26)
			bit_rw_t<T, 1>  RTCIF;    ///< (25)
			bit_rw_t<T, 0>  ADIF;     ///< (24)
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

			bit_rw_t<T, 7>  PIF10;
			bit_rw_t<T, 6>  PIF9;
			bit_rw_t<T, 5>  PIF8;
			bit_rw_t<T, 4>  PIF7;
			bit_rw_t<T, 3>  PIF6;
			bit_rw_t<T, 2>  TMIF07;
			bit_rw_t<T, 1>  TMIF06;
			bit_rw_t<T, 0>  TMIF05;
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

			bit_rw_t<T, 7>  FLIF;
			bit_rw_t<T, 6>  IICAIF1;
			bit_rw_t<T, 5>  MDIF;

			bit_rw_t<T, 4>  SREIF3;
			bit_rw_t<T, 4>  TMIF13H;

			bit_rw_t<T, 3>  TMIF12;
			bit_rw_t<T, 2>  TMIF11;
			bit_rw_t<T, 1>  TMIF10;
			bit_rw_t<T, 0>  PIF11;
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

			bit_rw_t<T, 5>  TMIF17;
			bit_rw_t<T, 4>  TMIF16;
			bit_rw_t<T, 3>  TMIF15;
			bit_rw_t<T, 2>  TMIF14;
			bit_rw_t<T, 1>  DMAIF3;
			bit_rw_t<T, 0>  DMAIF2;
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

			bit_rw_t<T, 7>  PMK5;
			bit_rw_t<T, 6>  PMK4;
			bit_rw_t<T, 5>  PMK3;
			bit_rw_t<T, 4>  PMK2;
			bit_rw_t<T, 3>  PMK1;
			bit_rw_t<T, 2>  PMK0;
			bit_rw_t<T, 1>  LVIMK;
			bit_rw_t<T, 0>  WDTIMK;
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

			bit_rw_t<T, 7>  SREMK0;
			bit_rw_t<T, 7>  TMMK01H;

			bit_rw_t<T, 6>  SRMK0;
			bit_rw_t<T, 6>  CSIMK01;
			bit_rw_t<T, 6>  IICMK01;

			bit_rw_t<T, 5>  STMK0;
			bit_rw_t<T, 5>  CSIMK00;
			bit_rw_t<T, 5>  IICMK00;

			bit_rw_t<T, 4>  DMAMK1;
			bit_rw_t<T, 3>  DMAMK0;

			bit_rw_t<T, 2>  SREMK2;
			bit_rw_t<T, 2>  TMMK11H;

			bit_rw_t<T, 1>  SRMK2;
			bit_rw_t<T, 1>  CSIMK21;
			bit_rw_t<T, 1>  IICMK21;

			bit_rw_t<T, 0>  STMK2;
			bit_rw_t<T, 0>  CSIMK20;
			bit_rw_t<T, 0>  IICMK20;
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

			bit_rw_t<T, 7>  TMMK03;
			bit_rw_t<T, 6>  TMMK02;
			bit_rw_t<T, 5>  TMMK01;
			bit_rw_t<T, 4>  TMMK00;
			bit_rw_t<T, 3>  IICAMK0;

			bit_rw_t<T, 2>  SREMK1;
			bit_rw_t<T, 2>  TMMK03H;

			bit_rw_t<T, 1>  SRMK1;
			bit_rw_t<T, 1>  CSIMK11;
			bit_rw_t<T, 1>  IICMK11;

			bit_rw_t<T, 0>  STMK1;
			bit_rw_t<T, 0>  CSIMK10;
			bit_rw_t<T, 0>  IICMK10;
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

			bit_rw_t<T, 7>  TMMK04;
			bit_rw_t<T, 6>  TMMK13;

			bit_rw_t<T, 5>  SRMK3;
			bit_rw_t<T, 5>  CSIMK31;
			bit_rw_t<T, 5>  IICMK31;

			bit_rw_t<T, 4>  STMK3;
			bit_rw_t<T, 4>  CSIMK30;
			bit_rw_t<T, 4>  IICMK30;

			bit_rw_t<T, 3>  KRMK;
			bit_rw_t<T, 2>  ITMK;
			bit_rw_t<T, 1>  RTCMK;
			bit_rw_t<T, 0>  ADMK;
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

			bit_rw_t<T, 7>  PMK10;
			bit_rw_t<T, 6>  PMK9;
			bit_rw_t<T, 5>  PMK8;
			bit_rw_t<T, 4>  PMK7;
			bit_rw_t<T, 3>  PMK6;
			bit_rw_t<T, 2>  TMMK07;
			bit_rw_t<T, 1>  TMMK06;
			bit_rw_t<T, 0>  TMMK05;
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

			bit_rw_t<T, 7>  FLMK;
			bit_rw_t<T, 6>  IICAMK1;
			bit_rw_t<T, 5>  MDMK;

			bit_rw_t<T, 4>  SREMK3;
			bit_rw_t<T, 4>  TMMK13H;

			bit_rw_t<T, 3>  TMMK12;
			bit_rw_t<T, 2>  TMMK11;
			bit_rw_t<T, 1>  TMMK10;
			bit_rw_t<T, 0>  PMK11;
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

			bit_rw_t<T, 5>  TMMK17;
			bit_rw_t<T, 4>  TMMK16;
			bit_rw_t<T, 3>  TMMK15;
			bit_rw_t<T, 2>  TMMK14;
			bit_rw_t<T, 1>  DMAMK3;
			bit_rw_t<T, 0>  DMAMK2;
		};
		static mk3l_t< rw8_t<0xFFFD6> > MK3L;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  優先順位指定フラグ・レジスタ(PRx0L)
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct pr0l_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, 7>  PPR5;
			bit_rw_t<T, 6>  PPR4;
			bit_rw_t<T, 5>  PPR3;
			bit_rw_t<T, 4>  PPR2;
			bit_rw_t<T, 3>  PPR1;
			bit_rw_t<T, 2>  PPR0;
			bit_rw_t<T, 1>  LVIPR;
			bit_rw_t<T, 0>  WDTIPR;
		};
		static pr0l_t< rw8_t<0xFFFE8> > PR00L;
		static pr0l_t< rw8_t<0xFFFEC> > PR10L;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief	優先順位指定フラグ・レジスタ(PRx0H)
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct pr0h_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, 7>  SREPR0;
			bit_rw_t<T, 7>  TMPR01H;

			bit_rw_t<T, 6>  SRPR0;
			bit_rw_t<T, 6>  CSIPR01;
			bit_rw_t<T, 6>  IICPR01;

			bit_rw_t<T, 5>  STPR0;
			bit_rw_t<T, 5>  CSIPR00;
			bit_rw_t<T, 5>  IICPR00;

			bit_rw_t<T, 4>  DMAPR1;
			bit_rw_t<T, 3>  DMAPR0;

			bit_rw_t<T, 2>  SREPR2;
			bit_rw_t<T, 2>  TMPR11H;

			bit_rw_t<T, 1>  SRPR2;
			bit_rw_t<T, 1>  CSIPR21;
			bit_rw_t<T, 1>  IICPR21;

			bit_rw_t<T, 0>  STPR2;
			bit_rw_t<T, 0>  CSIPR20;
			bit_rw_t<T, 0>  IICPR20;
		};
		static pr0h_t< rw8_t<0xFFFE9> > PR00H;
		static pr0h_t< rw8_t<0xFFFED> > PR10H;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  優先順位指定フラグ・レジスタ(PR1L)
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct pr1l_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, 7>  TMPR03;
			bit_rw_t<T, 6>  TMPR02;
			bit_rw_t<T, 5>  TMPR01;
			bit_rw_t<T, 4>  TMPR00;
			bit_rw_t<T, 3>  IICAPR0;

			bit_rw_t<T, 2>  SREPR1;
			bit_rw_t<T, 2>  TMPR03H;

			bit_rw_t<T, 1>  SRPR1;
			bit_rw_t<T, 1>  CSIPR11;
			bit_rw_t<T, 1>  IICPR11;

			bit_rw_t<T, 0>  STPR1;
			bit_rw_t<T, 0>  CSIPR10;
			bit_rw_t<T, 0>  IICPR10;
		};
		static pr1l_t< rw8_t<0xFFFEA> > PR01L;
		static pr1l_t< rw8_t<0xFFFEE> > PR11L;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  優先順位指定フラグ・レジスタ(PRx1H)
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct pr1h_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, 7>  TMPR04;
			bit_rw_t<T, 6>  TMPR13;

			bit_rw_t<T, 5>  SRPR3;
			bit_rw_t<T, 5>  CSIPR31;
			bit_rw_t<T, 5>  IICPR31;

			bit_rw_t<T, 4>  STPR3;
			bit_rw_t<T, 4>  CSIPR30;
			bit_rw_t<T, 4>  IICPR30;

			bit_rw_t<T, 3>  KRPR;
			bit_rw_t<T, 2>  ITPR;
			bit_rw_t<T, 1>  RTCPR;
			bit_rw_t<T, 0>  ADPR;
		};
		static pr1h_t< rw8_t<0xFFFEB> > PR01H;
		static pr1h_t< rw8_t<0xFFFEF> > PR11H;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  優先順位指定フラグ・レジスタ(PRx2L)
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct pr2l_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, 7>  PPR10;
			bit_rw_t<T, 6>  PPR9;
			bit_rw_t<T, 5>  PPR8;
			bit_rw_t<T, 4>  PPR7;
			bit_rw_t<T, 3>  PPR6;
			bit_rw_t<T, 2>  TMPR07;
			bit_rw_t<T, 1>  TMPR06;
			bit_rw_t<T, 0>  TMPR05;
		};
		static pr2l_t< rw8_t<0xFFFD8> > PR02L;
		static pr2l_t< rw8_t<0xFFFDC> > PR12L;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  優先順位指定フラグ・レジスタ(PRx2H)
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct pr2h_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, 7>  FLPR;
			bit_rw_t<T, 6>  IICAPR1;
			bit_rw_t<T, 5>  MDPR;

			bit_rw_t<T, 4>  SREPR3;
			bit_rw_t<T, 4>  TMPR13H;

			bit_rw_t<T, 3>  TMPR12;
			bit_rw_t<T, 2>  TMPR11;
			bit_rw_t<T, 1>  TMPR10;
			bit_rw_t<T, 0>  PPR11;
		};
		static pr2h_t< rw8_t<0xFFFD9> > PR02H;
		static pr2h_t< rw8_t<0xFFFDD> > PR12H;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  優先順位指定フラグ・レジスタ(PRx3L)
			@param[in]	T	アクセス・クラス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <class T>
		struct pr3l_t : public T {
			using T::operator =;
			using T::operator ();
			using T::operator |=;
			using T::operator &=;

			bit_rw_t<T, 5>  TMPR17;
			bit_rw_t<T, 4>  TMPR16;
			bit_rw_t<T, 3>  TMPR15;
			bit_rw_t<T, 2>  TMPR14;
			bit_rw_t<T, 1>  DMAPR3;
			bit_rw_t<T, 0>  DMAPR2;
		};
		static pr3l_t< rw8_t<0xFFFDA> > PR03L;
		static pr3l_t< rw8_t<0xFFFDE> > PR13L;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  外部割り込み立ち上がりエッジ許可レジスタ（EGP0, EGP1）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static basic_rw_t< rw8_t<0xFFF38> > EGP0;
		static basic_rw_t< rw8_t<0xFFF3A> > EGP1;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  外部割り込み立ち下がりエッジ許可レジスタ（EGN0, EGN1）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static basic_rw_t< rw8_t<0xFFF39> > EGN0;
		static basic_rw_t< rw8_t<0xFFF3B> > EGN1;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  キー・リターン・モード・レジスタ（KRM）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static basic_rw_t< rw8_t<0xFFF37> > KRM;
	}
}
