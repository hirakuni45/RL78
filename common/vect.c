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

// #pragma GCC optimize ("O0")
//-----------------------------------------------------------------//
/*!
	@brief  リセットベクター設定
*/
//-----------------------------------------------------------------//
const void* vec_[] __attribute__ ((section (".vec"))) = {
	start,
};


#define ATTR __attribute__((weak)) __attribute__ ((section (".lowtext")))

void NULL_intr(void) ATTR;
//-----------------------------------------------------------------//
/*!
	@brief  無効タスクエントリー（割り込み）
*/
//-----------------------------------------------------------------//
void NULL_intr(void)
{
}


void UART0_TX_intr(void) ATTR;
//-----------------------------------------------------------------//
/*!
	@brief  UART0-TX 割り込み
*/
//-----------------------------------------------------------------//
void UART0_TX_intr(void) { }


void UART0_RX_intr(void) ATTR;
//-----------------------------------------------------------------//
/*!
	@brief  UART0-RX 割り込み
*/
//-----------------------------------------------------------------//
void UART0_RX_intr(void) { }


void UART0_ER_intr(void) ATTR;
//-----------------------------------------------------------------//
/*!
	@brief  UART0-ER 割り込み
*/
//-----------------------------------------------------------------//
void UART0_ER_intr(void) { }


void UART1_TX_intr(void) ATTR;
//-----------------------------------------------------------------//
/*!
	@brief  UART1-TX 割り込み
*/
//-----------------------------------------------------------------//
void UART1_TX_intr(void) { }


void UART1_RX_intr(void) ATTR;
//-----------------------------------------------------------------//
/*!
	@brief  UART1-RX 割り込み
*/
//-----------------------------------------------------------------//
void UART1_RX_intr(void) { }


void UART1_ER_intr(void) ATTR;
//-----------------------------------------------------------------//
/*!
	@brief  UART1-ER 割り込み
*/
//-----------------------------------------------------------------//
void UART1_ER_intr(void) { }


void TM00_intr(void) ATTR;
//-----------------------------------------------------------------//
/*!
	@brief  TM00 割り込み
*/
//-----------------------------------------------------------------//
void TM00_intr(void) { }


void TM01_intr(void) ATTR;
//-----------------------------------------------------------------//
/*!
	@brief  TM01 割り込み
*/
//-----------------------------------------------------------------//
void TM01_intr(void) { }


void TM02_intr(void) ATTR;
//-----------------------------------------------------------------//
/*!
	@brief  TM02 割り込み
*/
//-----------------------------------------------------------------//
void TM02_intr(void) { }


void TM03_intr(void) ATTR;
//-----------------------------------------------------------------//
/*!
	@brief  TM03 割り込み
*/
//-----------------------------------------------------------------//
void TM03_intr(void) { }


void ADC_intr(void) ATTR;
//-----------------------------------------------------------------//
/*!
	@brief  ADC 割り込み
*/
//-----------------------------------------------------------------//
void ADC_intr(void) { }


void ITM_intr(void) ATTR;
//-----------------------------------------------------------------//
/*!
	@brief  ITM 割り込み
*/
//-----------------------------------------------------------------//
void ITM_intr(void) { }


void TM04_intr(void) ATTR;
//-----------------------------------------------------------------//
/*!
	@brief  TM04 割り込み
*/
//-----------------------------------------------------------------//
void TM04_intr(void) { }


void TM05_intr(void) ATTR;
//-----------------------------------------------------------------//
/*!
	@brief  TM05 割り込み
*/
//-----------------------------------------------------------------//
void TM05_intr(void) { }


void TM06_intr(void) ATTR;
//-----------------------------------------------------------------//
/*!
	@brief  TM06 割り込み
*/
//-----------------------------------------------------------------//
void TM06_intr(void) { }


void TM07_intr(void) ATTR;
//-----------------------------------------------------------------//
/*!
	@brief  TM07 割り込み
*/
//-----------------------------------------------------------------//
void TM07_intr(void) { }


void TM10_intr(void) ATTR;
//-----------------------------------------------------------------//
/*!
	@brief  TM10 割り込み
*/
//-----------------------------------------------------------------//
void TM10_intr(void) { }


void TM11_intr(void) ATTR;
//-----------------------------------------------------------------//
/*!
	@brief  TM11 割り込み
*/
//-----------------------------------------------------------------//
void TM11_intr(void) { }


void TM12_intr(void) ATTR;
//-----------------------------------------------------------------//
/*!
	@brief  TM12 割り込み
*/
//-----------------------------------------------------------------//
void TM12_intr(void) { }


void TM13_intr(void) ATTR;
//-----------------------------------------------------------------//
/*!
	@brief  TM13 割り込み
*/
//-----------------------------------------------------------------//
void TM13_intr(void) { }


void TM14_intr(void) ATTR;
//-----------------------------------------------------------------//
/*!
	@brief  TM14 割り込み
*/
//-----------------------------------------------------------------//
void TM14_intr(void) { }


void TM15_intr(void) ATTR;
//-----------------------------------------------------------------//
/*!
	@brief  TM15 割り込み
*/
//-----------------------------------------------------------------//
void TM15_intr(void) { }


void TM16_intr(void) ATTR;
//-----------------------------------------------------------------//
/*!
	@brief  TM16 割り込み
*/
//-----------------------------------------------------------------//
void TM16_intr(void) { }


void TM17_intr(void) ATTR;
//-----------------------------------------------------------------//
/*!
	@brief  TM17 割り込み
*/
//-----------------------------------------------------------------//
void TM17_intr(void) { }


//-----------------------------------------------------------------//
/*!
	@brief  割り込みベクターテーブルの定義
*/
//-----------------------------------------------------------------//
#if defined(SIG_G13)
const void* intr_vec_tables[] __attribute__ ((section (".ivec"))) = {
	/*  0 INTWDTI                  */  (void*)NULL_intr,  /* ウォッチドッグ・タイマのインターバル　*/
	/*  1 INTLVL                   */  (void*)NULL_intr,  /* 電圧検出 */
	/*  2 INTP0                    */  (void*)NULL_intr,
	/*  3 INTP1                    */  (void*)NULL_intr,
	/*  4 INTP2                    */  (void*)NULL_intr,
	/*  5 INTP3                    */  (void*)NULL_intr,
	/*  6 INTP4                    */  (void*)NULL_intr,
	/*  7 INTP5                    */  (void*)NULL_intr,
	/*  8 INTST2/INTCSI20/INTIIC20 */  (void*)NULL_intr,
	/*  9 INTSR2/INTCSI21/INTIIC21 */  (void*)NULL_intr,
	/* 10 INTSRE2/INTTM11H         */  (void*)NULL_intr,
	/* 11 INTDMA0                  */  (void*)NULL_intr,
	/* 12 INTDMA1                  */  (void*)NULL_intr,
	/* 13 UART0-TX                 */  (void*)UART0_TX_intr,
	/* 14 UART0-RX                 */  (void*)UART0_RX_intr,
	/* 15 UART0-ER                 */  (void*)UART0_ER_intr,
	/* 16 UART1-TX                 */  (void*)UART1_TX_intr,
	/* 17 UART1-RX                 */  (void*)UART1_RX_intr, 
	/* 18 UART1-ER                 */  (void*)UART1_ER_intr,
	/* 19 INTIICA0                 */  (void*)NULL_intr,
	/* 20 INTTM00                  */  (void*)TM00_intr,
	/* 21 INTTM01                  */  (void*)TM01_intr,
	/* 22 INTTM02                  */  (void*)TM02_intr,
	/* 23 INTTM03                  */  (void*)TM03_intr,
	/* 24 INTAD                    */  (void*)ADC_intr,
	/* 25 INTRTC                   */  (void*)NULL_intr,
	/* 26 INTIT                    */  (void*)ITM_intr,
	/* 27 INTKR                    */  (void*)NULL_intr,
	/* 28 INTST3/INTCSI30/INTIIC30 */  (void*)NULL_intr,
	/* 29 INTSR3/INTCSI31/INTIIC31 */  (void*)NULL_intr,
	/* 30 INTTM13                  */  (void*)NULL_intr,
	/* 31 INTTM04                  */  (void*)TM04_intr,
	/* 32 INTTM05                  */  (void*)TM05_intr,
	/* 33 INTTM06                  */  (void*)TM06_intr,
	/* 34 INTTM07                  */  (void*)TM07_intr,
	/* 35 INTP6                    */  (void*)NULL_intr,
	/* 36 INTP7                    */  (void*)NULL_intr,
	/* 37 INTP8                    */  (void*)NULL_intr,
	/* 38 INTP9                    */  (void*)NULL_intr,
	/* 39 INTP10                   */  (void*)NULL_intr,
	/* 40 INTP11                   */  (void*)NULL_intr,
	/* 41 INTTM10                  */  (void*)TM10_intr,
	/* 42 INTTM11                  */  (void*)TM11_intr,
	/* 43 INTTM12                  */  (void*)TM12_intr,
	/* 44 INTSRE3/INTTM13H         */  (void*)TM13_intr,
	/* 45 INTMD                    */  (void*)NULL_intr,
	/* 46 INTIICA1                 */  (void*)NULL_intr,
	/* 47 INTFL                    */  (void*)NULL_intr,
	/* 48 INTDMA2                  */  (void*)NULL_intr,
	/* 49 INTDMA3                  */  (void*)NULL_intr,
	/* 50 INTTM14                  */  (void*)TM14_intr,
	/* 51 INTTM15                  */  (void*)TM15_intr,
	/* 52 INTTM16                  */  (void*)TM16_intr,
	/* 53 INTTM17                  */  (void*)TM17_intr,
	/* 54                          */  (void*)NULL_intr,
	/* 55                          */  (void*)NULL_intr,
	/* 56                          */  (void*)NULL_intr,
	/* 57                          */  (void*)NULL_intr,
	/* 58                          */  (void*)NULL_intr,
	/* 59                          */  (void*)NULL_intr,
	/* 60                          */  (void*)NULL_intr,
	/* 61                          */  (void*)NULL_intr
};
#endif

#if defined(SIG_L1C)
const void* intr_vec_tables[] __attribute__ ((section (".ivec"))) = {
	/* 0x04   0 INTWDTI                  */  (void*)NULL_intr,  /* ウォッチドッグ・タイマのインターバル　*/
	/* 0x06   1 INTLVL                   */  (void*)NULL_intr,  /* 電圧検出 */
	/* 0x08   2 INTP0                    */  (void*)NULL_intr,
	/* 0x0A   3 INTP1                    */  (void*)NULL_intr,
	/* 0x0C   4 INTP2                    */  (void*)NULL_intr,
	/* 0x0E   5 INTP3                    */  (void*)NULL_intr,
	/* 0x10   6 INTP4                    */  (void*)NULL_intr,
	/* 0x12   7 INTP5                    */  (void*)NULL_intr,
	/* 0x14   8 INTST2/INTCSI20/INTIIC20 */  (void*)NULL_intr,
	/* 0x16   9 INTSR2/INTCSI21/INTIIC21 */  (void*)NULL_intr,
	/* 0x18  10 INTSRE2/INTTM11H         */  (void*)NULL_intr,
	/* 0x1A								 */  (void*)0xffff,
	/* 0x1C								 */  (void*)0xffff,
	/* 0x1E  11 UART0-TX                 */  (void*)UART0_TX_intr,
	/* 0x20  12 INTTM0                   */  (void*)TM00_intr,
	/* 0x22  13 UART0-RX                 */  (void*)UART0_RX_intr,
	/* 0x24  14 UART0-ER                 */  (void*)UART0_ER_intr,
	/* 0x26  15 UART1-TX                 */  (void*)UART1_TX_intr,
	/* 0x28  16 UART1-RX                 */  (void*)UART1_RX_intr, 
	/* 0x2A  17 UART1-ER                 */  (void*)UART1_ER_intr,
	/* 0x2C  18 INTIICA0                 */  (void*)NULL_intr,
	/* 0x2E  19 INTRTIT                  */  (void*)NULL_intr,
	/* 0x30								 */  (void*)0xffff,
	/* 0x32  20 INTTM01                  */  (void*)TM01_intr,
	/* 0x34  21 INTTM02                  */  (void*)TM02_intr,
	/* 0x36  22 INTTM03                  */  (void*)TM03_intr,
	/* 0x38  23 INTAD                    */  (void*)ADC_intr,
	/* 0x3A  24 INTRTC                   */  (void*)NULL_intr,
	/* 0x3C  25 INTIT                    */  (void*)ITM_intr,
	/* 0x3E  26 INTKR                    */  (void*)NULL_intr,
	/* 0x40  27 INTST3/INTCSI30/INTIIC30 */  (void*)NULL_intr,
	/* 0x42  28 INTSR3/INTCSI31/INTIIC31 */  (void*)NULL_intr,
	/* 29 INTTM04                  */  (void*)TM04_intr,
	/* 30 INTTM05                  */  (void*)TM05_intr,
	/* 31 INTTP6                   */  (void*)NULL_intr,
	/* 32 INTTP7                   */  (void*)NULL_intr,
	/* 33 INTCMP0                  */  (void*)NULL_intr,
	/* 34 INTCMP1                  */  (void*)NULL_intr,
	/* 35 INTTM06                  */  (void*)TM06_intr,
	/* 36 INTTM07                  */  (void*)TM07_intr,
	/* 37 INTPUSB                  */  (void*)NULL_intr,
	/* 38 INTRSUM                  */  (void*)NULL_intr,
	/* 39 INTSRE3                  */  (void*)NULL_intr,
	/* 40 INTKB2_0                 */  (void*)NULL_intr,
	/* 41 INTKB2_1                 */  (void*)NULL_intr,
	/* 42 INTFL                    */  (void*)NULL_intr,
	/* 43 INTKB2_2                 */  (void*)NULL_intr,
	/* 44 DTC0FIFO                 */  (void*)NULL_intr,
	/* 45 DTC1FIFO                 */  (void*)NULL_intr,
	/* 46                          */  (void*)NULL_intr,
	/* 47                          */  (void*)NULL_intr,
	/* 48                          */  (void*)NULL_intr,
	/* 49                          */  (void*)NULL_intr,
	/* 50                          */  (void*)NULL_intr,
	/* 51                          */  (void*)NULL_intr,
	/* 52                          */  (void*)NULL_intr,
	/* 53                          */  (void*)NULL_intr,
	/* 54                          */  (void*)NULL_intr,
	/* 55                          */  (void*)NULL_intr,
	/* 56                          */  (void*)NULL_intr,
	/* 57                          */  (void*)NULL_intr,
	/* 58                          */  (void*)NULL_intr,
};
#endif
