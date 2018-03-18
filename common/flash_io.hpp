#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78 グループ・データ・フラッシュ制御
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include "data_flash_lib/data_flash_util.h"

/// DEVICE_SIG はデータ・フラッシュ・メモリーの容量定義に必要で、設定が無ければエラーとする
#ifndef DEVICE_SIG
#  error "flash_io.hpp requires DEVICE_SIG to be defined"
#endif

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  フラッシュ制御クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	class flash_io {
	public:
		//-----------------------------------------------------------------//
		/*!
			@brief  データ・フラッシュ構成 @n
					R5F100LC, R5F100LE: 4K, 4 x 1024 @n
					R5F100LG, R5F100LJ: 8K, 8 x 1024
		*/
		//-----------------------------------------------------------------//
		static const uint16_t data_flash_block = 1024;	///< データ・フラッシュのブロックサイズ
#if (DEVICE_SIG == R5F100LC) || (DEVICE_SIG == R5F100LE)
		static const uint16_t data_flash_size  = 4096;	///< データ・フラッシュの容量
		static const uint16_t data_flash_bank  = 4;		///< データ・フラッシュのバンク数
#elif (DEVICE_SIG == R5F100LG) || (DEVICE_SIG == R5F100LJ)
		static const uint16_t data_flash_size  = 8192;	///< データ・フラッシュの容量
		static const uint16_t data_flash_bank  = 8;		///< データ・フラッシュのバンク数
#else
		static const uint16_t data_flash_size  = 0;	    ///< データ・フラッシュの容量
		static const uint16_t data_flash_bank  = 0;		///< データ・フラッシュのバンク数
#endif

		//-----------------------------------------------------------------//
		/*!
			@brief  エラー型
		*/
		//-----------------------------------------------------------------//
		enum class error : uint8_t {
			NONE,		///< エラー無し
			ADDRESS,	///< アドレス・エラー
			TIMEOUT,	///< タイム・アウト・エラー
			LOCK,		///< ロック・エラー
		};

	private:
		error		error_;

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief  コンストラクター
		*/
		//-----------------------------------------------------------------//
		flash_io() : error_(error::NONE) { }


		//-----------------------------------------------------------------//
		/*!
			@brief  開始
		*/
		//-----------------------------------------------------------------//
		bool start()
		{
			if(data_flash_size == 0) return false;

			error_ = error::NONE;
			return pfdl_open() == PFDL_OK;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  終了
		*/
		//-----------------------------------------------------------------//
		void end()
		{
			pfdl_close();
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  読み出し
			@param[in]	org	開始アドレス
			@param[out]	dst	先
			@param[in]	len	バイト数
			@return エラー無ければ「true」
		*/
		//-----------------------------------------------------------------//
		bool read(uint16_t org, void* dst, uint16_t len)
		{
			if(org >= data_flash_size) {
				error_ = error::ADDRESS;
				return false;
			}
			if((org + len) > data_flash_size) {
				len = data_flash_size - org;
			}

			return pfdl_read(org, static_cast<uint8_t*>(dst), len) == PFDL_OK;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  読み出し
			@param[in]	org	開始アドレス
			@return データ
		*/
		//-----------------------------------------------------------------//
		uint8_t read(uint16_t org)
		{
			uint8_t tmp[1];
			if(read(org, &tmp, 1)) {
				return tmp[0];
			}
			return 0;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  消去チェック
			@param[in]	org		開始アドレス
			@param[in]	len		検査長（バイト単位）
			@return 消去されていれば「true」（エラーは「false」）
		*/
		//-----------------------------------------------------------------//
		bool erase_check(uint16_t org, uint16_t len = data_flash_block)
		{
			if(org >= data_flash_size) {
				error_ = error::ADDRESS;
				return false;
			}

			return pfdl_blank_check(org, len) == PFDL_OK;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  消去
			@param[in]	org		開始アドレス
			@return エラーがあれば「false」
		*/
		//-----------------------------------------------------------------//
		bool erase(uint16_t org)
		{
			if(org >= data_flash_size) {
				error_ = error::ADDRESS;
				return false;
   			}

			return pfdl_erase_block(org / data_flash_block) == PFDL_OK;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  全消去
			@return エラーがあれば「false」
		*/
		//-----------------------------------------------------------------//
		bool erase_all()
		{
			for(uint16_t pos = 0; pos < data_flash_size; pos += data_flash_block) {
				if(!erase_check(pos)) {
					auto ret = erase(pos);
					if(!ret) {
						return false;
					}
				}
			}
			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  書き込み @n
					※仕様上、４バイト単位で書き込まれる。@n
					※４バイト未満の場合は、０ｘＦＦが書き込まれる
			@param[in]	org	開始オフセット
			@param[in]	src ソース
			@param[in]	len	バイト数
			@return エラーがあれば「false」
		*/
		//-----------------------------------------------------------------//
		bool write(uint16_t org, const void* src, uint16_t len)
		{
			if(org >= data_flash_size) {
				error_ = error::ADDRESS;
				return false;
			}

			return pfdl_write(org, static_cast<const uint8_t*>(src), len) == PFDL_OK;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  書き込み
			@param[in]	org	開始オフセット
			@param[in]	data	書き込みデータ
			@return エラーがあれば「false」
		*/
		//-----------------------------------------------------------------//
		bool write(uint16_t org, uint8_t data)
		{
			uint8_t d = data;
			return write(org, &d, 1);
		}
	};
}
