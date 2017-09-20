/***************************************************************************
 *
 * PURPOSE
 *   Data Flash Utility header file.
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
 * @file  data_flash_util.h
 * @brief RL78マイコン内蔵のデータ フラッシュROMを使うためのライブラリのヘッダファイルです。
 * 
 */

/***************************************************************************/
/*    Include Header Files                                                 */
/***************************************************************************/
/// #include "Arduino.h"
#include <stdint.h>
#include "pfdl.h"

/***************************************************************************/
/*    Macro Definitions                                                    */
/***************************************************************************/
/* Data Flush Parameters. */
#define PFDL_DATA_FLASH_BLOCK_SIZE		1024
#define PFDL_DATA_FLASH_NUM_OF_BLOCKS	8
#define PFDL_DATA_FLASH_TOTAL_SIZE		(PFDL_DATA_FLASH_BLOCK_SIZE * PFDL_DATA_FLASH_NUM_OF_BLOCKS)
#define PFDL_DATA_FLASH_FX_MHZ			32


/***************************************************************************/
/*    Type  Definitions                                                    */
/***************************************************************************/


/***************************************************************************/
/*    Function prototypes                                                  */
/***************************************************************************/


/***************************************************************************/
/*    Global Variables                                                     */
/***************************************************************************/


/***************************************************************************/
/*    Local Variables                                                      */
/***************************************************************************/


/***************************************************************************/
/*    Global Routines                                                      */
/***************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
pfdl_status_t pfdl_open(void);
void pfdl_close(void);
pfdl_status_t pfdl_blank_check(uint16_t u16Address, uint16_t u16Length);
pfdl_status_t pfdl_erase_block(uint16_t u16BlockNumber);
pfdl_status_t pfdl_write(uint16_t u16Address, uint8_t* pu8Data, uint16_t u16Length);
pfdl_status_t pfdl_read(uint16_t u16Address, uint8_t* pu8Data, uint16_t u16Length);
pfdl_status_t pfdl_verify(uint16_t u16Address, uint16_t u16Length);
pfdl_status_t pfdl_check_parameter(uint16_t u16DataFlashAddress, uint16_t u16Length);
#ifdef __cplusplus
}
#endif


/***************************************************************************/
/*    Local Routines                                                       */
/***************************************************************************/


/***************************************************************************/
/* End of module                                                           */
/***************************************************************************/
