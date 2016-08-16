#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/G13 グループ 12 ビット・インターバルタイマー制御 @n
			Copyright 2016 Kunihito Hiramatsu
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include "G13/timer.hpp"
#include "G13/system.hpp"
#include "G13/intr.hpp"
#include "common/task.hpp"

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  １２ビット・インターバル・タイマー制御クラス @n
				※シングルトン @n
				※内部低速オシレーターを使ったタイマー　@n
				※設定周波数に大きな制限がある。
		@param[in]	T	内部カウンターの「型」
		@param[in]	TASK	割り込みタスク
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <typename T, class TASK = utils::null_task>
	class itimer {

		static volatile T counter_;
		static TASK task_;

		uint8_t	intr_level_ = 0;

		inline void sleep_() const { asm("nop"); }

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief  インターバル・タイマー割り込みタスク
		*/
		//-----------------------------------------------------------------//
		static __attribute__ ((interrupt)) void task() __attribute__ ((section (".lowtext")))
		{
			++counter_;
			task_();
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  インターバル・タイマーを動作させる
			@param[in]	freq	インターバル周波数
			@param[in]	level	割り込みレベル（１～２）、０の場合はポーリング
			@return エラーなら「false」
		*/
		//-----------------------------------------------------------------//
		bool start(uint16_t freq, uint8_t level = 0)
		{
			intr_level_ = level;

			// 低速発振器は１５ＫＨｚ
			uint16_t v = 15000 / freq;
			if(v > 4096 || v < 2) {
				return false;
			}

			PER0.RTCEN = 1;

			OSMC.WUTMMCK0 = 1;
			ITMC = (v - 1) | 0x8000;

			counter_ = 0;

			// 割り込みレベル設定とマスク解除
			if(level > 0) {
				--level;
				level ^= 0x03;
				intr::PR01H.ITPR = (level) & 1;
				intr::PR11H.ITPR = (level & 2) >> 1;
				intr::MK1H.ITMK = 0;
			}

			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  同期回数を取得
			@return 同期回数
		*/
		//-----------------------------------------------------------------//
		T get_counter() const { return counter_; }


		//-----------------------------------------------------------------//
		/*!
			@brief  インターバル・タイマーとの同期
		*/
		//-----------------------------------------------------------------//
		void sync() const {
			if(intr_level_) {
				volatile T counter = counter_;
				while(counter == counter_) sleep_();
			} else {
				while(intr::IF1H.ITIF() == 0) sleep_();
				intr::IF1H.ITIF = 0;
				++counter_;
			}
		}
	};

	template<typename T, class TASK>
		volatile T itimer<T, TASK>::counter_ = 0;

	template<typename T, class TASK>
		TASK itimer<T, TASK>::task_;
}
