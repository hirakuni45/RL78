/*******************************************************************************
* Library       : Flash Data Library T04 (PicoFDL)
*
* File Name     : $Source: pfdl_types.h $
* Lib. Version  : $RL78_FDL_LIB_VERSION_T04_GNU: V1.04 $
* Mod. Revision : $Revision: 1.9 $
* Mod. Date     : $Date: 2013/02/19 09:59:39CET $
* Device(s)     : RL78
* Description   : Type definitions used by the library
*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

#ifndef __PFDL_TYPES_H_INCLUDED
#define __PFDL_TYPES_H_INCLUDED


/*==============================================================================================*/
/* unsigned type definitions                                                                    */
/*==============================================================================================*/
typedef unsigned char                       pfdl_u08;
typedef unsigned int                        pfdl_u16;
typedef unsigned long int                   pfdl_u32;


/*==============================================================================================*/
/* global constant definitions                                                                  */
/*==============================================================================================*/



/*==============================================================================================*/
/* global type definitions                                                                      */
/*==============================================================================================*/


/* PFDL command code set */
typedef enum 
{                                               /* ---------------------------------------------- */
    PFDL_CMD_READ_BYTES          = (0x00),      /* 0x00, reads data from flash memory             */
    PFDL_CMD_IVERIFY_BYTES       = (0x06),      /* 0x06, verifies data if flash content is stable */
    PFDL_CMD_BLANKCHECK_BYTES    = (0x08),      /* 0x08, checks if flash content is blank         */
    PFDL_CMD_WRITE_BYTES         = (0x04),      /* 0x04, writes data into flash memory            */
    PFDL_CMD_ERASE_BLOCK         = (0x03)       /* 0x03, erases one flash block                   */
} __attribute__ ((__packed__)) pfdl_command_t;  /* ---------------------------------------------- */


/* PFDL error code set */
typedef enum
{
    /* operation related status             */  /* ---------------------------------------------- */
    PFDL_IDLE                     = (0x30),     /* 0x30, PFDL ready to receive requests           */
    PFDL_OK                       = (0x00),     /* 0x00, command finished without problems        */
    PFDL_BUSY                     = (0xFF),     /* 0xFF, command is being processed               */
    /* flash related status                 */  /* ---------------------------------------------- */
    PFDL_ERR_PROTECTION           = (0x10),     /* 0x10, protection error (access right conflict) */
    PFDL_ERR_ERASE                = (0x1A),     /* 0x1A, erase error                              */
    PFDL_ERR_MARGIN               = (0x1B),     /* 0x1B, blankcheck or verify margin violated     */
    PFDL_ERR_WRITE                = (0x1C),     /* 0x1C, write error                              */
    PFDL_ERR_PARAMETER            = (0x05)      /* 0x05, parameter error                          */
} __attribute__ ((__packed__)) pfdl_status_t;   /* ---------------------------------------------- */


/* PFDL request type (base type for any PFDL access) */
typedef struct
{                                               /* ---------------------------------------------- */
    pfdl_u16              index_u16;            /* 2, W, virt. byte/block index inside PFDL-pool  */
    pfdl_u08*             data_pu08;            /* 2, W, pointer to the 1'st byte of data buffer  */
    pfdl_u16              bytecount_u16;        /* 2, W, number of bytes to be transfered         */
    pfdl_command_t        command_enu;          /* 1, W, command code                             */
} pfdl_request_t;                               /*------------------------------------------------*/
                                                /* 7 bytes in total                               */

/* PFDL descriptor type */
typedef struct
{                                               /* ---------------------------------------------- */
    pfdl_u08      fx_MHz_u08;                   /* 1, system frequency expressed in MHz           */
    pfdl_u08      wide_voltage_mode_u08;        /* 1, programming voltage mode ( full/wide )      */
} pfdl_descriptor_t;                            /*------------------------------------------------*/
                                                /* 2 bytes in total                               */


/*==============================================================================================*/
/* type definition plausibility check                                                           */
/*==============================================================================================*/

/* The following checks are implemented in order to check the correct size of the FDL type      */
/* definitions at compile time. In case of a compilation error in the following lines, please   */
/* check your compiler options for enumeration types and structures and contact your local      */
/* support, if necessary.                                                                       */

#define R_PFDLT04_ASSERT_CONCAT_(a, b) a##b
#define R_PFDLT04_ASSERT_CONCAT(a, b) R_PFDLT04_ASSERT_CONCAT_(a, b)
#define R_PFDLT04_STATIC_ASSERT(e) enum { R_PFDLT04_ASSERT_CONCAT(R_PFDLT04_ASSERT_LINE_, __LINE__) = 1/(!!(e)) }


/* assertion if unsigned data type size is not correct, please evaluate compiler settings for integer types */
R_PFDLT04_STATIC_ASSERT(sizeof(pfdl_u08)==1);
R_PFDLT04_STATIC_ASSERT(sizeof(pfdl_u16)==2);
R_PFDLT04_STATIC_ASSERT(sizeof(pfdl_u32)==4);

/* assertion if unsigned data type size is not correct, please evaluate compiler settings for enumeration types */
R_PFDLT04_STATIC_ASSERT(sizeof(pfdl_command_t)==1);
R_PFDLT04_STATIC_ASSERT(sizeof(pfdl_status_t)==1);

/* assertion if structure type size is not correct, please evaluate compiler settings for structure types */
R_PFDLT04_STATIC_ASSERT(sizeof(pfdl_request_t)>=7); /* sizeof(pfdl_request_t) == 7 for packed structures */
R_PFDLT04_STATIC_ASSERT(sizeof(pfdl_request_t)<=8); /* sizeof(pfdl_request_t) == 8 for unpacked structures */
R_PFDLT04_STATIC_ASSERT(sizeof(pfdl_descriptor_t)==2);


#endif
