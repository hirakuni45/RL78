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
		@brief  8 bits アクセス・テンプレート
		@param[in]	adr	アドレス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <address_type adr, uint8_t with_data = 0>
	struct io8 {
		typedef uint8_t value_type;

		//-----------------------------------------------------------------//
		/*!
			@brief  書き込み
			@param[in]	data	書き込み値
		*/
		//-----------------------------------------------------------------//
		static void write(uint8_t data) { wr8_(adr, data | with_data); }


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
		@brief  8 bits アクセス・テンプレート(RO)
		@param[in]	adr	アドレス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <address_type adr>
	struct io8_ro {
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
		@brief  8 bits アクセス・テンプレート(WO)
		@param[in]	adr	アドレス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <address_type adr>
	struct io8_wo {
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
		@brief  16 bits アクセス・テンプレート
		@param[in]	adr	アドレス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <address_type adr>
	struct io16 {
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
		@brief  16 bits アクセス・テンプレート(RO)
		@param[in]	adr	アドレス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <address_type adr>
	struct io16_ro {
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
		@brief  16 bits アクセス・テンプレート(WO)
		@param[in]	adr	アドレス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <address_type adr>
	struct io16_wo {
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
		@brief  32 bits アクセス・テンプレート
		@param[in]	adr	アドレス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <address_type adr>
	struct io32 {
		typedef uint32_t	value_type;

		//-----------------------------------------------------------------//
		/*!
			@brief  書き込み
			@param[in]	data	書き込み値
		*/
		//-----------------------------------------------------------------//
		static void write(uint32_t data) { wr32_(adr, data); }


		//-----------------------------------------------------------------//
		/*!
			@brief  書き込み
			@param[in]	data	書き込み値
		*/
		//-----------------------------------------------------------------//
		static uint32_t read() { return rd32_(adr); }

		void operator = (value_type data) const { write(data); }
		value_type operator () () const { return read(); }
		void operator |= (value_type data) const { write(read() | data); }
		void operator &= (value_type data) const { write(read() & data); }
	};


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  １ビット・アクセス・テンプレート(R/W direct)
		@param[in]	T	アクセス・クラス
		@param[in]	pos	ビット位置
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class T, uint8_t pos>
	struct bit {
		static bool get() {
			return (T::read() & (1 << pos)) != 0;
		}
		static void set(bool v) {
			T::write(v << pos);
		}

	    typename T::value_type b(bool f = true) const {
			return f << pos;
		}

		void operator = (bool v) const { set(v); }
		bool operator () () const { return get(); }
	};


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  １ビット・アクセス・テンプレート(R/W)
		@param[in]	T	アクセス・クラス
		@param[in]	pos	ビット位置
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class T, uint8_t pos>
	struct bit_t {
		static bool get() {
			return (T::read() & (1 << pos)) != 0;
		}
		static void set(bool v) {
			T::write((T::read() & ~(1 << pos)) | (v << pos));
		}

	    typename T::value_type b(bool v = true) const {
			return v << pos;
		}

		void operator = (bool v) const { set(v); }
		bool operator () () const { return get(); }
	};


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  ビット・アクセス・テンプレート(R/W direct)
		@param[in]	T	アクセス・クラス
		@param[in]	pos	初期位置
		@param[in]	len	ビット幅
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class T, uint8_t pos, uint8_t len>
	struct bits {
		static typename T::value_type get() {
			return (T::read() >> pos) & ((1 << len) - 1);
		}
		static void set(typename T::value_type v) {
			T::write(v << pos);
		}

	    typename T::value_type b(typename T::value_type v) const { return v << pos; }

		void operator = (typename T::value_type v) const { set(v); }
		typename T::value_type operator () () const { return get(); }
	};


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  ビット・アクセス・テンプレート(R/W)
		@param[in]	T	アクセス・クラス
		@param[in]	pos	初期位置
		@param[in]	len	ビット幅
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class T, uint8_t pos, uint8_t len, typename R = typename T::value_type>
	struct bits_t {
		static R get() {
			return static_cast<R>(T::read() >> pos) & ((1 << len) - 1);
		}
		static void set(R v) {
			typename T::value_type m = ((1 << static_cast<typename T::value_type>(len)) - 1) << pos;
			T::write((T::read() & ~m) | (static_cast<typename T::value_type>(v) << pos));
		}

	    R b(R v) const { return v << pos; }

		void operator = (R v) const { set(v); }
		R operator () () const { return get(); }
	};


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  １ビット・アクセス・テンプレート(RO)
		@param[in]	T	アクセス・クラス
		@param[in]	pos	ビット位置
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class T, uint8_t pos>
	struct bit_ro_t {
		static bool get() {
			return T::read() & (1 << pos);
		}

		bool operator () () const { return get(); }
	};


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  ビット・アクセス・テンプレート(RO)
		@param[in]	T	アクセス・クラス
		@param[in]	pos	初期位置
		@param[in]	len	ビット幅
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class T, uint8_t pos, uint8_t len>
	struct bits_ro_t {
		static typename T::value_type get() {
			return (T::read() >> pos) & ((1 << len) - 1);
		}
		typename T::value_type operator () () const { return get(); }
	};
}
