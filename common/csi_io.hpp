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

#include "common/format.hpp"

/// F_CLK はボーレートパラメーター計算で必要、設定が無いとエラーにします。
#ifndef F_CLK
#  error "uart_io.hpp requires F_CLK to be defined"
#endif

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  CSI 制御クラス・テンプレート
		@param[in]	SAUtx	シリアル・アレイ・ユニット・クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class SAU>
	class csi_io {

		uint8_t	intr_level_ = 0;

		inline void sleep_() { asm("nop"); }

	public:
		// 割り込み設定
		static inline void interrupt_mask_(bool f)
		{
			if(SAU::get_unit_no() == 0) {
				switch(SAU::get_chanel_no()) {
				case 0:
					intr::MK0H.CSIMK00 = f;
					break;
				case 1:
					intr::MK0H.CSIMK01 = f;
					break;
				case 2:
					intr::MK1L.CSIMK10 = f;
					break;
				case 3:
					intr::MK1L.CSIMK11 = f;
					break;
				}
			} else {
				switch(SAU::get_chanel_no()) {
				case 0:
					intr::MK0H.CSIMK20 = f;
					break;
				case 1:
					intr::MK0H.CSIMK21 = f;
					break;
				case 2:
					intr::MK1H.CSIMK30 = f;
					break;
				case 3:
					intr::MK1H.CSIMK31 = f;
					break;
				}
			}
		}

		static __attribute__ ((interrupt)) void send_task() __attribute__ ((section (".lowtext")))
		{
#if 0
			if(send_.length()) {
				tx_.SDR_L = send_.get();
			} else {
				send_intrrupt_mask_(true);
				send_stall_ = true;
			}
#endif
		}

		static __attribute__ ((interrupt)) void recv_task() __attribute__ ((section (".lowtext")))
		{
////			recv_.put(rx_.SDR_L());
		}


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

			// ボーレートから分周比の算出
			if(speed == 0) {
				return false;
			}
			auto div = static_cast<uint32_t>(F_CLK) / speed;
			if(div == 0) {
				return false;
			}
#if 0
			uint8_t master = 2;
			div /= 2;

			while(div > 256) {
				div /= 2;
				++master;
				if(master >= 16) {
					// 速度設定範囲外
					return false;
				}
			}
			--div;
			div &= 0xfe;
#endif
			uint8_t master = 4;
			div = 0;

			// 対応するユニットを有効にする
			if(SAU::get_unit_no() == 0) {
				PER0.SAU0EN = 1;
			} else {
				PER0.SAU1EN = 1;
			}

			// 各ユニットで、チャネル０、１、２、３で共有の為、
			// ０、１：PRS0、２、３：PRS1 を使う
			bool cks = 0;
			if(SAU::get_chanel_no() == 0 || SAU::get_chanel_no() == 1) {
				SAU::SPS.PRS0 = master;
			} else {
				SAU::SPS.PRS1 = master;
				cks = 1;
			}

			SAU::SMR = 0x0020 | SAU::SMR.CKS.b(cks) | SAU::SMR.MD0.b(1);

			// MSB first, 8 bits data
			SAU::SCR = 0x0004 | SAU::SCR.TXE.b(1) | SAU::SCR.RXE.b(1) | SAU::SCR.DLS.b(3);

			// ボーレート・ジェネレーター設定
			SAU::SDR = div << 8;

			SAU::SOL = 0;	// 
			SAU::CKO = 1;	// クロック出力設定
			SAU::SO  = 1;	// シリアル出力設定
			SAU::SOE = 1;	// シリアル出力許可

			// 対応するポートの設定
			if(SAU::get_unit_no() == 0) {
				if(SAU::get_chanel_no() == 0) {  // Chanel 0
					PM1.B2 = 0;	// P1-2 output (SO00)
					PM1.B1 = 1;	// P1-1 input  (SI00)
					PM1.B0 = 0; // P1-0 output (SCK00)
					P1.B2  = 1;	// ポートレジスター (SO00) 切り替え
					P1.B1  = 1; // ポートレジスター (SI00) 切り替え
					P1.B0  = 1; // ポートレジスター (SCK00) 切り替え
				} else if(SAU::get_chanel_no() == 1) {  // Chanel 1



				} else if(SAU::get_chanel_no() == 2) {  // Chanel 2
					PM0.B2  = 0;  // P0-2 output (SO10)
					PM0.B3  = 1;  // P0-3 input  (SI10)
					PM0.B4  = 0;  // P0-4 output (SCK10)
					PMC0.B2 = 0;  // ポートモードコントロール
					PMC0.B3 = 0;  // ポートモードコントロール
					PIM0.B3 = 1;  // ポート入力モード
					P0.B2   = 1;  // ポートレジスター SO10 切り替え
					P0.B3   = 1;  // ポートレジスター SI10 切り替え
					P0.B4   = 1;  // ポートレジスター SCK10 切り替え
				} else {  // chanel 3



				}
			} else {
				if(SAU::get_chanel_no() == 0) {  // Chanel 0
					PM1.B4 = 1;	// P1-4 input  (SI20)
					PM1.B3 = 0;	// P1-3 output (SO20)
					PM1.B5 = 0; // P1-5 output (SCK20)
					P1.B3  = 1;	// ポートレジスター SO20 切り替え
					P1.B5  = 1; // ポートレジスター SCK20 切り替え
				} else {  // Chanel3（128ピンデバイスでサポート）
					PM14.B3 = 1;  // P14-3 input (SI30)
					PM14.B4 = 0;  // P14-4 output (SO30)

					P14.B4  = 1;  // ポートレジスター SO30 切り替え
				}
			}

///			send_stall_ = true;

			SAU::SS = 1;	// enable

			// マスクをクリアして、割り込み許可
			if(intr_level_ > 0) {
				--level;
				level ^= 0x03;
				if(SAU::get_unit_no() == 0) {
					switch(SAU::get_chanel_no()) {
					case 0:
						intr::PR00H.CSIPR00 = (level) & 1;
						intr::PR10H.CSIPR00 = (level & 2) >> 1;
						break;
					case 1:
						intr::PR00H.CSIPR01 = (level) & 1;
						intr::PR10H.CSIPR01 = (level & 2) >> 1;
						break;
					case 2:
						intr::PR01L.CSIPR10 = (level) & 1;
						intr::PR11L.CSIPR10 = (level & 2) >> 1;
						break;
					case 3:
						intr::PR01L.CSIPR11 = (level) & 1;
						intr::PR11L.CSIPR11 = (level & 2) >> 1;
						break;
					}
				} else {
					switch(SAU::get_chanel_no()) {
					case 0:
						intr::PR00H.CSIPR20 = (level) & 1;
						intr::PR10H.CSIPR20 = (level & 2) >> 1;
						break;
					case 1:
						intr::PR00H.CSIPR21 = (level) & 1;
						intr::PR10H.CSIPR21 = (level & 2) >> 1;
						break;
					case 2:
						intr::PR01H.CSIPR30 = (level) & 1;
						intr::PR11H.CSIPR30 = (level & 2) >> 1;
						break;
					case 3:
						intr::PR01H.CSIPR31 = (level) & 1;
						intr::PR11H.CSIPR31 = (level & 2) >> 1;
						break;
					}
				}
				interrupt_mask_(0);
			}

			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  シリアル送信
			@param[in]	ch	送信データ
		*/
		//-----------------------------------------------------------------//
		void put(uint8_t ch)
		{
			if(intr_level_) {

			} else {
				while(SAU::SSR.TSF() != 0) sleep_();
				SAU::SDR_L = ch;
			}
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
