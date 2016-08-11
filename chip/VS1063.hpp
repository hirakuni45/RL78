#pragma once
//=====================================================================//
/*!	@file
	@brief	VS1063 VLSI Audio Codec ドライバー
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include <cstdint>
#include "common/csi_io.hpp"

namespace chip {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  VS1063 テンプレートクラス
		@param[in]	CSI	CSI(SPI) 制御クラス
		@param[in]	SEL	/xCS 制御クラス
		@param[in]	DCS /xDCS 制御クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class CSI, class SEL, class DCS>
	class VS1063 {

		CSI&	csi_;

#if 0
		inline void write_(CMD cmd) {
			csi_.write(static_cast<uint8_t>(cmd));
		}

		inline void write_(CMD cmd, uint8_t ord) {
			csi_.write(static_cast<uint8_t>(cmd) | ord);
		}

		inline void chip_enable_(bool f = true) const {
			device::P0.B1 = !f;
		}

		inline void reg_select_(bool f) const {
			device::P0.B0 = f;
		}
#endif
	public:
		//-----------------------------------------------------------------//
		/*!
			@brief  コンストラクター
		*/
		//-----------------------------------------------------------------//
		VS1063(CSI& csi) : csi_(csi) { }


		//-----------------------------------------------------------------//
		/*!
			@brief  開始
		*/
		//-----------------------------------------------------------------//
		void start()
		{
		}
	};
}
