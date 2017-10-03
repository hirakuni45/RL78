#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78 (G13/L1C) グループ SAU/CSI 制御 @n
			※現状では、割り込みに対応していない、ポーリングのみ動作可能
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016, 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include "common/renesas.hpp"
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
			manage::enable(SAU::get_peripheral());

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
			manage::set_csi_port(SAU::get_peripheral(), manage::csi_port::INOUT);

///			send_stall_ = true;

			SAU::SS = 1;	// enable

			// マスクをクリアして、割り込み許可
			if(intr_level_ > 0) {
				--level;
				level ^= 0x03;
				intr::set_level(SAU::get_peripheral(), level);
				intr::enable(SAU::get_peripheral());
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
				while(intr::get_request(SAU::get_peripheral()) == 0) sleep_();
				intr::set_request(itm::get_peripheral(), 0);
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
			intr::enable(SAU::get_peripheral(), false);
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
