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

			bit_rw_t<T, 7>  SREIF0;   ///< (15) UART0受信の通信エラー発生
			bit_rw_t<T, 7>  TMIF01H;  ///< (15) タイマ・チャネル01のカウント完了またはキャプチャ完了（上位8ビット・タイマ動作時）

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

			bit_rw_t<T, 7>  TMIF04;   ///< (31) タイマ・チャネル04のカウント完了またはキャプチャ完了
			bit_rw_t<T, 6>  TMIF13;   ///< (30) タイマ・チャネル13のカウント完了またはキャプチャ完了

			bit_rw_t<T, 5>  SRIF3;    ///< (29) UART3受信の転送完了
			bit_rw_t<T, 5>  CSIIF31;  ///< (29) CSI31の転送完了，バッファ空き割り込み
			bit_rw_t<T, 5>  IICIF31;  ///< (29) IIC31の転送完了

			bit_rw_t<T, 4>  STIF3;    ///< (28) UART3送信の転送完了、バッファ空き割り込み
			bit_rw_t<T, 4>  CSIIF30;  ///< (28) CSI30の転送完了，バッファ空き割り込み
			bit_rw_t<T, 4>  IICIF30;  ///< (28) IIC30の転送完了

			bit_rw_t<T, 3>  KRIF;     ///< (27) キー・リターン信号検出
			bit_rw_t<T, 2>  ITIF;     ///< (26) 12ビット・インターバル・タイマのインターバル信号検出
			bit_rw_t<T, 1>  RTCIF;    ///< (25) リアルタイム・クロックの定周期信号／アラーム一致検出
			bit_rw_t<T, 0>  ADIF;     ///< (24) A/D変換終了
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

			bit_rw_t<T, 7>  PIF10;   ///< (39) 端子入力エッジ検出１０
			bit_rw_t<T, 6>  PIF9;    ///< (38) 端子入力エッジ検出９
			bit_rw_t<T, 5>  PIF8;    ///< (37) 端子入力エッジ検出８
			bit_rw_t<T, 4>  PIF7;    ///< (36) 端子入力エッジ検出７
			bit_rw_t<T, 3>  PIF6;    ///< (35) 端子入力エッジ検出６
			bit_rw_t<T, 2>  TMIF07;  ///< (34) タイマ・チャネル07のカウント完了またはキャプチャ完了
			bit_rw_t<T, 1>  TMIF06;  ///< (33) タイマ・チャネル06のカウント完了またはキャプチャ完了
			bit_rw_t<T, 0>  TMIF05;  ///< (32) タイマ・チャネル05のカウント完了またはキャプチャ完了
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

			bit_rw_t<T, 7>  FLIF;     ///< (47) 予約
			bit_rw_t<T, 6>  IICAIF1;  ///< (46) IICA1通信完了
			bit_rw_t<T, 5>  MDIF;     ///< (45) 除算演算終了／積和演算結果のオーバフロー発生

			bit_rw_t<T, 4>  SREIF3;   ///< (44) UART3受信の通信エラー発生
			bit_rw_t<T, 4>  TMIF13H;  ///< (44) タイマ・チャネル13のカウント完了またはキャプチャ完了

			bit_rw_t<T, 3>  TMIF12;   ///< (43) タイマ・チャネル12のカウント完了またはキャプチャ完了
			bit_rw_t<T, 2>  TMIF11;   ///< (42) タイマ・チャネル11のカウント完了またはキャプチャ完了
			bit_rw_t<T, 1>  TMIF10;   ///< (41) タイマ・チャネル10のカウント完了またはキャプチャ完了
			bit_rw_t<T, 0>  PIF11;    ///< (40) 端子入力エッジ検出１１
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

			bit_rw_t<T, 5>  TMIF17;  ///< (53) タイマ・チャネル17のカウント完了またはキャプチャ完了
			bit_rw_t<T, 4>  TMIF16;  ///< (52) タイマ・チャネル16のカウント完了またはキャプチャ完了
			bit_rw_t<T, 3>  TMIF15;  ///< (51) タイマ・チャネル15のカウント完了またはキャプチャ完了
			bit_rw_t<T, 2>  TMIF14;  ///< (50) タイマ・チャネル14のカウント完了またはキャプチャ完了
			bit_rw_t<T, 1>  DMAIF3;  ///< (49) DMA3の転送完了
			bit_rw_t<T, 0>  DMAIF2;  ///< (48) DMA2の転送完了
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

			bit_rw_t<T, 7>  PMK5;    ///< ( 7)
			bit_rw_t<T, 6>  PMK4;    ///< ( 6)
			bit_rw_t<T, 5>  PMK3;    ///< ( 5)
			bit_rw_t<T, 4>  PMK2;    ///< ( 4)
			bit_rw_t<T, 3>  PMK1;    ///< ( 3)
			bit_rw_t<T, 2>  PMK0;    ///< ( 2)
			bit_rw_t<T, 1>  LVIMK;   ///< ( 1)
			bit_rw_t<T, 0>  WDTIMK;  ///< ( 0)
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

			bit_rw_t<T, 7>  SREMK0;   ///< (15)
			bit_rw_t<T, 7>  TMMK01H;  ///< (15)

			bit_rw_t<T, 6>  SRMK0;    ///< (14)
			bit_rw_t<T, 6>  CSIMK01;  ///< (14)
			bit_rw_t<T, 6>  IICMK01;  ///< (14)

			bit_rw_t<T, 5>  STMK0;    ///< (13)
			bit_rw_t<T, 5>  CSIMK00;  ///< (13)
			bit_rw_t<T, 5>  IICMK00;  ///< (13)

			bit_rw_t<T, 4>  DMAMK1;   ///< (12)
			bit_rw_t<T, 3>  DMAMK0;   ///< (11)

			bit_rw_t<T, 2>  SREMK2;   ///< (10)
			bit_rw_t<T, 2>  TMMK11H;  ///< (10)

			bit_rw_t<T, 1>  SRMK2;    ///< ( 9)
			bit_rw_t<T, 1>  CSIMK21;  ///< ( 9)
			bit_rw_t<T, 1>  IICMK21;  ///< ( 9)

			bit_rw_t<T, 0>  STMK2;    ///< ( 8)
			bit_rw_t<T, 0>  CSIMK20;  ///< ( 8)
			bit_rw_t<T, 0>  IICMK20;  ///< ( 8)
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

			bit_rw_t<T, 7>  TMMK03;   ///< (23)
			bit_rw_t<T, 6>  TMMK02;   ///< (22)
			bit_rw_t<T, 5>  TMMK01;   ///< (21)
			bit_rw_t<T, 4>  TMMK00;   ///< (20)
			bit_rw_t<T, 3>  IICAMK0;  ///< (19)

			bit_rw_t<T, 2>  SREMK1;   ///< (18)
			bit_rw_t<T, 2>  TMMK03H;  ///< (18)

			bit_rw_t<T, 1>  SRMK1;    ///< (17)
			bit_rw_t<T, 1>  CSIMK11;  ///< (17)
			bit_rw_t<T, 1>  IICMK11;  ///< (17)

			bit_rw_t<T, 0>  STMK1;    ///< (16)
			bit_rw_t<T, 0>  CSIMK10;  ///< (16)
			bit_rw_t<T, 0>  IICMK10;  ///< (16)
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

			bit_rw_t<T, 7>  TMMK04;   ///< (31)
			bit_rw_t<T, 6>  TMMK13;   ///< (30)

			bit_rw_t<T, 5>  SRMK3;    ///< (29)
			bit_rw_t<T, 5>  CSIMK31;  ///< (29)
			bit_rw_t<T, 5>  IICMK31;  ///< (29)

			bit_rw_t<T, 4>  STMK3;    ///< (28)
			bit_rw_t<T, 4>  CSIMK30;  ///< (28)
			bit_rw_t<T, 4>  IICMK30;  ///< (28)

			bit_rw_t<T, 3>  KRMK;     ///< (27)
			bit_rw_t<T, 2>  ITMK;     ///< (26)
			bit_rw_t<T, 1>  RTCMK;    ///< (25)
			bit_rw_t<T, 0>  ADMK;     ///< (24)
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

			bit_rw_t<T, 7>  PMK10;   ///< (39)
			bit_rw_t<T, 6>  PMK9;    ///< (38)
			bit_rw_t<T, 5>  PMK8;    ///< (37)
			bit_rw_t<T, 4>  PMK7;    ///< (36)
			bit_rw_t<T, 3>  PMK6;    ///< (35)
			bit_rw_t<T, 2>  TMMK07;  ///< (34)
			bit_rw_t<T, 1>  TMMK06;  ///< (33)
			bit_rw_t<T, 0>  TMMK05;  ///< (32)
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

			bit_rw_t<T, 7>  FLMK;     ///< (47)
			bit_rw_t<T, 6>  IICAMK1;  ///< (46)
			bit_rw_t<T, 5>  MDMK;     ///< (45)

			bit_rw_t<T, 4>  SREMK3;   ///< (44)
			bit_rw_t<T, 4>  TMMK13H;  ///< (44)

			bit_rw_t<T, 3>  TMMK12;   ///< (43)
			bit_rw_t<T, 2>  TMMK11;   ///< (42)
			bit_rw_t<T, 1>  TMMK10;   ///< (41)
			bit_rw_t<T, 0>  PMK11;    ///< (40)
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

			bit_rw_t<T, 5>  TMMK17;  ///< (53)
			bit_rw_t<T, 4>  TMMK16;  ///< (52)
			bit_rw_t<T, 3>  TMMK15;  ///< (51)
			bit_rw_t<T, 2>  TMMK14;  ///< (50)
			bit_rw_t<T, 1>  DMAMK3;  ///< (49)
			bit_rw_t<T, 0>  DMAMK2;  ///< (48)
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

			bit_rw_t<T, 7>  PPR5;    ///< ( 7)
			bit_rw_t<T, 6>  PPR4;    ///< ( 6)
			bit_rw_t<T, 5>  PPR3;    ///< ( 5)
			bit_rw_t<T, 4>  PPR2;    ///< ( 4)
			bit_rw_t<T, 3>  PPR1;    ///< ( 3)
			bit_rw_t<T, 2>  PPR0;    ///< ( 2)
			bit_rw_t<T, 1>  LVIPR;   ///< ( 1)
			bit_rw_t<T, 0>  WDTIPR;  ///< ( 0)
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

			bit_rw_t<T, 7>  SREPR0;   ///< (15)
			bit_rw_t<T, 7>  TMPR01H;  ///< (15)

			bit_rw_t<T, 6>  SRPR0;    ///< (14)
			bit_rw_t<T, 6>  CSIPR01;  ///< (14)
			bit_rw_t<T, 6>  IICPR01;  ///< (14)

			bit_rw_t<T, 5>  STPR0;    ///< (13)
			bit_rw_t<T, 5>  CSIPR00;  ///< (13)
			bit_rw_t<T, 5>  IICPR00;  ///< (13)

			bit_rw_t<T, 4>  DMAPR1;   ///< (12)
			bit_rw_t<T, 3>  DMAPR0;   ///< (11)

			bit_rw_t<T, 2>  SREPR2;   ///< (10)
			bit_rw_t<T, 2>  TMPR11H;  ///< (10)

			bit_rw_t<T, 1>  SRPR2;    ///< ( 9)
			bit_rw_t<T, 1>  CSIPR21;  ///< ( 9)
			bit_rw_t<T, 1>  IICPR21;  ///< ( 9)

			bit_rw_t<T, 0>  STPR2;    ///< ( 8)
			bit_rw_t<T, 0>  CSIPR20;  ///< ( 8)
			bit_rw_t<T, 0>  IICPR20;  ///< ( 8)
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

			bit_rw_t<T, 7>  TMPR03;   ///< (23)
			bit_rw_t<T, 6>  TMPR02;   ///< (22)
			bit_rw_t<T, 5>  TMPR01;   ///< (21)
			bit_rw_t<T, 4>  TMPR00;   ///< (20)
			bit_rw_t<T, 3>  IICAPR0;  ///< (19)

			bit_rw_t<T, 2>  SREPR1;   ///< (18)
			bit_rw_t<T, 2>  TMPR03H;  ///< (18)

			bit_rw_t<T, 1>  SRPR1;    ///< (17)
			bit_rw_t<T, 1>  CSIPR11;  ///< (17)
			bit_rw_t<T, 1>  IICPR11;  ///< (17)

			bit_rw_t<T, 0>  STPR1;    ///< (16)
			bit_rw_t<T, 0>  CSIPR10;  ///< (16)
			bit_rw_t<T, 0>  IICPR10;  ///< (16)
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

			bit_rw_t<T, 7>  TMPR04;   ///< (31)
			bit_rw_t<T, 6>  TMPR13;   ///< (30)

			bit_rw_t<T, 5>  SRPR3;    ///< (29)
			bit_rw_t<T, 5>  CSIPR31;  ///< (29)
			bit_rw_t<T, 5>  IICPR31;  ///< (29)

			bit_rw_t<T, 4>  STPR3;    ///< (28)
			bit_rw_t<T, 4>  CSIPR30;  ///< (28)
			bit_rw_t<T, 4>  IICPR30;  ///< (28)

			bit_rw_t<T, 3>  KRPR;     ///< (27)
			bit_rw_t<T, 2>  ITPR;     ///< (26)
			bit_rw_t<T, 1>  RTCPR;    ///< (25)
			bit_rw_t<T, 0>  ADPR;     ///< (24)
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

			bit_rw_t<T, 7>  PPR10;   ///< (39)
			bit_rw_t<T, 6>  PPR9;    ///< (38)
			bit_rw_t<T, 5>  PPR8;    ///< (37)
			bit_rw_t<T, 4>  PPR7;    ///< (36)
			bit_rw_t<T, 3>  PPR6;    ///< (35)
			bit_rw_t<T, 2>  TMPR07;  ///< (34)
			bit_rw_t<T, 1>  TMPR06;  ///< (33)
			bit_rw_t<T, 0>  TMPR05;  ///< (32)
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

			bit_rw_t<T, 7>  FLPR;     ///< (47)
			bit_rw_t<T, 6>  IICAPR1;  ///< (46)
			bit_rw_t<T, 5>  MDPR;     ///< (45)

			bit_rw_t<T, 4>  SREPR3;   ///< (44)
			bit_rw_t<T, 4>  TMPR13H;  ///< (44)

			bit_rw_t<T, 3>  TMPR12;   ///< (43)
			bit_rw_t<T, 2>  TMPR11;   ///< (42)
			bit_rw_t<T, 1>  TMPR10;   ///< (41)
			bit_rw_t<T, 0>  PPR11;    ///< (40)
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

			bit_rw_t<T, 5>  TMPR17;  ///< (53)
			bit_rw_t<T, 4>  TMPR16;  ///< (52)
			bit_rw_t<T, 3>  TMPR15;  ///< (51)
			bit_rw_t<T, 2>  TMPR14;  ///< (50)
			bit_rw_t<T, 1>  DMAPR3;  ///< (49)
			bit_rw_t<T, 0>  DMAPR2;  ///< (48)
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
