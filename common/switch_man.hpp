#pragma once
//=====================================================================//
/*!	@file
	@brief	スイッチ・マネージャー・テンプレート
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include <cstdint>

namespace utils {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	スイッチ・マネージャー・クラス
		@param[in]	BITS	ビット定義型（uint8_t, uint16_t, uint32_t)
		@param[in]	NAME	ビット位置定義型（enum class）
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <typename BITS, class NAME>
	class switch_man {

		BITS	level_;
		BITS	positive_;
		BITS	negative_;

		static bool get_bit_(BITS v, NAME pos) noexcept
		{
			return (v & (1 << static_cast<BITS>(pos))) != 0; 
		}

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief	コンストラクタ
		*/
		//-----------------------------------------------------------------//
		switch_man() noexcept : level_(0), positive_(), negative_() { }


		//-----------------------------------------------------------------//
		/*!
			@brief	レベルを取得
			@return レベル
		*/
		//-----------------------------------------------------------------//
		const BITS& get_level() const noexcept { return level_; }


		//-----------------------------------------------------------------//
		/*!
			@brief	レベルを取得
			@param[in]	pos	ビット位置
			@return 値
		*/
		//-----------------------------------------------------------------//
		bool get_level(NAME pos) const noexcept { return get_bit_(level_, pos); }


		//-----------------------------------------------------------------//
		/*!
			@brief	立ち上がりを取得（押した瞬間）
			@return 立ち上がり
		*/
		//-----------------------------------------------------------------//
		const BITS& get_positive() const noexcept { return positive_; }


		//-----------------------------------------------------------------//
		/*!
			@brief	立ち上がりを取得（押した瞬間）
			@param[in]	pos	ビット位置
			@return 立ち上がり
		*/
		//-----------------------------------------------------------------//
		bool get_positive(NAME pos) const noexcept { return get_bit_(positive_, pos); }


		//-----------------------------------------------------------------//
		/*!
			@brief	立ち下がりを取得（離した瞬間）
			@return 立ち下がり
		*/
		//-----------------------------------------------------------------//
		const BITS& get_negative() const noexcept { return negative_; }


		//-----------------------------------------------------------------//
		/*!
			@brief	立ち下がりを取得（離した瞬間）
			@param[in]	pos	ビット位置
			@return 立ち下がり
		*/
		//-----------------------------------------------------------------//
		bool get_negative(NAME pos) const noexcept { return get_bit_(negative_, pos); }


		//-----------------------------------------------------------------//
		/*!
			@brief	変化の取得
			@param[in]	pos	ビット位置
			@return 変化した場合「true」
		*/
		//-----------------------------------------------------------------//
		bool get_turn(NAME pos) const noexcept { return get_bit_(positive_ | negative_, pos); }


		//-----------------------------------------------------------------//
		/*!
			@brief	サービス
			@param[in]	level	スイッチ・レベル
		*/
		//-----------------------------------------------------------------//
		void service(BITS level) noexcept
		{
			positive_ = level & ~level_;
			negative_ =~level &  level_;
			level_ = level;
		}
	};
}
