#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/G13 グループ SAU/UART 制御
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include "common/renesas.hpp"

/// F_CLK はボーレートパラメーター計算で必要、設定が無いとエラーにします。
#ifndef F_CLK
#  error "uart_io.hpp requires F_CLK to be defined"
#endif

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  UART 制御クラス・テンプレート
		@param[in]	SAUtx	シリアル・アレイ・ユニット送信・クラス（偶数チャネル）
		@param[in]	SAUrx	シリアル・アレイ・ユニット受信・クラス（奇数チャネル）
		@param[in]	BUFtx	送信バッファサイズ（８バイト以上のサイズである事）
		@param[in]	BUFrx	受信バッファサイズ（８バイト以上のサイズである事）
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class SAUtx, class SAUrx, class BUFtx, class BUFrx>
	class uart_io {

		static SAUtx tx_;	///< 送信リソース
		static SAUrx rx_;	///< 受信リソース

		static BUFtx send_;
		static BUFrx recv_;

		static volatile bool	send_stall_;

		uint8_t	intr_level_;
		bool	crlf_;

		// ※必要なら、実装する
		inline void sleep_() { asm("nop"); }

		void send_restart_() {
			if(send_stall_ && send_.length() > 0) {
				while(tx_.SSR.TSF() != 0) sleep_();
				char ch = send_.get();
				send_stall_ = false;
				tx_.SDR_L = ch;
				send_intrrupt_mask_(false);
			}
		}

		void putch_(char ch)
		{
			if(intr_level_) {
				/// ７／８ を超えてた場合は、バッファが空になるまで待つ。
				if(send_.length() >= (send_.size() * 7 / 8)) {
					send_restart_();
					while(send_.length() != 0) sleep_();
				}
				send_.put(ch);
				send_restart_();
			} else {
				while(tx_.SSR.TSF() != 0) sleep_();
				tx_.SDR_L = ch;
			}
		}

		// 受信割り込みマスク設定
		static void recv_interrupt_mask_(bool f)
		{
#if 0
			switch(get_chanel_no()) {
			case 0:
				intr::MK0H.SRMK0 = f;
				break;
			case 1:
				intr::MK1L.SRMK1 = f;
				break;
			case 2:
				intr::MK0H.SRMK2 = f;
				break;
			case 3:
				intr::MK1H.SRMK3 = f;
				break;
			}
#endif
		}

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief  送信割り込みマスク
			@param[in]	f	マスク状態
		*/
		//-----------------------------------------------------------------//
		static void send_intrrupt_mask_(bool f)
		{
#if 0
			switch(get_chanel_no()) {
			case 0:
				intr::MK0H.STMK0 = f;
				break;
			case 1:
				intr::MK1L.STMK1 = f;
				break;
			case 2:
				intr::MK0H.STMK2 = f;
				break;
			case 3:
				intr::MK1H.STMK3 = f;
				break;
			}
#endif
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  送信割り込み
		*/
		//-----------------------------------------------------------------//
		static void send_task() __attribute__ ((section (".lowtext")))
		{
			if(send_.length()) {
				tx_.SDR_L = send_.get();
			} else {
				send_intrrupt_mask_(true);
				send_stall_ = true;
			}
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  受信割り込み
		*/
		//-----------------------------------------------------------------//
		static void recv_task() __attribute__ ((section (".lowtext")))
		{
			recv_.put(rx_.SDR_L());
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  エラー割り込み
		*/
		//-----------------------------------------------------------------//
		static void error_task() __attribute__ ((section (".lowtext")))
		{
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  コンストラクター
		*/
		//-----------------------------------------------------------------//
		uart_io() : intr_level_(0), crlf_(true) { }


		//-----------------------------------------------------------------//
		/*!
			@brief  ボーレートを設定して、UART を有効にする
			@param[in]	baud	ボーレート
			@param[in]	level	割り込みレベル（１～２）、０の場合はポーリング
			@return エラーなら「false」
		*/
		//-----------------------------------------------------------------//
		bool start(uint32_t baud, uint8_t level = 0)
		{
			intr_level_ = level;

			// ボーレートから分周比の算出
			auto div = static_cast<uint32_t>(F_CLK) / baud;
			uint8_t master = 0;
			while(div > 256) {
				div /= 2;
				++master;
				if(master >= 16) {
					/// ボーレート設定範囲外
					return false;
				}
			}
			--div;
			div &= 0xfe;
			if(div <= 2) {
				/// ボーレート設定範囲外
				return false;
			}

			// 対応するユニットを有効にする
			enable(SAUtx::get_peripheral());

			// 各ユニットで、チャネル０、１、２、３で共有の為、
			// ０、１：PRS0、２、３：PRS1 を使う
			bool cks = false;
			if(tx_.get_chanel_no() == 0) {
				tx_.SPS.PRS0 = master;
			} else {
				tx_.SPS.PRS1 = master;
				cks = true;
			}

			tx_.SMR = 0x0020 | SAUtx::SMR.CKS.b(cks) | SAUtx::SMR.MD.b(1) | SAUtx::SMR.MD0.b(1);
			rx_.SMR = 0x0020 | SAUtx::SMR.CKS.b(cks) | SAUrx::SMR.STS.b(1) | SAUrx::SMR.MD.b(1);

			// 8 date, 1 stop, no-parity LSB-first
			// 送信設定
			tx_.SCR = 0x0004 | SAUtx::SCR.TXE.b(1) | SAUtx::SCR.SLC.b(1) | SAUtx::SCR.DLS.b(3) |
					  SAUtx::SCR.DIR.b(1);
			// 受信設定
			rx_.SCR = 0x0004 | SAUrx::SCR.RXE.b(1) | SAUrx::SCR.SLC.b(1) | SAUrx::SCR.DLS.b(3) |
					  SAUrx::SCR.DIR.b(1);

			// ボーレート・ジェネレーター設定
			tx_.SDR = div << 8;
			rx_.SDR = div << 8;

			tx_.SOL = 0;	// TxD
			tx_.SO  = 1;	// シリアル出力設定
			tx_.SOE = 1;	// シリアル出力許可(Txd)

			// 対応するポートの設定
			if(tx_.get_unit_no() == 0) {
				if(tx_.get_chanel_no() == 0) {  // UART0
					PM1.B1 = 1;	// P1-1 input  (RxD0)
					PU1.B1 = 0; // P1-1 pullup offline
					PM1.B2 = 0;	// P1-2 output (TxD0)
					P1.B1  = 1;	// ポートレジスター RxD 切り替え
					P1.B2  = 1;	// ポートレジスター TxD 切り替え
				} else {  // UART1
					PM0.B3  = 1;  // P0-3 input  (RxD1)
					PU0.B3  = 0;  // P0-3 pullup offline
					PM0.B2  = 0;  // P0-2 output (TxD1)
					PIM0.B3 = 1;  // ポート入力モードレジスタ（RxD1:TTL）
					PMC0.B3 = 0;  // ポートモードコントロール
					PMC0.B2 = 0;  // ポートモードコントロール
					P0.B3   = 1;  // ポートレジスター RxD 切り替え
					P0.B2   = 1;  // ポートレジスター TxD 切り替え
				}
			} else {
				if(tx_.get_chanel_no() == 0) {  // UART2
					PM1.B4 = 1;	// P1-4 input  (RxD2)
					PU1.B4 = 0; // P1-4 pullup offline
					PM1.B3 = 0;	// P1-3 output (TxD2)
					P1.B3  = 1;	// ポートレジスター TxD 切り替え
				} else {  // UART3（128ピンデバイスでサポート）
					PM14.B3 = 1;  // P14-3 input (RxD3)
					PU14.B3 = 0;  // P14-3 pullup offline
					PM14.B4 = 0;  // P14-4 output (TxD3)
					P14.B4  = 1;  // ポートレジスター TxD 切り替え
				}
			}

			send_stall_ = true;

			tx_.SS = 1;	// TxD enable
			rx_.SS = 1;	// RxD enable

			// マスクをクリアして、割り込み許可
			if(intr_level_ > 0) {
				--level;
				level ^= 0x03;
				// 送信側優先順位
#if 0
				switch(get_chanel_no()) {
				case 0:
					intr::PR00H.STPR0 = (level) & 1;
					intr::PR10H.STPR0 = (level & 2) >> 1;
					intr::PR00H.SRPR0 = (level) & 1;
					intr::PR10H.SRPR0 = (level & 2) >> 1;
					break;
				case 1:
					intr::PR01L.STPR1 = (level) & 1;
					intr::PR11L.STPR1 = (level & 2) >> 1;
					intr::PR01L.SRPR1 = (level) & 1;
					intr::PR11L.SRPR1 = (level & 2) >> 1;
					break;
				case 2:
					intr::PR00H.STPR2 = (level) & 1;
					intr::PR10H.STPR2 = (level & 2) >> 1;
					intr::PR00H.SRPR2 = (level) & 1;
					intr::PR10H.SRPR2 = (level & 2) >> 1;
					break;
				case 3:
					intr::PR01H.STPR3 = (level) & 1;
					intr::PR11H.STPR3 = (level & 2) >> 1;
					intr::PR01H.SRPR3 = (level) & 1;
					intr::PR11H.SRPR3 = (level & 2) >> 1;
					break;
				}
				recv_interrupt_mask_(0);
#endif
			}

			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	CRLF 自動送出
			@param[in]	f	「false」なら無効
		 */
		//-----------------------------------------------------------------//
		void auto_crlf(bool f = true) { crlf_ = f; }


		//-----------------------------------------------------------------//
		/*!
			@brief	送信バッファのサイズを返す
			@return　バッファのサイズ
		 */
		//-----------------------------------------------------------------//
		uint16_t send_length() const {
			if(intr_level_) {
				return send_.length();
			} else {
				return tx_.SSR.TSF();
			}
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	受信バッファのサイズを返す @n
					※ポーリング時、データが無い：０、データがある：１
			@return　バッファのサイズ
		 */
		//-----------------------------------------------------------------//
		uint16_t recv_length() const {
			if(intr_level_) {
				return recv_.length();
			} else {
				return rx_.SSR.BFF();
			}
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	文字出力
			@param[in]	ch	文字コード
		 */
		//-----------------------------------------------------------------//
		void putch(char ch) {
			if(crlf_ && ch == '\n') {
				putch_('\r');
			}
			putch_(ch);
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	文字入力
			@return 文字コード
		 */
		//-----------------------------------------------------------------//
		char getch() {
			if(intr_level_) {
				while(recv_.length() == 0) sleep_();
				return recv_.get();
			} else {
				while(rx_.SSR.BFF() == 0) sleep_();
				return rx_.SDR_L();
			}
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	文字列出力
			@param[in]	s	出力ストリング
		 */
		//-----------------------------------------------------------------//
		void puts(const char* s) {
			char ch;
			while((ch = *s) != 0) {
				putch(ch);
				++s;
			}
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	UART チャネルの取得
			@return UART チャネル番号
		 */
		//-----------------------------------------------------------------//
		static uint8_t get_chanel_no() {
			return (SAUtx::get_unit_no() << 1) | ((SAUtx::get_chanel_no() >> 1) & 1);
		}
	};

	// send_、recv_, send_stall_ の実体を定義
	template<class SAUtx, class SAUrx, class BUFtx, class BUFrx>
		BUFtx uart_io<SAUtx, SAUrx, BUFtx, BUFrx>::send_;

	template<class SAUtx, class SAUrx, class BUFtx, class BUFrx>
		BUFrx uart_io<SAUtx, SAUrx, BUFtx, BUFrx>::recv_;

	template<class SAUtx, class SAUrx, class BUFtx, class BUFrx>
		volatile bool uart_io<SAUtx, SAUrx, BUFtx, BUFrx>::send_stall_ = true; 
}
