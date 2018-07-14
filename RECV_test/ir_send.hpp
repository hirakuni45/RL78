#pragma once
//=====================================================================//
/*!	@file
	@brief	赤外線リモコン送信クラス @n
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
		@brief  赤外線リモコン送信クラス
		@param[in]	OUTPUT	出力ファンクタ
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class OUTPUT>
	class ir_send {

		enum class TASK : uint8_t {
			idle,
			LeaderH,	// リーダーＨ出力
			LeaderL,	// リーダーＬ出力
			out0_ph0,	// 出力０、フェーズ０
			out0_ph1,	// 出力０、フェーズ１
			out1_ph0,	// 出力１、フェーズ０
			out1_ph1,	// 出力１、フェーズ１
			out1_ph2,	// 出力１、フェーズ２
			out1_ph3,	// 出力１、フェーズ３
		};

		enum class MODE : uint8_t {
			none,
			custom_l,	// カスタム L
			custom_h,	// カスタム H
			user,		// ユーザーデータ
		};

		OUTPUT		out_;

		TASK		task_;
		MODE		mode_;
		uint8_t		data_;
		uint8_t		bit_mask_;

		uint8_t		count_;
		uint16_t	custom_;
		uint8_t		user_;


		void next_bit_()
		{
			if(bit_mask_ == 0) {
				change_mode_();
			}
			if(bit_mask_ > 0) {
				if(data_ & bit_mask_) {
					task_ = TASK::out1_ph0;
				} else {
					task_ = TASK::out0_ph0;
				}
				bit_mask_ >>= 1;
			}
		}


		void change_mode_()
		{
			switch(mode_) {
			case MODE::none:
				task_ = TASK::idle;
				break;
			case MODE::custom_l:
				data_ = custom_ >> 8;
				bit_mask_ = 0x80;
				next_bit_();
				mode_ = MODE::custom_h;
				break;
			case MODE::custom_h:
				data_ = custom_;
				bit_mask_ = 0x80;
				mode_ = MODE::user;
				break;
			case MODE::user:
				data_ = user_;
				bit_mask_ = 0x80;
				mode_ = MODE::none;
				break;
			default:
				break;
			}
		}

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief  コンストラクタ
		*/
		//-----------------------------------------------------------------//
		ir_send() : out_(), task_(TASK::idle), mode_(MODE::none),
			data_(0), bit_mask_(0), count_(0),
			custom_(0xA153), user_(0) { }


		//-----------------------------------------------------------------//
		/*!
			@brief  ユーザー・データ出力
			@param[in]	user	ユーザー・データ
		*/
		//-----------------------------------------------------------------//
		void send_data(uint8_t user)
		{
			user_ = user;
			count_ = 16;
			task_ = TASK::LeaderH;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  サービス
		*/
		//-----------------------------------------------------------------//
		void service()
		{
			switch(task_) {
			case TASK::idle:
				break;

			case TASK::LeaderH:
				out_(1);
				--count_;
				if(count_ == 0) {
					count_ = 8;
					task_ = TASK::LeaderL;
				}
				break;
			case TASK::LeaderL:
				out_(0);
				--count_;
				if(count_ == 0) {
					mode_ = MODE::custom_l;
					change_mode_();
				}
				break;

			case TASK::out0_ph0:
				out_(1);
				task_ = TASK::out0_ph1;
				break;
			case TASK::out0_ph1:
				out_(0);
				next_bit_();
				break;

			case TASK::out1_ph0:
				out_(1);
				task_ = TASK::out1_ph1;
				break;
			case TASK::out1_ph1:
				out_(0);
				task_ = TASK::out1_ph2;
				break;
			case TASK::out1_ph2:
				out_(0);
				task_ = TASK::out1_ph3;
				break;
			case TASK::out1_ph3:
				out_(0);
				next_bit_();
				break;

			default:
				break;
			}		
		}
	};
}
