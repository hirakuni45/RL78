#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78 programmer クラス
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include "rl78_protocol.hpp"
#include <boost/format.hpp>

namespace rl78 {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	rl78_prog クラス
	 */
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	class prog {
		bool		verbose_;
		protocol	proto_;

		std::string out_section_(uint32_t n, uint32_t num) const {
			return (boost::format("#%02d/%02d: ") % n % num).str();
		}

	public:
		//-------------------------------------------------------------//
		/*!
			@brief	コンストラクター
		*/
		//-------------------------------------------------------------//
		prog(bool verbose = false) : verbose_(verbose) { }


		//-------------------------------------------------------------//
		/*!
			@brief	接続速度を変更する
			@param[in]	path	シリアル・デバイス・パス
			@param[in]	brate	ボーレート
			@param[in]	voltage	動作電圧
			@return エラー無ければ「true」
		*/
		//-------------------------------------------------------------//
		bool start(const std::string& path, uint32_t brate, uint32_t voltage) {
			speed_t spt;
			switch(brate) {
			case 115200:
				spt = B115200;
				break;
//			case 250000:
//				spt = B250000;
//				break;
			case 500000:
				spt = B500000;
				break;
			case 1000000:
				spt = B1000000;
				break;
			default:
				std::cerr << boost::format("False speed range: %d") % brate << std::endl;
				return false;
			}

			if(!proto_.start(path, spt, voltage)) {
				std::cerr << boost::format("RL78 connection error: '%s'") % path << std::endl;
				return false;
			}

//			proto_.reset();

			return true;
		}


		//-------------------------------------------------------------//
		/*!
			@brief	リード
			@param[in]	adr	開始アドレス
			@param[out]	dst	書き込みアドレス
			@param[in]	len	読み出しサイズ
			@return 成功なら「true」
		*/
		//-------------------------------------------------------------//
		bool read(uint32_t adr, uint8_t* dst, uint32_t len) {
#if 0
			if(!proto_.read(adr, len, dst)) {
				proto_.end();
				std::cerr << "Read error." << std::endl;
				return false;
			}
#endif
			return true;
		}


		//-------------------------------------------------------------//
		/*!
			@brief	ベリファイ
			@param[in]	adr	開始アドレス
			@param[in]	src	書き込みアドレス
			@param[in]	len	読み出しサイズ
			@return 成功なら「true」
		*/
		//-------------------------------------------------------------//
		bool verify(uint32_t adr, const uint8_t* src, uint32_t len) {
#if 0
			std::vector<uint8_t> dev;
			dev.resize(len);
			if(!read(adr, &dev[0], len)) {
				return false;
			}
			uint32_t errcnt = 0;
			for(auto d : dev) {
				auto m = *src++;
				if(d != m) {
					++errcnt;
					if(verbose_) {
						std::cerr << (boost::format("0x%08X: D(%02X) to M(%02X)") % adr %
							static_cast<uint32_t>(d) % static_cast<uint32_t>(m)) << std::endl;
					}
				}
				++adr;
			}
			if(errcnt > 0) {
				std::cerr << "Verify error: " << errcnt << std::endl;
				return false;
			}
#endif
			return true;
		}


		//-------------------------------------------------------------//
		/*!
			@brief	ライト開始
			@param[in]	data	「true」ならデータ領域
			@return 成功なら「true」
		*/
		//-------------------------------------------------------------//
		bool start_write(bool data) {
#if 0
			if(!proto_.select_write_area(data)) {
				proto_.end();
				std::cerr << "Write start error.(first)" << std::endl;
				return false;
			}
#endif
			return true;
		}


		//-------------------------------------------------------------//
		/*!
			@brief	ライト（２５６バイト）
			@param[in]	adr	開始アドレス
			@param[in]	src	書き込みアドレス
			@return 成功なら「true」
		*/
		//-------------------------------------------------------------//
		bool write(uint32_t adr, const uint8_t* src) {
#if 0
			if(!proto_.write_page(adr, src)) {
				proto_.end();
				std::cerr << "Write body error." << std::endl;
				return false;
			}
#endif
			return true;
		}


		//-------------------------------------------------------------//
		/*!
			@brief	ライト終了
			@return 成功なら「true」
		*/
		//-------------------------------------------------------------//
		bool final_write() {
#if 0
			if(!proto_.write_page(0xffffffff, nullptr)) {
				proto_.end();
				std::cerr << "Write final error. (fin)" << std::endl;
				return false;
			}
#endif
			return true;
		}


		//-------------------------------------------------------------//
		/*!
			@brief	終了
		*/
		//-------------------------------------------------------------//
		void end() {
			proto_.end();
		}
	};
}
