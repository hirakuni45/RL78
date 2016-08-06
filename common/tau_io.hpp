#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/G13 グループ タイマ・アレイ・ユニット制御 @n
			Copyright 2016 Kunihito Hiramatsu
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include "G13/system.hpp"
#include "G13/tau.hpp"
#include "G13/intr.hpp"

/// F_CLK はボーレートパラメーター計算で必要、設定が無いとエラーにします。
#ifndef F_CLK
#  error "tau_io.hpp requires F_CLK to be defined"
#endif

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  タイマ・アレイ・ユニット・クラス・テンプレート
		@param[in]	TAU	タイマ・アレイ・ユニット・クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class TAU>
	class tau_io {

		uint8_t	level_ = 0;

		void enable_per_(bool f = true) {  // 対応するユニットを有効にする
			if(TAU::get_unit_no() == 0) {
				PER0.TAU0EN = f;
			} else {
				PER0.TAU1EN = f;
			}
		}

		bool set_freq_(uint32_t freq, uint8_t md) {
			// 設定周期の算出
			auto div = static_cast<uint32_t>(F_CLK) / freq;
			uint8_t master = 0;
			while(div > 65536) {
				div /= 2;
				++master;
				if(master >= 16) {
					return false;
				}
			}

			uint8_t cks = 0;
			if(TAU::get_chanel_no() < 4) {
				TAU::TPS.PRS0 = master;
			} else {
				TAU::TPS.PRS1 = master;
				cks = 2;
			}

			TAU::TMR = TAU::TMR.CKS.b(cks) | TAU::TMR.MD.b(md) | TAU::TMR.MD0.b(1);
			TAU::TDR = div - 1;

			return true;
		}

		void set_port_dir_(bool outena) {
			switch(TAU::get_chanel_no()) {
			case 0:
				PM0.B1 = !outena;
				P0.B1 = 0;
				PMC0.B1 = 0;
				break;
			case 1:
				PM1.B6 = !outena;
				P1.B6 = 0;
				break;
			case 2:
				PM1.B7 = !outena;
				P1.B7 = 0;
				break;
			case 3:
				PM3.B1 = !outena;
				P3.B1 = 0;
				break;
			case 4:
				PM4.B2 = !outena;
				P4.B2 = 0;
				break;
			case 5:
				PM0.B5 = !outena;
				P0.B5 = 0;
				break;
			case 6:
				PM0.B6 = !outena;
				P0.B6 = 0;
				break;
			case 7:
				PM4.B1 = !outena;
				P4.B1 = 0;
				break;
			}
		}

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief	インターバル・タイマー機能
			@param[in]	freq	周波数
			@param[in]	level	割り込みレベル（１～２）、０の場合はポーリング
			@param[in]	outena	出力ポートに信号を出す（周波数の半分）
			@return エラーなら「false」（設定周期範囲外）
		 */
		//-----------------------------------------------------------------//
		bool start_interval(uint32_t freq, uint8_t level, bool outena = false)
		{
			level_ = level;

			enable_per_();

			if(!set_freq_(freq, 0)) {
				return false;
			}

			set_port_dir_(outena);

			TAU::TO = 0;  // 出力初期値
			TAU::TOE = outena;

			TAU::TS = 1;  // タイマースタート

			return true;
		}
	};
}
