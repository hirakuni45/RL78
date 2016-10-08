#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78 プログラミング・プロトコル・クラス
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include "rs232c_io.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <boost/format.hpp>

namespace rl78 {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	RL78 プログラミング・プロトコル・クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	class protocol {

		enum class CMD : uint8_t {
			RESET = 0x00,
			BLOCK_ERASE = 0x22,
			PROGRAMMING = 0x40,
			VERIFY = 0x13,
			BLOCK_BLANK_CHECK = 0x32,
			BAUD_RATE_SET = 0x9A,
			SILICON_SIGNATURE = 0xc0,
			SECURITY_SET = 0xA0,
			SECURITY_GET = 0xA1,
			SECURITY_RELEASE = 0xA2,
			CHECKSUM = 0xB0,

			send_feed_ = 0xfe,

			none_ = 0xff
		};

		static uint32_t hex3_(const uint8_t* ptr) {
			uint32_t v;
			v  = static_cast<uint32_t>(ptr[2]) << 16;
			v |= static_cast<uint32_t>(ptr[1]) << 8;
			v |= static_cast<uint32_t>(ptr[0]);
			return v;
		}

		static void strnout_(const void* ptr, uint32_t len) {
			char str[len + 1];
			std::memcpy(str, ptr, len);
			str[len] = 0;
			std::cout << str;
		}

	public:

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief	ステータス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		enum class status : uint8_t {
			NONE = 0x00,		///< なし
			ACK = 0x06,			///< 正常終了
			PARAM = 0x05,		///< パラメーター・エラー
			CHECKSUM = 0x07,	///< チェック・サム・エラー
			VERIFY = 0x0F,		///< べりファイ・エラー
			PROTECT = 0x10,		///< プロテクト・エラー
			NACK = 0x15,		///< 否定応答
			BLANK = 0x1B,		///< ブランク・エラー
			W_VERIFY = 0x1B,	///< ライト時、べりファイ・エラー
			WRITE = 0x1C,		///< ライト・エラー
		};


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief	ステート構造体（CPUの状態）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		struct state_t {
			uint8_t	D01;	///< 動作周波数（MHz）
			uint8_t	D02;	///< プログラミング・モード

			state_t() : D01(0), D02(0) { }

			void info(const std::string& head = "") const {
				std::cout << head << (boost::format("Frq: %02X, Mode: %02X")
					% static_cast<uint32_t>(D01)
					% static_cast<uint32_t>(D02)) << std::endl;
			}
		};


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief	シグネチュア構造体
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		struct signature_t {
			uint8_t	DEC[3];
			uint8_t	DEV[10];
			uint8_t	CEN[3];
			uint8_t	DEN[3];
			uint8_t	VER[3];

			signature_t() {
				std::memset(DEC, 0, 3);
				std::memset(DEV, 0, 10);
				std::memset(CEN, 0, 3);
				std::memset(DEN, 0, 3);
				std::memset(VER, 0, 3);
			}

			void copy(const uint8_t* src) {
				std::memcpy(DEC, src, 3);  src += 3;
				std::memcpy(DEV, src, 10); src += 10;
				std::memcpy(CEN, src, 3);  src += 3;
				std::memcpy(DEN, src, 3);  src += 3;
				std::memcpy(VER, src, 3);
			}

			void info(const std::string& head = "") const {
				std::cout << head << boost::format("Device code: %06X") % hex3_(DEC) << std::endl;
				std::cout << head << "Device: ";
				strnout_(DEV, 10);
				std::cout << std::endl;
				std::cout << head << boost::format("Flash end: %06X") % hex3_(CEN) << std::endl;
				std::cout << head << boost::format("Data  end: %06X") % hex3_(DEN) << std::endl;
				std::cout << head << boost::format("Version:   %06X") % hex3_(VER) << std::endl;
			}
		};


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief	セキリティ構造体
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		struct security_t {
			uint8_t		FLG;
			uint8_t		BOT;
			uint16_t	SS;
			uint16_t	SE;

			security_t() : FLG(0), BOT(0), SS(0), SE(0) { }

			void copy(const uint8_t* src) {
				FLG = src[0];
				BOT = src[1];
				SS = src[2] | (static_cast<uint16_t>(src[3]) << 8);
				SE = src[4] | (static_cast<uint16_t>(src[5]) << 8);
			}

			void info(const std::string& head = "") const {
				std::cout << head << boost::format("Flag:  %02X") % static_cast<uint32_t>(FLG) << std::endl;
				std::cout << head << boost::format("Boot:  %02X") % static_cast<uint32_t>(BOT) << std::endl;
				std::cout << head << boost::format("Start: %04X") % static_cast<uint32_t>(SS) << std::endl;
				std::cout << head << boost::format("Final: %04X") % static_cast<uint32_t>(SE) << std::endl;
			}
		};

	private:
		typedef utils::rs232c_io rs232c;
		rs232c		rs232c_;

		uint32_t	baud_ = 0;
		status		status_ = status::NONE;
		bool		entry_program_ = false;
		bool		entry_verify_ = false;

		state_t		state_;
		bool		blank_ = false;
		uint16_t	checksum_ = 0;

		uint32_t	block_org_ = 0;
		uint32_t	block_end_ = 0;

		static uint8_t gen_checksum_(const void *src, uint32_t len)
		{
			uint8_t sum = 0;
			const uint8_t* p = static_cast<const uint8_t*>(src);
			for (; len; --len) {
				sum -= *p++;
			}
			return sum;
		}

		bool send_cmd_(CMD cmd, const void* src, uint32_t len) {
			uint8_t buf[len + 5];
			buf[0] = 0x01;  // SOH
			buf[1] = (len + 1) & 0xff;
			buf[2] = static_cast<uint8_t>(cmd);
			if(src != nullptr) std::memcpy(&buf[3], src, len);
			buf[len + 3] = gen_checksum_(&buf[1], len + 2);
			buf[len + 4] = 0x03;  // ETX;
			if(rs232c_.send(buf, sizeof(buf)) != sizeof(buf)) {
				return false;
			}
			timeval tv;
			tv.tv_sec  = 0;
			// (base: 100ms) + (((1 / baud) * 10) * 1.5) * n bytes
			tv.tv_usec = (sizeof(buf) * 1000000 * (10 + 5) / baud_) + 100000;
			return rs232c_.recv(buf, sizeof(buf), tv) == sizeof(buf);
		}


		bool send_data_(const void* src, uint32_t len, bool last) {
			uint8_t buf[len + 4];
			buf[0] = 0x02;  // STX
			buf[1] = len & 0xff;
			std::memcpy(&buf[2], src, len);
			buf[len + 2] = gen_checksum_(&buf[1], len + 1);
			buf[len + 3] = last ? 0x03 : 0x17;
			if(rs232c_.send(buf, sizeof(buf)) != sizeof(buf)) {
				return false;
			}
//			rs232c_.sync_send();
			timeval tv;
			tv.tv_sec  = 0;
			// (base: 100ms) + (((1 / baud) * 10) * 1.5) * n bytes
//			tv.tv_usec = (sizeof(buf) * 1000000 * (10 + 5) / baud_) + 500000;
			tv.tv_usec = 500000;
			return rs232c_.recv(buf, sizeof(buf), tv) == sizeof(buf);
		}


		bool recv_status_(CMD cmd, void* dst, uint32_t len) {
			uint8_t buf[len + 4];
			timeval tv;
			tv.tv_sec  = 0;
			tv.tv_usec = (sizeof(buf) * 1000000 * (10 + 5) / baud_) + 100000;
			switch(cmd) {
			case CMD::RESET:
				break;
			case CMD::BLOCK_ERASE:
				tv.tv_usec += 500000;
				break;
			case CMD::PROGRAMMING:
				tv.tv_usec += 10000;
				break;
			case CMD::send_feed_:
				tv.tv_usec += 500000;
				break;
			case CMD::VERIFY:
				tv.tv_usec += 50000;
				break;
			case CMD::BLOCK_BLANK_CHECK:
				tv.tv_usec += 10000;
				break;
			case CMD::BAUD_RATE_SET:
				tv.tv_usec += 10000;
				break;
			case CMD::SILICON_SIGNATURE:
				break;
			case CMD::SECURITY_SET:
			case CMD::SECURITY_GET:
			case CMD::SECURITY_RELEASE:
				break;
			case CMD::CHECKSUM:
				tv.tv_usec += 50000;
				break;
			default:
				break;
			}
			if(rs232c_.recv(buf, sizeof(buf), tv) != sizeof(buf)) {
				return false;
			}

			uint8_t sum = gen_checksum_(&buf[1], len + 1);
			if(buf[0] == 0x02 && buf[1] == len && buf[len + 2] == sum && buf[len + 3] == 0x03) ;
			else {
				return false;
			}

			std::memcpy(dst, &buf[2], len);
			return true;
		}

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief	開始
			@param[in]	path	シリアルデバイスパス
			@param[in]	brate	接続する速度
			@param[in]	voltage	動作電圧
			@return エラー無ければ「true」
		*/
		//-----------------------------------------------------------------//
		bool start(const std::string& path, speed_t brate, uint8_t voltage)
		{
			// 8 bits, 2 stop
			if(!rs232c_.open(path, brate, rs232c::char_len::bits8, rs232c::stop_len::two)) {
				std::cerr << boost::format("Can't open serial port: '%s'") % path << std::endl;
				return false;
			}

			// RESET to 0
			rs232c_.enable_RTS(false);
			// TOOL0 -> 0
			rs232c_.set_TXD(0);
			rs232c_.flush();
			usleep(1000);  // 1ms
			// RESET -> 1
			rs232c_.enable_RTS();
			usleep(10);  // 10us
			// TOOL0 -> 1
			rs232c_.set_TXD(1);
			usleep(1000);  // 1ms
			rs232c_.flush();

			if(!rs232c_.send(0x3a)) {
				std::cerr << "First byte send error..." << std::endl;
				return false;
			}
			timeval tv;
			tv.tv_sec  = 0;
			tv.tv_usec = 1000000;  // 100ms
			int rd = rs232c_.recv(tv);
			if(rd != 0x3a) {
				std::cerr << boost::format("First byte recv error: %02X") % rd << std::endl;
				return false;
			}
			usleep(1000);
			status_ = status::NONE;
			return baud_rate_set(brate, voltage);
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	リセット
			@return 成功なら「true」
		*/
		//-----------------------------------------------------------------//
		bool reset()
		{
			status_ = status::NONE;

			if(!send_cmd_(CMD::RESET, nullptr, 0)) {
				std::cerr << "RESET send error" << std::endl;
				return false;
			}

			uint8_t state[1];
			if(!recv_status_(CMD::RESET, state, 1)) {
				std::cerr << "RESET recv error" << std::endl;
				return false;
			}
			status_ = static_cast<status>(state[0]);

			if(status_ != status::ACK) {
				std::cerr << boost::format("RESET status error: %02X")
					% static_cast<uint32_t>(status_) << std::endl;
				return false;
			}

			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	ボーレート設定
			@param[in]	baud	ボーレート
			@param[in]	voltage	電圧
			@return 成功なら「true」
		*/
		//-----------------------------------------------------------------//
		bool baud_rate_set(speed_t baud, uint32_t voltage)
		{
			status_ = status::NONE;

			uint8_t bc;
			switch(baud) {
			case B115200:
				bc = 0;
				baud_ = 115200;
				break;
//			case B250000:
//				bc = 1;
//				baud_ = 250000;
//				break;
			case B500000:
				bc = 2;
				baud_ = 500000;
				break;
			case B1000000:
				bc = 3;
				baud_ = 1000000;
				break;
			default:
				return false;
			}
			uint8_t buf[2];
    		buf[0] = bc;
    		buf[1] = voltage;

    		if(!send_cmd_(CMD::BAUD_RATE_SET, buf, 2)) {
				std::cerr << "BAUD_RATE_SET send error" << std::endl;
				return false;
			}

			uint8_t state[3];
			if(!recv_status_(CMD::BAUD_RATE_SET, state, 3)) {
				std::cerr << "BAUD_RATE_SET recv error" << std::endl;
				return false;
			}
			status_ = static_cast<status>(state[0]);
			state_.D01 = state[1];
			state_.D02 = state[2];

			if(status_ != status::ACK) {
				std::cerr << boost::format("BAUD_RATE_SET status error: %02X")
					% static_cast<uint32_t>(status_) << std::endl;
				return false;
			}

			if(0) {
				std::cout << boost::format("Frequency: %u MHz")
					% static_cast<uint32_t>(state[1]) << std::endl;
				std::cout << boost::format("Mode: %s")
					% (state[2] == 0 ? "full-speed mode" : "wide-voltage mode") << std::endl;
			}

			if(baud == B115200) {
				return true;
			}

			return rs232c_.change_speed(baud);
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	ブロック消去（１Ｋバイト単位）
			@param[in]	org	先頭アドレス（下位１０ビットは無効）
			@return 成功なら「true」
		*/
		//-----------------------------------------------------------------//
		bool block_erase(uint32_t org)
		{
			status_ = status::NONE;

			uint8_t buf[3];
			buf[0] = org & 0xff;
			buf[1] = (org >> 8) & 0xff;
			buf[2] = (org >> 16) & 0xff;
			if(!send_cmd_(CMD::BLOCK_ERASE, buf, 3)) {
				std::cerr << "BLOCK_ERASE send error" << std::endl;
				return false;
			}

			uint8_t state[1];
			if(!recv_status_(CMD::BLOCK_ERASE, state, 1)) {
				std::cerr << "BLOCK_ERASE recv error" << std::endl;
				return false;
			}
			status_ = static_cast<status>(state[0]);

			if(status_ != status::ACK) {
				if(status_ == status::BLANK) {
					std::cerr << std::endl;
					std::cerr << boost::format("Erase fail at: %06X") % org << std::endl << std::flush;
				} else {
					std::cerr << boost::format("BLOCK_ERASE status error: %02X")
						% static_cast<uint32_t>(status_) << std::endl;
				}
				return false;
			}

			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	プログラミング
			@param[in]	org	開始アドレス
			@param[in]	end 終了アドレス
			@return 成功なら「true」
		*/
		//-----------------------------------------------------------------//
		bool programming(uint32_t org, uint32_t end)
		{
			if(entry_program_) return false;

			entry_program_ = false;

			status_ = status::NONE;

			uint8_t buf[6];
			buf[0] = org & 0xff;
			buf[1] = (org >> 8) & 0xff;
			buf[2] = (org >> 16) & 0xff;
			buf[3] = end & 0xff;
			buf[4] = (end >> 8) & 0xff;
			buf[5] = (end >> 16) & 0xff;
			if(!send_cmd_(CMD::PROGRAMMING, buf, 6)) {
				std::cerr << "PROGRAMMING send error" << std::endl;
				return false;
			}

			uint8_t state[1];
			if(!recv_status_(CMD::PROGRAMMING, state, 1)) {
				std::cerr << "PROGRAMMING recv error" << std::endl;
				return false;
			}
			status_ = static_cast<status>(state[0]);

			if(status_ != status::ACK) {
				std::cerr << boost::format("PROGRAMMING status error: %02X")
					% static_cast<uint32_t>(status_) << std::endl;
				return false;
			}

			entry_program_ = true;
			block_org_ = org;
			block_end_ = end;
/// std::cerr << boost::format("Adr: %06X, %06X") % org % end << std::endl << std::flush;

			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	プログラミング・データ転送
			@param[in]	src	ソースデータ
			@param[in]	len	全体の長さ（最大２５６）
			@param[in]	last	プログラム終了の場合「true」
			@return 成功なら「true」
		*/
		//-----------------------------------------------------------------//
		bool send_program_data(const void* src, uint32_t len, bool last)
		{
			if(!entry_program_) {
				std::cerr << "PROGRAMMING (data) start error" << std::endl;
				return false;
			}
			status_ = status::NONE;

// std::cerr << boost::format("Len: %d") % len << std::endl << std::flush;
// if(last) std::cerr << "Last..." << std::endl << std::flush;

			if(!send_data_(src, len, last)) {
				std::cerr << "PROGRAMMING (data) send error" << std::endl;
				entry_program_ = false;
				return false;
			}

			uint8_t ds[2];
			if(!recv_status_(CMD::send_feed_, ds, 2)) {
				std::cerr << "PROGRAMMING (data) recv2 error" << std::endl;
				entry_program_ = false;
				return false;
			}
			auto st1 = static_cast<status>(ds[0]);
			auto st2 = static_cast<status>(ds[1]);

			if(st1 != status::ACK || st2 != status::ACK) {
				if(st2 == status::WRITE) {
					std::cerr << std::endl;
					std::cerr << boost::format("Write fail at: %06X to %06X") % block_org_ % block_end_
						<< std::endl << std::flush;
				} else if(st2 == status::W_VERIFY) {
					std::cerr << std::endl;
					std::cerr << boost::format("Verify fail at: %06X to %06X") % block_org_ % block_end_
						<< std::endl << std::flush;
				} else {
					std::cerr << boost::format("PROGRAMMING (data) status error: %02X, %02X")
						% static_cast<uint32_t>(ds[0]) % static_cast<uint32_t>(ds[1]) << std::endl;
				}
				entry_program_ = false;
				return false;
			}

			if(!last) {
				block_org_ += len;
				return true;
			}

			uint8_t state[1];
			if(!recv_status_(CMD::PROGRAMMING, state, 1)) {
				std::cerr << "PROGRAMMING (fin) recv error" << std::endl;
				entry_program_ = false;
				return false;
			}
			status_ = static_cast<status>(state[0]);

			if(status_ != status::ACK) {
				std::cerr << boost::format("PROGRAMMING (fin) status error: %02X")
					% static_cast<uint32_t>(status_) << std::endl;
				entry_program_ = false;
				return false;
			}

			entry_program_ = false;
			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	ベリファイ（比較）
			@param[in]	org	開始アドレス
			@param[in]	end 終了アドレス
			@return 成功なら「true」
		*/
		//-----------------------------------------------------------------//
		bool verify(uint32_t org, uint32_t end)
		{
			if(entry_verify_) return false;

			entry_verify_ = false;

			status_ = status::NONE;

			uint8_t buf[6];
			buf[0] = org & 0xff;
			buf[1] = (org >> 8) & 0xff;
			buf[2] = (org >> 16) & 0xff;
			buf[3] = end & 0xff;
			buf[4] = (end >> 8) & 0xff;
			buf[5] = (end >> 16) & 0xff;
			if(!send_cmd_(CMD::VERIFY, buf, 6)) {
				std::cerr << "VERIFY send error" << std::endl;
				return false;
			}

			uint8_t state[1];
			if(!recv_status_(CMD::VERIFY, state, 1)) {
				std::cerr << "VERIFY recv error" << std::endl;
				return false;
			}
			status_ = static_cast<status>(state[0]);

			if(status_ != status::ACK) {
				std::cerr << boost::format("VERIFY status error: %02X")
					% static_cast<uint32_t>(status_) << std::endl;
				return false;
			}

			entry_verify_ = true;
			block_org_ = org;
			block_end_ = end;
/// std::cerr << boost::format("Adr: %06X, %06X") % org % end << std::endl << std::flush;

			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	ベリファイ・データ転送
			@param[in]	src	ソースデータ
			@param[in]	len	全体の長さ（最大２５６）
			@param[in]	last	ベリファイ終了の場合「true」
			@return 成功なら「true」
		*/
		//-----------------------------------------------------------------//
		bool send_verify_data(const void* src, uint32_t len, bool last)
		{
			if(!entry_verify_) {
				std::cerr << "VERIFY (data) start error" << std::endl;
				return false;
			}

			status_ = status::NONE;

// std::cerr << boost::format("Len: %d") % len << std::endl << std::flush;
// if(last) std::cerr << "Last..." << std::endl << std::flush;

			if(!send_data_(src, len, last)) {
				std::cerr << "VERIFY (data) send error" << std::endl;
				entry_verify_ = false;
				return false;
			}

			uint8_t ds[2];
			if(!recv_status_(CMD::send_feed_, ds, 2)) {
				std::cerr << "VERIFY (data) recv2 error" << std::endl;
				entry_verify_ = false;
				return false;
			}
			auto st1 = static_cast<status>(ds[0]);
			auto st2 = static_cast<status>(ds[1]);

			if(st1 != status::ACK || st2 != status::ACK) {
				if(st2 == status::VERIFY) {
					std::cerr << std::endl;
					std::cerr << boost::format("Verify fail: %06X to %06X") % block_org_ % block_end_
						<< std::endl << std::flush;
				} else {
					std::cerr << boost::format("VERIFY (data) status error: %02X, %02X")
						% static_cast<uint32_t>(ds[0]) % static_cast<uint32_t>(ds[1]) << std::endl;
				}
				entry_verify_ = false;
				return false;
			}

			if(last) {
				entry_verify_ = false;
			} else {
				block_org_ += len;
			}
			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	ブロック消去チェック
			@param[in]	org	開始アドレス
			@param[in]	end	終了アドレス
			@param[in]	area	エリア（０、１）
			@return 成功なら「true」
		*/
		//-----------------------------------------------------------------//
		bool block_blank_check(uint32_t org, uint32_t end, uint8_t area)
		{
			status_ = status::NONE;
			blank_ = false;

			uint8_t buf[7];
			buf[0] = org & 0xff;
			buf[1] = (org >> 8) & 0xff;
			buf[2] = (org >> 16) & 0xff;
			buf[3] = end & 0xff;
			buf[4] = (end >> 8) & 0xff;
			buf[5] = (end >> 16) & 0xff;
			buf[6] = area;
			if(!send_cmd_(CMD::BLOCK_BLANK_CHECK, buf, 7)) {
				std::cerr << "BLOCK_BLANK_CHECH send error" << std::endl;
				return false;
			}

			uint8_t state[1];
			if(!recv_status_(CMD::BLOCK_BLANK_CHECK, state, 1)) {
				std::cerr << "BLOCK_BLANK_CHECH recv error" << std::endl;
				return false;
			}
			status_ = static_cast<status>(state[0]);

			if(status_ == status::BLANK) {
				return true;
			}
			if(status_ != status::ACK) {
				std::cerr << boost::format("BLOCK_BLANK_CHECH status error: %02X")
					% static_cast<uint32_t>(status_) << std::endl;
				return false;
			}
			blank_ = true;
			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	シリコン・シグネチュア
			@param[out]	dst	所得先
			@return 成功なら「true」
		*/
		//-----------------------------------------------------------------//
		bool silicon_signature(signature_t& dst)
		{
			status_ = status::NONE;

			if(!send_cmd_(CMD::SILICON_SIGNATURE, nullptr, 0)) {
				std::cerr << "SILICON_SIGNATURE send error" << std::endl;
				return false;
			}

			uint8_t state[1];
			if(!recv_status_(CMD::SILICON_SIGNATURE, state, 1)) {
				std::cerr << "SILICON_SIGNATURE recv error" << std::endl;
				return false;
			}
			status_ = static_cast<status>(state[0]);

			if(status_ != status::ACK) {
				std::cerr << boost::format("SILICON_SIGNATURE status error: %02X")
					% static_cast<uint32_t>(status_) << std::endl;
				return false;
			}

			uint8_t data[3+10+3+3+3];
			if(!recv_status_(CMD::none_, data, sizeof(data))) {
				std::cerr << "SILICON_SIGNATURE data error" << std::endl;
				return false;
			}

			dst.copy(data);

			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	チェック・サム
			@return 成功なら「true」
		*/
		//-----------------------------------------------------------------//
		bool checksum(uint32_t org, uint32_t end)
		{
			status_ = status::NONE;
			checksum_ = 0;

			uint8_t buf[6];
			buf[0] = org & 0xff;
			buf[1] = (org >> 8) & 0xff;
			buf[2] = (org >> 16) & 0xff;
			buf[3] = end & 0xff;
			buf[4] = (end >> 8) & 0xff;
			buf[5] = (end >> 16) & 0xff;
			if(!send_cmd_(CMD::CHECKSUM, buf, 6)) {
				std::cerr << "CHECHSUM send error" << std::endl;
				return false;
			}

			uint8_t state[1];
			if(!recv_status_(CMD::CHECKSUM, state, 1)) {
				std::cerr << "CHECKSUM recv error" << std::endl;
				return false;
			}
			status_ = static_cast<status>(state[0]);

			if(status_ != status::ACK) {
				std::cerr << boost::format("CHECKSUM status error: %02X")
					% static_cast<uint32_t>(status_) << std::endl;
				return false;
			}

			uint8_t data[2];
			if(!recv_status_(CMD::CHECKSUM, data, sizeof(data))) {
				std::cerr << "CHECKSUM frame error" << std::endl;
				return false;
			}

			checksum_ = data[0];
			checksum_ |= static_cast<uint16_t>(data[1]) << 8;

			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	セキリティー・セット
			@param[in]	sec	セキリティー設定データ
			@return 成功なら「true」
		*/
		//-----------------------------------------------------------------//
		bool security_set(const security_t& sec)
		{
			status_ = status::NONE;

			if(!send_cmd_(CMD::SECURITY_SET, nullptr, 0)) {
				std::cerr << "SECURITY_SET send error" << std::endl;
				return false;
			}

			uint8_t state[1];
			if(!recv_status_(CMD::SECURITY_SET, state, 1)) {
				std::cerr << "SECURITY_SET recv error" << std::endl;
				return false;
			}
			status_ = static_cast<status>(state[0]);

			if(status_ != status::ACK) {
				std::cerr << boost::format("SECURITY_SET status error: %02X")
					% static_cast<uint32_t>(status_) << std::endl;
				return false;
			}

			uint8_t data[8];
			data[0] = sec.FLG;
			data[1] = sec.BOT;
			data[2] = sec.SS & 0xff;
			data[3] = sec.SS >> 8;
			data[4] = sec.SE & 0xff;
			data[5] = sec.SE >> 8;
			data[6] = 0;
			data[7] = 0;
			if(!send_data_(data, sizeof(data), true)) {
				std::cerr << "SECURITY_SET data send error" << std::endl;
				return false;
			}

			if(!recv_status_(CMD::SECURITY_SET, state, 1)) {
				std::cerr << "SECURITY_SET data recv error" << std::endl;
				return false;
			}
			status_ = static_cast<status>(state[0]);

			if(status_ != status::ACK) {
				std::cerr << boost::format("SECURITY_SET data status error: %02X")
					% static_cast<uint32_t>(status_) << std::endl;
				return false;
			}

			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	セキリティー・ゲット
			@param[out]	dst	取得先
			@return 成功なら「true」
		*/
		//-----------------------------------------------------------------//
		bool security_get(security_t& dst)
		{
			status_ = status::NONE;

			if(!send_cmd_(CMD::SECURITY_GET, nullptr, 0)) {
				std::cerr << "SECURITY_GET send error" << std::endl;
				return false;
			}

			uint8_t state[1];
			if(!recv_status_(CMD::SECURITY_GET, state, 1)) {
				std::cerr << "SECURITY_GET recv error" << std::endl;
				return false;
			}
			status_ = static_cast<status>(state[0]);

			if(status_ != status::ACK) {
				std::cerr << boost::format("SECURITY_GET status error: %02X")
					% static_cast<uint32_t>(status_) << std::endl;
				return false;
			}

			uint8_t data[1 + 1 + 2 + 2 + 2];
			if(!recv_status_(CMD::none_, data, sizeof(data))) {
				std::cerr << "SECURITY_GET data error" << std::endl;
				return false;
			}

			dst.copy(data);

			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	セキリティー・リリース
			@return 成功なら「true」
		*/
		//-----------------------------------------------------------------//
		bool security_release()
		{
			status_ = status::NONE;

			if(!send_cmd_(CMD::SECURITY_RELEASE, nullptr, 0)) {
				std::cerr << "SECURITY_RELEASE send error" << std::endl;
				return false;
			}

			uint8_t state[1];
			if(!recv_status_(CMD::SECURITY_RELEASE, state, 1)) {
				std::cerr << "SECURITY_RELEASE recv error" << std::endl;
				return false;
			}
			status_ = static_cast<status>(state[0]);

			if(status_ != status::ACK) {
				std::cerr << boost::format("SECURITY_RELEASE status error: %02X")
					% static_cast<uint32_t>(status_) << std::endl;
				return false;
			}

			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	ステータスの取得
			@return ステータス
		*/
		//-----------------------------------------------------------------//
		status get_status() const { return status_; }


		//-----------------------------------------------------------------//
		/*!
			@brief	ステートの取得
			@return ステート
		*/
		//-----------------------------------------------------------------//
		const state_t& get_state() const { return state_; }


		//-----------------------------------------------------------------//
		/*!
			@brief	ブランク結果の取得
			@return 「true」なら消去完了
		*/
		//-----------------------------------------------------------------//
		bool get_blank() const { return blank_; }


		//-----------------------------------------------------------------//
		/*!
			@brief	チェック・サムの取得
			@return チェック・サム
		*/
		//-----------------------------------------------------------------//
		uint16_t get_checksum() const { return checksum_; }


		//-----------------------------------------------------------------//
		/*!
			@brief	終了
		*/
		//-----------------------------------------------------------------//
		void end()
		{
			rs232c_.close();
		}
	};
}
