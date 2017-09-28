#pragma once
//=====================================================================//
/*!	@file
	@brief	TLV320ADC3001 クラス @n
			Low-Power Stereo ADC With Embedded miniDSP
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
		@brief  TLV320ADC3001 テンプレートクラス
		@param[in]	I2C_IO	I2C I/O クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class I2C_IO>
	class TLV320ADC3001 {
	public:

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  タイプ
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		enum class TYPE : uint8_t {
			FS44_1,		///< 44.1KHz
			FS48,       ///< 48.0KHz
		};

	private:
		I2C_IO&		i2c_io_;

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief	コンストラクター
			@param[in]	i2c	iica_io クラスを参照で渡す
		 */
		//-----------------------------------------------------------------//
		TLV320ADC3001(I2C_IO& i2c) : i2c_io_(i2c) { }


		//-----------------------------------------------------------------//
		/*!
			@brief	開始
			@param[in]	type	タイプ
			@return エラーなら「false」を返す
		 */
		//-----------------------------------------------------------------//
		bool start(TYPE type) {


			return true;
		}


	};
}
