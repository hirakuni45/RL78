#pragma once
//=====================================================================//
/*!	@file
	@brief	PCM1772 ドライバー・クラス @n
			LOW-VOLTAGE AND LOW-POWER STEREO AUDIO @n
			DIGITAL-TO-ANALOG CONVERTER WITH LINEOUT AMPLIFIER
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include <cstdint>
#include "common/delay.hpp"

namespace chip {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  PCM1772 テンプレートクラス
		@param[in]	CSI_IO	CSI(SPI) 制御クラス
		@param[in]	SEL	デバイス選択
		@param[in]	PD	パワー・ダウン選択
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class CSI_IO, class SEL, class PD>
	class PCM1772 {

		CSI_IO&	csi_;

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief  コンストラクター
		*/
		//-----------------------------------------------------------------//
		PCM1772(CSI_IO& csi) : csi_(csi) { }

		//-----------------------------------------------------------------//
		/*!
			@brief  開始
		*/
		//-----------------------------------------------------------------//
		void start()
		{
			PD::DIR = 1;    // output /PD (power down)
			PD::P = 0;      // power down (assert reset)
			SEL::DIR = 1;	// output /MS (device select)
			SEL::P = 1;		// device disable

			utils::delay::milli_second(20);

			PD::P = 1;		// disable power down

			utils::delay::milli_second(5);

			SEL::P = 0;		// enable device
			csi_.xchg(0x01);  		// Register 1
			csi_.xchg(0b00111111);	// B7:MUTR, B6:MUTL, B5-B0:ATL
									// MUTR = 0 Soft Mute Control R (0:disable)
									// MUTL = 0 Soft Mute Control L (0:disable)
									// ATL = 111111 --->  Digital Attenuation Level Setting, L (0dB) 

			csi_.xchg(0x02);  		// Register 2
			csi_.xchg(0b00111111);	// B7-B6:RSV, B5-B0:ATR
									// ATR = 111111 --->  Digital Attenuation Level Setting, R (0dB) 

			csi_.xchg(0x03);  		// Register 3
			csi_.xchg(0b10000000);	// B7:OVER, B6:RINV, B5:AMIX, B4:DEM, B3-B0:FMT
									// B7 = 1 ---> 44.1KHz 256Fs
									// AMIX = 0 ---> Analog Mixing Control for External Analog Signal, AIN
									// DEM = 0 --->  44.1-kHz De-Emphasis Control (Disable)
									// FMT = 000 ---> 16- to 24-bit, left-justified format (000)

			csi_.xchg(0x04);  		// Register 4
			csi_.xchg(0b00000000);	// B7-B5:RSV, B4:ZCAT, B3-B1:RSV, B0:PWRD 
									// ZCAT = 0 --->  Zero Cross Attenuation (0:Normal attenuation)
									// PWRD = 0 ---> Power Down Control (0:Normal operation)
			SEL::P = 1;		// disable device
		}
	};
}
