#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/G13 グループ SAU/CSI 制御 @n
			Copyright 2016 Kunihito Hiramatsu
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include "G13/system.hpp"
#include "G13/sau.hpp"
#include "G13/intr.hpp"

/// F_CLK はボーレートパラメーター計算で必要、設定が無いとエラーにします。
#ifndef F_CLK
#  error "uart_io.hpp requires F_CLK to be defined"
#endif

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  CSI 制御クラス・テンプレート
		@param[in]	SAU		シリアル・アレイ・ユニット・クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class SAU>
	class csi_io {

		static SAU sau_;	///< インターフェース・リソース

		uint8_t	intr_level_ = 0;

		inline void sleep_() { asm("nop"); }

	public:
#if 0
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
#endif

		//-----------------------------------------------------------------//
		/*!
			@brief  通信速度を設定して、CSI を有効にする
			@param[in]	speed	通信速度
			@param[in]	level	割り込みレベル（１～２）、０の場合はポーリング
			@return エラーなら「false」
		*/
		//-----------------------------------------------------------------//
		bool start(uint32_t speed, uint8_t level = 0)
		{
			intr_level_ = level;

#if 0
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

			tx_.SS = 1;	// TxD enable
			rx_.SS = 1;	// RxD enable

			// マスクをクリアして、割り込み許可
			if(intr_level_ > 0) {
				--level;
				level ^= 0x03;
				if(rx_.get_unit_no() == 0) {
					if(rx_.get_chanel_no() == 1) {  // UART0
						intr::PR00H.SRPR0 = (level) & 1;
						intr::PR10H.SRPR0 = (level & 2) >> 1;
					} else {  // UART1
						intr::PR01L.SRPR1 = (level) & 1;
						intr::PR11L.SRPR1 = (level & 2) >> 1;
					}
				} else {
					if(rx_.get_chanel_no() == 1) {  // UART2
						intr::PR00H.SRPR2 = (level) & 1;
						intr::PR10H.SRPR2 = (level & 2) >> 1;
					} else {  //UART3
						intr::PR01H.SRPR3 = (level) & 1;
						intr::PR11H.SRPR3 = (level & 2) >> 1;
					}
				}
				recv_interrupt_mask_(0);
			}
#endif

			return true;
		}
	};

#if 0
	// send_、recv_, send_stall_ の実体を定義
	template<class SAUtx, class SAUrx, class BUFtx, class BUFrx>
		BUFtx uart_io<SAUtx, SAUrx, BUFtx, BUFrx>::send_;

	template<class SAUtx, class SAUrx, class BUFtx, class BUFrx>
		BUFrx uart_io<SAUtx, SAUrx, BUFtx, BUFrx>::recv_;

	template<class SAUtx, class SAUrx, class BUFtx, class BUFrx>
		volatile bool uart_io<SAUtx, SAUrx, BUFtx, BUFrx>::send_stall_ = true; 
#endif
}
