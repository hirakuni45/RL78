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

		protocol::signature_t	sig_;

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
			@brief	シグネチュアを取得
			@return シグナチュア
		*/
		//-------------------------------------------------------------//
		const protocol::signature_t& get_signature() const { return sig_; }


		//-------------------------------------------------------------//
		/*!
			@brief	接続速度を変更する
			@param[in]	path	シリアル・デバイス・パス
			@param[in]	brate	ボーレート
			@param[in]	voltage	動作電圧
			@return エラー無ければ「true」
		*/
		//-------------------------------------------------------------//
		bool start(const std::string& path, uint32_t brate, uint32_t voltage)
		{
			switch(brate) {
			case 115200:
				break;
//			case 250000:
//				spt = B250000;
//				break;
			case 500000:
				break;
			case 1000000:
				break;
			default:
				return false;
			}

			if(!proto_.start(path, brate, voltage)) {
//				std::cerr << boost::format("RL78 connection error: '%s'") % path << std::endl;
				return false;
			}

			if(!proto_.reset()) {
				return false;
			}

			if(!proto_.silicon_signature(sig_)) {
				return false;
			}

			return true;
		}


		//-------------------------------------------------------------//
		/*!
			@brief	ブロック消去
			@param[in]	org	開始アドレス（ブロック：１０２４バイト）
			@return 成功なら「true」
		*/
		//-------------------------------------------------------------//
		bool block_erase(uint32_t org) {
			bool f = proto_.block_blank_check(org, org + 1023, 0);
			if(!f) {
				return false;
			}
			if(!proto_.get_blank()) { 
				return proto_.block_erase(org);
			}
			return true;
		}


		//-------------------------------------------------------------//
		/*!
			@brief	ライト開始（１０２４バイトブロック単位）
			@param[in]	org	開始アドレス
			@param[in]	end 終了アドレス
			@return 成功なら「true」
		*/
		//-------------------------------------------------------------//
		bool start_write(uint32_t org, uint32_t end) {
			if(!proto_.programming(org, end)) {
				proto_.end();
				return false;
			}
			return true;
		}


		//-------------------------------------------------------------//
		/*!
			@brief	ライト・ページ（最大２５６バイト）
			@param[in]	src		書き込みアドレス
			@param[in]	len		書き込み長
			@param[in]	last	最終フレームの場合に「true」
			@return 成功なら「true」
		*/
		//-------------------------------------------------------------//
		bool write_page(const void* src, uint32_t len, bool last) {
			if(!proto_.send_program_data(src, len, last)) {
				proto_.end();
				return false;
			}
			return true;
		}


		//-------------------------------------------------------------//
		/*!
			@brief	ライト開始（１０２４バイトブロック単位）
			@param[in]	org	開始アドレス
			@param[in]	end 終了アドレス
			@return 成功なら「true」
		*/
		//-------------------------------------------------------------//
		bool start_verify(uint32_t org, uint32_t end) {
			if(!proto_.verify(org, end)) {
				proto_.end();
				return false;
			}
			return true;
		}


		//-------------------------------------------------------------//
		/*!
			@brief	ベリファイ・ページ（最大２５６バイト）
			@param[in]	src		書き込みアドレス
			@param[in]	len		書き込み長
			@param[in]	last	最終フレームの場合に「true」
			@return 成功なら「true」
		*/
		//-------------------------------------------------------------//
		bool verify_page(const void* src, uint32_t len, bool last) {
			if(!proto_.send_verify_data(src, len, last)) {
				proto_.end();
				return false;
			}
			return true;
		}


		//-------------------------------------------------------------//
		/*!
			@brief	セキリティ登録
			@param[in]	src	セキリティ・ソース
			@return 成功なら「true」
		*/
		//-------------------------------------------------------------//
		bool set_security(const protocol::security_t& src) {
			if(!proto_.security_set(src)) {
				proto_.end();
				return false;
			}
			return true;
		}


		//-------------------------------------------------------------//
		/*!
			@brief	セキリティ取得
			@param[out]	dst	セキリティ保存先
			@return 成功なら「true」
		*/
		//-------------------------------------------------------------//
		bool get_security(protocol::security_t& dst) {
			if(!proto_.security_get(dst)) {
				proto_.end();
				return false;
			}
			return true;
		}


		//-------------------------------------------------------------//
		/*!
			@brief	セキリティ・リリース
			@return 成功なら「true」
		*/
		//-------------------------------------------------------------//
		bool security_release() {
			if(!proto_.security_release()) {
				proto_.end();
				return false;
			}
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
