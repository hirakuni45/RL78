#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78 グループ・データ・フラッシュ制御
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2017, 2018 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include "data_flash_lib/data_flash_util.h"

/// リンカースクリプトで、データ・フラッシュの容量を設定
extern const uint16_t _dataflashsize;

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
		uint16_t	size_;
		uint16_t	bank_;
		error		error_;

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief  コンストラクター
		*/
		//-----------------------------------------------------------------//
		flash_io() : size_(_dataflashsize), bank_(_dataflashsize / data_flash_block),
			error_(error::NONE) { }


		//-----------------------------------------------------------------//
		/*!
			@brief  データ・フラッシュ・サイズ取得
			@return データ・フラッシュ・サイズ
		*/
		//-----------------------------------------------------------------//
		uint16_t size() const { return size_; }


		//-----------------------------------------------------------------//
		/*!
			@brief  データ・フラッシュ・バンク取得
			@return データ・フラッシュ・バンク
		*/
		//-----------------------------------------------------------------//
		uint16_t bank() const { return bank_; }


		//-----------------------------------------------------------------//
		/*!
			@brief  開始
		*/
		//-----------------------------------------------------------------//
		bool start()
		{
			if(size() == 0) return false;

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
			if(org >= size()) {
				error_ = error::ADDRESS;
				return false;
			}
			if((org + len) > size()) {
				len = size() - org;
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
			if(org >= size()) {
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
			if(org >= size()) {
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
			for(uint16_t pos = 0; pos < size(); pos += data_flash_block) {
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
			if(org >= size()) {
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
