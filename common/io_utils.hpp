#pragma once
//=====================================================================//
/*!	@file
	@brief	I/O ユーティリティー @n
			Copyright 2013,2016 Kunihito Hiramatsu
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include <cstdint>

namespace device {

	// RL78
	typedef uint32_t address_type;

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  ８ビット書き込み
		@param[in]	adr		書き込みアドレス
		@param[in]	data	書き込みデータ
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	static inline void wr8_(address_type adr, uint8_t data) {
		*reinterpret_cast<volatile uint8_t*>(adr) = data;
	}


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  ８ビット読み込み
		@param[in]	adr		読み込みアドレス
		@param[in]	data	読み込みデータ
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	static inline uint8_t rd8_(address_type adr) {
		return *reinterpret_cast<volatile uint8_t*>(adr);
	}


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  １６ビット書き込み
		@param[in]	adr		書き込みアドレス
		@param[in]	data	書き込みデータ
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	static inline void wr16_(address_type adr, uint16_t data) {
		*reinterpret_cast<volatile uint16_t*>(adr) = data;
	}


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  １６ビット読み込み
		@param[in]	adr		読み込みアドレス
		@param[in]	data	読み込みデータ
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	static inline uint16_t rd16_(address_type adr) {
		return *reinterpret_cast<volatile uint16_t*>(adr);
	}


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  ３２ビット書き込み
		@param[in]	adr		書き込みアドレス
		@param[in]	data	書き込みデータ
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	static inline void wr32_(address_type adr, uint32_t data) {
		*reinterpret_cast<volatile uint32_t*>(adr) = data;
	}


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  ３２ビット読み込み
		@param[in]	adr		読み込みアドレス
		@param[in]	data	読み込みデータ
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	static inline uint32_t rd32_(address_type adr) {
		return *reinterpret_cast<volatile uint32_t*>(adr);
	}


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  Read/Write 8 bits アクセス・テンプレート
		@param[in]	adr	アドレス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <address_type adr>
	struct rw8_t {
		typedef uint8_t value_type;

		//-----------------------------------------------------------------//
		/*!
			@brief  書き込み
			@param[in]	data	書き込み値
		*/
		//-----------------------------------------------------------------//
		static void write(uint8_t data) { wr8_(adr, data); }


		//-----------------------------------------------------------------//
		/*!
			@brief 読み出し
			@return 読み出し値
		*/
		//-----------------------------------------------------------------//
		static uint8_t read() { return rd8_(adr); }

		void operator = (value_type data) const { write(data); }
		value_type operator () () const { return read(); }
		void operator |= (value_type data) const { write(read() | data); }
		void operator &= (value_type data) const { write(read() & data); }
	};


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  Read Only 8 bits アクセス・テンプレート
		@param[in]	adr	アドレス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <address_type adr>
	struct ro8_t {
		typedef uint8_t value_type;

		//-----------------------------------------------------------------//
		/*!
			@brief 読み出し
			@return 読み出し値
		*/
		//-----------------------------------------------------------------//
		static uint8_t read() { return rd8_(adr); }

		value_type operator () () const { return read(); }
	};


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  Write Only 8 bits アクセス・テンプレート
		@param[in]	adr	アドレス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <address_type adr>
	struct wo8_t {
		typedef uint8_t value_type;

		//-----------------------------------------------------------------//
		/*!
			@brief  書き込み
			@param[in]	data	書き込み値
		*/
		//-----------------------------------------------------------------//
		static void write(uint8_t data) { wr8_(adr, data); }

		void operator = (value_type data) const { write(data); }
	};


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  Read/Write 16 bits アクセス・テンプレート
		@param[in]	adr	アドレス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <address_type adr>
	struct rw16_t {
		typedef uint16_t value_type;

		//-----------------------------------------------------------------//
		/*!
			@brief  書き込み
			@param[in]	data	書き込み値
		*/
		//-----------------------------------------------------------------//
		static void write(uint16_t data) { wr16_(adr, data); }


		//-----------------------------------------------------------------//
		/*!
			@brief 読み出し
			@return 読み出し値
		*/
		//-----------------------------------------------------------------//
		static uint16_t read() { return rd16_(adr); }

		void operator = (value_type data) const { write(data); }
		value_type operator () () const { return read(); }
		void operator |= (value_type data) const { write(read() | data); }
		void operator &= (value_type data) const { write(read() & data); }
	};


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  Read Only 16 bits アクセス・テンプレート
		@param[in]	adr	アドレス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <address_type adr>
	struct ro16_t {
		typedef uint16_t value_type;

		//-----------------------------------------------------------------//
		/*!
			@brief 読み出し
			@return 読み出し値
		*/
		//-----------------------------------------------------------------//
		static uint16_t read() { return rd16_(adr); }

		value_type operator () () const { return read(); }
	};


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  Write Only 16 bits アクセス・テンプレート
		@param[in]	adr	アドレス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <address_type adr>
	struct wo16_t {
		typedef uint16_t value_type;

		//-----------------------------------------------------------------//
		/*!
			@brief  書き込み
			@param[in]	data	書き込み値
		*/
		//-----------------------------------------------------------------//
		static void write(uint16_t data) { wr16_(adr, data); }

		void operator = (value_type data) const { write(data); }
	};


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  Read/Write ビット・アクセス・テンプレート
		@param[in]	T	アクセス・クラス
		@param[in]	pos	初期位置
		@param[in]	len	ビット幅
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class T, uint8_t pos, uint8_t len = 1>
	struct bit_rw_t {
		static typename T::value_type get() {
			return (typename T::read() >> pos) & ((1 << len) - 1);
		}
		static void set(typename T::value_type v) {
			auto m = static_cast<typename T::value_type>(((1 << len) - 1) << pos);
			T::write((T::read() & ~m) | (static_cast<typename T::value_type>(v) << pos));
		}

	    typename T::value_type b(typename T::value_type v) const {
			return (((1 << len) - 1) & v) << pos;
		}

		void operator = (typename T::value_type v) const { set(v); }
		typename T::value_type operator () () const { return get(); }
	};


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  Read ビット・アクセス・テンプレート
		@param[in]	T	アクセス・クラス
		@param[in]	pos	初期位置
		@param[in]	len	ビット幅
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class T, uint8_t pos, uint8_t len = 1>
	struct bit_ro_t {
		static typename T::value_type get() {
			return (typename T::read() >> pos) & ((1 << len) - 1);
		}

	    typename T::value_type b(typename T::value_type v) const {
			return (((1 << len) - 1) & v) << pos;
		}

		typename T::value_type operator () () const { return get(); }
	};

}
