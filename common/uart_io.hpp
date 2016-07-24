#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/G13 グループ SAU/UART 制御 @n
			Copyright 2016 Kunihito Hiramatsu
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include "G13/system.hpp"
#include "G13/sau.hpp"
#include "G13/intr.hpp"
#include "common/fifo.hpp"

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
		@param[in]	send_size	送信バッファサイズ
		@param[in]	recv_size	受信バッファサイズ
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class SAUtx, class SAUrx, uint16_t send_size, uint16_t recv_size>
	class uart_io {

		static SAUtx tx_;	///< 送信リソース
		static SAUrx rx_;	///< 受信リソース

		static utils::fifo<send_size> send_;
		static utils::fifo<recv_size> recv_;

		static volatile bool	send_stall_;

		uint8_t	intr_level_ = 0;
		bool	crlf_ = true;

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

	public:
		// 送信完了割り込み設定
		static inline void send_intrrupt_mask_(bool f)
		{
			if(tx_.get_unit_no() == 0) {
				if(tx_.get_chanel_no() == 0) {  // UART0
					intr::MK0H.STMK0 = f;
				} else {  // UART1
					intr::MK1L.STMK1 = f;
				}
			} else {
				if(tx_.get_chanel_no() == 0) {  // UART2
					intr::MK0H.STMK2 = f;
				} else {  // UART3
					intr::MK1H.STMK3 = f;
				}
			}
		}

		// 受信割り込みマスク設定
		static inline void recv_interrupt_mask_(bool f)
		{
			if(rx_.get_unit_no() == 0) {
				if(rx_.get_chanel_no() == 1) {  // UART0
					intr::MK0H.SRMK0 = f;
				} else {  // UART1
					intr::MK1L.SRMK1 = f;
				}
			} else {
				if(rx_.get_chanel_no() == 1) {  // UART2
					intr::MK0H.SRMK2 = f;
				} else {  //UART3
					intr::MK1H.SRMK3 = f;
				}
			}
		}

		static __attribute__ ((interrupt)) void send_task() __attribute__ ((section (".lowtext")))
		{
			if(send_.length()) {
				tx_.SDR_L = send_.get();
			} else {
				send_intrrupt_mask_(true);
				send_stall_ = true;
			}
		}

		static __attribute__ ((interrupt)) void recv_task() __attribute__ ((section (".lowtext")))
		{
			recv_.put(rx_.SDR_L());
		}

		static __attribute__ ((interrupt)) void error_task()
		{
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  ボーレートを設定して、UART を有効にする
			@param[in]	baud	ボーレート
			@param[in]	level	割り込みレベル（１～２）、０の場合はポーリング
			@return エラーなら「false」
		*/
		//-----------------------------------------------------------------//
		bool start(uint32_t baud, uint8_t level = 0) {
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
			if(tx_.get_unit_no() == 0) {
				PER0.SAU0EN = 1;
			} else {
				PER0.SAU1EN = 1;
			}

			// チャネル０、１で共有の為、どちらか片方のみの設定
			bool cks = false;
			if(tx_.get_chanel_no() == 0) {
				tx_.SPS = SAUtx::SPS.PRS0.b(master);
			} else {
				tx_.SPS = SAUtx::SPS.PRS1.b(master);
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
					PM1.B2 = 0;	// P1-2 output (TxD0)
					P1.B2  = 1;	// ポートレジスター TxD 切り替え
				} else {  // UART1
					PM1.B4 = 1;	// P1-1 input  (RxD0)
					PM1.B3 = 0;	// P1-2 output (TxD0)
					P1.B3  = 1;	// ポートレジスター TxD 切り替え
				}
			} else {
				if(tx_.get_chanel_no() == 0) {  // UART2
					PM0.B3  = 1;  // P1-3 input  (RxD2)
					PM0.B2  = 0;  // P1-2 output (TxD2)
					PMC0.B2 = 0;  // ポートモードコントロール
					P0.B2   = 1;  // ポートレジスター TxD 切り替え
				} else {  // UART3（128ピンデバイスでサポート）
					PM14.B3 = 1;  // P14-3 input (RxD3)
					PM14.B4 = 0;  // P14-4 output (TxD3)
					P14.B4  = 1;  // ポートレジスター TxD 切り替え
				}
			}

			send_stall_ = true;

			tx_.SS = 1;	/// TxD enable
			rx_.SS = 1;	/// RxD enable

			// マスクをクリアして、割り込み許可
			if(intr_level_ > 0) {
				recv_interrupt_mask_(0);
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
	};

	// send_、recv_, send_stall_ の実体を定義
	template<class SAUtx, class SAUrx, uint16_t send_size, uint16_t recv_size>
		utils::fifo<send_size> uart_io<SAUtx, SAUrx, send_size, recv_size>::send_;

	template<class SAUtx, class SAUrx, uint16_t send_size, uint16_t recv_size>
		utils::fifo<recv_size> uart_io<SAUtx, SAUrx, send_size, recv_size>::recv_;

	template<class SAUtx, class SAUrx, uint16_t send_size, uint16_t recv_size>
		volatile bool uart_io<SAUtx, SAUrx, send_size, recv_size>::send_stall_ = true; 
}
