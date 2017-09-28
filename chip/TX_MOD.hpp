#pragma once
//=====================================================================//
/*!	@file
	@brief	TX_MOD ドライバー @n
			※「RX_MOD.hpp」とペア
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include <cstdint>
#include <cstring>

namespace chip {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  送信データ変調 テンプレートクラス
		@param[in]	PORT	送信ポートクラス（標準４）※必ず偶数
		@param[in]	HNUM	ヘッダー・フレーム数（標準４）
		@param[in]	SNUM	転送最大数
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class PORT, uint8_t HNUM = 4, uint8_t SNUM = 4>
	class TX_MOD {

		volatile uint8_t	head_;
		volatile uint8_t	sync_;
		volatile uint8_t	len_;
		uint8_t	dat_;
		volatile uint8_t	bit_pos_;

		uint8_t		buff_[SNUM];

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief	コンストラクター
		 */
		//-----------------------------------------------------------------//
		TX_MOD() noexcept : head_(0), sync_(0), len_(0), dat_(0), bit_pos_(0) { }


		//-----------------------------------------------------------------//
		/*!
			@brief	開始
		 */
		//-----------------------------------------------------------------//
		void start() noexcept
		{
			PORT::DIR = 1;  // output
			PORT::P = 0;  // 初期ポート値
			len_ = 0;
			head_ = 0;
			sync_ = 0;
			bit_pos_ = 0;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	転送状態を取得
			@return 転送中なら「true」
		 */
		//-----------------------------------------------------------------//
		bool probe() const noexcept { return len_ > 0; }


		//-----------------------------------------------------------------//
		/*!
			@brief	転送バイトを取得
			@return 転送バイト
		 */
		//-----------------------------------------------------------------//
		uint8_t get_send_pos() const noexcept {
			return bit_pos_ >> 3;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	送信要求
			@param[in]	src	送信ソース
			@param[in]	len	送信バイト数
			@return 送信開始なら「true」
		 */
		//-----------------------------------------------------------------//
		bool send(const void* src, uint8_t len) noexcept
		{
			if(len > SNUM ) return false;

			std::memcpy(buff_, src, len);
			head_ = 0;
			sync_ = 0;
			bit_pos_ = 0;
			len_ = len;
			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	サービス @n
					正確なタイマー割り込みで起動されるタスク
		 */
		//-----------------------------------------------------------------//
		void service() noexcept
		{
			if(len_ == 0) return;

			if(head_ < HNUM) {
				PORT::P = head_ & 1;
				++head_;
			} else if(sync_ < 2) {
				PORT::P = 0;
				++sync_;
				dat_ = 2;
			} else if((bit_pos_ >> 3) < len_) {
				static const uint8_t mask[8] = {
					0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01
				};
				if(buff_[bit_pos_ >> 3] & mask[bit_pos_ & 7]) {
					PORT::P = !PORT::P();
				}
				++dat_;
				if(dat_ >= 2) {
					++bit_pos_;
					dat_ = 0;
				}
			} else {
				len_ = 0;
			}
		}
	};
}
