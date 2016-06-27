#pragma once
//=====================================================================//
/*!	@file
	@brief	R78 グループ・ベクター関係 @n
			Copyright 2016 Kunihito Hiramatsu
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include <unistd.h>

#define INTERRUPT_FUNC __attribute__ ((interrupt))

#ifdef __cplusplus
extern "C" {
#endif
	//-----------------------------------------------------------------//
	/*!
		@brief  無効タスクエントリー（割り込み）
	*/
	//-----------------------------------------------------------------//
	void null_task_(void);


	//-----------------------------------------------------------------//
	/*!
		@brief  割り込み無効
	*/
	//-----------------------------------------------------------------//
	void di(void);


	//-----------------------------------------------------------------//
	/*!
		@brief  割り込み有効
	*/
	//-----------------------------------------------------------------//
	void ei(void);
#ifdef __cplusplus
};
#endif