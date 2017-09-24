#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/G13 グループ・データ・フラッシュ制御
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include "data_flash_lib/data_flash_util.h"

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
			@brief  データ・バンク定義
		*/
		//-----------------------------------------------------------------//
		enum class area : uint8_t {
			bank0,	///< 0xF1000 to 0xF13FF (1024)
			bank1,	///< 0xF1400 to 0xF17FF (1024)
			bank2,	///< 0xF1800 to 0xF1BFF (1024)
			bank3,	///< 0xF1C00 to 0xF1FFF (1024)
			bank4,	///< 0xF2000 to 0xF23FF (1024)
			bank5,	///< 0xF2400 to 0xF27FF (1024)
			bank6,	///< 0xF2800 to 0xF2BFF (1024)
			bank7,	///< 0xF2C00 to 0xF2FFF (1024)
		};

	private:


	public:
		//-----------------------------------------------------------------//
		/*!
			@brief  コンストラクター
		*/
		//-----------------------------------------------------------------//
		flash_io() { }


		//-----------------------------------------------------------------//
		/*!
			@brief  開始
		*/
		//-----------------------------------------------------------------//
		bool start()
		{
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
			@brief  消去
			@param[in]	bank	バンク
			@return エラーがあれば「false」
		*/
		//-----------------------------------------------------------------//
		bool erase(area bank) const {

			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  読み出し
			@param[in]	ofs	開始オフセット
			@param[in]	len	バイト数
			@param[out]	dst	先
		*/
		//-----------------------------------------------------------------//
		void read(uint16_t ofs, uint16_t len, void* dst) const {
#if 0
			if(ofs >= 0x0800 || (ofs + len) > 0x0800) {
				return;
			}

			// ※リードアレイコマンド発行済みが前提
			for(uint16_t i = 0; i < len; ++i) {
				*dst = rd8_(0x3000 + ofs + i);
				++dst;
			}
#endif
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  読み出し
			@param[in]	ofs	開始オフセット
			@return データ
		*/
		//-----------------------------------------------------------------//
		uint8_t read(uint16_t ofs) const {
#if 0
			if(ofs >= 0x0800) {
				return 0;
			}

			return rd8_(0x3000 + ofs);
#endif
			return 0;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  書き込み
			@param[in]	ofs	開始オフセット
			@param[in]	data	書き込みデータ
			@return 
		*/
		//-----------------------------------------------------------------//
		bool write(uint16_t ofs, uint8_t data) const {
#if 0
			if(ofs >= 0x0800) {
				return false;
			}

			di();
			enable_(ofs);

			bool ret = write_(ofs, data);

			wr8_(0x3000, 0xff);

			disable_(ofs);
			ei();

			return ret;
#endif
			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  書き込み
			@param[in]	src ソース
			@param[in]	ofs	開始オフセット
			@param[in]	len	バイト数
		*/
		//-----------------------------------------------------------------//
		bool write(const uint8_t* src, uint16_t ofs, uint16_t len) const {
#if 0
			if(ofs >= 0x0800 || (ofs + len) > 0x0800) {
				return false;
			}

			di();
			enable_(ofs);

			bool ret;
			for(uint16_t i = 0; i < len; ++i) {
				ret = write_(ofs + i, *src);
				if(!ret) break;
				++src;
			}

			wr8_(0x3000, 0xff);

			disable_(ofs);
			ei();

			return ret;
#endif
			return true;
		}
	};
}
