#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/(G13/L1C) グループ タイマ・アレイ・ユニット制御
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016, 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include "common/renesas.hpp"
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
	public:
		typedef TAU tau_type;

	private:
		static TASK task_;

		uint8_t	intr_level_ = 0;

		enum class mode {
			INTERVAL = 0,           ///< インターバル・タイマ
			CAPTURE = 2,            ///< キャプチャ
			EVENT = 3,              ///< イベント・カウンタ
			ONE_COUNT = 4,          ///< ワンカウント
			CAPTURE_ONE_COUNT = 6,  ///< キャプチャ＆ワンカウント
		};

		mode	mode_;

		void set_interrupt_() {

			if(intr_level_ == 0) {
				intr::enable(TAU::get_peripheral(), false);
				return;
			}

			auto level = intr_level_;
			--level;
			level ^= 0x03;
			intr::set_level(TAU::get_peripheral(), level);
			intr::enable(TAU::get_peripheral());
		}

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief	割り込みタスク
		 */
		//-----------------------------------------------------------------//
		static void task() __attribute__ ((section (".lowtext")))
		{
			task_();
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	割り込みタスクへの参照
			@return 割り込みタスク（参照）
		 */
		//-----------------------------------------------------------------//
		TASK& at_task() { return task_; }


		//-----------------------------------------------------------------//
		/*!
			@brief	値の設定
			@param[in]	val	設定値
		 */
		//-----------------------------------------------------------------//
		void set_value(uint16_t val)
		{
			TAU::TDR = val;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	値の取得
			@return 値
		 */
		//-----------------------------------------------------------------//
		uint16_t get_value() const
		{
			return TAU::TDR();
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	周波数設定（初期周波数からの変更）
			@param[in]	freq	周波数
			@return エラーなら「false」（設定周期範囲外）
		 */
		//-----------------------------------------------------------------//
		bool set_freq(uint32_t freq)
		{
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

			// プリスケーラーは、０～３：PRS0、４～７：PRS1
			uint8_t cks = 0;
			if(TAU::get_chanel_no() < 4) {
				TAU::TPS.PRS0 = master;
			} else {
				TAU::TPS.PRS1 = master;
				cks = 2;
			}

			TAU::TMR = TAU::TMR.CKS.b(cks) | TAU::TMR.MD.b(static_cast<uint8_t>(mode_)) | TAU::TMR.MD0.b(1);
			TAU::TDR = div - 1;

			return true;
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

			manage::enable(TAU::get_peripheral());

			mode_ = mode::INTERVAL;
			if(!set_freq(freq)) {
				return false;
			}

			if(outena) {
				manage::set_tau_port(TAU::get_peripheral(), true);
			}

			TAU::TOM = 0;  // タイマ出力モード（マスター）
			TAU::TO = 0;  // 出力初期値
			TAU::TOE = outena;

			TAU::TS = 1;  // タイマースタート

			set_interrupt_();

			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	インターバル・タイマー機能（レジスター直接指定）
			@param[in]	master	マスター分周期（０～１５）
		   	@param[in]	value	ダウンカウンター（１６ビット）
			@param[in]	level	割り込みレベル（１～２）、０の場合はポーリング
			@param[in]	outena	出力ポートに信号を出す（周波数の半分）
			@return エラーなら「false」（設定周期範囲外）
		 */
		//-----------------------------------------------------------------//
		bool start_interval(uint8_t master, uint16_t value, uint8_t level, bool outena = false)
		{
			intr_level_ = level;

			manage::enable(TAU::get_peripheral());

			mode_ = mode::INTERVAL;

			// プリスケーラーは、０～３：PRS0、４～７：PRS1
			uint8_t cks = 0;
			if(TAU::get_chanel_no() < 4) {
				TAU::TPS.PRS0 = master;
			} else {
				TAU::TPS.PRS1 = master;
				cks = 2;
			}

			TAU::TMR = TAU::TMR.CKS.b(cks) | TAU::TMR.MD.b(static_cast<uint8_t>(mode_)) | TAU::TMR.MD0.b(1);
			TAU::TDR = value;

			if(outena) {
				manage::set_tau_port(TAU::get_peripheral(), true);
			}

			TAU::TOM = 0;  // タイマ出力モード（マスター）
			TAU::TO = 0;  // 出力初期値
			TAU::TOE = outena;

			TAU::TS = 1;  // タイマースタート

			set_interrupt_();

			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	ＰＷＭ機能
			@param[in]	master	マスター・チャネル
			@param[in]	val		初期値
			@param[in]	level	割り込みレベル（１～２）、０の場合はポーリング
			@return エラーなら「false」
		 */
		//-----------------------------------------------------------------//
		template <class MASTER>
		bool start_pwm(uint16_t val, uint8_t level)
		{
			if(MASTER::get_unit_no() != TAU::get_unit_no()) {
				return false;
			}

			switch(MASTER::get_chanel_no()) {
			case 0:
			case 2:
			case 4:
			case 6:
				break;
			default:
				return false;
			}

			intr_level_ = level;

			if(MASTER::get_chanel_no() != 0) {
				MASTER::TMR.MAS = 1;
			}
			uint8_t cks = MASTER::TMR.CKS();

			TAU::TMR = TAU::TMR.CKS.b(cks) | TAU::TMR.STS.b(0b100)
				| TAU::TMR.MD.b(static_cast<uint8_t>(mode::ONE_COUNT)) | TAU::TMR.MD0.b(1);
			TAU::TDR = val;

			manage::set_tau_port(TAU::get_peripheral(), true);

			TAU::TOM = 1;  // タイマ出力モード（スレーブ）
			TAU::TO = 0;  // 出力初期値
			TAU::TOE = 1;

			TAU::TS = 1;  // タイマースタート

			set_interrupt_();

			return true;
		}
	};

	template<class TAU, class TASK>
		TASK tau_io<TAU, TASK>::task_;
}
