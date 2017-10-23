#pragma once
//=====================================================================//
/*!	@file
	@brief	TLV320ADC3001 クラス @n
			Low-Power Stereo ADC With Embedded miniDSP
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include <cstdint>
#include "common/format.hpp"

namespace chip {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  TLV320ADC3001 テンプレートクラス
		@param[in]	I2C_IO	I2C I/O クラス
		@param[in]	DEV_ADR	I2C デバイス・アドレス @n
					I2C_ADR1:0, I2C_ADR0:0 ---> 0b0011000 @n
					I2C_ADR1:0, I2C_ADR0:1 ---> 0b0011001 @n
					I2C_ADR1:1, I2C_ADR0:0 ---> 0b0011010 @n
					I2C_ADR1:1, I2C_ADR0:1 ---> 0b0011011
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class I2C_IO, uint8_t DEV_ADR = 0b0011000>
	class TLV320ADC3001 {
	public:

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  Interface 形
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		enum class INF : uint8_t {
			I2S_16_SLAVE,		///< I2S/16Bits, BCLK:inp, WCLK:inp
			I2S_16_MASTER,		///< I2S/16Bits, BCLK:out, WCLK:out
			I2S_20_SLAVE,		///< I2S/20Bits, BCLK:inp, WCLK:inp
			I2S_20_MASTER,		///< I2S/20Bits, BCLK:out, WCLK:out
			I2S_24_SLAVE,		///< I2S/16Bits, BCLK:inp, WCLK:inp
			I2S_24_MASTER,		///< I2S/16Bits, BCLK:out, WCLK:out
			I2S_32_SLAVE,		///< I2S/16Bits, BCLK:inp, WCLK:inp
			I2S_32_MASTER,		///< I2S/16Bits, BCLK:out, WCLK:out
		};


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  サンプリング・タイプ 形
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		enum class TYPE : uint8_t {
			FS44_1,		///< 44.1KHz
			FS48,       ///< 48.0KHz
		};


		static const uint8_t PAGE_CTRL = 0x00;  ///< PAGE CTRL command


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  ページ（０）・マップ型
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		enum class CMD_PAGE0 : uint8_t {
			PAGE_CTRL,				///< Page control register
			SW_RESET,				///< S/W RESET

			CLOCK_GEN = 4,			///< Clock-gen multiplexing
			PLL_P_R,				///< PLL P and R-VAL
			PLL_J,					///< PLL J-VAL
			PLL_D_MSB,				///< PLL D-VAL MSB
			PLL_D_LSB,				///< PLL D-VAL LSB

			ADC_NADC = 18,			///< ADC NADC (0x12)
			ADC_MADC,				///< ADC MADC
			ADC_AOSR,				///< ADC AOSR
			ADC_IADC,				///< ADC IADC
			ADC_DSP,				///< ADC miniDSP engine decimation

			CLKOUT_MUX = 25,		///< CLKOUT MUX (0x19)
			CLKOUT_M_DIV,			///< CLKOUT M Divider
			ADC_AIFC,				///< ADC audio interface control 1
			CH_OFFSET_1,			///< Data slot offset programmabillity 1 (Ch_Offset_1)
			ADC_IFC_2,				///< ADC interface control 2
			BCLK_N_DIV,				///< BCLK N Divider
			S_AIFC_1,				///< Secondary audio interface control 1
			S_AIFC_2,				///< Secondary audio interface control 2
			S_AIFC_3,				///< Secondary audio interface control 3
			I2S_SYNC,				///< I^2S sync

			ADC_FLAG = 36,			///< ADC flag register
			CH_OFFSET_2,			///< Data slot offset programmabillity 2 (Ch_Offset_2)
			I2S_TDM,				///< I^2S TDM control register

			INTR_FLAG_1 = 42,  		///< Interrupt flags (overflow)
			INTR_FLAG_2,	  		///< Interrupt flags (overflow)

			INTR_FLAG_ADC_1 = 45,	///< Interrupt flags-ADC

			INTR_FLAG_ADC_2 = 47,	///< Interrupt flags-ADC
			INT1_CTRL,				///< INT1 interrupt control
			INT2_CTRL,				///< INT2 interrupt control

			DMCLK_GPIO2 = 51,		///< DMCLK/GPIO2 control
			DMDIN_GPIO1,			///< DMDIN/GPIO1 control
			DOUT,					///< DOUT (out pin) control

			ADC_SYNC_1 = 57,		///< ADC sync control 1
			ADC_SYNC_2,				///< ADC sync control 2
			ADC_CIC_FILTER,			///< ADC CIC filter gain control

			ADC_PROC = 61,			///< ADC processing block selection
			PROG_INST,				///< Programmable instruction mode control bits

			DMIC_PLARITY = 80,		///< Digital microphone polarity control
			ADC_DIGITAL,			///< ADC digital
			ADC_FINE_VOLUME,		///< ADC fine volume control
			LEFT_ADC_VOLUME,		///< Left ADC volume control
			RIGHT_ADC_VOLUME,		///< Right ADC volume control
			ADC_PHASE,				///< ADC phase compensation
			LEFT_AGC_CTRL_1,		///< Left AGC control 1
			LEFT_AGC_CTRL_2,		///< Left AGC control 2
			LEFT_AGC_MAX_GAIN,		///< Left AGC maximum gain
			LEFT_AGC_ATTACK_TIME,	///< Left AGC attack time
			LEFT_AGC_DECAY_TIME,	///< Left AGC decay time
			LEFT_AGC_NOISE_DEBOUNCE,	///< Left AGC noise debounce
			LEFT_AGC_SIGNAL_DEBOUNCE,	///< Left AGC signal debounce
			LEFT_AGC_GAIN,			///< Left AGC gain
			RIGHT_AGC_CTRL_1,		///< Right AGC control 1
			RIGHT_AGC_CTRL_2,		///< Right AGC control 2
			RIGHT_AGC_MAX_GAIN,		///< Right AGC maximum gain
			RIGHT_AGC_ATTACK_TIME,	///< Right AGC attack time
			RIGHT_AGC_DECAY_TIME,	///< Right AGC decay time
			RIGHT_AGC_NOISE_DEBOUNCE,	///< Right AGC noise debounce
			RIGHT_AGC_SIGNAL_DEBOUNCE,	///< Right AGC signal debounce
			RIGHT_AGC_GAIN,			///< Right AGC gain
		};


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  ページ１・マップ型
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		enum class CMD_PAGE1 : uint8_t {
			PAGE_CTRL,				///< Page control register

			DITHER_CTRL = 26,		///< Dither control

			MICBIAS_CTRL = 51,		///< 51 MICBIAS control
			LEFT_INPSEL_1 = 52,		///< 52 Left ADC input selection for left PGA

			LEFT_INPSEL_2 = 54,		///< 54 Left ADC input selection for left PGA
			RIGHT_INPSEL_1 = 55,	///< 55 Right ADC input selection for right PGA

			RIGHT_INPSEL_2 = 57,	///< 57 Right ADC input selection for right PGA

			LEFT_ANALOG = 59,		///< 59 Left analog PGA setting
			RIGHT_ANALOG,			///< 60 Right analog PGA setting
			ADC_LOW_CUR_MODE,		///< 61 ADC low-current modes
			ADC_ANALOG_FLAGS,		///< 62 ADC analog PGA flags
		};

	private:

		I2C_IO&		i2c_io_;

		uint8_t		cur_page_;

		bool read_(uint8_t cmd, uint8_t& data)
		{
			uint8_t tmp[2];
			tmp[0] = cmd;
			if(!i2c_io_.send(DEV_ADR, tmp, 1)) {
				return false;
			}
			tmp[0] = 0;
			tmp[1] = 0;
			if(!i2c_io_.recv(DEV_ADR, tmp, 2)) {
				return false;
			}
			data = tmp[1];
			return true;
		}


		bool write_(uint8_t cmd, uint8_t data)
		{
			uint8_t tmp[2];
			tmp[0] = cmd;
			tmp[1] = data;
			return i2c_io_.send(DEV_ADR, tmp, 2);
		}


		bool set_page_(uint8_t page)
		{
			bool ret = true;
			if(cur_page_ != 0) {
				ret = write_(PAGE_CTRL, page);
				if(ret) {
					cur_page_ = page;
				}
			}
			return ret;
		}


		bool set_(CMD_PAGE0 cmd, uint8_t data)
		{
			if(!set_page_(0x00)) {
				return false;
			}
			return write_(static_cast<uint8_t>(cmd), data);
		}


		bool get_(CMD_PAGE0 cmd, uint8_t& data)
		{
			if(!set_page_(0x00)) {
				return false;
			}
			return read_(static_cast<uint8_t>(cmd), data);
		}


		bool set_(CMD_PAGE1 cmd, uint8_t data)
		{
			if(!set_page_(0x01)) {
				return false;
			}
			return write_(static_cast<uint8_t>(cmd), data);
		}


	public:
		//-----------------------------------------------------------------//
		/*!
			@brief	コンストラクター
			@param[in]	i2c	iica_io クラスを参照で渡す
		 */
		//-----------------------------------------------------------------//
		TLV320ADC3001(I2C_IO& i2c) : i2c_io_(i2c), cur_page_(0xff) { }


		//-----------------------------------------------------------------//
		/*!
			@brief	開始
			@param[in]	inf		インターフェースタイプ
			@param[in]	type	タイプ
			@return エラーなら「false」を返す
		 */
		//-----------------------------------------------------------------//
		bool start(INF inf, TYPE type)
		{
			if(!set_page_(0)) {
				utils::format("set_page_: NG: %d\n") % static_cast<uint16_t>(i2c_io_.get_last_error()); 
				return false;
			}


			if(!set_(CMD_PAGE0::SW_RESET, 1)) {
				return false;
			}

			// In EVM, the ADC3001 receives: MCLK = 11.2896 MHz,
			// BCLK = 2.8224 MHz, WCLK = 44.1 kHz
			set_(CMD_PAGE0::CLOCK_GEN, 0x00);
			set_(CMD_PAGE0::PLL_P_R,   0x11);
			set_(CMD_PAGE0::PLL_J,     0x04);
			set_(CMD_PAGE0::PLL_D_MSB, 0x00);
			set_(CMD_PAGE0::PLL_D_LSB, 0x00);

			// (b) Power up PLL (if PLL is necessary) - Not Used in this Example
			set_(CMD_PAGE0::PLL_P_R,   0x11);

			//  NADC = 1, divider powered on
			set_(CMD_PAGE0::ADC_NADC, 0x81);

			// MADC = 2, divider powered on
			set_(CMD_PAGE0::ADC_MADC, 0x82);

			//  AOSR = 128 (default)
			set_(CMD_PAGE0::ADC_AOSR, 0x80);

			// mode is i2s, wordlength
			uint8_t aifc = 0;
			switch(inf) {
			case INF::I2S_16_SLAVE:  aifc = 0b00000000; break;
			case INF::I2S_16_MASTER: aifc = 0b00001100; break;
			case INF::I2S_20_SLAVE:  aifc = 0b00010000; break;
			case INF::I2S_20_MASTER: aifc = 0b00011100; break;
			case INF::I2S_24_SLAVE:  aifc = 0b00100000; break;
			case INF::I2S_24_MASTER: aifc = 0b00101100; break;
			case INF::I2S_32_SLAVE:  aifc = 0b00110000; break;
			case INF::I2S_32_MASTER: aifc = 0b00111100; break;
			default:
				break;
			}
			set_(CMD_PAGE0::ADC_AIFC, aifc);

			// PRB_P1 (0x3D, 01)
			set_(CMD_PAGE0::ADC_PROC, 0x01);

			// 3. Program Analog Blocks
			// (a) Set register Page to 1 (00 01)
			// (b) Program MICBIAS if appicable
			// Not used (default) (0x33(51) 0x00)
			set_(CMD_PAGE1::MICBIAS_CTRL, 0x00);

			// (c) Program MicPGA
			// Left Analog PGA Seeting = 0dB (0x3b(59) 0x00)
			set_(CMD_PAGE1::LEFT_ANALOG, 0x00);

			// Right Analog PGA Seeting = 0dB (0x3c(60) 0x00)
			set_(CMD_PAGE1::RIGHT_ANALOG, 0x00);

			// (d) Routing of inputs/common mode to ADC input
			// (e) Unmute analog PGAs and set analog gain
			// Left ADC Input selection for Left PGA = IN1L(P) as Single-Ended (0x34(52) 0xfc)
			set_(CMD_PAGE1::LEFT_INPSEL_1, 0xFC);

			// Right ADC Input selection for Right PGA = IN1R(M) as Single-Ended (0x37(55) 0xfc)
			set_(CMD_PAGE1::RIGHT_INPSEL_1, 0xFC);

			// 4. Program ADC
			// (a) Set register Page to 0
			// (b) Power up ADC channel
			// Power-up Left ADC and Right ADC (0x51 0xc2)
			set_(CMD_PAGE0::ADC_DIGITAL, 0xC2);

			// (c) Unmute digital volume control and set gain = 0 dB
			// UNMUTE (0x52, 0x00)
			set_(CMD_PAGE0::ADC_FINE_VOLUME, 0x00);

#if 0
			{
				uint8_t data;
				data = get_(CMD_PAGE0::CLOCK_GEN);
				utils::format("CLOCK_GEN: %02X\n") % static_cast<uint16_t>(data);
				data = get_(CMD_PAGE0::PLL_P_R);
				utils::format("PLL_P_R:   %02X\n") % static_cast<uint16_t>(data);
				data = get_(CMD_PAGE0::PLL_J);
				utils::format("PLL_J:     %02X\n") % static_cast<uint16_t>(data);
				data = get_(CMD_PAGE0::PLL_D_MSB);
				utils::format("PLL_D_MSB: %02X\n") % static_cast<uint16_t>(data);
				data = get_(CMD_PAGE0::PLL_D_LSB);
				utils::format("PLL_D_LSB: %02X\n") % static_cast<uint16_t>(data);
			}
#endif

#if 0
			if(type == TYPE::FS48) {
				utils::format("Sampling: 48.0KHz\n");

			} else if(type == TYPE::FS44_1) {
				utils::format("Sampling: 44.1KHz\n");
			}
#endif
			return true;
		}
	};
}
