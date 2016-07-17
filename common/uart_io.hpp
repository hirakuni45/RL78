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
		@param[in]	recv_size	受信バッファサイズ
		@param[in]	send+size	送信バッファサイズ
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <uint16_t recv_size, uint16_t send_size>
	class uart_io {

		static utils::fifo<recv_size> recv_;
		static utils::fifo<send_size> send_;

		bool	polling_ = false;
		bool	crlf_ = true;

		// ※必要なら、実装する
		void sleep_() { }

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

			PER0.SAU0EN = 1;

			SPS0.PRS0 = 1;	// 16MHz
			SMR00 = 0x20 | SMR00.MD.b(1);
			// 1 stop, 8 data
			SCR00 = 0x0004 | SCR00.TXE.b(1) | SCR00.RXE.b(1) | SCR00.SLC.b(1) | SCR00.DLS.b(3);
			SDR00H = 68;  // 16MHz / 136 := 115200 (115942)
			SOL00 = 0;
			SO00 = 1;
			SOE00 = 1;

			PM1.B2 = 0;	/// P12 output
///			POM1.B2 = 1;  /// open drain output
			P1.B2 = 1;	/// "1"

			SS00 = 1;

			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	出力バッファのサイズを返す
			@return　バッファのサイズ
		 */
		//-----------------------------------------------------------------//
		uint16_t send_length() const {
			if(polling_) {
				return 0;
			} else {
				return send_.length();
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
				while(SSR00.BFF() != 0) sleep_();
				SDR00L = ch;
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
			@brief	文字列出力
			@param[in]	s	出力ストリング
		 */
		//-----------------------------------------------------------------//
		void puts(const char* s) {
			char ch;
			while((ch = *s++) != 0) {
				putch(ch);
			}
		}
	};

	// recv_、send_ の実体を定義
	template<uint16_t recv_size, uint16_t send_size>
		utils::fifo<recv_size> uart_io<recv_size, send_size>::recv_;
	template<uint16_t recv_size, uint16_t send_size>
		utils::fifo<send_size> uart_io<recv_size, send_size>::send_;
}
