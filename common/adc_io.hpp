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
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
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
		inline void sleep_() { asm("nop"); }

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief	コンストラクター
		 */
		//-----------------------------------------------------------------//
		adc_io() { }


		//-----------------------------------------------------------------//
		/*!
			@brief	スタート
			@param[in]	refp	＋基準電圧選択
			@param[in]	refm	－基準電圧選択
		 */
		//-----------------------------------------------------------------//
		void start(REFP refp, REFM refm)
		{
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

			utils::delay::micro_second(1);
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	A/D 変換結果を取得
			@param[in]	ch	変換チャネル
			@return 変換結果（上位１０ビット）
		 */
		//-----------------------------------------------------------------//
		uint16_t get(uint8_t ch)
		{
			ADS = ch;
			ADM0.ADCS = 1;  // start
///			while(intr::IF1H.ADIF() == 0) sleep_();
///			intr::IF1H.ADIF = 0;
			while(ADM0.ADCS() != 0) sleep_();
			return ADCR();
		}
	};
}
