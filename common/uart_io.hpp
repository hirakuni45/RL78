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
#include "common/fifo.hpp"

/// F_CLK はボーレートパラメーター計算で必要、設定が無いとエラーにします。
#ifndef F_CLK
#  error "uart_io.hpp requires F_CLK to be defined"
#endif

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  UART 制御クラス
		@param[in]	SAUtx	シリアル・アレイ・ユニット送信・クラス
		@param[in]	SAUrx	シリアル・アレイ・ユニット受信・クラス
		@param[in]	send+size	送信バッファサイズ
		@param[in]	recv_size	受信バッファサイズ
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class SAUtx, class SAUrx, uint16_t send_size, uint16_t recv_size>
	class uart_io {

		static SAUtx tx_;	///< 送信リソース
		static SAUrx rx_;	///< 受信リソース

		static utils::fifo<send_size> send_;
		static utils::fifo<recv_size> recv_;

		bool	polling_ = false;
		bool	crlf_ = true;

		// ※必要なら、実装する
		void sleep_() { asm("nop"); }

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief  ボーレートを設定して、UART を有効にする
			@param[in]	baud	ボーレート
			@param[in]	polling	ポーリングの場合「true」
			@return エラーなら「false」
		*/
		//-----------------------------------------------------------------//
		bool start(uint32_t baud, bool polling = false) {
			polling_ = polling;

			// ボーレートと分周比の計算
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
			tx_.SPS = SAUtx::SPS.PRS0.b(master);
			// rx_.SPS = SAUrx::SPS.PRS0.b(master);

			tx_.SMR = 0x20 | SAUtx::SMR.MD.b(1) | SAUtx::SMR.MD0.b(1);
			rx_.SMR = 0x20 | SAUrx::SMR.STS.b(1) | SAUrx::SMR.MD.b(1);

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

			PM1.B1 = 1;	/// P1-1 input  (RxD)
			PM1.B2 = 0;	/// P1-2 output (TxD)

			P1.B2  = 1;	/// ポートレジスター (TxD)

			tx_.SS = 1;	/// TxD enable
			rx_.SS = 1;	/// RxD enable

			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	送信バッファのサイズを返す
			@return　バッファのサイズ
		 */
		//-----------------------------------------------------------------//
		uint16_t send_length() const {
			if(polling_) {
				return tx_.SSR.TSF();
			} else {
				return send_.length();
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
			if(polling_) {
				return rx_.SSR.BFF();
			} else {
				return recv_.length();
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
				putch('\r');
			}

			if(polling_) {
				while(tx_.SSR.TSF() != 0) sleep_();
				tx_.SDR_L = ch;
			} else {
				/// ７／８ を超えてた場合は、バッファが空になるまで待つ。
				if(send_.length() >= (send_.size() * 7 / 8)) {
					while(send_.length() != 0) sleep_();
				}
				send_.put(ch);
///				SCIx::SCR.TEIE = 1;
			}
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	文字入力
			@return 文字コード
		 */
		//-----------------------------------------------------------------//
		char getch() {
			if(polling_) {
				while(rx_.SSR.BFF() == 0) sleep_();
				return rx_.SDR_L();
			} else {
				while(recv_.length() == 0) sleep_();
				return recv_.get();
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

	// send_、recv_ の実体を定義
	template<class SAUtx, class SAUrx, uint16_t send_size, uint16_t recv_size>
		utils::fifo<send_size> uart_io<SAUtx, SAUrx, send_size, recv_size>::send_;

	template<class SAUtx, class SAUrx, uint16_t send_size, uint16_t recv_size>
		utils::fifo<recv_size> uart_io<SAUtx, SAUrx, send_size, recv_size>::recv_;
}
