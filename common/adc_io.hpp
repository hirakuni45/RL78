#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/G13 グループ A/D 制御 @n
			Copyright 2016 Kunihito Hiramatsu
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include "G13/system.hpp"
#include "G13/adc.hpp"
#include "G13/intr.hpp"
#include "common/delay.hpp"

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  A/D 制御クラス
		@param[in]	NUM		最大チャネル数
		@param[in]	TASK	割り込みタスク
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <uint16_t NUM, class TASK>
	class adc_io {
	public:

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  ＋基準電圧タイプ
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		enum class REFP : uint8_t {
			VDD,      ///< VDD
			VREFP,    ///< P20/VREFP
			INT_1_45  ///< 内臓 1.45V リファレンス
		};


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  －基準電圧タイプ
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		enum class REFM : uint8_t {
			VSS,    ///< VSS
			VREFM,  ///< P21/VREFM
		};

	private:
		static TASK task_;

		uint8_t	level_;

		static volatile uint16_t value_[NUM];

		static inline void sleep_() { asm("nop"); }

	public:
#pragma GCC optimize ("-fno-lto")
		//-----------------------------------------------------------------//
		/*!
			@brief  インターバル・タイマー割り込みタスク
		*/
		//-----------------------------------------------------------------//
		static __attribute__ ((interrupt)) void task() __attribute__ ((section (".lowtext")))
		{ 
			value_[ADS()] = ADCR();
			if(ADS() < NUM) {
				ADS = ADS() + 1;
				ADM0.ADCS = 1;  // start
			}
			task_();
		}
#pragma GCC optimize ("-flto")

		//-----------------------------------------------------------------//
		/*!
			@brief	コンストラクター
		 */
		//-----------------------------------------------------------------//
		adc_io() : level_(0) {
			for(uint8_t i = 0; i < NUM; ++i) value_[i] = 0;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	スタート
			@param[in]	refp	＋基準電圧選択
			@param[in]	refm	－基準電圧選択
			@param[in]	level	割り込みレベル（１～２）、０の場合はポーリング
		 */
		//-----------------------------------------------------------------//
		void start(REFP refp, REFM refm, uint8_t level)
		{
			level_ = level;

			// ADC 許可
			PER0.ADCEN = 1;

			// ポート設定
//			ADPC = 0x00;  // all port A/D

			ADM0.FR = 6;  // fclk/4
			ADM0.LV = 0;  // 19 fAD

			ADM2.ADREFP = static_cast<uint8_t>(refp);  // 0:VDD, 1:P20/VREFP, 2:Internal 1.45V
			ADM2.ADREFM = static_cast<uint8_t>(refm);  // 0:VSS, 1:P21/VREFM
			ADM2.ADTYP  = 0;  // 0: 10bits, 1:8bits

			ADM0.ADCE = 1;  // A/D 許可
			ADM0.ADMD = 0;  // セレクトモード

			ADM1.ADTMD = 0;  // soft trigger
			ADM1.ADSCM = 1;  // one shot convert

			// 割り込みレベル設定とマスク解除
			if(level > 0) {
				--level;
				level ^= 0x03;
				intr::PR01H.ADPR = (level) & 1;
				intr::PR11H.ADPR = (level & 2) >> 1;
				intr::MK1H.ADMK = 0;
			}

			utils::delay::micro_second(1);
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	読み込み同期（ポーリングの場合は無視される）
		 */
		//-----------------------------------------------------------------//
		void sync() const {
			if(level_ == 0) return;
			while(ADS() < NUM) sleep_();
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	スキャン開始（割り込みモードの場合のみ有効）
			@param[in]	top	開始チャネル
		 */
		//-----------------------------------------------------------------//
		void start_scan(uint8_t top = 0)
		{
			ADS = top;
			ADM0.ADCS = 1;  // start
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	A/D 変換結果を取得
			@param[in]	ch	変換チャネル
			@return 変換結果（上位１０ビットが有効な値）
		 */
		//-----------------------------------------------------------------//
		uint16_t get(uint8_t ch)
		{
			if(ch >= NUM) {
				return 0xffff;
			}

			if(level_ == 0) {
				ADS = ch;
				ADM0.ADCS = 1;  // start
				while(intr::IF1H.ADIF() == 0) sleep_();
				intr::IF1H.ADIF = 0;
				return ADCR();
			} else {
				return value_[ch];
			}
		}
	};

	template<uint16_t NUM, class TASK>
		volatile uint16_t adc_io<NUM, TASK>::value_[NUM]; 
}
