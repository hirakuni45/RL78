#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78 グループ・データ・フラッシュ・マネージャー
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2018 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include "common/flash_io.hpp"
#include "common/format.hpp"

namespace utils {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  フラッシュ・マネージャー・クラス
		@param[in]	ST	構造体
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class ST>
	class flash_man {

		typedef device::flash_io IO;

		IO&		io_;

		struct stt_t {
			uint32_t	magic_;
			ST			st_;
			stt_t(uint32_t magic) : magic_(magic), st_() { }
		};

		stt_t		stt_;

		uint16_t	pacs_;

		uint32_t	magic_;

		uint16_t scan_free_()
		{
			uint16_t adr = 0;
			for(uint16_t i = 0; i < io_.size(); i += pacs_) {
				adr = i;
				if(io_.erase_check(adr, sizeof(stt_t))) {
					return adr;
				}
			}
			return io_.size();  // full !
		}

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief  コンストラクター
			@param[in]	io		フラッシュ I/O（参照）
			@param[in]	magic	マジックワード
		*/
		//-----------------------------------------------------------------//
		flash_man(IO& io, uint32_t magic = 0x329F4B71) : io_(io),
			stt_(magic), pacs_(0), magic_(magic)
		{ }


		//-----------------------------------------------------------------//
		/*!
			@brief  開始
			@return 成功なら「true」
		*/
		//-----------------------------------------------------------------//
		bool start()
		{
			uint16_t pacs = 0;
			for(uint16_t i = 4; i < 15; ++i) {
				pacs = 1 << i;
				if(sizeof(stt_t) < pacs) {
					break;
				}
			}
			pacs_ = pacs;  // パックサイズ
			utils::format("FlashMan: pac size: %d\n") % pacs_;
			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  書き込み
			@param[in]	st	構造体
			@return 成功なら「true」
		*/
		//-----------------------------------------------------------------//
		bool write(const ST& st)
		{
			uint16_t adr = scan_free_();
			if(adr >= io_.size()) return false;
			stt_.magic_ = magic_;
			stt_.st_ = st;
			bool ret = io_.write(adr, &stt_, sizeof(stt_t));
			utils::format("Write Flash: %04X at %d bytes (%d)\n")
				% adr % sizeof(stt_t) % static_cast<int>(ret);
			return ret;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  読み込み
			@param[out]	st	構造体
			@return 成功なら「true」
		*/
		//-----------------------------------------------------------------//
		bool read(ST& st)
		{
			bool ret = false;
			uint16_t adr;
			for(adr = 0; adr < io_.size(); adr += pacs_) {
				if(io_.erase_check(adr, sizeof(stt_t))) {
					break;
				}
				if(!io_.read(adr, &stt_, sizeof(stt_t))) {
					ret = false;
					break;
				} else {
					ret = true;
				}
			}
			if(ret) {
				utils::format("Read Flash: %04X at %d bytes (%d)\n")
					% adr % sizeof(stt_t) % static_cast<int>(ret);
				if(stt_.magic_ == magic_) {
					st = stt_.st_;
				} else {
					ret = false;
					utils::format("Read Flash: magic id error: %08X (%08X)\n")
						% stt_.magic_ % magic_; 
				}
			}
			return ret;
		}
	};
}
