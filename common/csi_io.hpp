#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/G13 グループ SAU/CSI 制御
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include "G13/system.hpp"
#include "G13/sau.hpp"
#include "G13/intr.hpp"

#include "common/format.hpp"

/// F_CLK はボーレートパラメーター計算で必要、設定が無いとエラーにします。
#ifndef F_CLK
#  error "csi_io.hpp requires F_CLK to be defined"
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
	public:

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  データ、クロック位相タイプ
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		enum class PHASE : uint8_t {
			TYPE1,  ///< タイプ１
			TYPE2,  ///< タイプ２
			TYPE3,  ///< タイプ３
			TYPE4,  ///< タイプ４ (SD カードアクセス）
		};

	private:
		uint8_t	intr_level_;

		inline void sleep_() { asm("nop"); }

		void sync_() {
			if(SAU::get_unit_no() == 0) {
				switch(SAU::get_chanel_no()) {
				case 0:
					while(intr::IF0H.CSIIF00() == 0) sleep_();
					intr::IF0H.CSIIF00 = 0;
					break;
				case 1:
					while(intr::IF0H.CSIIF01() == 0) sleep_();
					intr::IF0H.CSIIF01 = 0;
					break;
				case 2:
					while(intr::IF1L.CSIIF10() == 0) sleep_();
					intr::IF1L.CSIIF10 = 0;
					break;
				case 3:
					while(intr::IF1L.CSIIF11() == 0) sleep_();
					intr::IF1L.CSIIF11 = 0;
					break;
				}
			} else {
				switch(SAU::get_chanel_no()) {
				case 0:
					while(intr::IF0H.CSIIF20() == 0) sleep_();
					intr::IF0H.CSIIF20 = 0;
					break;
				case 1:
					while(intr::IF0H.CSIIF21() == 0) sleep_();
					intr::IF0H.CSIIF21 = 0;
					break;
				case 2:
					while(intr::IF1H.CSIIF30() == 0) sleep_();
					intr::IF1H.CSIIF30 = 0;
					break;
				case 3:
					while(intr::IF1H.CSIIF31() == 0) sleep_();
					intr::IF1H.CSIIF31 = 0;
					break;
				}
			}
		}

		// 割り込み設定
		static void interrupt_mask_(bool f)
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

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief  割り込みエントリー
		*/
		//-----------------------------------------------------------------//
		static void task() __attribute__ ((section (".lowtext")))
		{
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  コンストラクター
		*/
		//-----------------------------------------------------------------//
		csi_io() : intr_level_(0) { }


		//-----------------------------------------------------------------//
		/*!
			@brief  通信速度を設定して、CSI を有効にする
			@param[in]	speed	通信速度
			@param[in]	dctype	データ、クロック位相タイプ
			@param[in]	level	割り込みレベル（１～２）、０の場合はポーリング
			@return エラー（速度設定範囲外）なら「false」
		*/
		//-----------------------------------------------------------------//
		bool start(uint32_t speed, PHASE dctype, uint8_t level = 0)
		{
			intr_level_ = level;

			destroy();

			// ボーレートから分周比の算出
			if(speed == 0) {
				return false;
			}
			if(SAU::get_unit_no() == 0 && SAU::get_chanel_no() == 0) {
				if(speed > 16000000) return false;
			} else {
				if(speed > 8000000) return false;
			}
			auto div = static_cast<uint32_t>(F_CLK) / speed;
			uint8_t master = 0;
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

			// 対応するユニットを有効にする
			if(SAU::get_unit_no() == 0) {
				PER0.SAU0EN = 1;
			} else {
				PER0.SAU1EN = 1;
			}

			// 各ユニットで、チャネル０、１、２、３で共有の為、
			// ０、１：PRS0、２、３：PRS1 を使う
			bool cks = 0;
			if(SAU::get_chanel_no() < 2) {
				SAU::SPS.PRS0 = master;
			} else {
				SAU::SPS.PRS1 = master;
				cks = 1;
			}

			SAU::SMR = 0x0020 | SAU::SMR.CKS.b(cks);

			// MSB first, 8 bits data
			SAU::SCR = 0x0004 | SAU::SCR.TXE.b(1) | SAU::SCR.RXE.b(1) | SAU::SCR.DLS.b(3)
				| SAU::SCR.DAP.b(static_cast<uint8_t>(dctype) >> 1)
				| SAU::SCR.CKP.b(static_cast<uint8_t>(dctype) & 1);

			// ボーレート・ジェネレーター設定
			SAU::SDR = div << 8;

			SAU::SOL = 0;
			SAU::CKO = !SAU::SCR.CKP();	// クロック出力設定
			SAU::SO  = 1;	// シリアル出力設定
			SAU::SOE = 1;	// シリアル出力許可

			// 対応するポートの設定
			if(SAU::get_unit_no() == 0) {
				if(SAU::get_chanel_no() == 0) {  // Chanel 0
					PU1.B2 = 0;
					PU1.B1 = 0;
					PU1.B0 = 0;
					PM1.B2 = 0;	 // P1-2 output (SO00)
					PM1.B1 = 1;	 // P1-1 input  (SI00)
					PM1.B0 = 0;  // P1-0 output (SCK00)
					PIM1.B1 = 1; // P1-1 TTL input
					P1.B2  = 1;	 // ポートレジスター (SO00)  切り替え
					P1.B1  = 1;  // ポートレジスター (SI00)  切り替え
					P1.B0  = 1;  // ポートレジスター (SCK00) 切り替え
				} else if(SAU::get_chanel_no() == 1) {  // Chanel 1
					PU7.B3 = 0;
					PU7.B4 = 0;
					PU7.B5 = 0;
					PM7.B3 = 0;  // P7-3 output (SO01)
					PM7.B4 = 1;  // P7-4 input  (SI01)
					PM7.B5 = 0;  // P7-5 output (SCK01)
					P7.B3 = 1;   // ポートレジスター (SO01)  切り替え
					P7.B4 = 1;   // ポートレジスター (SI01)  切り替え
					P7.B5 = 1;   // ポートレジスター (SCK01) 切り替え
				} else if(SAU::get_chanel_no() == 2) {  // Chanel 2
					PU0.B2 = 0;
					PU0.B3 = 0;
					PU0.B4 = 0;
					PM0.B2  = 0;  // P0-2 output (SO10)
					PM0.B3  = 1;  // P0-3 input  (SI10)
					PM0.B4  = 0;  // P0-4 output (SCK10)
					PMC0.B2 = 0;  // ポートモードコントロール
					PMC0.B3 = 0;  // ポートモードコントロール
					PIM0.B3 = 1;  // ポート入力モード
					P0.B2   = 1;  // ポートレジスター SO10  切り替え
					P0.B3   = 1;  // ポートレジスター SI10  切り替え
					P0.B4   = 1;  // ポートレジスター SCK10 切り替え
				} else {  // chanel 3
					PU5.B2 = 0;
					PU5.B1 = 0;
					PU3.B0 = 0;
					PM5.B1 = 0;  // P5-1 output (SO11)
					PM5.B0 = 1;  // P5-0 input  (SI11)
					PM3.B0 = 0;  // P3-0 output (SCK11)
					P5.B1 = 1;   // ポートレジスター SO11  切り替え
					P5.B0 = 1;   // ポートレジスター SI11  切り替え
					P3.B0 = 1;   // ポートレジスター SCK11 切り替え
				}
			} else {
				if(SAU::get_chanel_no() == 0) {  // Chanel 0
					PU1.B3 = 0;
					PU1.B4 = 0;
					PU1.B5 = 0;
					PM1.B3 = 0;	 // P1-3 output (SO20)
					PM1.B4 = 1;	 // P1-4 input  (SI20)
					PM1.B5 = 0;  // P1-5 output (SCK20)
					P1.B3  = 1;	 // ポートレジスター SO20  切り替え
					P1.B4  = 1;  // ポートレジスター SI20  切り替え
					P1.B5  = 1;  // ポートレジスター SCK20 切り替え
				} else if(SAU::get_chanel_no() == 1) {  // Chanel 1
					PU7.B2 = 0;
					PU7.B1 = 0;
					PU7.B0 = 0;
					PM7.B2 = 0;  // P7-2 output (SO21)
					PM7.B1 = 1;  // P7-1 input  (SI21)
					PM7.B0 = 0;  // P7-0 output (SCK21)
					P7.B2  = 1;  // ポートレジスター SO21  切り替え
					P7.B1  = 1;  // ポートレジスター SI21  切り替え
					P7.B0  = 1;  // ポートレジスター SCK21 切り替え
				} else if(SAU::get_chanel_no() == 2) {  // Chanel 2（128ピンデバイスでサポート）

				} else {  // Chanel 3（128ピンデバイスでサポート）

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
			@brief  送受信
			@param[in]	ch	送信データ
			@return	受信データ
		*/
		//-----------------------------------------------------------------//
		inline uint8_t xchg(uint8_t ch = 0xff)
		{
			if(intr_level_) {
				return 0;
			} else {
				SAU::SDR_L = ch;
				sync_();
				return SAU::SDR_L();
			}
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  シリアル送信
			@param[in]	src	送信ソース
			@param[in]	cnt	送信サイズ
		*/
		//-----------------------------------------------------------------//
		void send(const void* src, uint16_t size)
		{
			const uint8_t* p = static_cast<const uint8_t*>(src);
			auto end = p + size;
			while(p < end) {
				xchg(*p);
				++p;
			}
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  シリアル受信
			@param[out]	dst	受信先
			@param[in]	cnt	受信サイズ
		*/
		//-----------------------------------------------------------------//
		void recv(void* dst, uint16_t size)
		{
			uint8_t* p = static_cast<uint8_t*>(dst);
			auto end = p + size;
			while(p < end) {
				*p = xchg();
				++p;
			}
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  CSI をストールさせて、無効にする。
		*/
		//-----------------------------------------------------------------//
		void destroy()
		{
			interrupt_mask_(1);  // set mask
			SAU::ST = 1;  // SAU stop
			SAU::SS = 0;	// unit disable
			SAU::SOE = 0;
			SAU::CKO = 0;
			SAU::SO = 0;
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
