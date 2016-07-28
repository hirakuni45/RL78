#pragma once
//=====================================================================//
/*!	@file
	@brief	ST7565 LCD ドライバー
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include <cstdint>
#include "common/spi_io.hpp"

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  ST7565 テンプレートクラス
		@param[in]	SPI_IO	SPI インターフェース・クラス
		@param[in]	CTRL	デバイス選択、レジスター選択、制御クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class SPI_IO, class CTRL>
	class ST7565_io {

		SPI_IO&	spi_;

		CTRL	ctrl_;

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief  コンストラクター
		*/
		//-----------------------------------------------------------------//
		ST7565_io(SPI_IO& spi) : spi_(spi) { }


		//-----------------------------------------------------------------//
		/*!
			@brief  開始
		*/
		//-----------------------------------------------------------------//
		void start() const {
			// 100ms setup...
			for(uint8_t i = 0; i < 10; ++i) {
				utils::delay::micro_second(10000);
			}

			ctrl_.a0_out(0);
			ctrl_.lcd_sel(0);	// device enable

			spi_.write(0xae);	// display off
			spi_.write(0x40);	// display start line of 0
			spi_.write(0xa1);	// ADC set to reverse
			spi_.write(0xc0);	// common output mode: set scan direction normal operation
			spi_.write(0xa6);	// display normal (none reverse)
			spi_.write(0xa2);	// select bias b0:0=1/9, b0:1=1/7
			spi_.write(0x2f);	// all power control circuits on

			spi_.write(0xf8);	// set booster ratio to
			spi_.write(0x00);	// 4x

			spi_.write(0x27);	// set V0 voltage resistor ratio to large

			spi_.write(0x81);	// set contrast
			spi_.write(0x0);	// contrast value, EA default: 0x016

			spi_.write(0xac);	// indicator
			spi_.write(0x00);	// disable

			spi_.write(0xa4);	// all pixel on disable
			spi_.write(0xaf);	// display on

			for(uint8_t i = 0; i < 5; ++i) {
				utils::delay::micro_second(10000);
			}

			ctrl_.lcd_sel(1);	// device disable
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  コピー
			@param[in]	p	フレームバッファソース
		*/
		//-----------------------------------------------------------------//
		void copy(const uint8_t* p) const {
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
		}

	};
}
