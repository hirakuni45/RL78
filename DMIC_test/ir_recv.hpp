#pragma once
//=====================================================================//
/*!	@file
	@brief	赤外線リモコン受信クラス @n
			(NEC Format) Carrier: 38KHz, 1/3 Duty, T(変調単位): 562 uS @n
			Data: 0: 1T(1)-1T(0), 1:1T(1)-3T(0) @n
			Leader, CustomerCode(16bits), Data... @n
			Frame:  Leader[16T(1)-8T(0)], CustomerCode, Data... @n
			Repeat: 16T(1)-4T(0) @n
			Frame to Repeat (108 ms)
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2018 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/R8C/blob/master/LICENSE
*/
//=====================================================================//
#include <cstdint>

namespace chip {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  赤外線リモコン受信クラス
		@param[in]	INPUT	入力ポート
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class INPUT>
	class ir_recv {

		enum class TASK : uint8_t {
			idle,
			LeaderH,
			LeaderL,
			CustomCode,
			UserData,
			Term,
		};

		INPUT		input_;

		TASK		task_;

		bool		back_;
		uint8_t		count_;
		uint8_t		data_;

		uint8_t		frame_count_;
		uint16_t	custom_code_;

		uint8_t		user_data_;

		enum class DATA_STATE : uint8_t {
			busy,
			low,
			high,
		};


		DATA_STATE data_service_(bool lvl)
		{
			data_ <<= 1;
			if(lvl) data_ |= 1;
			if((data_ & 0b1111) == 0b1001) {
				return DATA_STATE::low;
			} else if((data_ & 0b1111) == 0b1000) {
				return DATA_STATE::high;
			}
			return DATA_STATE::busy;
		}

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief  コンストラクタ
		*/
		//-----------------------------------------------------------------//
		ir_recv() : input_(), task_(TASK::idle), back_(false), count_(0),
			data_(0), frame_count_(0), custom_code_(0), user_data_(0)
			{ }


		//-----------------------------------------------------------------//
		/*!
			@brief  フレームカウントの取得
			@return フレームカウント
		*/
		//-----------------------------------------------------------------//
		uint8_t get_frame_count() const { return frame_count_; }


		//-----------------------------------------------------------------//
		/*!
			@brief  カスタムコードの取得
			@return カスタムコード
		*/
		//-----------------------------------------------------------------//
		uint16_t get_custom_code() const { return custom_code_; }


		//-----------------------------------------------------------------//
		/*!
			@brief  ユーザー・データの取得
			@return ユーザー・データ
		*/
		//-----------------------------------------------------------------//
		uint8_t get_user_data() const { return user_data_; }


		//-----------------------------------------------------------------//
		/*!
			@brief  サービス
		*/
		//-----------------------------------------------------------------//
		void service()
		{
			bool lvl = input_();
			bool posi = !back_ & lvl;
			back_ = lvl;
			switch(task_) {
			case TASK::idle:
				if(posi) {
					count_ = 16 * 2 - 1;
					task_ = TASK::LeaderH;
				}
				break;
			case TASK::LeaderH:
				if(lvl) {
					if(count_ > 0) {
						--count_;
						if(count_ == 0) {
							count_ = 8 * 2;
							task_ = TASK::LeaderL;
						}
					}
				} else {
					task_ = TASK::idle;  // for Noise ?
				}
				break;
			case TASK::LeaderL:
				if(lvl) {
					task_ = TASK::idle;  // for Noise ?
				} else {
					if(count_ > 0) {
						--count_;
						if(count_ == 0) {
							count_ = 16;
							task_ = TASK::CustomCode;
						}
					}
				}
				break;

			case TASK::CustomCode:
				{
					auto st = data_service_(lvl);
					if(st == DATA_STATE::low) {
						custom_code_ <<= 1;
						--count_;
					} else if(st == DATA_STATE::high) {
						custom_code_ <<= 1;
						custom_code_ |= 1;
						--count_;
					}
					if(count_ == 0) {
						count_ = 8;
						task_ = TASK::UserData;
					}
				}
				break;

			case TASK::UserData:
				{
					auto st = data_service_(lvl);
					if(st == DATA_STATE::low) {
						user_data_ <<= 1;
						--count_;
					} else if(st == DATA_STATE::high) {
						user_data_ <<= 1;
						user_data_ |= 1;
						--count_;
					}
					if(count_ == 0) {
						task_ = TASK::Term;
					}
				}
				break;

			case TASK::Term:
				if(lvl == 0) {
					++frame_count_;
					task_ = TASK::idle;
				}
				break;

			default:
				break;
			}
		}
	};
}
