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
#include "common/task.hpp"

/// F_CLK はボーレートパラメーター計算で必要、設定が無いとエラーにします。
#ifndef F_CLK
#  error "tau_io.hpp requires F_CLK to be defined"
#endif

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  タイマ・アレイ・ユニット・クラス・テンプレート
		@param[in]	TAU		タイマ・アレイ・ユニット・クラス
		@param[in]	TASK	割り込み内処理
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class TAU, class TASK = utils::null_task>
	class tau_io {

		static TASK task_;

		uint8_t	intr_level_ = 0;

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

		void interrupt_mask_(bool f)
		{
			if(TAU::get_unit_no() == 0) {
				switch(TAU::get_chanel_no()) {
				case 0:
					intr::MK1L.TMMK00 = f;
					break;
				case 1:
					intr::MK1L.TMMK01 = f;
					break;
				case 2:
					intr::MK1L.TMMK02 = f;
					break;
				case 3:
					intr::MK1L.TMMK03 = f;
					break;
				case 4:
					intr::MK1H.TMMK04 = f;
					break;
				case 5:
					intr::MK2L.TMMK05 = f;
					break;
				case 6:
					intr::MK2L.TMMK06 = f;
					break;
				case 7:
					intr::MK2L.TMMK07 = f;
					break;
				}
			} else {
				switch(TAU::get_chanel_no()) {
				case 0:
					intr::MK2H.TMMK10 = f;
					break;
				case 1:
					intr::MK2H.TMMK11 = f;
					break;
				case 2:
					intr::MK2H.TMMK12 = f;
					break;
				case 3:
					intr::MK1H.TMMK13 = f;
					break;
				case 4:
					intr::MK3L.TMMK14 = f;
					break;
				case 5:
					intr::MK3L.TMMK15 = f;
					break;
				case 6:
					intr::MK3L.TMMK16 = f;
					break;
				case 7:
					intr::MK3L.TMMK17 = f;
					break;
				}
			}
		}

		void set_interrupt_() {
			if(intr_level_ == 0) {
				interrupt_mask_(1);
				return;
			}

			auto level = intr_level_;
			--level;
			level ^= 0x03;
			if(TAU::get_unit_no() == 0) {
				switch(TAU::get_chanel_no()) {
				case 0:
					intr::PR01L.TMPR00 = (level) & 1;
					intr::PR11L.TMPR00 = (level & 2) >> 1;
					break;
				case 1:
					intr::PR01L.TMPR01 = (level) & 1;
					intr::PR11L.TMPR01 = (level & 2) >> 1;
					break;
				case 2:
					intr::PR01L.TMPR02 = (level) & 1;
					intr::PR11L.TMPR02 = (level & 2) >> 1;
					break;
				case 3:
					intr::PR01L.TMPR03 = (level) & 1;
					intr::PR11L.TMPR03 = (level & 2) >> 1;
					break;
				case 4:
					intr::PR01H.TMPR04 = (level) & 1;
					intr::PR11H.TMPR04 = (level & 2) >> 1;
					break;
				case 5:
					intr::PR02L.TMPR05 = (level) & 1;
					intr::PR12L.TMPR05 = (level & 2) >> 1;
					break;
				case 6:
					intr::PR02L.TMPR06 = (level) & 1;
					intr::PR12L.TMPR06 = (level & 2) >> 1;
					break;
				case 7:
					intr::PR02L.TMPR07 = (level) & 1;
					intr::PR12L.TMPR07 = (level & 2) >> 1;
					break;
				}
			} else {
				switch(TAU::get_chanel_no()) {
				case 0:
					intr::PR02H.TMPR10 = (level) & 1;
					intr::PR12H.TMPR10 = (level & 2) >> 1;
					break;
				case 1:
					intr::PR02H.TMPR11 = (level) & 1;
					intr::PR12H.TMPR11 = (level & 2) >> 1;
					break;
				case 2:
					intr::PR02H.TMPR12 = (level) & 1;
					intr::PR12H.TMPR12 = (level & 2) >> 1;
					break;
				case 3:
					intr::PR01H.TMPR13 = (level) & 1;
					intr::PR11H.TMPR13 = (level & 2) >> 1;
					break;
				case 4:
					intr::PR03L.TMPR14 = (level) & 1;
					intr::PR13L.TMPR14 = (level & 2) >> 1;
					break;
				case 5:
					intr::PR03L.TMPR15 = (level) & 1;
					intr::PR13L.TMPR15 = (level & 2) >> 1;
					break;
				case 6:
					intr::PR03L.TMPR16 = (level) & 1;
					intr::PR13L.TMPR16 = (level & 2) >> 1;
					break;
				case 7:
					intr::PR03L.TMPR17 = (level) & 1;
					intr::PR13L.TMPR17 = (level & 2) >> 1;
					break;
				}
			}
			interrupt_mask_(0);
		}

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief	割り込みタスク
		 */
		//-----------------------------------------------------------------//
		static __attribute__ ((interrupt)) void task() __attribute__ ((section (".lowtext")))
		{
			task_();
		}


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
			intr_level_ = level;

			enable_per_();

			if(!set_freq_(freq, 0)) {
				return false;
			}

			set_port_dir_(outena);

			TAU::TO = 0;  // 出力初期値
			TAU::TOE = outena;

			TAU::TS = 1;  // タイマースタート

			set_interrupt_();

			return true;
		}
	};

	template<class TAU, class TASK>
		TASK tau_io<TAU, TASK>::task_;
}
