#pragma once
//=====================================================================//
/*!	@file
	@brief	１２×１２漢字フォント・クラス
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include <cstdint>
#include "ff12a/src/ff.h"

namespace graphics {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	漢字フォント・クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <uint16_t CASH_SIZE>
	class kfont12 {

		struct kanji_cash {
			uint16_t	sjis;
			uint8_t		bitmap[18];
		};
		kanji_cash cash_[CASH_SIZE];
		uint8_t cash_idx_;


		static uint16_t sjis_to_liner_(uint16_t sjis)
		{
			uint16_t code;
			uint8_t up = sjis >> 8;
			uint8_t lo = sjis & 0xff;
			if(0x81 <= up && up <= 0x9f) {
				code = up - 0x81;
			} else if(0xe0 <= up && up <= 0xef) {
				code = (0x9f + 1 - 0x81) + up - 0xe0;
			} else {
				return 0xffff;
			}
			int loa = (0x7e + 1 - 0x40) + (0xfc + 1 - 0x80);
			if(0x40 <= lo && lo <= 0x7e) {
				code *= loa;
				code += lo - 0x40;
			} else if(0x80 <= lo && lo <= 0xfc) {
				code *= loa;
				code += 0x7e + 1 - 0x40;
				code += lo - 0x80;
			} else {
				return 0xffff;
			}
			return code;
		}

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief	コンストラクター
		*/
		//-----------------------------------------------------------------//
		kfont12() : cash_idx_(0) {
			for(uint8_t i = 0; i < CASH_SIZE; ++i) {
				cash_[i].sjis = 0;
			}
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	文字の横幅
		*/
		//-----------------------------------------------------------------//
		static const uint8_t width = 12;


		//-----------------------------------------------------------------//
		/*!
			@brief	文字の高さ
		*/
		//-----------------------------------------------------------------//
		static const uint8_t height = 12;


		//-----------------------------------------------------------------//
		/*!
			@brief	文字のビットマップを取得
			@param[in]	code	文字コード（unicode）
			@return 文字のビットマップ
		*/
		//-----------------------------------------------------------------//
		const uint8_t* get(uint16_t code) {
			uint16_t sjis = ff_convert(code, 0);
			uint32_t lin = sjis_to_liner_(sjis);
			if(lin == 0xffff) {
				return nullptr;
			}

			// キャッシュ内検索
			for(uint8_t i = 0; i < CASH_SIZE; ++i) {
				if(cash_[cash_idx_].sjis == sjis) {
					return &cash_[cash_idx_].bitmap[0];
				}
				++cash_idx_;
				if(cash_idx_ >= CASH_SIZE) {
					cash_idx_ = 0;
				}
			}

			FIL fp;
			if(f_open(&fp, "kfont12.bin", FA_READ) != FR_OK) {
				return nullptr;
			}
 
			if(f_lseek(&fp, lin * 18) != FR_OK) {
				f_close(&fp);
				return nullptr;
			}

			UINT rs;
			if(f_read(&fp, &cash_[cash_idx_].bitmap[0], 18, &rs) != FR_OK) {
				f_close(&fp);
				return nullptr;
			}
			cash_[cash_idx_].sjis = sjis;

			f_close(&fp);

			return &cash_[cash_idx_].bitmap[0];
		}
	};
}