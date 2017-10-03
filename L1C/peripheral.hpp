#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/L1C グループ・ペリフェラル
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include <cstdint>
#include "L1C/system.hpp"
#include "L1C/port.hpp"

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  ペリフェラル種別
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	enum class peripheral : uint8_t {
		ITM,		///< １２ビットインターバルタイマー

		ADC,		///< A/D コンバーター

		I2C,		///< I2C インターフェース

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
	};


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  機能マネージメント
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	struct manage {

		//-------------------------------------------------------------//
		/*!
			@brief  ペリフェラル有効（無効）
			@param[in]	per	ペリフェラル型
			@param[in]	ena	無効の場合「false」
		*/
		//-------------------------------------------------------------//
		static void enable(peripheral per, bool ena = true)
		{
			static uint8_t sau_refc = 0;
			static uint8_t tau_refc = 0;

			switch(per) {
			case peripheral::ITM:
				PER1.TMKAEN = ena;
				break;

			case peripheral::ADC:
				PER0.ADCEN = ena;
				break;

			case peripheral::SAU00:
			case peripheral::SAU01:
			case peripheral::SAU02:
			case peripheral::SAU03:
				{
					uint8_t refc = 1 << (static_cast<uint8_t>(per) - static_cast<uint8_t>(peripheral::SAU00));
					if(ena) sau_refc |= refc;
					else sau_refc &= ~refc;
					if(sau_refc & 0b00001111) {
						PER0.SAU0EN = 1;
					} else {
						PER0.SAU0EN = 0;
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
						PER0.SAU1EN = 1;
					} else {
						PER0.SAU1EN = 0;
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
						PER0.TAU0EN = 1;
					} else {
						PER0.TAU0EN = 0;
					}
				}
				break;

			default:
				break;
			}
		}


		//-------------------------------------------------------------//
		/*!
			@brief  UART ポートの設定
			@param[in]	per	ペリフェラル型
			@return SAUxx 型では無い場合「false」
		*/
		//-------------------------------------------------------------//
		static bool set_uart_port(peripheral per)
		{
			switch(per) {

			case peripheral::SAU00:  // UART0-TX TxD0 (P26)
				PM2.B6 = 0;	// output
				PU2.B6 = 0; // pullup offline
				P2.B6  = 1;	// ポートレジスター TxD 切り替え
				break;
			case peripheral::SAU01:  // UART0-RX RxD0 (P25)
				PM2.B5 = 1;	// input
				PU2.B5 = 0; // pullup offline
				P2.B5  = 1;	// ポートレジスター RxD 切り替え
				break;

			case peripheral::SAU02:  // UART1-TX TxD1 (P02)
				PM0.B2 = 0;	// output
				PU0.B2 = 0; // pullup offline
				P0.B2  = 1;	// ポートレジスター TxD 切り替え
				break;

			case peripheral::SAU03:  // UART1-RX RxD1 (P01)
				PM0.B1 = 1;	// input
				PU0.B1 = 0; // pullup offline
				P0.B1  = 1;	// ポートレジスター RxD 切り替え
				break;

			case peripheral::SAU10:  // UART2-TX TxD2 (P12)
				PM1.B2 = 0;	// output
				PU1.B2 = 0; // pullup offline
				P1.B2  = 1;	// ポートレジスター TxD 切り替え
				break;

			case peripheral::SAU11:  // UART2-RX RxD2 (P11)
				PM1.B1 = 1;	// input
				PU1.B1 = 0; // pullup offline
				P1.B1  = 1;	// ポートレジスター RxD 切り替え
				break;

			case peripheral::SAU12:  // UART3-TX TxD3 (P35)
				PM3.B5 = 0;	// output
				PU3.B5 = 0; // pullup offline
				P3.B5  = 1;	// ポートレジスター TxD 切り替え
				break;

			case peripheral::SAU13:  // UART3-RX RxD3 (P34)
				PM3.B4 = 1;	// input
				PU3.B4 = 0; // pullup offline
				P3.B4  = 1;	// ポートレジスター RxD 切り替え
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
			@return SAUxx 型では無い場合「false」
		*/
		//-------------------------------------------------------------//
		static bool set_tau_port(peripheral per, bool dir)
		{
			switch(per) {

			case peripheral::TAU00:
				PM0.B3 = !dir;
				P0.B3  = 0;
				break;

			case peripheral::TAU01:
				PM3.B2 = !dir;
				P3.B2  = 0;
				break;

			case peripheral::TAU02:
				PM0.B5 = !dir;
				P5.B5  = 0;
				break;

			case peripheral::TAU03:
				PM3.B0 = !dir;
				P3.B0  = 0;
				break;

			case peripheral::TAU04:
				PM2.B2 = !dir;
				P2.B2  = 0;
				break;

			case peripheral::TAU05:
				PM2.B7 = !dir;
				P2.B7  = 0;
				break;

			case peripheral::TAU06:
				PM0.B7 = !dir;
				P0.B7  = 0;
				break;

			case peripheral::TAU07:
				PM2.B3 = !dir;
				P2.B3 = 0;
				break;

			default:
				return false;		
			}
			return true;
		}
	};
}
