#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/G13 グループ・ペリフェラル
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2017, 2021 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include "G13/system.hpp"

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  ペリフェラル種別
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	enum class peripheral : uint8_t {
		ITM,		///< １２ビットインターバルタイマー

		ADC,		///< A/D コンバーター

		IICA0,		///< IICA0 (I2C-0) インターフェース
		IICA1,		///< IICA1 (I2C-0) インターフェース

		SAU00,		///< シリアル・アレイ・ユニット００
		SAU01,		///< シリアル・アレイ・ユニット０１
		SAU02,		///< シリアル・アレイ・ユニット０２
		SAU03,		///< シリアル・アレイ・ユニット０３
		SAU10,		///< シリアル・アレイ・ユニット１０
		SAU11,		///< シリアル・アレイ・ユニット１１
		SAU12,		///< シリアル・アレイ・ユニット１２
		SAU13,		///< シリアル・アレイ・ユニット１３

		TAU00,		///< タイマー・アレイ・ユニット００
		TAU01,		///< タイマー・アレイ・ユニット０１
		TAU02,		///< タイマー・アレイ・ユニット０２
		TAU03,		///< タイマー・アレイ・ユニット０３
		TAU04,		///< タイマー・アレイ・ユニット０４
		TAU05,		///< タイマー・アレイ・ユニット０５
		TAU06,		///< タイマー・アレイ・ユニット０６
		TAU07,		///< タイマー・アレイ・ユニット０７

		TAU10,		///< タイマー・アレイ・ユニット１０
		TAU11,		///< タイマー・アレイ・ユニット１１
		TAU12,		///< タイマー・アレイ・ユニット１２
		TAU13,		///< タイマー・アレイ・ユニット１３
		TAU14,		///< タイマー・アレイ・ユニット１４
		TAU15,		///< タイマー・アレイ・ユニット１５
		TAU16,		///< タイマー・アレイ・ユニット１６
		TAU17,		///< タイマー・アレイ・ユニット１７

		DMA0,		///< ダイレクト・メモリー・アクセス・ユニット０
		DMA1,		///< ダイレクト・メモリー・アクセス・ユニット１
		DMA2,		///< ダイレクト・メモリー・アクセス・ユニット２
		DMA3,		///< ダイレクト・メモリー・アクセス・ユニット３
	};


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  機能マネージメント
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	struct manage {

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  UART シリアル・クロック型
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		enum class uart_sck : uint8_t {
			NONE,	///< クロック無し
			INPUT,	///< クロック入力
			OUTPUT,	///< クロック出力
		};


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  CSI ポート型
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		enum class csi_port : uint8_t {
			INPUT,		///< 入力(MISO) のみ有効
			OUTPUT,		///< 出力(MOSI) のみ有効
			INOUT,		///< 入出力有効
		};


		//-------------------------------------------------------------//
		/*!
			@brief  ペリフェラル許可
			@param[in]	per	ペリフェラル型
			@param[in]	ena	不許可の場合「false」
			@return 有効なペリフェラル型では無い場合
		*/
		//-------------------------------------------------------------//
		static bool enable(peripheral per, bool ena = true)
		{
			static uint8_t sau_refc = 0;
			static uint8_t tau_refc = 0;

			switch(per) {
			case peripheral::ITM:
				system::PER0.RTCEN = ena;
				break;

			case peripheral::ADC:
				system::PER0.ADCEN = ena;
				break;

			case peripheral::IICA0:
				system::PER0.IICA0EN = ena;
				break;
			case peripheral::IICA1:
				system::PER0.IICA1EN = ena;
				break;

			case peripheral::SAU00:
			case peripheral::SAU01:
			case peripheral::SAU02:
			case peripheral::SAU03:
				{
					uint8_t refc = 1 << (static_cast<uint8_t>(per) - static_cast<uint8_t>(peripheral::SAU00));
					if(ena) sau_refc |= refc;
					else sau_refc &= ~refc;
					if(sau_refc & 0b1111) {
						system::PER0.SAU0EN = 1;
					} else {
						system::PER0.SAU0EN = 0;
					}
				}
				break;
			case peripheral::SAU10:
			case peripheral::SAU11:
			case peripheral::SAU12:
			case peripheral::SAU13:
				{
					uint8_t refc = 1 << (static_cast<uint8_t>(per) - static_cast<uint8_t>(peripheral::SAU00));
					if(ena) sau_refc |= refc;
					else sau_refc &= ~refc;
					if(sau_refc & 0b11110000) {
						system::PER0.SAU1EN = 1;
					} else {
						system::PER0.SAU1EN = 0;
					}
				}
				break;

			case peripheral::TAU00:
			case peripheral::TAU01:
			case peripheral::TAU02:
			case peripheral::TAU03:
			case peripheral::TAU04:
			case peripheral::TAU05:
			case peripheral::TAU06:
			case peripheral::TAU07:
				{
					uint8_t refc = 1 << (static_cast<uint8_t>(per) - static_cast<uint8_t>(peripheral::TAU00));
					if(ena) tau_refc |= refc;
					else tau_refc &= ~refc;
					if(tau_refc) {
						system::PER0.TAU0EN = 1;
					} else {
						system::PER0.TAU0EN = 0;
					}
				}

			default:
				return false;
				break;
			}
			return true;
		}


		//-------------------------------------------------------------//
		/*!
			@brief  IICA ポートの設定
			@param[in]	per	ペリフェラル型
			@return IICAx 型では無い場合「false」
		*/
		//-------------------------------------------------------------//
		static bool set_iica_port(peripheral per)
		{
			switch(per) {

			case peripheral::IICA0:
				PM6 &= 0b1111'1100;
				P6  &= 0b1111'1100;
				break;

			case peripheral::IICA1:
				PM6 &= 0b1111'0011;
				P6  &= 0b1111'0011;
				break;

			default:
				return false;		
			}
			return true;
		}


		//-------------------------------------------------------------//
		/*!
			@brief  UART ポートの設定
			@param[in]	per	ペリフェラル型
			@param[in]	sec	セカンド候補ポートの場合「true」
			@param[in]	sck	UART クロック入出力型
			@return SAUxx 型では無い場合「false」
		*/
		//-------------------------------------------------------------//
		static bool set_uart_port(peripheral per, bool sec = false, uart_sck sck = uart_sck::NONE)
		{
			switch(per) {

			case peripheral::SAU00:  // UART0-TX TxD0 (P12)
				PM1.B2 = 0;	// output
				PU1.B2 = 0; // pullup offline
				P1.B2  = 1;	// ポートレジスター TxD 切り替え
				break;
			case peripheral::SAU01:  // UART0-RX RxD0 (P11)
				PM1.B1 = 1;	// input
				PU1.B1 = 0; // pullup offline
				P1.B1  = 1;	// ポートレジスター RxD 切り替え
				break;

			case peripheral::SAU02:  // UART1-TX TxD1 (P02)
				if(sec) {
					PM0.B0 = 0;	// output
					PU0.B0 = 0; // pullup offline
					P0.B0  = 1;	// ポートレジスター TxD 切り替え
				} else {
					PM0.B2 = 0;	// output
					PU0.B2 = 0; // pullup offline
					PMC0.B2 = 0;  // ポートモードコントロール
					P0.B2  = 1;	// ポートレジスター TxD 切り替え
				}
				break;
			case peripheral::SAU03:  // UART1-RX RxD1 (P03)
				if(sec) {
					PM0.B1 = 1;	// input
					PU0.B1 = 0; // pullup offline
					P0.B1  = 1;	// ポートレジスター RxD 切り替え
				} else {
					PM0.B3 = 1;	// input
					PU0.B3 = 0; // pullup offline
					PMC0.B3 = 0;  // ポートモードコントロール
					P0.B3  = 1;	// ポートレジスター RxD 切り替え
				}
				break;

			case peripheral::SAU10:  // UART2-TX TxD2 (P13)
				PM1.B3 = 0;	// output
				PU1.B3 = 0; // pullup offline
				P1.B3  = 1;	// ポートレジスター TxD 切り替え
				break;
			case peripheral::SAU11:  // UART2-RX RxD2 (P14)
				PM1.B4 = 1;	// input
				PU1.B4 = 0; // pullup offline
				P1.B4  = 1;	// ポートレジスター RxD 切り替え
				break;

			case peripheral::SAU12:  // UART3-TX TxD3 (P144)
				PM14.B4 = 0;	// output
				PU14.B4 = 0; // pullup offline
				P14.B4  = 1;	// ポートレジスター TxD 切り替え
				break;
			case peripheral::SAU13:  // UART3-RX RxD3 (P143)
				PM14.B3 = 1;	// input
				PU14.B3 = 0; // pullup offline
				P14.B3  = 1;	// ポートレジスター RxD 切り替え
				break;

			default:
				return false;		
			}
			return true;
		}


		//-------------------------------------------------------------//
		/*!
			@brief  CSI(SPI) ポートの設定
			@param[in]	per		ペリフェラル型
			@param[in]	port	入力／出力の設定型（クロックは常に有効）
			@return SAUxx 型では無い場合「false」
		*/
		//-------------------------------------------------------------//
		static bool set_csi_port(peripheral per, csi_port port)
		{
			bool inp = false;
			bool out = false;
			if(port == csi_port::INPUT  || port == csi_port::INOUT) inp = true;
			if(port == csi_port::OUTPUT || port == csi_port::INOUT) out = true;

			switch(per) {

			case peripheral::SAU00:
				if(inp) {
					PU1.B1  = 0;
					PM1.B1  = 1;  // P1-1 input  (SI00)
					PIM1.B1 = 1;  // P1-1 TTL input
					P1.B1   = 1;  // ポートレジスター (SI00)  切り替え
				}
				if(out) {
					PU1.B2 = 0;
					PM1.B2 = 0;  // P1-2 output (SO00)
					P1.B2  = 1;  // ポートレジスター (SO00)  切り替え
				}
				PU1.B0 = 0;
				PM1.B0 = 0;  // P1-0 output (SCK00)
				P1.B0  = 1;  // ポートレジスター (SCK00) 切り替え
				break;

			case peripheral::SAU01:
				if(inp) {
					PU7.B4 = 0;
					PM7.B4 = 1;  // P7-4 input  (SI01)
					P7.B4  = 1;   // ポートレジスター (SI01)  切り替え
				}
				if(out) {
					PU7.B3 = 0;
					PM7.B3 = 0;  // P7-3 output (SO01)
					P7.B3  = 1;   // ポートレジスター (SO01)  切り替え
				}
				PU7.B5 = 0;
				PM7.B5 = 0;  // P7-5 output (SCK01)
				P7.B5  = 1;   // ポートレジスター (SCK01) 切り替え
				break;

			case peripheral::SAU02:
				if(inp) {
					PU0.B3  = 0;
					PM0.B3  = 1;  // P0-3 input  (SI10)
					PMC0.B3 = 0;  // ポートモードコントロール
					PIM0.B3 = 1;  // ポート入力モード
					P0.B3   = 1;  // ポートレジスター SI10  切り替え
				}
				if(out) {
					PU0.B2  = 0;
					PM0.B2  = 0;  // P0-2 output (SO10)
					PMC0.B2 = 0;  // ポートモードコントロール
					P0.B2   = 1;  // ポートレジスター SO10  切り替え
				}
				PU0.B4 = 0;
				PM0.B4 = 0;  // P0-4 output (SCK10)
				P0.B4  = 1;  // ポートレジスター SCK10 切り替え
				break;

			case peripheral::SAU03:
				if(inp) {
					PU5.B1 = 0;
					PM5.B1 = 0;  // P5-1 output (SO11)
					P5.B1  = 1;  // ポートレジスター SO11  切り替え
				}
				if(out) {
					PU5.B0 = 0;
					PM3.B0 = 0;  // P3-0 output (SCK11)
					P5.B0  = 1;  // ポートレジスター SI11  切り替え
				}
				PU3.B0 = 0;
				PM5.B0 = 1;  // P5-0 input  (SI11)
				P3.B0  = 1;  // ポートレジスター SCK11 切り替え
				break;

			case peripheral::SAU10:
				if(inp) {
					PU1.B4 = 0;
					PM1.B4 = 1;	 // P1-4 input  (SI20)
					P1.B4  = 1;  // ポートレジスター SI20  切り替え
				}
				if(out) {
					PU1.B3 = 0;
					PM1.B3 = 0;	 // P1-3 output (SO20)
					P1.B3  = 1;	 // ポートレジスター SO20  切り替え
				}
				PU1.B5 = 0;
				PM1.B5 = 0;  // P1-5 output (SCK20)
				P1.B5  = 1;  // ポートレジスター SCK20 切り替え
				break;

			case peripheral::SAU11:
				if(inp) {
					PU7.B1 = 0;
					PM7.B1 = 1;  // P7-1 input  (SI21)
					P7.B1  = 1;  // ポートレジスター SI21  切り替え
				}
				if(out) {
					PU7.B2 = 0;
					PM7.B2 = 0;  // P7-2 output (SO21)
					P7.B2  = 1;  // ポートレジスター SO21  切り替え
				}
				PU7.B0 = 0;
				PM7.B0 = 0;  // P7-0 output (SCK21)
				P7.B0  = 1;  // ポートレジスター SCK21 切り替え
				break;

			case peripheral::SAU12:  // CSI30: P33:SCK30, P34:SI30, P35:SO30
				if(inp) {
					PU3.B4 = 0;
					PM3.B4 = 1;  // P3-3 input  (SI30)
					P3.B4  = 1;  // ポートレジスター SI30  切り替え
				}
				if(out) {
					PU3.B5 = 0;
					PM3.B5 = 0;  // P3-5 output (SO30)
					P3.B5  = 1;  // ポートレジスター SO30  切り替え
				}
				PU3.B3 = 0;
				PM3.B3 = 0;  // P3-3 output (SCK30)
				P3.B3  = 1;  // ポートレジスター SCK30 切り替え
				break;

			default:
				return false;		
			}
			return true;
		}


		//-------------------------------------------------------------//
		/*!
			@brief  TAU ポートの設定
			@param[in]	per	ペリフェラル型
			@param[in]	dir	出力の場合「true」
			@return TAUxx 型では無い場合「false」
		*/
		//-------------------------------------------------------------//
		static bool set_tau_port(peripheral per, bool dir)
		{
			switch(per) {

			case peripheral::TAU00:
				PMC0.B1 = 0;
				PM0.B1 = !dir;
				P0.B1 = 0;
				break;

			case peripheral::TAU01:
				PM1.B6 = !dir;
				P1.B6 = 0;
				break;

			case peripheral::TAU02:
				PM1.B7 = !dir;
				P1.B7 = 0;
				break;

			case peripheral::TAU03:
				PM3.B1 = !dir;
				P3.B1 = 0;
				break;

			case peripheral::TAU04:
				PM4.B2 = !dir;
				P4.B2 = 0;
				break;

			case peripheral::TAU05:
				PM0.B5 = !dir;
				P0.B5 = 0;
				break;

			case peripheral::TAU06:
				PM0.B6 = !dir;
				P0.B6 = 0;
				break;

			case peripheral::TAU07:
				PM4.B1 = !dir;
				P4.B1 = 0;
				break;

			default:
				return false;		
			}
			return true;
		}
	};
}
