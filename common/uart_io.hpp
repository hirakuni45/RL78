#pragma once
//=========================================================================//
/*!	@file
	@brief	RL78/(G13/L1C) グループ SAU/UART 制御
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016, 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=========================================================================//
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

		static BUFtx send_;
		static BUFrx recv_;

		static volatile bool	send_stall_;

		uint8_t	intr_level_;
		bool	crlf_;


		// ※必要なら、実装する
		inline void sleep_() { asm("nop"); }


		void send_restart_()
		{
			if(send_stall_ && send_.length() > 0) {
				while(SAUtx::SSR.TSF() != 0) sleep_();
				char ch = send_.get();
				send_stall_ = false;
				SAUtx::SDR_L = ch;
				intr::enable(SAUtx::get_peripheral());
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
				while(SAUtx::SSR.TSF() != 0) sleep_();
				SAUtx::SDR_L = ch;
			}
		}


	public:
		//-----------------------------------------------------------------//
		/*!
			@brief  送信割り込み
		*/
		//-----------------------------------------------------------------//
		static void send_task() __attribute__ ((section (".lowtext")))
		{
			if(send_.length()) {
				SAUtx::SDR_L = send_.get();
			} else {
				intr::enable(SAUtx::get_peripheral(), false);
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
			recv_.put(SAUrx::SDR_L());
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

			// 対応するユニットを有効にする（SAUtx を代表とする）
			enable(SAUtx::get_peripheral());

			// 各ユニットで、チャネル０、１、２、３で共有の為、
			// ０、１：PRS0、２、３：PRS1 を使う
			bool cks = false;
			if(SAUtx::get_chanel_no() == 0) {
				SAUtx::SPS.PRS0 = master;
			} else {
				SAUtx::SPS.PRS1 = master;
				cks = true;
			}

			SAUtx::SMR = 0x0020 | SAUtx::SMR.CKS.b(cks) | SAUtx::SMR.MD.b(1) | SAUtx::SMR.MD0.b(1);
			SAUrx::SMR = 0x0020 | SAUtx::SMR.CKS.b(cks) | SAUrx::SMR.STS.b(1) | SAUrx::SMR.MD.b(1);

			// 8 date, 1 stop, no-parity LSB-first
			// 送信設定
			SAUtx::SCR = 0x0004 | SAUtx::SCR.TXE.b(1) | SAUtx::SCR.SLC.b(1) | SAUtx::SCR.DLS.b(3) |
					     SAUtx::SCR.DIR.b(1);
			// 受信設定
			SAUrx::SCR = 0x0004 | SAUrx::SCR.RXE.b(1) | SAUrx::SCR.SLC.b(1) | SAUrx::SCR.DLS.b(3) |
						 SAUrx::SCR.DIR.b(1);

			// ボーレート・ジェネレーター設定
			SAUtx::SDR = div << 8;
			SAUrx::SDR = div << 8;

			SAUtx::SOL = 0;	// TxD
			SAUtx::SO  = 1;	// シリアル出力設定
			SAUtx::SOE = 1;	// シリアル出力許可(Txd)

			// 対応するポートの設定
			set_uart_port(SAUtx::get_peripheral());
			set_uart_port(SAUrx::get_peripheral());

			send_stall_ = true;

			SAUtx::SS = 1;	// TxD enable
			SAUrx::SS = 1;	// RxD enable

			// マスクをクリアして、割り込み許可
			if(intr_level_ > 0) {
				--level;
				level ^= 0x03;
				// 送信側優先順位
				intr::set_level(SAUtx::get_peripheral(), level);
				intr::set_level(SAUrx::get_peripheral(), level);
				intr::enable(SAUrx::get_peripheral());
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
				return SAUtx::SSR.TSF();
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
				return SAUrx::SSR.BFF();
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
				while(SAUrx::SSR.BFF() == 0) sleep_();
				return SAUrx::SDR_L();
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
