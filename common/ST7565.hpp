#pragma once
//=====================================================================//
/*!	@file
	@brief	ST7565 LCD ドライバー
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include <cstdint>
#include "common/csi_io.hpp"

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  ST7565 テンプレートクラス
		@param[in]	CSI_IO	CSI(SPI) 制御クラス
		@param[in]	CTRL	デバイス選択、レジスター選択、制御クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class CSI_IO>
	class ST7565 {

		CSI_IO&	csi_;

//		CTRL	ctrl_;

		enum class CMD : uint8_t {
			DISPLAY_OFF = 0xAE,
			DISPLAY_ON  = 0xAF,
			SET_DISP_START_LINE = 0x40,
			CMD_SET_PAGE = 0xB0,

			SET_COLUMN_UPPER = 0x10,
			SET_COLUMN_LOWER = 0x00,

			SET_ADC_NORMAL  = 0xA0,
			SET_ADC_REVERSE = 0xA1,

			SET_DISP_NORMAL = 0xA6,
			SET_DISP_REVERSE = 0xA7,

			SET_ALLPTS_NORMAL = 0xA4,
			SET_ALLPTS_ON = 0xA5,
			SET_BIAS_9 = 0xA2, 
			SET_BIAS_7 = 0xA3,

			RMW = 0xE0,
			RMW_CLEAR = 0xEE,
			INTERNAL_RESET = 0xE2,
			SET_COM_NORMAL = 0xC0,
			SET_COM_REVERSE = 0xC8,
			SET_POWER_CONTROL = 0x28,
			SET_RESISTOR_RATIO = 0x20,
			SET_VOLUME_FIRST = 0x81,
			SET_VOLUME_SECOND = 0x00,
			SET_STATIC_OFF = 0xAC,
			SET_STATIC_ON = 0xAD,
			SET_STATIC_REG = 0x00,
			SET_BOOSTER_FIRST = 0xF8,
			SET_BOOSTER_234 = 0,
			SET_BOOSTER_5 = 1,
			SET_BOOSTER_6 = 3,
			NOP = 0xE3,
			TEST = 0xF0,
		};

		inline void write_(CMD cmd) {
			csi_.write(static_cast<uint8_t>(cmd));
		}

		inline void write_(CMD cmd, uint8_t ord) {
			csi_.write(static_cast<uint8_t>(cmd) | ord);
		}

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief  コンストラクター
		*/
		//-----------------------------------------------------------------//
		ST7565(CSI_IO& csi) : csi_(csi) { }


		//-----------------------------------------------------------------//
		/*!
			@brief  ブライトネス設定
			@param[in]	val	ブライトネス値
		*/
		//-----------------------------------------------------------------//
		void set_brightness(uint8_t val)
		{
    		write_(CMD::SET_VOLUME_FIRST);
    		write_(CMD::SET_VOLUME_SECOND, (val & 0x3f));
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  開始
			@param[in]	contrast コントラスト
		*/
		//-----------------------------------------------------------------//
		void start(uint8_t contrast)
		{
			init();
			write_(CMD::DISPLAY_ON);
	  		write_(CMD::SET_ALLPTS_NORMAL);
			set_brightness(contrast);

			utils::delay::milli_second(10);

			P0.B1 = 1;  // /CS = 1

			P0.B1 = 0;
			for(uint8_t page = 0; page < 8; ++page) {
				P0.B0 = 0;
				csi_.write(0xb0 + page);
				csi_.write(0x10);  // column upper
				csi_.write(0x04);  // column lower
				P0.B0 = 1;
				for(uint8_t i = 0; i < 128; ++i) {
					csi_.write(i);
				}
			}
			P0.B1 = 1;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  初期化
		*/
		//-----------------------------------------------------------------//
		void init()
		{
			// set pin directions
			// pinMode(sid, OUTPUT);
			// pinMode(sclk, OUTPUT);
			// pinMode(a0, OUTPUT);
			// pinMode(rst, OUTPUT);
			// pinMode(cs, OUTPUT);
			PM0.B0 = 0;  // (A0) output
			PM0.B1 = 0;  // (/CS) output

			P0.B0 = 0;  // /CS = 0
			P0.B1 = 0;  // A0 = 0;

			// toggle RST low to reset; CS low so it'll listen to us
			// if (cs > 0)
    		//     digitalWrite(cs, LOW);

			// digitalWrite(rst, LOW);
			// _delay_ms(500);
			// digitalWrite(rst, HIGH);

			// LCD bias select
			write_(CMD::SET_BIAS_7);
			// ADC select
			write_(CMD::SET_ADC_NORMAL);
			// SHL select
			write_(CMD::SET_COM_NORMAL);
			// Initial display line
			write_(CMD::SET_DISP_START_LINE);

			// turn on voltage converter (VC=1, VR=0, VF=0)
			write_(CMD::SET_POWER_CONTROL, 0x4);
			// wait for 50% rising
			utils::delay::milli_second(50);

			// turn on voltage regulator (VC=1, VR=1, VF=0)
			write_(CMD::SET_POWER_CONTROL, 0x6);

			// wait >=50ms
			utils::delay::milli_second(50);

			// turn on voltage follower (VC=1, VR=1, VF=1)
			write_(CMD::SET_POWER_CONTROL, 0x7);

			// wait 10ms
			utils::delay::milli_second(10);

			// set lcd operating voltage (regulator resistor, ref voltage resistor)
			write_(CMD::SET_RESISTOR_RATIO, 0x6);
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  コピー
			@param[in]	p	フレームバッファソース
		*/
		//-----------------------------------------------------------------//
		void copy(const uint8_t* p) const {
#if 0
			ctrl_.lcd_sel(0);
			for(uint8_t page = 0; page < 4; ++page) {
				ctrl_.a0_out(0);
				spi_.write(0xb0 + page);
				spi_.write(0x10);  // column upper
				spi_.write(0x04);  // column lower
				ctrl_.a0_out(1);
				for(uint8_t i = 0; i < 128; ++i) {
					spi_.write(*p);
					++p;
				}
			}
			ctrl_.lcd_sel(1);
#endif
		}

	};
}
