/***************************************************************************
 *
 * PURPOSE
 *   Data Flash Utilty module file.
 *
 * TARGET DEVICE
 *   RL78/G13
 *
 * AUTHOR
 *   Renesas Solutions Corp.
 *
 * $Date:: 2013-02-28 14:27:07 +0900#$
 *
 ***************************************************************************
 * Copyright (C) 2012 Renesas Electronics. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * See file LICENSE.txt for further informations on licensing terms.
 ***************************************************************************/
/**
 * @file  data_flash_util.c
 * @brief RL78マイコン内蔵のデータ フラッシュROMを使うためのライブラリです。
 * 
 */

/***************************************************************************/
/*    Include Header Files                                                 */
/***************************************************************************/
/// #include "Arduino.h"
#include "data_flash_util.h"
#include "pfdl.h"

inline void NOP(void)
{
	asm("nop");
}

inline void _NOP(void)
{
	asm("nop");
}


/***************************************************************************/
/*    Macro Definitions                                                    */
/***************************************************************************/
/* Additional Error code. */
#define PFDL_ERR_STATUS				0x80
#define PFDL_ERR_PARAM_BLOCK_NUMBER	0x81
#define PFDL_ERR_PARAM_BLCOK_SIZE	0x82
#define PFDL_ERR_PARAM_BLOCK_ALIGN	0x83
#define PFDL_ERR_PARAM_ADDRESS		0x84

/* PFDL Open/Close status. */
#define PFDL_CLOSED 0
#define PFDL_OPENED	1


/***************************************************************************/
/*    Type  Definitions                                                    */
/***************************************************************************/


/***************************************************************************/
/*    Function prototypes                                                  */
/***************************************************************************/
pfdl_status_t pfdl_check_parameter(uint16_t u16Address, uint16_t u16Length);

/***************************************************************************/
/*    Global Variables                                                     */
/***************************************************************************/


/***************************************************************************/
/*    Local Variables                                                      */
/***************************************************************************/
static uint8_t			g_pfdl_status = PFDL_CLOSED;


/***************************************************************************/
/*    Global Routines                                                      */
/***************************************************************************/
/***************************************************************************/
/**
 * データ・フラッシュ・ユーティリティをオープンします。
 *
 * @attention なし
 ***************************************************************************/
pfdl_status_t pfdl_open(void)
{
	pfdl_status_t		error;
	pfdl_descriptor_t	pfdl_desc;

	// PFDLのオープン
	pfdl_desc.fx_MHz_u08			= PFDL_DATA_FLASH_FX_MHZ;
	pfdl_desc.wide_voltage_mode_u08	= 0x00;
	error = PFDL_Open(&pfdl_desc);
	if (error == PFDL_OK) {
		g_pfdl_status = PFDL_OPENED;
	}
	return error;
}


/**
 * データ・フラッシュ・ユーティリティをクローズします。
 *
 * @attention なし
 ***************************************************************************/
void pfdl_close(void)
{
	if (g_pfdl_status == PFDL_OPENED) {
		PFDL_Close();
		g_pfdl_status = PFDL_CLOSED;
	}
}


/**
 * データ・フラッシュのブランクチェックを行います。
 *
 * param[in] u16Address ブランクチェックする先頭アドレスを指定します。
 * param[in] u16Length  ブランクチェックするサイズを指定します。
 *
 * @return 実行結果を返します。
 *
 * @attention なし
 ***************************************************************************/
pfdl_status_t pfdl_blank_check(uint16_t u16Address, uint16_t u16Length)
{
	pfdl_status_t	error;
	pfdl_request_t	request;

	if (g_pfdl_status == PFDL_OPENED) {
		// 書き込みサイズとブロック境界のチェック
		error = pfdl_check_parameter(u16Address, u16Length);
		if (error == PFDL_OK) {
			request.command_enu		= PFDL_CMD_BLANKCHECK_BYTES;
			request.index_u16		= u16Address;
			request.bytecount_u16	= u16Length;
			request.data_pu08		= 0;
			error = PFDL_Execute(&request);
			/* コマンド終了待ち */
			while( error == PFDL_BUSY )
			{
				_NOP();
				_NOP();
				/* 終了確認処理 */
				error = PFDL_Handler();
			}
		}
	}
	else {
		error = PFDL_ERR_STATUS;
	}
	return error;
}


/**
 * データ・フラッシュを消去します。
 *
 * param[in] u16BlockNumber 消去するブロックを指定します。
 *
 * @return 実行結果を返します。
 *
 * @attention なし
 ***************************************************************************/
pfdl_status_t pfdl_erase_block(uint16_t u16BlockNumber)
{
	pfdl_status_t	error;
	pfdl_request_t	request;

	if (g_pfdl_status == PFDL_OPENED) {
		if (u16BlockNumber >= PFDL_DATA_FLASH_NUM_OF_BLOCKS) {
			error = PFDL_ERR_PARAM_BLOCK_NUMBER;
		}
		else {
			request.command_enu		= PFDL_CMD_ERASE_BLOCK;
			request.index_u16		= u16BlockNumber;
			error = PFDL_Execute(&request);
			/* コマンド終了待ち */
			while( error == PFDL_BUSY )
			{
				_NOP();
				_NOP();
				/* 終了確認処理 */
				error = PFDL_Handler();
			}
		}
	}
	else {
		error = PFDL_ERR_STATUS;
	}
	return error;
}


/**
 * データ・フラッシュにデータを書き込みます。
 *
 * param[in] u16Address 書き込むアドレスを指定します。
 * param[in] pvData     書き込むデータのアドレスを指定します。
 * param[in] u16Length  書き込むデータのサイズを指定します。
 *
 * @return 実行結果を返します。
 *
 * @attention なし
 ***************************************************************************/
pfdl_status_t pfdl_write(uint16_t u16Address, uint8_t* pu8Data, uint16_t u16Length)
{
	pfdl_status_t	error;
	pfdl_request_t	request;

	if (g_pfdl_status == PFDL_OPENED) {
		// 書き込みサイズとブロック境界のチェック
		error = pfdl_check_parameter(u16Address, u16Length);
		if (error == PFDL_OK) {
			request.command_enu		= PFDL_CMD_WRITE_BYTES;
			request.index_u16		= u16Address;
			request.bytecount_u16	= u16Length;
			request.data_pu08		= pu8Data;
			error = PFDL_Execute(&request);
			/* コマンド終了待ち */
			while( error == PFDL_BUSY )
			{
				_NOP();
				_NOP();
				/* 終了確認処理 */
				error = PFDL_Handler();
			}
		}
	}
	else {
		error = PFDL_ERR_STATUS;
	}
	return error;
}


/**
 * データ・フラッシュからデータを読み込みます。
 *
 * param[in] u16Address 読み込みむアドレスを指定します。
 * param[in] pvData     読み込み先のアドレスを指定します。
 * param[in] u16Length  読み込むデータのサイズを指定します。
 *
 * @return 実行結果を返します。
 *
 * @attention なし
 ***************************************************************************/
pfdl_status_t pfdl_read(uint16_t u16Address, uint8_t* pu8Data, uint16_t u16Length)
{
	pfdl_status_t	error;
	pfdl_request_t	request;

	if (g_pfdl_status == PFDL_OPENED) {
		// 読み込みサイズとブロック境界のチェック
		error = pfdl_check_parameter(u16Address, u16Length);
		if (error == PFDL_OK) {
			request.command_enu		= PFDL_CMD_READ_BYTES;
			request.index_u16		= u16Address;
			request.bytecount_u16	= u16Length;
			request.data_pu08		= pu8Data;
			error = PFDL_Execute(&request);
		}
	}
	else {
		error = PFDL_ERR_STATUS;
	}
	return error;
}


/**
 * データ・フラッシュをベリファイします。
 *
 * param[in] u16Address ベリファイするアドレスを指定します。
 * param[in] u16Length  ベリファイするサイズを指定します。
 *
 * @return 実行結果を返します。
 *
 * @attention なし
 ***************************************************************************/
pfdl_status_t pfdl_verify(uint16_t u16Address, uint16_t u16Length)
{
	pfdl_status_t	error;
	pfdl_request_t	request;

	if (g_pfdl_status == PFDL_OPENED) {
		// 読み込みサイズとブロック境界のチェック
		error = pfdl_check_parameter(u16Address, u16Length);
		if (error == PFDL_OK) {
			request.command_enu		= PFDL_CMD_IVERIFY_BYTES;
			request.index_u16		= u16Address;
			request.bytecount_u16	= u16Length;
			error = PFDL_Execute(&request);
			/* コマンド終了待ち */
			while( error == PFDL_BUSY )
			{
				NOP();
				NOP();
				/* 終了確認処理 */
				error = PFDL_Handler();
			}
		}
	}
	else {
		error = PFDL_ERR_STATUS;
	}
	return error;
}

/***************************************************************************/
/*    Local Routines                                                       */
/***************************************************************************/

pfdl_status_t pfdl_check_parameter(uint16_t u16DataFlashAddress, uint16_t u16Length)
{
	pfdl_status_t	error = PFDL_OK;

	// 書き込みサイズのチェック
	if (u16Length > PFDL_DATA_FLASH_BLOCK_SIZE) {
		error = PFDL_ERR_PARAM_BLCOK_SIZE;
	}
	// データフラッシュのアドレスチェック
	else if (u16DataFlashAddress > PFDL_DATA_FLASH_TOTAL_SIZE) {
		error = PFDL_ERR_PARAM_ADDRESS;
	}
	// ブロックの境界チェック
	else if (((u16DataFlashAddress + u16Length) / PFDL_DATA_FLASH_BLOCK_SIZE) > PFDL_DATA_FLASH_BLOCK_SIZE) {
		error = PFDL_ERR_PARAM_BLOCK_ALIGN;
	}

	return error;
}


/***************************************************************************/
/* End of module                                                           */
/***************************************************************************/
