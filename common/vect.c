//=====================================================================//
/*!	@file
	@brief	R78 グループ・ベクター関係
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include "common/vect.h"

extern void start(void);

//-----------------------------------------------------------------//
/*!
	@brief  無効タスクエントリー（割り込み）
*/
//-----------------------------------------------------------------//
void NULL_intr(void)
{
}


#pragma GCC optimize ("O0")
//-----------------------------------------------------------------//
/*!
	@brief  リセットベクター設定
*/
//-----------------------------------------------------------------//
const void* vec_[] __attribute__ ((section (".vec"))) = {
	start,
};


void UART0_TX_intr(void) __attribute__((weak));
//-----------------------------------------------------------------//
/*!
	@brief  UART1-TX 割り込み
*/
//-----------------------------------------------------------------//
void UART0_TX_intr(void) { }


void UART0_RX_intr(void) __attribute__((weak));
//-----------------------------------------------------------------//
/*!
	@brief  UART1-RX 割り込み
*/
//-----------------------------------------------------------------//
void UART0_RX_intr(void) { }


void UART0_ER_intr(void) __attribute__((weak));
//-----------------------------------------------------------------//
/*!
	@brief  UART1-ER 割り込み
*/
//-----------------------------------------------------------------//
void UART0_ER_intr(void) { }


void UART1_TX_intr(void) __attribute__((weak));
//-----------------------------------------------------------------//
/*!
	@brief  UART1-TX 割り込み
*/
//-----------------------------------------------------------------//
void UART1_TX_intr(void) { }


void UART1_RX_intr(void) __attribute__((weak));
//-----------------------------------------------------------------//
/*!
	@brief  UART1-RX 割り込み
*/
//-----------------------------------------------------------------//
void UART1_RX_intr(void) { }


void UART1_ER_intr(void) __attribute__((weak));
//-----------------------------------------------------------------//
/*!
	@brief  UART1-ER 割り込み
*/
//-----------------------------------------------------------------//
void UART1_ER_intr(void) { }


void ADC_intr(void) __attribute__((weak));
//-----------------------------------------------------------------//
/*!
	@brief  UART1-ER 割り込み
*/
//-----------------------------------------------------------------//
void ADC_intr(void) { }


void ITM_intr(void) __attribute__((weak));
//-----------------------------------------------------------------//
/*!
	@brief  UART1-ER 割り込み
*/
//-----------------------------------------------------------------//
void ITM_intr(void) { }


//-----------------------------------------------------------------//
/*!
	@brief  割り込みベクターテーブルの定義
*/
//-----------------------------------------------------------------//
const void* intr_vec_tables[] __attribute__ ((section (".ivec"))) = {
	/*  0 */  (void*)NULL_intr,
	/*  1 */  (void*)NULL_intr,
	/*  2 */  (void*)NULL_intr,
	/*  3 */  (void*)NULL_intr,
	/*  4 */  (void*)NULL_intr,
	/*  5 */  (void*)NULL_intr,
	/*  6 */  (void*)NULL_intr,
	/*  7 */  (void*)NULL_intr,
	/*  8 */  (void*)NULL_intr,
	/*  9 */  (void*)NULL_intr,
	/* 10 */  (void*)NULL_intr,
	/* 11 */  (void*)NULL_intr,
	/* 12 */  (void*)NULL_intr,
	/* 13 */  (void*)UART0_TX_intr,  // UART0-TX
	/* 14 */  (void*)UART0_RX_intr,  // UART0-RX
	/* 15 */  (void*)UART0_ER_intr,  // UART0-ER
	/* 16 */  (void*)UART1_TX_intr,  // UART1-TX
	/* 17 */  (void*)UART1_RX_intr,  // UART1-RX
	/* 18 */  (void*)UART1_ER_intr,  // UART1-ER
	/* 19 */  (void*)NULL_intr,
	/* 20 */  (void*)NULL_intr,
	/* 21 */  (void*)NULL_intr,
	/* 22 */  (void*)NULL_intr,
	/* 23 */  (void*)NULL_intr,
	/* 24 */  (void*)ADC_intr,
	/* 25 */  (void*)NULL_intr,
	/* 26 */  (void*)ITM_intr,
	/* 27 */  (void*)NULL_intr,
	/* 28 */  (void*)NULL_intr,
	/* 29 */  (void*)NULL_intr,
	/* 30 */  (void*)NULL_intr,
	/* 31 */  (void*)NULL_intr,
	/* 32 */  (void*)NULL_intr,
	/* 33 */  (void*)NULL_intr,
	/* 34 */  (void*)NULL_intr,
	/* 35 */  (void*)NULL_intr,
	/* 36 */  (void*)NULL_intr,
	/* 37 */  (void*)NULL_intr,
	/* 38 */  (void*)NULL_intr,
	/* 39 */  (void*)NULL_intr,
	/* 40 */  (void*)NULL_intr,
	/* 41 */  (void*)NULL_intr,
	/* 42 */  (void*)NULL_intr,
	/* 43 */  (void*)NULL_intr,
	/* 44 */  (void*)NULL_intr,
	/* 45 */  (void*)NULL_intr,
	/* 46 */  (void*)NULL_intr,
	/* 47 */  (void*)NULL_intr,
	/* 48 */  (void*)NULL_intr,
	/* 49 */  (void*)NULL_intr,
	/* 50 */  (void*)NULL_intr,
	/* 51 */  (void*)NULL_intr,
	/* 52 */  (void*)NULL_intr,
	/* 53 */  (void*)NULL_intr,
	/* 54 */  (void*)NULL_intr,
	/* 55 */  (void*)NULL_intr,
	/* 56 */  (void*)NULL_intr,
	/* 57 */  (void*)NULL_intr,
	/* 58 */  (void*)NULL_intr,
	/* 59 */  (void*)NULL_intr,
	/* 60 */  (void*)NULL_intr,
	/* 61 */  (void*)NULL_intr
};

