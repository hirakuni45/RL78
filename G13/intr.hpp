#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/G13 割り込み機能を制御するレジスタ定義
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016, 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include "common/io_utils.hpp"
#include "G13/peripheral.hpp"

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

			bit_rw_t<T, bitpos::B7>  SREIF0;   ///< (15) UART0受信の通信エラー発生
			bit_rw_t<T, bitpos::B7>  TMIF01H;  ///< (15) タイマ・チャネル01のカウント完了またはキャプチャ完了（上位8ビット・タイマ動作時）

			bit_rw_t<T, bitpos::B6>  SRIF0;    ///< (14) UART0受信の転送完了
			bit_rw_t<T, bitpos::B6>  CSIIF01;  ///< (14) CSI01の転送完了，バッファ空き割り込み
			bit_rw_t<T, bitpos::B6>  IICIF01;  ///< (14) IIC01の転送完了

			bit_rw_t<T, bitpos::B5>  STIF0;    ///< (13) UART0送信の転送完了，バッファ空き割り込み
			bit_rw_t<T, bitpos::B5>  CSIIF00;  ///< (13) CSI00の転送完了，バッファ空き割り込み
			bit_rw_t<T, bitpos::B5>  IICIF00;  ///< (13) IIC00の転送完了

			bit_rw_t<T, bitpos::B4>  DMAIF1;   ///< (12) DMA1の転送完了
			bit_rw_t<T, bitpos::B3>  DMAIF0;   ///< (11) DMA0の転送完了

			bit_rw_t<T, bitpos::B2>  SREIF2;   ///< (10) UART2受信の通信エラー発生
			bit_rw_t<T, bitpos::B2>  TMIF11H;  ///< (10) タイマ・チャネル11のカウント完了またはキャプチャ完了（上位8ビット・タイマ動作時）

			bit_rw_t<T, bitpos::B1>  SRIF2;    ///< ( 9) UART2受信の転送完了
			bit_rw_t<T, bitpos::B1>  CSIIF21;  ///< ( 9) CSI21の転送完了，バッファ空き割り込み
			bit_rw_t<T, bitpos::B1>  IICIF21;  ///< ( 9) IIC21の転送完了

			bit_rw_t<T, bitpos::B0>  STIF2;    ///< ( 8) UART2送信の転送完了，バッファ空き割り込み
			bit_rw_t<T, bitpos::B0>  CSIIF20;  ///< ( 8) CSI20の転送完了，バッファ空き割り込み
			bit_rw_t<T, bitpos::B0>  IICIF20;  ///< ( 8) IIC20の転送完了
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

			bit_rw_t<T, bitpos::B7>  TMIF03;   ///< (23) タイマ・チャネル03のカウント完了またはキャプチャ完了（16ビット／下位8ビット・タイマ動作時）
			bit_rw_t<T, bitpos::B6>  TMIF02;   ///< (22) タイマ・チャネル02のカウント完了またはキャプチャ完了
			bit_rw_t<T, bitpos::B5>  TMIF01;   ///< (21) タイマ・チャネル01のカウント完了またはキャプチャ完了（16ビット／下位8ビット・タイマ動作時）
			bit_rw_t<T, bitpos::B4>  TMIF00;   ///< (20) タイマ・チャネル00のカウント完了またはキャプチャ完了
			bit_rw_t<T, bitpos::B3>  IICAIF0;  ///< (19) IICA0通信完了

			bit_rw_t<T, bitpos::B2>  SREIF1;   ///< (18) UART1受信の通信エラー発生
			bit_rw_t<T, bitpos::B2>  TMIF03H;  ///< (18) タイマ・チャネル03のカウント完了またはキャプチャ完了（上位8ビット・タイマ動作時）

			bit_rw_t<T, bitpos::B1>  SRIF1;    ///< (17) UART1受信の転送完了
			bit_rw_t<T, bitpos::B1>  CSIIF11;  ///< (17) CSI11の転送完了，バッファ空き割り込み
			bit_rw_t<T, bitpos::B1>  IICIF11;  ///< (17) IIC11の転送完了

			bit_rw_t<T, bitpos::B0>  STIF1;    ///< (16) UART1送信の転送完了，バッファ空き割り込み
			bit_rw_t<T, bitpos::B0>  CSIIF10;  ///< (16) CSI10の転送完了，バッファ空き割り込み
			bit_rw_t<T, bitpos::B0>  IICIF10;  ///< (16) IIC10の転送完了
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

			bit_rw_t<T, bitpos::B7>  TMIF04;   ///< (31) タイマ・チャネル04のカウント完了またはキャプチャ完了
			bit_rw_t<T, bitpos::B6>  TMIF13;   ///< (30) タイマ・チャネル13のカウント完了またはキャプチャ完了

			bit_rw_t<T, bitpos::B5>  SRIF3;    ///< (29) UART3受信の転送完了
			bit_rw_t<T, bitpos::B5>  CSIIF31;  ///< (29) CSI31の転送完了，バッファ空き割り込み
			bit_rw_t<T, bitpos::B5>  IICIF31;  ///< (29) IIC31の転送完了

			bit_rw_t<T, bitpos::B4>  STIF3;    ///< (28) UART3送信の転送完了、バッファ空き割り込み
			bit_rw_t<T, bitpos::B4>  CSIIF30;  ///< (28) CSI30の転送完了，バッファ空き割り込み
			bit_rw_t<T, bitpos::B4>  IICIF30;  ///< (28) IIC30の転送完了

			bit_rw_t<T, bitpos::B3>  KRIF;     ///< (27) キー・リターン信号検出
			bit_rw_t<T, bitpos::B2>  ITIF;     ///< (26) 12ビット・インターバル・タイマのインターバル信号検出
			bit_rw_t<T, bitpos::B1>  RTCIF;    ///< (25) リアルタイム・クロックの定周期信号／アラーム一致検出
			bit_rw_t<T, bitpos::B0>  ADIF;     ///< (24) A/D変換終了
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

			bit_rw_t<T, bitpos::B7>  PIF10;   ///< (39) 端子入力エッジ検出１０
			bit_rw_t<T, bitpos::B6>  PIF9;    ///< (38) 端子入力エッジ検出９
			bit_rw_t<T, bitpos::B5>  PIF8;    ///< (37) 端子入力エッジ検出８
			bit_rw_t<T, bitpos::B4>  PIF7;    ///< (36) 端子入力エッジ検出７
			bit_rw_t<T, bitpos::B3>  PIF6;    ///< (35) 端子入力エッジ検出６
			bit_rw_t<T, bitpos::B2>  TMIF07;  ///< (34) タイマ・チャネル07のカウント完了またはキャプチャ完了
			bit_rw_t<T, bitpos::B1>  TMIF06;  ///< (33) タイマ・チャネル06のカウント完了またはキャプチャ完了
			bit_rw_t<T, bitpos::B0>  TMIF05;  ///< (32) タイマ・チャネル05のカウント完了またはキャプチャ完了
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

			bit_rw_t<T, bitpos::B7>  FLIF;     ///< (47) 予約
			bit_rw_t<T, bitpos::B6>  IICAIF1;  ///< (46) IICA1通信完了
			bit_rw_t<T, bitpos::B5>  MDIF;     ///< (45) 除算演算終了／積和演算結果のオーバフロー発生

			bit_rw_t<T, bitpos::B4>  SREIF3;   ///< (44) UART3受信の通信エラー発生
			bit_rw_t<T, bitpos::B4>  TMIF13H;  ///< (44) タイマ・チャネル13のカウント完了またはキャプチャ完了

			bit_rw_t<T, bitpos::B3>  TMIF12;   ///< (43) タイマ・チャネル12のカウント完了またはキャプチャ完了
			bit_rw_t<T, bitpos::B2>  TMIF11;   ///< (42) タイマ・チャネル11のカウント完了またはキャプチャ完了
			bit_rw_t<T, bitpos::B1>  TMIF10;   ///< (41) タイマ・チャネル10のカウント完了またはキャプチャ完了
			bit_rw_t<T, bitpos::B0>  PIF11;    ///< (40) 端子入力エッジ検出１１
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

			bit_rw_t<T, bitpos::B5>  TMIF17;  ///< (53) タイマ・チャネル17のカウント完了またはキャプチャ完了
			bit_rw_t<T, bitpos::B4>  TMIF16;  ///< (52) タイマ・チャネル16のカウント完了またはキャプチャ完了
			bit_rw_t<T, bitpos::B3>  TMIF15;  ///< (51) タイマ・チャネル15のカウント完了またはキャプチャ完了
			bit_rw_t<T, bitpos::B2>  TMIF14;  ///< (50) タイマ・チャネル14のカウント完了またはキャプチャ完了
			bit_rw_t<T, bitpos::B1>  DMAIF3;  ///< (49) DMA3の転送完了
			bit_rw_t<T, bitpos::B0>  DMAIF2;  ///< (48) DMA2の転送完了
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

			bit_rw_t<T, bitpos::B7>  PMK5;    ///< ( 7)
			bit_rw_t<T, bitpos::B6>  PMK4;    ///< ( 6)
			bit_rw_t<T, bitpos::B5>  PMK3;    ///< ( 5)
			bit_rw_t<T, bitpos::B4>  PMK2;    ///< ( 4)
			bit_rw_t<T, bitpos::B3>  PMK1;    ///< ( 3)
			bit_rw_t<T, bitpos::B2>  PMK0;    ///< ( 2)
			bit_rw_t<T, bitpos::B1>  LVIMK;   ///< ( 1)
			bit_rw_t<T, bitpos::B0>  WDTIMK;  ///< ( 0)
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

			bit_rw_t<T, bitpos::B7>  SREMK0;   ///< (15)
			bit_rw_t<T, bitpos::B7>  TMMK01H;  ///< (15)

			bit_rw_t<T, bitpos::B6>  SRMK0;    ///< (14)
			bit_rw_t<T, bitpos::B6>  CSIMK01;  ///< (14)
			bit_rw_t<T, bitpos::B6>  IICMK01;  ///< (14)

			bit_rw_t<T, bitpos::B5>  STMK0;    ///< (13)
			bit_rw_t<T, bitpos::B5>  CSIMK00;  ///< (13)
			bit_rw_t<T, bitpos::B5>  IICMK00;  ///< (13)

			bit_rw_t<T, bitpos::B4>  DMAMK1;   ///< (12)
			bit_rw_t<T, bitpos::B3>  DMAMK0;   ///< (11)

			bit_rw_t<T, bitpos::B2>  SREMK2;   ///< (10)
			bit_rw_t<T, bitpos::B2>  TMMK11H;  ///< (10)

			bit_rw_t<T, bitpos::B1>  SRMK2;    ///< ( 9)
			bit_rw_t<T, bitpos::B1>  CSIMK21;  ///< ( 9)
			bit_rw_t<T, bitpos::B1>  IICMK21;  ///< ( 9)

			bit_rw_t<T, bitpos::B0>  STMK2;    ///< ( 8)
			bit_rw_t<T, bitpos::B0>  CSIMK20;  ///< ( 8)
			bit_rw_t<T, bitpos::B0>  IICMK20;  ///< ( 8)
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

			bit_rw_t<T, bitpos::B7>  TMMK03;   ///< (23)
			bit_rw_t<T, bitpos::B6>  TMMK02;   ///< (22)
			bit_rw_t<T, bitpos::B5>  TMMK01;   ///< (21)
			bit_rw_t<T, bitpos::B4>  TMMK00;   ///< (20)
			bit_rw_t<T, bitpos::B3>  IICAMK0;  ///< (19)

			bit_rw_t<T, bitpos::B2>  SREMK1;   ///< (18)
			bit_rw_t<T, bitpos::B2>  TMMK03H;  ///< (18)

			bit_rw_t<T, bitpos::B1>  SRMK1;    ///< (17)
			bit_rw_t<T, bitpos::B1>  CSIMK11;  ///< (17)
			bit_rw_t<T, bitpos::B1>  IICMK11;  ///< (17)

			bit_rw_t<T, bitpos::B0>  STMK1;    ///< (16)
			bit_rw_t<T, bitpos::B0>  CSIMK10;  ///< (16)
			bit_rw_t<T, bitpos::B0>  IICMK10;  ///< (16)
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

			bit_rw_t<T, bitpos::B7>  TMMK04;   ///< (31)
			bit_rw_t<T, bitpos::B6>  TMMK13;   ///< (30)

			bit_rw_t<T, bitpos::B5>  SRMK3;    ///< (29)
			bit_rw_t<T, bitpos::B5>  CSIMK31;  ///< (29)
			bit_rw_t<T, bitpos::B5>  IICMK31;  ///< (29)

			bit_rw_t<T, bitpos::B4>  STMK3;    ///< (28)
			bit_rw_t<T, bitpos::B4>  CSIMK30;  ///< (28)
			bit_rw_t<T, bitpos::B4>  IICMK30;  ///< (28)

			bit_rw_t<T, bitpos::B3>  KRMK;     ///< (27)
			bit_rw_t<T, bitpos::B2>  ITMK;     ///< (26)
			bit_rw_t<T, bitpos::B1>  RTCMK;    ///< (25)
			bit_rw_t<T, bitpos::B0>  ADMK;     ///< (24)
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

			bit_rw_t<T, bitpos::B7>  PMK10;   ///< (39)
			bit_rw_t<T, bitpos::B6>  PMK9;    ///< (38)
			bit_rw_t<T, bitpos::B5>  PMK8;    ///< (37)
			bit_rw_t<T, bitpos::B4>  PMK7;    ///< (36)
			bit_rw_t<T, bitpos::B3>  PMK6;    ///< (35)
			bit_rw_t<T, bitpos::B2>  TMMK07;  ///< (34)
			bit_rw_t<T, bitpos::B1>  TMMK06;  ///< (33)
			bit_rw_t<T, bitpos::B0>  TMMK05;  ///< (32)
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

			bit_rw_t<T, bitpos::B7>  FLMK;     ///< (47)
			bit_rw_t<T, bitpos::B6>  IICAMK1;  ///< (46)
			bit_rw_t<T, bitpos::B5>  MDMK;     ///< (45)

			bit_rw_t<T, bitpos::B4>  SREMK3;   ///< (44)
			bit_rw_t<T, bitpos::B4>  TMMK13H;  ///< (44)

			bit_rw_t<T, bitpos::B3>  TMMK12;   ///< (43)
			bit_rw_t<T, bitpos::B2>  TMMK11;   ///< (42)
			bit_rw_t<T, bitpos::B1>  TMMK10;   ///< (41)
			bit_rw_t<T, bitpos::B0>  PMK11;    ///< (40)
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

			bit_rw_t<T, bitpos::B5>  TMMK17;  ///< (53)
			bit_rw_t<T, bitpos::B4>  TMMK16;  ///< (52)
			bit_rw_t<T, bitpos::B3>  TMMK15;  ///< (51)
			bit_rw_t<T, bitpos::B2>  TMMK14;  ///< (50)
			bit_rw_t<T, bitpos::B1>  DMAMK3;  ///< (49)
			bit_rw_t<T, bitpos::B0>  DMAMK2;  ///< (48)
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

			bit_rw_t<T, bitpos::B7>  PPR5;    ///< ( 7)
			bit_rw_t<T, bitpos::B6>  PPR4;    ///< ( 6)
			bit_rw_t<T, bitpos::B5>  PPR3;    ///< ( 5)
			bit_rw_t<T, bitpos::B4>  PPR2;    ///< ( 4)
			bit_rw_t<T, bitpos::B3>  PPR1;    ///< ( 3)
			bit_rw_t<T, bitpos::B2>  PPR0;    ///< ( 2)
			bit_rw_t<T, bitpos::B1>  LVIPR;   ///< ( 1)
			bit_rw_t<T, bitpos::B0>  WDTIPR;  ///< ( 0)
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

			bit_rw_t<T, bitpos::B7>  SREPR0;   ///< (15)
			bit_rw_t<T, bitpos::B7>  TMPR01H;  ///< (15)

			bit_rw_t<T, bitpos::B6>  SRPR0;    ///< (14)
			bit_rw_t<T, bitpos::B6>  CSIPR01;  ///< (14)
			bit_rw_t<T, bitpos::B6>  IICPR01;  ///< (14)

			bit_rw_t<T, bitpos::B5>  STPR0;    ///< (13)
			bit_rw_t<T, bitpos::B5>  CSIPR00;  ///< (13)
			bit_rw_t<T, bitpos::B5>  IICPR00;  ///< (13)

			bit_rw_t<T, bitpos::B4>  DMAPR1;   ///< (12)
			bit_rw_t<T, bitpos::B3>  DMAPR0;   ///< (11)

			bit_rw_t<T, bitpos::B2>  SREPR2;   ///< (10)
			bit_rw_t<T, bitpos::B2>  TMPR11H;  ///< (10)

			bit_rw_t<T, bitpos::B1>  SRPR2;    ///< ( 9)
			bit_rw_t<T, bitpos::B1>  CSIPR21;  ///< ( 9)
			bit_rw_t<T, bitpos::B1>  IICPR21;  ///< ( 9)

			bit_rw_t<T, bitpos::B0>  STPR2;    ///< ( 8)
			bit_rw_t<T, bitpos::B0>  CSIPR20;  ///< ( 8)
			bit_rw_t<T, bitpos::B0>  IICPR20;  ///< ( 8)
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

			bit_rw_t<T, bitpos::B7>  TMPR03;   ///< (23)
			bit_rw_t<T, bitpos::B6>  TMPR02;   ///< (22)
			bit_rw_t<T, bitpos::B5>  TMPR01;   ///< (21)
			bit_rw_t<T, bitpos::B4>  TMPR00;   ///< (20)
			bit_rw_t<T, bitpos::B3>  IICAPR0;  ///< (19)

			bit_rw_t<T, bitpos::B2>  SREPR1;   ///< (18)
			bit_rw_t<T, bitpos::B2>  TMPR03H;  ///< (18)

			bit_rw_t<T, bitpos::B1>  SRPR1;    ///< (17)
			bit_rw_t<T, bitpos::B1>  CSIPR11;  ///< (17)
			bit_rw_t<T, bitpos::B1>  IICPR11;  ///< (17)

			bit_rw_t<T, bitpos::B0>  STPR1;    ///< (16)
			bit_rw_t<T, bitpos::B0>  CSIPR10;  ///< (16)
			bit_rw_t<T, bitpos::B0>  IICPR10;  ///< (16)
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

			bit_rw_t<T, bitpos::B7>  TMPR04;   ///< (31)
			bit_rw_t<T, bitpos::B6>  TMPR13;   ///< (30)

			bit_rw_t<T, bitpos::B5>  SRPR3;    ///< (29)
			bit_rw_t<T, bitpos::B5>  CSIPR31;  ///< (29)
			bit_rw_t<T, bitpos::B5>  IICPR31;  ///< (29)

			bit_rw_t<T, bitpos::B4>  STPR3;    ///< (28)
			bit_rw_t<T, bitpos::B4>  CSIPR30;  ///< (28)
			bit_rw_t<T, bitpos::B4>  IICPR30;  ///< (28)

			bit_rw_t<T, bitpos::B3>  KRPR;     ///< (27)
			bit_rw_t<T, bitpos::B2>  ITPR;     ///< (26)
			bit_rw_t<T, bitpos::B1>  RTCPR;    ///< (25)
			bit_rw_t<T, bitpos::B0>  ADPR;     ///< (24)
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

			bit_rw_t<T, bitpos::B7>  PPR10;   ///< (39)
			bit_rw_t<T, bitpos::B6>  PPR9;    ///< (38)
			bit_rw_t<T, bitpos::B5>  PPR8;    ///< (37)
			bit_rw_t<T, bitpos::B4>  PPR7;    ///< (36)
			bit_rw_t<T, bitpos::B3>  PPR6;    ///< (35)
			bit_rw_t<T, bitpos::B2>  TMPR07;  ///< (34)
			bit_rw_t<T, bitpos::B1>  TMPR06;  ///< (33)
			bit_rw_t<T, bitpos::B0>  TMPR05;  ///< (32)
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

			bit_rw_t<T, bitpos::B7>  FLPR;     ///< (47)
			bit_rw_t<T, bitpos::B6>  IICAPR1;  ///< (46)
			bit_rw_t<T, bitpos::B5>  MDPR;     ///< (45)

			bit_rw_t<T, bitpos::B4>  SREPR3;   ///< (44)
			bit_rw_t<T, bitpos::B4>  TMPR13H;  ///< (44)

			bit_rw_t<T, bitpos::B3>  TMPR12;   ///< (43)
			bit_rw_t<T, bitpos::B2>  TMPR11;   ///< (42)
			bit_rw_t<T, bitpos::B1>  TMPR10;   ///< (41)
			bit_rw_t<T, bitpos::B0>  PPR11;    ///< (40)
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

			bit_rw_t<T, bitpos::B5>  TMPR17;  ///< (53)
			bit_rw_t<T, bitpos::B4>  TMPR16;  ///< (52)
			bit_rw_t<T, bitpos::B3>  TMPR15;  ///< (51)
			bit_rw_t<T, bitpos::B2>  TMPR14;  ///< (50)
			bit_rw_t<T, bitpos::B1>  DMAPR3;  ///< (49)
			bit_rw_t<T, bitpos::B0>  DMAPR2;  ///< (48)
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
				return IF1H.ITIF();

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
				IF1H.ITIF = ena;
				break;

			default:
				break;
			}
		}


		//-------------------------------------------------------------//
		/*!
			@brief  割り込み許可
		*/
		//-------------------------------------------------------------//
		static void enable(peripheral per, bool ena = true)
		{
			switch(per) {
			case peripheral::ITM:
				intr::MK1H.ITMK = !ena;
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
			@param[in]	level	割り込みレベル（０、１、２）
		*/
		//-------------------------------------------------------------//
		static void set_level(peripheral per, uint8_t level)
		{
			switch(per) {

			case peripheral::ITM:
				intr::PR01H.ITPR = (level) & 1;
				intr::PR11H.ITPR = (level & 2) >> 1;
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
	}
}
