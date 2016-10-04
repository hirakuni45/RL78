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
			BLOCK_BLANK_CHECH = 0x32,
			BAUD_RATE_SET = 0x9A,
			SILLICON_SIGNATURE = 0xc0,
			SECURITY_SET = 0xA0,
			SECURITY_GET = 0xA1,
			SECURITY_RELEASE = 0xA2,
			CHECK_SUM = 0xB0
		};

	public:

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief	ステータス・構造体
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		struct status_t {
			uint8_t	ST1;
			uint8_t	D01;
			uint8_t	D02;

			void info(const std::string& head = "") const {
				std::cout << head << (boost::format("Sync: %02X, Frq: %02X, Mode: %02X")
					% static_cast<uint32_t>(ST1)
					% static_cast<uint32_t>(D01)
					% static_cast<uint32_t>(D02)) << std::endl;
			}
		};


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief	area 構造体
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		struct area {
			uint32_t	org_ = 0;
			uint32_t	end_ = 0;

			void info(const std::string& head = "") const {
				std::cout << head << (boost::format("Area: %08X, %08X") % org_ % end_) << std::endl;
			}
		};
		typedef std::vector<area> areas;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief	rl78_t 構造体
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		struct rl78_t {
		};

	private:
		typedef utils::rs232c_io rs232c;
		rs232c	rs232c_;

		status_t	status_;

		static uint8_t checksum_(const void *src, uint8_t len)
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
			buf[len + 3] = checksum_(&buf[1], len + 2);
			buf[len + 4] = 0x03;  // ETX;
			if(rs232c_.send(buf, sizeof(buf)) != sizeof(buf)) {
				return false;
			}
			timeval tv;
			tv.tv_sec  = 0;
			tv.tv_usec = 100000;  // 100ms
			return rs232c_.recv(buf, sizeof(buf), tv) == sizeof(buf);
		}

		bool recv_status_(void* dst, uint32_t len) {
			uint8_t buf[len + 4];
			timeval tv;
			tv.tv_sec  = 0;
			tv.tv_usec = 100000;  // 100ms
			if(rs232c_.recv(buf, sizeof(buf)) != sizeof(buf)) {
				return false;
			}

			uint8_t sum = checksum_(&buf[1], len + 1);
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
			tv.tv_usec = 75000;  // 75ms
			int rd = rs232c_.recv(tv);
			if(rd != 0x3a) {
				std::cerr << boost::format("First byte recv error: %02X") % rd << std::endl;
				return false;
			}
			usleep(1000);
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
			if(!send_cmd_(CMD::RESET, nullptr, 0)) {
				std::cerr << "RESET send error" << std::endl;
				return false;
			}

			int len = 0;
			uint8_t state[1];
			if(!recv_status_(state, 1)) {
				std::cerr << "RESET recv error" << std::endl;
				return false;
			}

			if(state[1] != 0x06) {
				std::cerr << boost::format("RESET status error: %02X")
					% static_cast<uint32_t>(state[1]) << std::endl;
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
			uint8_t bc;
			switch(baud) {
			case B115200:
				bc = 0;
				break;
//			case B250000:
//				bc = 1;
//				break;
			case B500000:
				bc = 2;
				break;
			case B1000000:
				bc = 3;
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
			if(!recv_status_(state, 3)) {
				std::cerr << "BAUD_RATE_SET recv error" << std::endl;
				return false;
			}

			status_.ST1 = state[0];
			status_.D01 = state[1];
			status_.D02 = state[2];

			if(state[0] != 0x06) {
				std::cerr << boost::format("BAUD_RATE_SET status error: %02X")
					% static_cast<uint32_t>(state[1]) << std::endl;
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
			@brief	ステータスの取得
			@return ステータス
		*/
		//-----------------------------------------------------------------//
		const status_t& get_status() const { return status_; }


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
