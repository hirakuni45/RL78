#pragma once
//=====================================================================//
/*!	@file
	@brief	MFRC522 クラス @n
			NXP Semiconductors @n
			Interface: SPI, Vcc: 3.3V
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include "common/delay.hpp"

#define DEBUG_MFRC522

namespace chip {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  MFRC522 テンプレートクラス
		@param[in]	SPI	spi クラス
		@param[in]	CS	チップ・セレクト
		@param[in]	RES	リセット信号
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class SPI, class CS, class RES>
	class MFRC522 {

#ifdef DEBUG_MFRC522
		typedef utils::format debug_format;
#else
		typedef utils::null_format debug_format;
#endif

	public:

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  ステータス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		enum class status : uint8_t {
			OK,				///< Success
			ERROR,			///< Error in communication
			COLLISION,		///< Collission detected
			TIMEOUT,		///< Timeout in communication.
			NO_ROOM,		///< A buffer is not big enough.
			INTERNAL_ERROR,	///< Internal error in the code. Should not happen ;-)
			INVALID,		///< Invalid argument.
			CRC_WRONG,		///< The CRC_A does not match
			MIFARE_NACK	= 0xff	///< A MIFARE PICC responded with NAK.
		};


		// MFRC522 RxGain[2:0] masks, defines the receiver's signal voltage gain factor (on the PCD).
		// Described in 9.3.3.6 / table 98 of the datasheet at
		// http://www.nxp.com/documents/data_sheet/MFRC522.pdf
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  アンテナ・ゲイン
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		enum class RxGain {
			v_18dB		= 0x00,	// 000b - 18 dB, minimum
			v_23dB		= 0x01,	// 001b - 23 dB
			v_18dB_2	= 0x02,	// 010b - 18 dB, it seems 010b is a duplicate for 000b
			v_23dB_2	= 0x03,	// 011b - 23 dB, it seems 011b is a duplicate for 001b
			v_33dB		= 0x04,	// 100b - 33 dB, average, and typical default
			v_38dB		= 0x05,	// 101b - 38 dB
			v_43dB		= 0x06,	// 110b - 43 dB
			v_48dB		= 0x07,	// 111b - 48 dB, maximum
			min			= 0x00,	// 000b - 18 dB, minimum, convenience for RxGain_18dB
			avg			= 0x04,	// 100b - 33 dB, average, convenience for RxGain_33dB
			max			= 0x07	// 111b - 48 dB, maximum, convenience for RxGain_48dB
		};


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  uid_t 構造体
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		struct uid_t {
			uint8_t	size;	///< Number of bytes in the UID. 4, 7 or 10.
			uint8_t	uid_byte[10];
			uint8_t	sak;	///< (Select acknowledge) byte returned from the PICC after successful selection.
		};


	private:
		SPI&	spi_;

		enum class reg_adr : uint8_t {
			// Page 0: Command and status
			//            0x00
			Command		= 0x01 << 1,	// starts and stops command execution
			ComIEn		= 0x02 << 1,	// enable and disable interrupt request control bits
			DivIEn		= 0x03 << 1,	// enable and disable interrupt request control bits
			ComIrq		= 0x04 << 1,	// interrupt request bits
			DivIrq		= 0x05 << 1,	// interrupt request bits

			Error		= 0x06 << 1,	// error bits showing the error status of the last command executed 
			Status1		= 0x07 << 1,	// communication status bits
			Status2		= 0x08 << 1,	// receiver and transmitter status bits
			FIFOData	= 0x09 << 1,	// input and output of 64 byte FIFO buffer
			FIFOLevel	= 0x0A << 1,	// number of bytes stored in the FIFO buffer
			WaterLevel	= 0x0B << 1,	// level for FIFO underflow and overflow warning
			Control		= 0x0C << 1,	// miscellaneous control registers
			BitFraming	= 0x0D << 1,	// adjustments for bit-oriented frames
			Coll		= 0x0E << 1,	// bit position of the first bit-collision detected on the RF interface
			//			  0x0F			// reserved for future use

			// Page 1: Command
			//            0x10
			Mode		= 0x11 << 1,	// defines general modes for transmitting and receiving 
			TxMode		= 0x12 << 1,	// defines transmission data rate and framing
			RxMode		= 0x13 << 1,	// defines reception data rate and framing
			TxControl	= 0x14 << 1,	// controls the logical behavior of the antenna driver pins TX1 and TX2
			TxASK		= 0x15 << 1,	// controls the setting of the transmission modulation
			TxSel		= 0x16 << 1,	// selects the internal sources for the antenna driver
			RxSel		= 0x17 << 1,	// selects internal receiver settings
			RxThreshold	= 0x18 << 1,	// selects thresholds for the bit decoder
			Demod		= 0x19 << 1,	// defines demodulator settings
			// 			  0x1A			// reserved for future use
			// 			  0x1B			// reserved for future use
			MfTx		= 0x1C << 1,	// controls some MIFARE communication transmit parameters
			MfRx		= 0x1D << 1,	// controls some MIFARE communication receive parameters
			// 			  0x1E			// reserved for future use
			SerialSpeed	= 0x1F << 1,	// selects the speed of the serial UART interface
		
			// Page 2: Configuration
			// 			  0x20			// reserved for future use
			CRCResultH	= 0x21 << 1,	// shows the MSB and LSB values of the CRC calculation
			CRCResultL	= 0x22 << 1,
			// 			  0x23			// reserved for future use
			ModWidth	= 0x24 << 1,	// controls the ModWidth setting?
			// 			  0x25			// reserved for future use
			RFCfg		= 0x26 << 1,	// configures the receiver gain
			GsN			= 0x27 << 1,	// selects the conductance of the antenna driver pins TX1 and TX2 for modulation
			CWGsP		= 0x28 << 1,	// defines the conductance of the p-driver output during periods of no modulation
			ModGsP		= 0x29 << 1,	// defines the conductance of the p-driver output during periods of modulation
			TMode		= 0x2A << 1,	// defines settings for the internal timer
			TPrescaler	= 0x2B << 1,	// the lower 8 bits of the TPrescaler value. The 4 high bits are in TModeReg.
			TReloadH	= 0x2C << 1,	// defines the 16-bit timer reload value
			TReloadL	= 0x2D << 1,
			TCounterValueH = 0x2E << 1,	// shows the 16-bit timer value
			TCounterValueL = 0x2F << 1,
		
			// Page 3: Test Registers
			// 			  0x30			// reserved for future use
			TestSel1	= 0x31 << 1,	// general test signal configuration
			TestSel2	= 0x32 << 1,	// general test signal configuration
			TestPinEn	= 0x33 << 1,	// enables pin output driver on pins D1 to D7
			TestPinValue= 0x34 << 1,	// defines the values for D1 to D7 when it is used as an I/O bus
			TestBus		= 0x35 << 1,	// shows the status of the internal test bus
			AutoTest	= 0x36 << 1,	// controls the digital self-test
			Version		= 0x37 << 1,	// shows the software version
			AnalogTest	= 0x38 << 1,	// controls the pins AUX1 and AUX2
			TestDAC1	= 0x39 << 1,	// defines the test value for TestDAC1
			TestDAC2	= 0x3A << 1,	// defines the test value for TestDAC2
			TestADC		= 0x3B << 1		// shows the value of ADC I and Q channels
			// 			  0x3C			// reserved for production tests
			// 			  0x3D			// reserved for production tests
			// 			  0x3E			// reserved for production tests
			// 			  0x3F			// reserved for production tests
		};


		// MFRC522 commands. Described in chapter 10 of the datasheet.
		enum class Command : uint8_t {
			Idle			= 0x00,		// no action, cancels current command execution
			Mem				= 0x01,		// stores 25 bytes into the internal buffer
			GenerateRandomID= 0x02,		// generates a 10-byte random ID number
			CalcCRC			= 0x03,		// activates the CRC coprocessor or performs a self-test
			Transmit		= 0x04,		// transmits data from the FIFO buffer
			NoCmdChange		= 0x07,		// no command change, can be used to modify the CommandReg register bits without affecting the command, for example, the PowerDown bit
			Receive			= 0x08,		// activates the receiver circuits
			Transceive 		= 0x0C,		// transmits data from FIFO buffer to antenna and automatically activates the receiver after transmission
			MFAuthent 		= 0x0E,		// performs the MIFARE standard authentication as a reader
			SoftReset		= 0x0F		// resets the MFRC522
		};


		// Commands sent to the PICC.
		enum class PICC_Command : uint8_t {
			// The commands used by the PCD to manage communication with several
			// PICCs (ISO 14443-3, Type A, section 6.4)
			REQA	= 0x26,		// REQuest command, Type A. Invites PICCs in state IDLE to go to READY and prepare for anticollision or selection. 7 bit frame.
			WUPA	= 0x52,		// Wake-UP command, Type A. Invites PICCs in state IDLE and HALT to go to READY(*) and prepare for anticollision or selection. 7 bit frame.
			CT		= 0x88,		// Cascade Tag. Not really a command, but used during anti collision.
			SEL_CL1	= 0x93,		// Anti collision/Select, Cascade Level 1
			SEL_CL2	= 0x95,		// Anti collision/Select, Cascade Level 2
			SEL_CL3	= 0x97,		// Anti collision/Select, Cascade Level 3
			HLTA	= 0x50,		// HaLT command, Type A. Instructs an ACTIVE PICC to go to state HALT.
			// The commands used for MIFARE Classic (from http://www.mouser.com/ds/2/302/MF1S503x-89574.pdf, Section 9)
			// Use PCD_MFAuthent to authenticate access to a sector, then use these commands to read/write/modify the blocks on the sector.
			// The read/write commands can also be used for MIFARE Ultralight.
			MF_AUTH_KEY_A	= 0x60,		// Perform authentication with Key A
			MF_AUTH_KEY_B	= 0x61,		// Perform authentication with Key B
			MF_READ			= 0x30,		// Reads one 16 byte block from the authenticated sector of the PICC. Also used for MIFARE Ultralight.
			MF_WRITE		= 0xA0,		// Writes one 16 byte block to the authenticated sector of the PICC. Called "COMPATIBILITY WRITE" for MIFARE Ultralight.
			MF_DECREMENT	= 0xC0,		// Decrements the contents of a block and stores the result in the internal data register.
			MF_INCREMENT	= 0xC1,		// Increments the contents of a block and stores the result in the internal data register.
			MF_RESTORE		= 0xC2,		// Reads the contents of a block into the internal data register.
			MF_TRANSFER     = 0xB0,		// Writes the contents of the internal data register to a block.
			// The commands used for MIFARE Ultralight (from http://www.nxp.com/documents/data_sheet/MF0ICU1.pdf, Section 8.6)
			// The PICC_CMD_MF_READ and PICC_CMD_MF_WRITE can also be used for MIFARE Ultralight.
			UL_WRITE		= 0xA2		// Writes one 4 byte page to the PICC.
		};


		void write_reg_(reg_adr reg, uint8_t value) noexcept
		{
			CS::P = 0;  // enable device
			// MSB == 0 is for writing. LSB is not used in address.
			spi_.xchg(static_cast<uint8_t>(reg) & 0x7E);
			spi_.xchg(value);
			CS::P = 1;  // disable device
		}


		inline void write_reg_(reg_adr reg, Command value) noexcept
		{
			write_reg_(reg, static_cast<uint8_t>(value));
		}


		void write_reg_(reg_adr reg, const void* data, uint8_t size) noexcept
		{
			CS::P = 0;  // enable device
			// MSB == 0 is for writing. LSB is not used in address.
			spi_.xchg(static_cast<uint8_t>(reg) & 0x7E);
			spi_.send(data, size);
			CS::P = 1;  // disable device
		}


		uint8_t read_reg_(reg_adr reg) noexcept
		{
			CS::P = 0;  // enable device
			// MSB == 0 is for writing. LSB is not used in address.
			spi_.xchg(0x80 | (static_cast<uint8_t>(reg) & 0x7E));
			auto data = spi_.xchg(0);
			CS::P = 1;  // disable device
			return data;
		}


		void read_reg_(reg_adr reg, void* data, uint8_t size, uint8_t rx_align = 0) noexcept
		{
			if(size == 0) {
				return;
			}

			// MSB == 1 is for reading. LSB is not used in address.
			uint8_t adr = 0x80 | (static_cast<uint8_t>(reg) & 0x7E);
			CS::P = 0;  // enable device
			spi_.xchg(adr);

			uint8_t mask = 0;
			if(rx_align > 0) {
				// Create bit mask for bit positions rx_align..7
				for(uint8_t i = rx_align; i <= 7; i++) {
					mask |= (1 << i);
				}
			}

			uint8_t idx = 0;
			uint8_t* p = static_cast<uint8_t*>(data);
			size--;
			while(idx < size) {
				// Only update bit positions rx_align..7 in values[0]
				if(idx == 0 && rx_align > 0) {
					uint8_t value = spi_.xchg(adr);
					// Apply mask to both current value of values[0] and the new data in value.
					p[0] = (p[idx] & ~mask) | (value & mask);
				} else {
					p[idx] = spi_.xchg(adr);
				}
				idx++;
			}
			p[idx] = spi_.xchg(0);  // Read the final byte. Send 0 to stop reading.

			CS::P = 1;  // disable device
		}


		void set_reg_bit_(reg_adr reg, uint8_t mask, bool set = true)
		{
			auto tmp = read_reg_(reg);
			if(set) {  // for '1'
				write_reg_(reg, tmp | mask);
			} else {   // for '0'
				write_reg_(reg, tmp & (~mask));
			}
		}


		status calculate_CRC_(const void* data, uint8_t size, uint16_t& result)
		{
			write_reg_(reg_adr::Command, Command::Idle);	// Stop any active command.
			write_reg_(reg_adr::DivIrq,  0x04);				// Clear the CRCIRq interrupt request bit
			set_reg_bit_(reg_adr::FIFOLevel, 0x80);			// FlushBuffer = 1, FIFO initialization
			write_reg_(reg_adr::FIFOData, data, size);		// Write data to the FIFO
			write_reg_(reg_adr::Command, Command::CalcCRC);	// Start the calculation

			// Wait for the CRC calculation to complete. Each iteration of the while-loop takes 17.73μs.
			// TODO check/modify for other architectures than Arduino Uno 16bit
			uint16_t loop = 5000;
			while(1) {
				// DivIrqReg[7..0] bits are:
				//     Set2 reserved reserved MfinActIRq reserved CRCIRq reserved reserved
				auto n = read_reg_(reg_adr::DivIrq);
				if(n & 0x04) {  // CRCIRq bit set - calculation done
					break;
				}
				--loop;
				// The emergency break.
				// We will eventually terminate on this one after 89ms.
				// Communication with the MFRC522 might be down.
				if(loop == 0) {
					return status::TIMEOUT;
				}
			}
			// Stop calculating CRC for new content in the FIFO.
			write_reg_(reg_adr::Command, Command::Idle);

			// Transfer the result from the registers to the result buffer
			uint16_t d = static_cast<uint16_t>(read_reg_(reg_adr::CRCResultH)) << 8;
			result = d | read_reg_(reg_adr::CRCResultL);

			return status::OK;
		}


	public:
		//-----------------------------------------------------------------//
		/*!
			@brief	コンストラクタ
		 */
		//-----------------------------------------------------------------//
		MFRC522(SPI& spi) noexcept : spi_(spi) { }


		//-----------------------------------------------------------------//
		/*!
			@brief	ソフト・リセット
		 */
		//-----------------------------------------------------------------//
		void reset() noexcept
		{
			write_reg_(reg_adr::Command, Command::SoftReset);	// Issue the SoftReset command.
			// The datasheet does not mention how long the SoftRest command takes to complete.
			// But the MFRC522 might have been in soft power-down mode (triggered by bit 4 of CommandReg) 
			// Section 8.8.2 in the datasheet says the oscillator start-up time is the start up time of
			// the crystal + 37,74μs. Let us be generous: 50ms.
			utils::delay::milli_second(50);
			// Wait for the PowerDown bit in CommandReg to be cleared
			while(read_reg_(reg_adr::Command) & (1 << 4)) {
				// PCD still restarting - unlikely after waiting 50ms, but better safe than sorry.
			}
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	アンテナを許可（無効）
			@param[in]	ena	無効にする場合「false」
		 */
		//-----------------------------------------------------------------//
		void enable_antenna(bool ena = true) noexcept
		{
			if(ena) {
				auto value = read_reg_(reg_adr::TxControl);
				if((value & 0x03) != 0x03) {
					write_reg_(reg_adr::TxControl, value | 0x03);
				}
			} else {
				set_reg_bit_(reg_adr::TxControl, 0x03, false);
			}
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	開始
		 */
		//-----------------------------------------------------------------//
		void start() noexcept
		{
			CS::DIR = 1;
			CS::P = 1;  // disable device
			RES::DIR = 1;
			RES::P = 0;  // low-power mode

			/// SPI.beginTransaction(SPISettings(SPI_CLOCK_DIV4, MSBFIRST, SPI_MODE0));
			if(RES::port_no == 0xff) {
				reset();
			} else {  // Let us be generous: 50ms.
				RES::P = 1;  // Device RESET are positive edge.
				utils::delay::milli_second(50);
			}

			// When communicating with a PICC we need a timeout if something goes wrong.
			// f_timer = 13.56 MHz / (2*TPreScaler+1) where TPreScaler = [TPrescaler_Hi:TPrescaler_Lo].
			// TPrescaler_Hi are the four low bits in TModeReg. TPrescaler_Lo is TPrescalerReg.

			// TAuto=1; timer starts automatically at the end of the transmission in all
			// communication modes at all speeds
			write_reg_(reg_adr::TMode, 0x80);
			// TPreScaler = TModeReg[3..0]:TPrescalerReg, ie 0x0A9 = 169 => f_timer=40kHz,
			// ie a timer period of 25μs.
			write_reg_(reg_adr::TPrescaler, 0xA9);
			// Reload timer with 0x3E8 = 1000, ie 25ms before timeout.
			write_reg_(reg_adr::TReloadH, 0x03);
			write_reg_(reg_adr::TReloadL, 0xE8);

			// Default 0x00. Force a 100 % ASK modulation independent of the ModGsPReg register setting
			write_reg_(reg_adr::TxASK, 0x40);
			// Default 0x3F. Set the preset value for the CRC coprocessor for
			// the CalcCRC command to 0x6363 (ISO 14443-3 part 6.2.4)
			write_reg_(reg_adr::Mode, 0x3D);
			// Enable the antenna driver pins TX1 and TX2 (they were disabled by the reset)
			enable_antenna();
///			set_antenna_gain(RxGain::min);
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	アンテナ・ゲイン取得
			@return アンテナ・ゲイン
		 */
		//-----------------------------------------------------------------//
		RxGain get_antenna_gain() noexcept
		{
			return static_cast<RxGain>((read_reg_(reg_adr::RFCfg) >> 4) & 0x07);
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	アンテナ・ゲイン取得
			@param[in]	gain	RxGain 型
		 */
		//-----------------------------------------------------------------//
		void set_antenna_gain(RxGain gain) noexcept
		{
			if(get_antenna_gain() != gain) {		// only bother if there is a change
				set_reg_bit_(reg_adr::RFCfg, (0x07 << 4), false);		// clear bits
				set_reg_bit_(reg_adr::RFCfg, static_cast<uint8_t>(gain) << 4);
			}
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	セルフ・テスト
			@return 正常なら「true」
		 */
		//-----------------------------------------------------------------//
		bool self_test()
		{
			reset();

			// 2. Clear the internal buffer by writing 25 bytes of 00h
			uint8_t tmp[25] = { 0x00 };
			set_reg_bit_(reg_adr::FIFOLevel, 0x80);	// flush the FIFO buffer
			write_reg_(reg_adr::FIFOData, tmp, sizeof(tmp));	// write 25 bytes of 00h to FIFO
			write_reg_(reg_adr::Command, Command::Mem);

			// 3. Enable self-test
			write_reg_(reg_adr::AutoTest, 0x09);

			// 4. Write 00h to FIFO buffer
			write_reg_(reg_adr::FIFOData, 0x00);

			// 5. Start self-test by issuing the CalcCRC command
			write_reg_(reg_adr::Command, Command::CalcCRC);

			// 6. Wait for self-test to complete
			for(uint8_t i = 0; i < 0xFF; i++) {
				// The datasheet does not specify exact completion condition except
				// that FIFO buffer should contain 64 bytes.
				// While selftest is initiated by CalcCRC command
				// it behaves differently from normal CRC computation,
				// so one can't reliably use DivIrqReg to check for completion.
				// It is reported that some devices does not trigger CRCIRq flag
				// during selftest.
				auto n = read_reg_(reg_adr::FIFOLevel);
				if(n >= 64) {
					break;
				}
			}
			// Stop calculating CRC for new content in the FIFO.
			write_reg_(reg_adr::Command, Command::Idle);

			// 7. Read out resulting 64 bytes from the FIFO buffer.
			uint8_t res[64];
			read_reg_(reg_adr::FIFOData, res, sizeof(res));
	
			// Auto self-test done
			// Reset AutoTestReg register to be 0 again. Required for normal operation.
			write_reg_(reg_adr::AutoTest, 0x00);
	
			// Determine firmware version (see section 9.3.4.8 in spec)
			uint8_t version = read_reg_(reg_adr::Version);

			// Pick the appropriate reference values
			const uint8_t* ref;
			switch (version) {
			case 0x88:	// Fudan Semiconductor FM17522 clone
				{
					static const uint8_t FM17522_firmware_reference[] = {
						0x00, 0xD6, 0x78, 0x8C, 0xE2, 0xAA, 0x0C, 0x18,
						0x2A, 0xB8, 0x7A, 0x7F, 0xD3, 0x6A, 0xCF, 0x0B,
						0xB1, 0x37, 0x63, 0x4B, 0x69, 0xAE, 0x91, 0xC7,
						0xC3, 0x97, 0xAE, 0x77, 0xF4, 0x37, 0xD7, 0x9B,
						0x7C, 0xF5, 0x3C, 0x11, 0x8F, 0x15, 0xC3, 0xD7,
						0xC1, 0x5B, 0x00, 0x2A, 0xD0, 0x75, 0xDE, 0x9E,
						0x51, 0x64, 0xAB, 0x3E, 0xE9, 0x15, 0xB5, 0xAB,
						0x56, 0x9A, 0x98, 0x82, 0x26, 0xEA, 0x2A, 0x62
					};
					ref = FM17522_firmware_reference;
				}
				break;
			case 0x90:	// Version 0.0
				{
					static const uint8_t MFRC522_firmware_referenceV0_0[] = {
						0x00, 0x87, 0x98, 0x0f, 0x49, 0xFF, 0x07, 0x19,
						0xBF, 0x22, 0x30, 0x49, 0x59, 0x63, 0xAD, 0xCA,
						0x7F, 0xE3, 0x4E, 0x03, 0x5C, 0x4E, 0x49, 0x50,
						0x47, 0x9A, 0x37, 0x61, 0xE7, 0xE2, 0xC6, 0x2E,
						0x75, 0x5A, 0xED, 0x04, 0x3D, 0x02, 0x4B, 0x78,
						0x32, 0xFF, 0x58, 0x3B, 0x7C, 0xE9, 0x00, 0x94,
						0xB4, 0x4A, 0x59, 0x5B, 0xFD, 0xC9, 0x29, 0xDF,
						0x35, 0x96, 0x98, 0x9E, 0x4F, 0x30, 0x32, 0x8D
					};
					ref = MFRC522_firmware_referenceV0_0;
				}
				break;
			case 0x91:	// Version 1.0
				{
					static const uint8_t MFRC522_firmware_referenceV1_0[] = {
						0x00, 0xC6, 0x37, 0xD5, 0x32, 0xB7, 0x57, 0x5C,
						0xC2, 0xD8, 0x7C, 0x4D, 0xD9, 0x70, 0xC7, 0x73,
						0x10, 0xE6, 0xD2, 0xAA, 0x5E, 0xA1, 0x3E, 0x5A,
						0x14, 0xAF, 0x30, 0x61, 0xC9, 0x70, 0xDB, 0x2E,
						0x64, 0x22, 0x72, 0xB5, 0xBD, 0x65, 0xF4, 0xEC,
						0x22, 0xBC, 0xD3, 0x72, 0x35, 0xCD, 0xAA, 0x41,
						0x1F, 0xA7, 0xF3, 0x53, 0x14, 0xDE, 0x7E, 0x02,
						0xD9, 0x0F, 0xB5, 0x5E, 0x25, 0x1D, 0x29, 0x79
					};
					ref = MFRC522_firmware_referenceV1_0;
				}
				break;

			case 0x92:	// Version 2.0
				{
					static const uint8_t MFRC522_firmware_referenceV2_0[] = {
						0x00, 0xEB, 0x66, 0xBA, 0x57, 0xBF, 0x23, 0x95,
						0xD0, 0xE3, 0x0D, 0x3D, 0x27, 0x89, 0x5C, 0xDE,
						0x9D, 0x3B, 0xA7, 0x00, 0x21, 0x5B, 0x89, 0x82,
						0x51, 0x3A, 0xEB, 0x02, 0x0C, 0xA5, 0x00, 0x49,
						0x7C, 0x84, 0x4D, 0xB3, 0xCC, 0xD2, 0x1B, 0x81,
						0x5D, 0x48, 0x76, 0xD5, 0x71, 0x61, 0x21, 0xA9,
						0x86, 0x96, 0x83, 0x38, 0xCF, 0x9D, 0x5B, 0x6D,
						0xDC, 0x15, 0xBA, 0x3E, 0x7D, 0x95, 0x3B, 0x2F
					};
					ref = MFRC522_firmware_referenceV2_0;
				}
				break;
			default:	// Unknown version
				debug_format("Unknown version: %02X\n") % static_cast<uint16_t>(version);
				return false; // abort test
			}

			// Verify that the results match up to our expectations
			uint8_t match = 0;
			for(uint8_t i = 0; i < 64; i++) {
				if(res[i] == ref[i]) {
					++match;
				} else {
					debug_format("(%2d) Res:0x%02X x Ref:0x%02X\n")
						% static_cast<uint16_t>(i)
						% static_cast<uint16_t>(res[i])
						% static_cast<uint16_t>(ref[i]);
				}
			}
			return match == 64;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	communicate_with
			@param[in]	cmd		コマンド
			@param[in]	waitIRq	
			@return ステータス
		 */
		//-----------------------------------------------------------------//
		status communicate_with(Command cmd, uint8_t waitIRq, const void* send_ptr, uint8_t send_len,
			void* back_ptr, uint8_t& back_len,
			uint8_t* valid_bits = nullptr, uint8_t rx_align = 0, bool check_crc = false)
		{
			// Prepare values for BitFramingReg
			uint8_t txLastBits = valid_bits != nullptr ? *valid_bits : 0;
			// RxAlign = BitFramingReg[6..4]. TxLastBits = BitFramingReg[2..0]
			uint8_t bitFraming = (rx_align << 4) + txLastBits;
	
			write_reg_(reg_adr::Command, Command::Idle);		// Stop any active command.
			write_reg_(reg_adr::ComIrq, 0x7F);					// Clear all seven interrupt request bits
			set_reg_bit_(reg_adr::FIFOLevel, 0x80);				// FlushBuffer = 1, FIFO initialization
			write_reg_(reg_adr::FIFOData, send_ptr, send_len);	// Write sendData to the FIFO
			write_reg_(reg_adr::BitFraming, bitFraming);		// Bit adjustments
			write_reg_(reg_adr::Command, cmd);					// Execute the command
			if(cmd == Command::Transceive) {
				set_reg_bit_(reg_adr::BitFraming, 0x80);		// StartSend=1, transmission of data starts
			}

			// Wait for the command to complete.
			// In PCD_Init() we set the TAuto flag in TModeReg.
			// This means the timer automatically starts when the PCD stops transmitting.
			// TODO check/modify for other architectures than Arduino Uno 16bit
			uint16_t i = 2000;
//uint8_t nnn = 0;
			while(1) {
				// ComIrqReg[7..0] bits are: Set1 TxIRq RxIRq IdleIRq HiAlertIRq LoAlertIRq ErrIRq TimerIRq
				auto n = read_reg_(reg_adr::ComIrq);
//if(n != nnn) {
//	nnn = n;
//	debug_format("State: %02X\n") % static_cast<uint16_t>(n);
//}
				if(n & waitIRq) {	// One of the interrupts that signal success has been set.
					debug_format("Card detect !\n");
					break;
				}
				if(n & 0x01) {		// Timer interrupt - nothing received in 25ms
///					debug_format("timeout step: %d\n") % i;
					return status::TIMEOUT;
				}
				--i;
				// The emergency break. If all other conditions fail we will eventually
				// terminate on this one after 35.7ms. Communication with the MFRC522 might be down.
				if(i == 0) {
/// utils::format("timeout step 1\n");
					return status::TIMEOUT;
				}
			}

			// Stop now if any errors except collisions were detected.
			// ErrorReg[7..0] bits are: WrErr TempErr reserved BufferOvfl CollErr CRCErr ParityErr ProtocolErr
			uint8_t errorRegValue = read_reg_(reg_adr::Error);
			if(errorRegValue & 0x13) {	 // BufferOvfl ParityErr ProtocolErr
				return status::ERROR;
			}

			uint8_t validBits = 0;
			// If the caller wants data back, get it from the MFRC522.
			if(back_ptr != nullptr) {
				auto n = read_reg_(reg_adr::FIFOLevel);		// Number of bytes in the FIFO
				if (n > back_len) {
					return status::NO_ROOM;
				}
				back_len = n;											// Number of bytes returned
				read_reg_(reg_adr::FIFOData, back_ptr, n, rx_align);	// Get received data from FIFO
				// RxLastBits[2:0] indicates the number of valid bits in the last received byte.
				// If this value is 000b, the whole byte is valid.
				validBits = read_reg_(reg_adr::Control) & 0x07;
				if(valid_bits != nullptr) {
					*valid_bits = validBits;
				}
			}

			// Tell about collisions
			if(errorRegValue & 0x08) {
				return status::COLLISION;
			}
	
			// Perform CRC_A validation if requested.
			if(back_ptr != nullptr && back_ptr != nullptr && check_crc) {
				// In this case a MIFARE Classic NAK is not OK.
				if(back_len == 1 && validBits == 4) {
					return status::MIFARE_NACK;
				}
				// We need at least the CRC_A value and all 8 bits of the last byte must be received.
				if(back_len < 2 || validBits != 0) {
					return status::CRC_WRONG;
				}
				// Verify CRC_A - do our own calculation and store the control in controlBuffer.
				uint16_t ans;
				auto st = calculate_CRC_(back_ptr, back_len - 2, ans);
				if(st != status::OK) {
					return st;
				}
				const uint8_t* p = static_cast<const uint8_t*>(back_ptr);
				uint16_t crc = (static_cast<uint16_t>(p[back_len - 1]) << 8) | p[back_len - 2];
				if(crc != ans) {
					return status::CRC_WRONG;
				}
			}
			return status::OK;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	
			@return ステータス
		 */
		//-----------------------------------------------------------------//
		status transceive_data(const void* send_ptr, uint8_t send_len, uint8_t* back_ptr, uint8_t& back_len,
			uint8_t* valid_bits = nullptr, uint8_t rx_align = 0, bool check_crc = false)
		{
			uint8_t waitIRq = 0x30;		// RxIRq and IdleIRq
			return communicate_with(Command::Transceive, waitIRq, send_ptr, send_len, back_ptr, back_len,
				valid_bits, rx_align, check_crc);
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	REQ/WUP
			@param[in]	cmd	コマンド
			@param[in]	atqa	バッファ
			@param[in/out]	size	サイズ
			@return ステータス
		 */
		//-----------------------------------------------------------------//
		status req_or_wup(PICC_Command cmd, uint8_t* atqa, uint8_t& size)
		{
			if(atqa == nullptr || size < 2) {	// The ATQA response is 2 bytes long.
				return status::NO_ROOM;
			}

			// ValuesAfterColl=1 => Bits received after collision are cleared.
			set_reg_bit_(reg_adr::Coll, 0x80, false);
			// For REQA and WUPA we need the short frame format - transmit
			// only 7 bits of the last (and only) byte. TxLastBits = BitFramingReg[2..0]
			uint8_t validBits = 7;
			uint8_t tmp;
			tmp = static_cast<uint8_t>(cmd);
			auto st = transceive_data(&tmp, 1, atqa, size, &validBits);
			if(st != status::OK) {
				return st;
			}

			if(size != 2 || validBits != 0) {		// ATQA must be exactly 16 bits.
				return status::ERROR;
			}

			return status::OK;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	リクエスト
			@param[in]	atqa	バッファ
			@param[in/out]	size	サイズ
			@return ステータス
		 */
		//-----------------------------------------------------------------//
		status request(uint8_t* atqa, uint8_t& size)
		{
			return req_or_wup(PICC_Command::REQA, atqa, size);
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	wake-up
			@return ステータス
		 */
		//-----------------------------------------------------------------//
		status wakeup(uint8_t* bufferATQA, uint8_t* bufferSize)
		{
			return req_or_wup(PICC_Command::WUPA, bufferATQA, bufferSize);
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	セレクト
			@return ステータス
		 */
		//-----------------------------------------------------------------//
		status select(uid_t& uid, uint8_t validBits = 0)
		{
			// Description of buffer structure:
			//		Byte 0: SEL 		Indicates the Cascade Level: PICC_CMD_SEL_CL1, PICC_CMD_SEL_CL2 or PICC_CMD_SEL_CL3
			//		Byte 1: NVB			Number of Valid Bits (in complete command, not just the UID): High nibble: complete bytes, Low nibble: Extra bits. 
			//		Byte 2: UID-data or CT		See explanation below. CT means Cascade Tag.
			//		Byte 3: UID-data
			//		Byte 4: UID-data
			//		Byte 5: UID-data
			//		Byte 6: BCC			Block Check Character - XOR of bytes 2-5
			//		Byte 7: CRC_A
			//		Byte 8: CRC_A
			// The BCC and CRC_A are only transmitted if we know all the UID bits of the current Cascade Level.
			//
			// Description of bytes 2-5: (Section 6.5.4 of the ISO/IEC 14443-3 draft: UID contents and cascade levels)
			//		UID size	Cascade level	Byte2	Byte3	Byte4	Byte5
			//		========	=============	=====	=====	=====	=====
			//		 4 bytes		1			uid0	uid1	uid2	uid3
			//		 7 bytes		1			CT		uid0	uid1	uid2
			//						2			uid3	uid4	uid5	uid6
			//		10 bytes		1			CT		uid0	uid1	uid2
			//						2			CT		uid3	uid4	uid5
			//						3			uid6	uid7	uid8	uid9
	
			// Sanity checks
			if(validBits > 80) {
				return status::INVALID;
			}

			// Prepare MFRC522
			// ValuesAfterColl=1 => Bits received after collision are cleared.
			set_reg_bit_(reg_adr::Coll, 0x80, false);

			// Repeat Cascade Level loop until we have a complete UID.
			bool uidComplete = false;
			uint8_t cascadeLevel = 1;
			uint8_t buffer[9];
			uint8_t uidIndex;
			bool useCascadeTag;
			while(!uidComplete) {
				// Set the Cascade Level in the SEL byte, find out if we need to use the Cascade Tag in byte 2.
				switch(cascadeLevel) {
					case 1:
						buffer[0] = static_cast<uint8_t>(PICC_Command::SEL_CL1);
						uidIndex = 0;
						useCascadeTag = validBits && uid->size > 4;
						break;
			
					case 2:
						buffer[0] = static_cast<uint8_t>(PICC_Command::SEL_CL2);
						uidIndex = 3;
						useCascadeTag = validBits && uid->size > 7;
						break;
			
					case 3:
						buffer[0] = static_cast<uint8_t>(PICC_Command::SEL_CL3);
						uidIndex = 6;
						useCascadeTag = false;						// Never used in CL3.
						break;
			
					default:
						return status::INTERNAL_ERROR;
						break;
				}
		
				// How many UID bits are known in this Cascade Level?
				int8_t currentLevelKnownBits = validBits - (8 * uidIndex);
				if(currentLevelKnownBits < 0) {
					currentLevelKnownBits = 0;
				}
				// Copy the known bits from uid->uidByte[] to buffer[]
				uint8_t index = 2; // destination index in buffer[]
				if(useCascadeTag) {
					buffer[index++] = static_cast<uint8_t>(PICC_Command::CT);
				}
				// The number of bytes needed to represent the known bits for this level.
				uint8_t bytesToCopy = currentLevelKnownBits / 8 + (currentLevelKnownBits % 8 ? 1 : 0);
				if(bytesToCopy) {
					// Max 4 bytes in each Cascade Level. Only 3 left if we use the Cascade Tag
					uint8_t maxBytes = useCascadeTag ? 3 : 4;
					if(bytesToCopy > maxBytes) {
						bytesToCopy = maxBytes;
					}
					for(uint8_t count = 0; count < bytesToCopy; count++) {
						buffer[index++] = uid->uid_byte[uidIndex + count];
					}
				}
				// Now that the data has been copied we need to include the 8 bits in CT
				// in currentLevelKnownBits
				if(useCascadeTag) {
					currentLevelKnownBits += 8;
				}

				// Repeat anti collision loop until we can transmit all UID bits + BCC and
				// receive a SAK - max 32 iterations.
				bool selectDone = false;
				uint8_t txLastBits;
				uint8_t bufferUsed;
				uint8_t count;
				uint8_t* responseBuffer;
				uint8_t responseLength;
				while(!selectDone) {
					// Find out how many bits and bytes to send and receive.
					// All UID bits in this Cascade Level are known. This is a SELECT.
					if(currentLevelKnownBits >= 32) {
					//Serial.print(F("SELECT: currentLevelKnownBits=")); Serial.println(currentLevelKnownBits, DEC);
						buffer[1] = 0x70; // NVB - Number of Valid Bits: Seven whole bytes
						// Calculate BCC - Block Check Character
						buffer[6] = buffer[2] ^ buffer[3] ^ buffer[4] ^ buffer[5];
						// Calculate CRC_A
						uint16_t crc;
						auto result = calculate_CRC_(buffer, 7, crc);
						if(result != status::OK) {
							return result;
						}
						txLastBits		= 0; // 0 => All 8 bits are valid.
						bufferUsed		= 9;
						// Store response in the last 3 bytes of buffer (BCC and CRC_A - not needed after tx)
						responseBuffer = &buffer[6];
						responseLength = 3;
					} else {
						//Serial.print(F("ANTICOLLISION: currentLevelKnownBits=")); Serial.println(currentLevelKnownBits, DEC);
						uint8_t txLastBits		= currentLevelKnownBits % 8;
						count			= currentLevelKnownBits / 8;
						index			= 2 + count;			// Number of whole bytes: SEL + NVB + UIDs
						buffer[1]		= (index << 4) + txLastBits;	// NVB - Number of Valid Bits
						bufferUsed		= index + (txLastBits ? 1 : 0);
						// Store response in the unused part of buffer
						responseBuffer	= &buffer[index];
						responseLength	= sizeof(buffer) - index;
					}

					// Set bit adjustments
					// Having a separate variable is overkill. But it makes the next line easier to read.
					uint8_t rxAlign = txLastBits;
					// RxAlign = BitFramingReg[6..4]. TxLastBits = BitFramingReg[2..0]
					write_reg_(reg_adr::BitFraming, (rxAlign << 4) + txLastBits);
			
					// Transmit the buffer and receive the response.
					auto result = transceive_data(buffer, bufferUsed, responseBuffer, responseLength, &txLastBits, rxAlign);

					if(result == status::COLLISION) { // More than one PICC in the field => collision.
						// CollReg[7..0] bits are: ValuesAfterColl reserved CollPosNotValid CollPos[4:0]
						uint8_t valueOfCollReg = read_reg_(reg_adr::Coll);
						if(valueOfCollReg & 0x20) { // CollPosNotValid
							// Without a valid collision position we cannot continue
							return status::COLLISION;
						}
						uint8_t collisionPos = valueOfCollReg & 0x1F; // Values 0-31, 0 means bit 32.
						if(collisionPos == 0) {
							collisionPos = 32;
						}
						if(collisionPos <= currentLevelKnownBits) { // No progress - should not happen 
							return status::INTERNAL_ERROR;
						}
						// Choose the PICC with the bit set.
						currentLevelKnownBits = collisionPos;
						count			= (currentLevelKnownBits - 1) % 8; // The bit to modify
						index			= 1 + (currentLevelKnownBits / 8) + (count ? 1 : 0); // First byte is index 0.
						buffer[index]	|= (1 << count);
					} else if (result != status::OK) {
						return result;
					} else { // STATUS_OK
						if(currentLevelKnownBits >= 32) { // This was a SELECT.
							selectDone = true; // No more anticollision 
							// We continue below outside the while.
						} else {
							// We now have all 32 bits of the UID in this Cascade Level
							currentLevelKnownBits = 32;
							// Run loop again to do the SELECT.
						}
					}
				}
				// We do not check the CBB - it was constructed by us above.
		
				// Copy the found UID bytes from buffer[] to uid->uidByte[]
				index = (buffer[2] == PICC_Command::CT) ? 3 : 2; // source index in buffer[]
				bytesToCopy	= (buffer[2] == PICC_Command::CT) ? 3 : 4;
				for(count = 0; count < bytesToCopy; count++) {
					uid->uid_byte[uidIndex + count] = buffer[index++];
				}
		
				// Check response SAK (Select Acknowledge)
				if(responseLength != 3 || txLastBits != 0) { // SAK must be exactly 24 bits (1 byte + CRC_A).
					return status::ERROR;
				}
				// Verify CRC_A - do our own calculation and store the control in buffer[2..3]
				// - those bytes are not needed anymore.
				uint16_t crc;
				auto result = calculate_CRC_(responseBuffer, 1, crc);
				if(result != status::OK) {
					return result;
				}
				uint16_t res = (static_cast<uint16_t>(responseBuffer[1]) << 8) | responseBuffer[2];
				if(crc != res) {
					return status::CRC_WRONG;
				}
				if(responseBuffer[0] & 0x04) { // Cascade bit set - UID not complete yes
					cascadeLevel++;
				} else {
					uidComplete = true;
					uid->sak = responseBuffer[0];
				}
			}

			// Set correct uid->size
			uid->size = 3 * cascadeLevel + 1;

			return status::OK;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	halt
			@return ステータス
		 */
		//-----------------------------------------------------------------//
		status halt()
		{
			uint8_t buffer[4];
			// Build command buffer
			buffer[0] = static_cast<uint8_t>(PICC_Command::HLTA);
			buffer[1] = 0;
			// Calculate CRC_A
			uint16_t crc;
			auto st = calculate_CRC_(buffer, 2, crc);
			if(st != status::OK) {
				return st;
			}
	
			// Send the command.
			// The standard says:
			//   If the PICC responds with any modulation during a period of
			//   1 ms after the end of the frame containing the
			//	 HLTA command, this response shall be interpreted as 'not acknowledge'.
			// We interpret that this way: Only STATUS_TIMEOUT is a success.
			buffer[2] = crc & 0xff;
			buffer[3] = crc >> 8;
			st = transceive_data(buffer, sizeof(buffer), nullptr, 0);
			if(st == status::TIMEOUT) {
				return status::OK;
			}
			if(st == status::OK) { // That is ironically NOT ok in this case ;-)
				return status::ERROR;
			}
			return st;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	カード検出
			@return カードを検出した場合「true」
		 */
		//-----------------------------------------------------------------//
		bool detect_card()
		{
			// Reset baud rates
			write_reg_(reg_adr::TxMode, 0x00);
			write_reg_(reg_adr::RxMode, 0x00);
			// Reset ModWidthReg
			write_reg_(reg_adr::ModWidth, 0x26);

			uint8_t tmp[2];
			uint8_t size = sizeof(tmp);
			auto result = request(tmp, size);
			return (result == status::OK || result == status::COLLISION);
		}






		//-----------------------------------------------------------------//
		/*!
			@brief	バージョン情報
			@param[out]	out		情報文字列
			@param[in]	size	配列サイズ
			@return エラーが無い場合「true」
		 */
		//-----------------------------------------------------------------//
		bool list_version(char* out, uint16_t size)
		{
			// Get the MFRC522 firmware version
			auto v = read_reg_(reg_adr::Version);
			utils::sformat("Firmware: 0x%02X: ", out, size) % static_cast<uint16_t>(v);
			// Lookup which version
			switch(v) {
			case 0x88: utils::sformat("(clone)", out, size, true); break;
			case 0x90: utils::sformat("Ver 0.0", out, size, true); break;
			case 0x91: utils::sformat("Ver 1.0", out, size, true); break;
			case 0x92: utils::sformat("Ver 2.0", out, size, true); break;
			default:   utils::sformat("(unknown)", out, size, true);
			}
			// When 0x00 or 0xFF is returned, communication probably failed
			if(v == 0x00 || v == 0xFF) {
				utils::sformat("WARNING: Communication failure\n", out, size, true);
				return false;
			} else {
				utils::sformat("\n", out, size, true);
				return true;
			}
		}
	};
}
