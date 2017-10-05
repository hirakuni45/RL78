#pragma once
//=====================================================================//
/*!	@file
	@brief	シリアル制御クラス（デジタル・マイク）
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include <cstdint>
#include "common/renesas.hpp"
#include "sw.hpp"

namespace dmic {

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
    /*!
        @brief  シリアル制御クラス
		@param[in]	UART	UARTクラス
		@param[in]	SW5		5 bits SW クラス
		@param[in]	SW2		2 bits SW クラス
    */
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class UART, class SW5, class SW2>
	class serial {

		UART&	uart_;
		SW5&	sw5_;
		SW2&	sw2_;

		uint8_t	volume_;

	public:
        //-----------------------------------------------------------------//
        /*!
            @brief  コンストラクター
        */
        //-----------------------------------------------------------------//
		serial(UART& uart, SW5& sw5, SW2& sw2) noexcept : uart_(uart), sw5_(sw5), sw2_(sw2),
			volume_(0) { }


        //-----------------------------------------------------------------//
        /*!
            @brief  開始
        */
        //-----------------------------------------------------------------//
		void start() noexcept
		{
			uint8_t intr_level = 1;
			uart_.start(19200, intr_level);

			sw2_.start();
			device::PMC12 = 0b11111110;  // setup P12_0: digital port
			sw5_.start();
		}


        //-----------------------------------------------------------------//
        /*!
            @brief  ボリューム値の取得
			@return ボリューム値
        */
        //-----------------------------------------------------------------//
		uint16_t get_volume() const noexcept { return static_cast<uint16_t>(volume_); }


        //-----------------------------------------------------------------//
        /*!
            @brief  サービス
			@param[in]	volp	ボリューム＋
			@param[in]	volm	ボリューム－
        */
        //-----------------------------------------------------------------//
		void service(bool volp, bool volm) noexcept
		{
			if(volp) {
				if(volume_ < 8) {
					++volume_;
				}
			}

			if(volm) {
				if(volume_ > 0) {
					--volume_;
				}
			}

			if(uart_.recv_length() > 0) {
				char ch = uart_.getch();

				if(ch == 'C') {  // CH-SW (0 to 31)
					auto n = sw5_.get();
					uart_.putch((n / 10) + '0');
					uart_.putch((n % 10) + '0');
					uart_.putch('\n');
				} else if(ch == 'M') {  // MIC-SW (0 to 3)
					auto n = sw2_.get();
					uart_.putch((n % 10) + '0');
					uart_.putch('\n');
				} else if(ch == 'F') {  // 混信フラグ (0 to 1)
					uart_.putch('0');
					uart_.putch('\n');
				} else if(ch == 'V') {  // ボリューム値
					uart_.putch('0' + volume_);
					uart_.putch('\n');
				}
			}
		}
	};
}
