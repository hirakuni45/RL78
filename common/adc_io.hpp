#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78 (G13/L1C) グループ A/D 制御
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016, 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include "G13/system.hpp"
#include "G13/adc.hpp"
#include "G13/intr.hpp"
#include "common/delay.hpp"

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  A/D 制御クラス @n
				温度センサ： @n
				  +25度：1.05V @n
				  + 1度：-3.6mV
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
		static volatile uint16_t temp_;  // 温度センサ
		static volatile uint8_t	temp_task_;
		static volatile bool	conv_fin_;

		static inline void sleep_() { asm("nop"); }

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief  インターバル・タイマー割り込みタスク
		*/
		//-----------------------------------------------------------------//
		static void task() __attribute__ ((section (".lowtext")))
		{ 
			uint8_t ch = ADS();
			if(ch < NUM) {
				value_[ch] = ADCR();
				++ch;
				if(ch < NUM) {
					ADS = ch;
					ADM0.ADCS = 1;  // start
				} else if(temp_task_) {
					ADS = 0x80;
					ADM0.ADCS = 1;  // start 1st
				} else {
					conv_fin_ = true;
				}
			} else if(ch == 0x80) {  // temp
				if(temp_task_ == 1) {
					temp_task_ = 2;
					ADM0.ADCS = 1;  // start 2nd
				} else {
					temp_ = ADCR();
					conv_fin_ = true;
				}
			}
			task_();
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	コンストラクター
		 */
		//-----------------------------------------------------------------//
		adc_io() : level_(0) { }


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

			while(!conv_fin_) sync();
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	スキャン開始（割り込みモードの場合のみ有効）
			@param[in]	top		開始チャネル
			@param[in]	temp	温度を取得する場合「true」
			@return エラーが無ければ「true」
		 */
		//-----------------------------------------------------------------//
		bool start_scan(uint8_t top = 0, bool temp = false)
		{
			if(level_ == 0) return false;

			conv_fin_ = false;

			if(temp) {
				temp_task_ = 1;
			}

			if(top < NUM) {
				ADS = top;
				ADM0.ADCS = 1;  // start
			} else if(temp) {
				ADS = 0x80;
				ADM0.ADCS = 1;  // start
			}

			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	A/D 変換結果を取得
			@param[in]	ch	変換チャネル
			@return 変換結果（上位１０ビットが有効な値）
		 */
		//-----------------------------------------------------------------//
		uint16_t get(uint8_t ch) const
		{
			if(ch >= NUM) {
				return 0x0000;
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


		//-----------------------------------------------------------------//
		/*!
			@brief	A/D 変換結果を取得
			@param[in]	ch	変換チャネル
			@return 変換結果（上位１０ビットが有効な値）
		 */
		//-----------------------------------------------------------------//
		uint16_t get_temp() const
		{
			if(level_ == 0) {
				ADS = 0x80;
				ADM0.ADCS = 1;  // start
				while(intr::IF1H.ADIF() == 0) sleep_();
				intr::IF1H.ADIF = 0;
				return ADCR();
			} else {
				return temp_;
			}
		}
	};


	template<uint16_t NUM, class TASK>
		volatile uint16_t adc_io<NUM, TASK>::value_[NUM] = { 0 };
	template<uint16_t NUM, class TASK>
		volatile uint16_t adc_io<NUM, TASK>::temp_ = 0;
	template<uint16_t NUM, class TASK>
		volatile uint8_t adc_io<NUM, TASK>::temp_task_ = 0;
	template<uint16_t NUM, class TASK>
		volatile bool adc_io<NUM, TASK>::conv_fin_ = false;
}
