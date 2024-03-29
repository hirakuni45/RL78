#pragma once
//=====================================================================//
/*!	@file
	@brief	I/O ユーティリティー
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2013, 2021 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#ifdef VSC_INTELL_SENSE
typedef char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef long int32_t;
typedef unsigned long uint32_t;
#endif
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
		static void write(value_type data) { wr8_(adr, data); }


		//-----------------------------------------------------------------//
		/*!
			@brief 読み出し
			@return 読み出し値
		*/
		//-----------------------------------------------------------------//
		static value_type read() { return rd8_(adr); }

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
		static value_type read() { return rd8_(adr); }

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
		static void write(value_type data) { wr16_(adr, data); }


		//-----------------------------------------------------------------//
		/*!
			@brief 読み出し
			@return 読み出し値
		*/
		//-----------------------------------------------------------------//
		static value_type read() { return rd16_(adr); }

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
		@brief  ビット位置定義
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	enum class bitpos : uint8_t {
		B0,		///< ビット０
		B1,		///< ビット１
		B2,		///< ビット２
		B3,		///< ビット３
		B4,		///< ビット４
		B5,		///< ビット５
		B6,		///< ビット６
		B7,		///< ビット７
		B8,		///< ビット８
		B9,		///< ビット９
		B10,	///< ビット１０
		B11,	///< ビット１１
		B12,	///< ビット１２
		B13,	///< ビット１３
		B14,	///< ビット１４
		B15,	///< ビット１５
	};


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  Read/Write ビット・アクセス・テンプレート
		@param[in]	T	アクセス・クラス
		@param[in]	pos	ビット位置
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class T, bitpos pos>
	struct bit_rw_t {
		static bool get() {
			return (T::read() >> static_cast<uint8_t>(pos)) & 1;
		}
		static void set(bool v) {
			if(v) {
				T::write(T::read() | (1 << static_cast<uint8_t>(pos)));
			} else {
				T::write(T::read() & ~(1 << static_cast<uint8_t>(pos)));
			}
		}

	    typename T::value_type b(bool v) const {
			return v << static_cast<uint8_t>(pos);
		}

		void operator = (bool v) { set(v); }
		bool operator () () { return get(); }
	};


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  Read/Write ビット・アクセス・テンプレート
		@param[in]	T	アクセス・クラス
		@param[in]	pos	初期ビット位置
		@param[in]	len	ビット幅
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class T, bitpos pos, uint8_t len>
	struct bits_rw_t {
		static typename T::value_type get() {
			return (T::read() >> static_cast<uint8_t>(pos)) & ((1 << len) - 1);
		}
		static void set(typename T::value_type v) {
			auto m = static_cast<typename T::value_type>(((1 << len) - 1) << static_cast<uint8_t>(pos));
			T::write((T::read() & ~m) | (static_cast<typename T::value_type>(v) << static_cast<uint8_t>(pos)));
		}

	    typename T::value_type b(typename T::value_type v) const {
			return (((1 << len) - 1) & v) << static_cast<uint8_t>(pos);
		}

		void operator = (typename T::value_type v) const { set(v); }
		typename T::value_type operator () () const { return get(); }
	};


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  Read ビット・アクセス・テンプレート
		@param[in]	T	アクセス・クラス
		@param[in]	pos	ビット位置
		@param[in]	len	ビット幅
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class T, bitpos pos>
	struct bit_ro_t {
		static bool get() {
			return (T::read() >> static_cast<uint8_t>(pos)) & 1;
		}

	    typename T::value_type b(typename T::value_type v) const {
			return 1 << static_cast<uint8_t>(pos);
		}

		bool operator () () const { return get(); }
	};


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  標準 Read/Write アクセス・テンプレート
		@param[in]	T	アクセステンプレート
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class T>
	struct basic_rw_t : public T {

		using T::operator =;
		using T::operator ();
		using T::operator |=;
		using T::operator &=;

		bit_rw_t<T, bitpos::B7> B7;	///< B7 アクセス
		bit_rw_t<T, bitpos::B6> B6;	///< B6 アクセス
		bit_rw_t<T, bitpos::B5> B5;	///< B5 アクセス
		bit_rw_t<T, bitpos::B4> B4;	///< B4 アクセス
		bit_rw_t<T, bitpos::B3> B3;	///< B3 アクセス
		bit_rw_t<T, bitpos::B2> B2;	///< B2 アクセス
		bit_rw_t<T, bitpos::B1> B1;	///< B1 アクセス
		bit_rw_t<T, bitpos::B0> B0;	///< B0 アクセス
	};
}
