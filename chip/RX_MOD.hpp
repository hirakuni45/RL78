#pragma once
//=====================================================================//
/*!	@file
	@brief	RX_MOD ドライバー @n
			※「TX_MOD.hpp」とペア
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include <cstdint>

namespace chip {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  受信データ変調 テンプレートクラス
		@param[in]	PORT	受信ポートクラス
		@param[in]	HNUM	ヘッダー・フレーム数（標準４）
		@param[in]	SNUM	転送最大数
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class PORT, uint8_t HNUM = 4, uint8_t SNUM = 4>
	class RX_MOD {

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
		RX_MOD() noexcept : head_(0), sync_(0), len_(0), dat_(0), bit_pos_(0) { }


		//-----------------------------------------------------------------//
		/*!
			@brief	開始
		 */
		//-----------------------------------------------------------------//
		void start() noexcept
		{
			PORT::DIR = 0;  // input
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
			@brief	サービス @n
					正確なタイマー割り込みで起動されるタスク
		 */
		//-----------------------------------------------------------------//
		void service() noexcept
		{



		}
	};
}
