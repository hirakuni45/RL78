#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/G13 割り込み機能を制御するレジスタ定義
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016, 2021 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include "common/io_utils.hpp"
#include "G13/peripheral.hpp"

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  割り込みクラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class _>
	struct intr_t {

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
		typedef if0l_t< rw8_t<0xFFFE0> > IF0L_;
		static IF0L_ IF0L;


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
		typedef if0h_t< rw8_t<0xFFFE1> > IF0H_;
		static IF0H_ IF0H;


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
		typedef if1l_t< rw8_t<0xFFFE2> > IF1L_;
		static IF1L_ IF1L;


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
		typedef if1h_t< rw8_t<0xFFFE3> > IF1H_;
		static IF1H_ IF1H;


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
		typedef if2l_t< rw8_t<0xFFFD0> > IF2L_;
		static IF2L_ IF2L;
	

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
		typedef if2h_t< rw8_t<0xFFFD1> > IF2H_;
		static IF2H_ IF2H;


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
		typedef if3l_t< rw8_t<0xFFFD2> > IF3L_;
		static IF3L_ IF3L;


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
		typedef mk0l_t< rw8_t<0xFFFE4> > MK0L_;
		static MK0L_ MK0L;


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
		typedef mk0h_t< rw8_t<0xFFFE5> > MK0H_;
		static MK0H_ MK0H;


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
		typedef mk1l_t< rw8_t<0xFFFE6> > MK1L_;
		static MK1L_ MK1L;


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
		typedef mk1h_t< rw8_t<0xFFFE7> > MK1H_;
		static MK1H_ MK1H;


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
		typedef mk2l_t< rw8_t<0xFFFD4> > MK2L_;
		static MK2L_ MK2L;


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
		typedef mk2h_t< rw8_t<0xFFFD5> > MK2H_;
		static MK2H_ MK2H;


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
		typedef mk3l_t< rw8_t<0xFFFD6> > MK3L_;
		static MK3L_ MK3L;


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
		typedef pr0l_t< rw8_t<0xFFFE8> > PR00L_;
		static PR00L_ PR00L;
		typedef pr0l_t< rw8_t<0xFFFEC> > PR10L_;
		static PR10L_ PR10L;


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
		typedef pr0h_t< rw8_t<0xFFFE9> > PR00H_;
		static PR00H_ PR00H;
		typedef pr0h_t< rw8_t<0xFFFED> > PR10H_;
		static PR10H_ PR10H;


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
		typedef pr1l_t< rw8_t<0xFFFEA> > PR01L_;
		static PR01L_ PR01L;
		typedef pr1l_t< rw8_t<0xFFFEE> > PR11L_;
		static PR11L_ PR11L;


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
		typedef pr1h_t< rw8_t<0xFFFEB> > PR01H_;
		static PR01H_ PR01H;
		typedef pr1h_t< rw8_t<0xFFFEF> > PR11H_;
		static PR11H_ PR11H;


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
		typedef pr2l_t< rw8_t<0xFFFD8> > PR02L_;
		static PR02L_ PR02L;
		typedef pr2l_t< rw8_t<0xFFFDC> > PR12L_;
		static PR12L_ PR12L;


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
		typedef pr2h_t< rw8_t<0xFFFD9> > PR02H_;
		static PR02H_ PR02H;
		typedef pr2h_t< rw8_t<0xFFFDD> > PR12H_;
		static PR12H_ PR12H;


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
		typedef pr3l_t< rw8_t<0xFFFDA> > PR03L_;
		static PR03L_ PR03L;
		typedef pr3l_t< rw8_t<0xFFFDE> > PR13L_;
		static PR13L_ PR13L;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  外部割り込み立ち上がりエッジ許可レジスタ（EGP0, EGP1）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef basic_rw_t< rw8_t<0xFFF38> > EGP0_;
		static EGP0_ EGP0;
		typedef basic_rw_t< rw8_t<0xFFF3A> > EGP1_;
		static EGP1_ EGP1;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  外部割り込み立ち下がりエッジ許可レジスタ（EGN0, EGN1）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef basic_rw_t< rw8_t<0xFFF39> > EGN0_;
		static EGN0_ EGN0;
		typedef basic_rw_t< rw8_t<0xFFF3B> > EGN1_;
		static EGN1_ EGN1;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  キー・リターン・モード・レジスタ（KRM）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef basic_rw_t< rw8_t<0xFFF37> > KRM_;
		static KRM_ KRM;


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

			case peripheral::ADC:
				return IF1H.ADIF();

			case peripheral::IICA0:
				return IF1L.IICAIF0();
			case peripheral::IICA1:
				return IF2H.IICAIF1();

			case peripheral::SAU00:  // UART0-TX
				return IF0H.STIF0();
			case peripheral::SAU01:  // UART0-RX
				return IF0H.SRIF0();
			case peripheral::SAU02:  // UART1-TX
				return IF1L.STIF1();
			case peripheral::SAU03:  // UART1-RX
				return IF1L.SRIF1();
			case peripheral::SAU10:  // UART2-TX
				return IF0H.STIF2();
			case peripheral::SAU11:  // UART2-RX
				return IF0H.SRIF2();
			case peripheral::SAU12:  // UART3-TX
				return IF1H.STIF3();
			case peripheral::SAU13:  // UART3-RX
				return IF1H.SRIF3();

			case peripheral::TAU00:  // TAU00
				return IF1L.TMIF00();
			case peripheral::TAU01:  // TAU01
				return IF1L.TMIF01();
			case peripheral::TAU02:  // TAU02
				return IF1L.TMIF02();
			case peripheral::TAU03:  // TAU03
				return IF1L.TMIF03();
			case peripheral::TAU04:  // TAU04
				return IF1H.TMIF04();
			case peripheral::TAU05:  // TAU05
				return IF2L.TMIF05();
			case peripheral::TAU06:  // TAU06
				return IF2L.TMIF06();
			case peripheral::TAU07:  // TAU07
				return IF2L.TMIF07();
			case peripheral::TAU10:  // TAU10
				return IF2H.TMIF10();
			case peripheral::TAU11:  // TAU11
				return IF2H.TMIF11();
			case peripheral::TAU12:  // TAU12
				return IF2H.TMIF12();
			case peripheral::TAU13:  // TAU13
				return IF1H.TMIF13();
			case peripheral::TAU14:  // TAU14
				return IF3L.TMIF14();
			case peripheral::TAU15:  // TAU15
				return IF3L.TMIF15();
			case peripheral::TAU16:  // TAU16
				return IF3L.TMIF16();
			case peripheral::TAU17:  // TAU17
				return IF3L.TMIF17();

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

			case peripheral::ADC:
				IF1H.ADIF = ena;
				break;

			case peripheral::IICA0:
				IF1L.IICAIF0 = ena;
				break;
			case peripheral::IICA1:
				IF2H.IICAIF1 = ena;
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

			case peripheral::TAU00:  // TAU00
				IF1L.TMIF00 = ena;
				break;
			case peripheral::TAU01:  // TAU01
				IF1L.TMIF01 = ena;
				break;
			case peripheral::TAU02:  // TAU02
				IF1L.TMIF02 = ena;
				break;
			case peripheral::TAU03:  // TAU03
				IF1L.TMIF03 = ena;
				break;
			case peripheral::TAU04:  // TAU04
				IF1H.TMIF04 = ena;
				break;
			case peripheral::TAU05:  // TAU05
				IF2L.TMIF05 = ena;
				break;
			case peripheral::TAU06:  // TAU06
				IF2L.TMIF06 = ena;
				break;
			case peripheral::TAU07:  // TAU07
				IF2L.TMIF07 = ena;
				break;
			case peripheral::TAU10:  // TAU10
				IF2H.TMIF10 = ena;
				break;
			case peripheral::TAU11:  // TAU11
				IF2H.TMIF11 = ena;
				break;
			case peripheral::TAU12:  // TAU12
				IF2H.TMIF12 = ena;
				break;
			case peripheral::TAU13:  // TAU13
				IF1H.TMIF13 = ena;
				break;
			case peripheral::TAU14:  // TAU14
				IF3L.TMIF14 = ena;
				break;
			case peripheral::TAU15:  // TAU15
				IF3L.TMIF15 = ena;
				break;
			case peripheral::TAU16:  // TAU16
				IF3L.TMIF16 = ena;
				break;
			case peripheral::TAU17:  // TAU17
				IF3L.TMIF17 = ena;
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
				MK1H.ITMK = !ena;
				break;

			case peripheral::ADC:
				MK1H.ADMK = !ena;
				break;

			case peripheral::IICA0:
				MK1L.IICAMK0 = !ena;
				break;
			case peripheral::IICA1:
				MK2H.IICAMK1 = !ena;
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

			case peripheral::TAU00:  // TAU00
				MK1L.TMMK00 = !ena;
				break;
			case peripheral::TAU01:  // TAU01
				MK1L.TMMK01 = !ena;
				break;
			case peripheral::TAU02:  // TAU02
				MK1L.TMMK02 = !ena;
				break;
			case peripheral::TAU03:  // TAU03
				MK1L.TMMK03 = !ena;
				break;
			case peripheral::TAU04:  // TAU04
				MK1H.TMMK04 = !ena;
				break;
			case peripheral::TAU05:  // TAU05
				MK2L.TMMK05 = !ena;
				break;
			case peripheral::TAU06:  // TAU06
				MK2L.TMMK06 = !ena;
				break;
			case peripheral::TAU07:  // TAU07
				MK2L.TMMK07 = !ena;
				break;
			case peripheral::TAU10:  // TAU10
				MK2H.TMMK10 = !ena;
				break;
			case peripheral::TAU11:  // TAU11
				MK2H.TMMK11 = !ena;
				break;
			case peripheral::TAU12:  // TAU12
				MK2H.TMMK12 = !ena;
				break;
			case peripheral::TAU13:  // TAU13
				MK1H.TMMK13 = !ena;
				break;
			case peripheral::TAU14:  // TAU14
				MK3L.TMMK14 = !ena;
				break;
			case peripheral::TAU15:  // TAU15
				MK3L.TMMK15 = !ena;
				break;
			case peripheral::TAU16:  // TAU16
				MK3L.TMMK16 = !ena;
				break;
			case peripheral::TAU17:  // TAU17
				MK3L.TMMK17 = !ena;
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
				PR01H.ITPR = (level) & 1;
				PR11H.ITPR = (level & 2) >> 1;
				break;

			case peripheral::ADC:
				PR01H.ADPR = (level) & 1;
				PR11H.ADPR = (level & 2) >> 1;
				break;

			case peripheral::IICA0:
				PR01L.IICAPR0 = (level) & 1;
				PR11L.IICAPR0 = (level & 2) >> 1;
				break;
			case peripheral::IICA1:
				PR02H.IICAPR1 = (level) & 1;
				PR12H.IICAPR1 = (level & 2) >> 1;
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

			case peripheral::TAU00:  // TAU00
				PR01L.TMPR00 = (level) & 1;
				PR11L.TMPR00 = (level & 2) >> 1;
				break;
			case peripheral::TAU01:  // TAU01
				PR01L.TMPR01 = (level) & 1;
				PR11L.TMPR01 = (level & 2) >> 1;
				break;
			case peripheral::TAU02:  // TAU02
				PR01L.TMPR02 = (level) & 1;
				PR11L.TMPR02 = (level & 2) >> 1;
				break;
			case peripheral::TAU03:  // TAU03
				PR01L.TMPR03 = (level) & 1;
				PR11L.TMPR03 = (level & 2) >> 1;
				break;
			case peripheral::TAU04:  // TAU04
				PR01H.TMPR04 = (level) & 1;
				PR11H.TMPR04 = (level & 2) >> 1;
				break;
			case peripheral::TAU05:  // TAU05
				PR02L.TMPR05 = (level) & 1;
				PR12L.TMPR05 = (level & 2) >> 1;
				break;
			case peripheral::TAU06:  // TAU06
				PR02L.TMPR06 = (level) & 1;
				PR12L.TMPR06 = (level & 2) >> 1;
				break;
			case peripheral::TAU07:  // TAU07
				PR02L.TMPR07 = (level) & 1;
				PR12L.TMPR07 = (level & 2) >> 1;
				break;
			case peripheral::TAU10:  // TAU10
				PR02H.TMPR10 = (level) & 1;
				PR12H.TMPR10 = (level & 2) >> 1;
				break;
			case peripheral::TAU11:  // TAU11
				PR02H.TMPR11 = (level) & 1;
				PR12H.TMPR11 = (level & 2) >> 1;
				break;
			case peripheral::TAU12:  // TAU12
				PR02H.TMPR12 = (level) & 1;
				PR12H.TMPR12 = (level & 2) >> 1;
				break;
			case peripheral::TAU13:  // TAU13
				PR01H.TMPR13 = (level) & 1;
				PR11H.TMPR13 = (level & 2) >> 1;
				break;
			case peripheral::TAU14:  // TAU14
				PR03L.TMPR14 = (level) & 1;
				PR13L.TMPR14 = (level & 2) >> 1;
				break;
			case peripheral::TAU15:  // TAU15
				PR03L.TMPR15 = (level) & 1;
				PR13L.TMPR15 = (level & 2) >> 1;
				break;
			case peripheral::TAU16:  // TAU16
				PR03L.TMPR16 = (level) & 1;
				PR13L.TMPR16 = (level & 2) >> 1;
				break;
			case peripheral::TAU17:  // TAU17
				PR03L.TMPR17 = (level) & 1;
				PR13L.TMPR17 = (level & 2) >> 1;
				break;

			default:
				break;
			}
		}
	};
	// テンプレート内、スタティック定義、実態：
	template<class _> typename intr_t<_>::IF0L_ intr_t<_>::IF0L;
	template<class _> typename intr_t<_>::IF0H_ intr_t<_>::IF0H;
	template<class _> typename intr_t<_>::IF1L_ intr_t<_>::IF1L;
	template<class _> typename intr_t<_>::IF1H_ intr_t<_>::IF1H;
	template<class _> typename intr_t<_>::IF2L_ intr_t<_>::IF2L;
	template<class _> typename intr_t<_>::IF2H_ intr_t<_>::IF2H;
	template<class _> typename intr_t<_>::IF3L_ intr_t<_>::IF3L;
	template<class _> typename intr_t<_>::MK0L_ intr_t<_>::MK0L;
	template<class _> typename intr_t<_>::MK0H_ intr_t<_>::MK0H;
	template<class _> typename intr_t<_>::MK1L_ intr_t<_>::MK1L;
	template<class _> typename intr_t<_>::MK1H_ intr_t<_>::MK1H;
	template<class _> typename intr_t<_>::MK2L_ intr_t<_>::MK2L;
	template<class _> typename intr_t<_>::MK2H_ intr_t<_>::MK2H;
	template<class _> typename intr_t<_>::MK3L_ intr_t<_>::MK3L;
	template<class _> typename intr_t<_>::PR00L_ intr_t<_>::PR00L;
	template<class _> typename intr_t<_>::PR10L_ intr_t<_>::PR10L;
	template<class _> typename intr_t<_>::PR00H_ intr_t<_>::PR00H;
	template<class _> typename intr_t<_>::PR10H_ intr_t<_>::PR10H;
	template<class _> typename intr_t<_>::PR01L_ intr_t<_>::PR01L;
	template<class _> typename intr_t<_>::PR11L_ intr_t<_>::PR11L;
	template<class _> typename intr_t<_>::PR01H_ intr_t<_>::PR01H;
	template<class _> typename intr_t<_>::PR11H_ intr_t<_>::PR11H;
	template<class _> typename intr_t<_>::PR02L_ intr_t<_>::PR02L;
	template<class _> typename intr_t<_>::PR12L_ intr_t<_>::PR12L;
	template<class _> typename intr_t<_>::PR02H_ intr_t<_>::PR02H;
	template<class _> typename intr_t<_>::PR12H_ intr_t<_>::PR12H;
	template<class _> typename intr_t<_>::PR03L_ intr_t<_>::PR03L;
	template<class _> typename intr_t<_>::PR13L_ intr_t<_>::PR13L;
	template<class _> typename intr_t<_>::EGP0_ intr_t<_>::EGP0;
	template<class _> typename intr_t<_>::EGP1_ intr_t<_>::EGP1;
	template<class _> typename intr_t<_>::EGN0_ intr_t<_>::EGN0;
	template<class _> typename intr_t<_>::EGN1_ intr_t<_>::EGN1;
	template<class _> typename intr_t<_>::KRM_ intr_t<_>::KRM;

	 typedef intr_t<void> intr;
}
