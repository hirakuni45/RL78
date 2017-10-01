#pragma once
//=====================================================================//
/*!	@file
	@brief	MCP2515 CAN ドライバー @n
			※MCP2515 の電源は２．７Ｖ～５．５Ｖ @n
			※ドライバーの電源は通常５Ｖなので、電源が分離されていない場合は５Ｖ駆動
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include <cstdint>

namespace chip {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  MCP2515 テンプレートクラス
		@param[in]	SPI	SPI クラス
		@param[in]	SEL	選択クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class SPI, class SEL>
	class MCP2515 {

		enum class REG : uint8_t {
			RXF0SIDH  = 0x00,
			RXF0SIDL  = 0x01,
			RXF0EID8  = 0x02,
			RXF0EID0  = 0x03,
			RXF1SIDH  = 0x04,
			RXF1SIDL  = 0x05,
			RXF1EID8  = 0x06,
			RXF1EID0  = 0x07,
			RXF2SIDH  = 0x08,
			RXF2SIDL  = 0x09,
			RXF2EID8  = 0x0A,
			RXF2EID0  = 0x0B,
			BFPCTRL   = 0x0C,
			TXRTSCTRL = 0x0D,
			CANSTAT   = 0x0E,
			CANCTRL   = 0x0F,
			RXF3SIDH  = 0x10,
			RXF3SIDL  = 0x11,
			RXF3EID8  = 0x12,
			RXF3EID0  = 0x13,
			RXF4SIDH  = 0x14,
			RXF4SIDL  = 0x15,
			RXF4EID8  = 0x16,
			RXF4EID0  = 0x17,
			RXF5SIDH  = 0x18,
			RXF5SIDL  = 0x19,
			RXF5EID8  = 0x1A,
			RXF5EID0  = 0x1B,
			TEC       = 0x1C,
			REC       = 0x1D,
			RXM0SIDH  = 0x20,
			RXM0SIDL  = 0x21,
			RXM0EID8  = 0x22,
			RXM0EID0  = 0x23,
			RXM1SIDH  = 0x24,
			RXM1SIDL  = 0x25,
			RXM1EID8  = 0x26,
			RXM1EID0  = 0x27,
			CNF3      = 0x28,
			CNF2      = 0x29,
			CNF1      = 0x2A,
			CANINTE   = 0x2B,
			CANINTF   = 0x2C,
			EFLG      = 0x2D,
			TXB0CTRL  = 0x30,
			TXB1CTRL  = 0x40,
			TXB2CTRL  = 0x50,
			RXB0CTRL  = 0x60,
			RXB0SIDH  = 0x61,
			RXB1CTRL  = 0x70,
			RXB1SIDH  = 0x71,
		};

#if 0
/*
 *   Define SPI Instruction Set
 */
#define MCP_WRITE           0x02

#define MCP_READ            0x03

#define MCP_BITMOD          0x05

#define MCP_LOAD_TX0        0x40
#define MCP_LOAD_TX1        0x42
#define MCP_LOAD_TX2        0x44

#define MCP_RTS_TX0         0x81
#define MCP_RTS_TX1         0x82
#define MCP_RTS_TX2         0x84
#define MCP_RTS_ALL         0x87

#define MCP_READ_RX0        0x90
#define MCP_READ_RX1        0x94

#define MCP_READ_STATUS     0xA0

#define MCP_RX_STATUS       0xB0

#define MCP_RESET           0xC0
#endif

		SPI&		spi_;

		uint8_t read_(REG adr) noexcept
		{
//    SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
			SEL = 0; // MCP2515_SELECT();
//    spi_readwrite(READ);
//    spi_readwrite(address);
//    ret = spi_read();
			uint8_t ret = spi_.recv();
//    MCP2515_UNSELECT();
//    SPI.endTransaction();
			return ret;
		}



	public:
		//-----------------------------------------------------------------//
		/*!
			@brief	コンストラクタ
		 */
		//-----------------------------------------------------------------//
		MCP2515(SPI& spi) noexcept : spi_(spi) { }


		//-----------------------------------------------------------------//
		/*!
			@brief	開始
			@return 成功なら「true」
		 */
		//-----------------------------------------------------------------//
		bool start() noexcept
		{
			SEL::DIR = 1;  // output
			SEL::P = 1;    // device disable
			// SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
			// MCP2515_SELECT();
			// spi_readwrite(MCP_RESET);
			// MCP2515_UNSELECT();
			// SPI.endTransaction();
			// delayMicroseconds(10);
			return true;
		}
	};
}
