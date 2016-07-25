#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/G13 グループ IICA 制御 @n
			Copyright 2016 Kunihito Hiramatsu
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include "G13/system.hpp"
#include "G13/iica.hpp"
#include "common/delay.hpp"

/// F_CLK はボーレートパラメーター計算で必要、設定が無いとエラーにします。
#ifndef F_CLK
#  error "uart_io.hpp requires F_CLK to be defined"
#endif

extern "C" {
	void sci_puts(const char* str);
}

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  IICA 制御クラス
		@param[in]	IICA	IICA クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class IICA>
	class iica_io {
	public:

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  I2C の速度タイプ
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		enum class speed : uint8_t {
			standard,	///< 100K b.p.s. (Standard mode)
			fast,		///< 400K b.p.s. (Fast mode)
			fast_plus,	///< 1M b.p.s. (Fast plus mode)
		};


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  I2C エラー・タイプ
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		enum class error : uint8_t {
			none,		///< エラー無し
			start,		///< スタート
			address,	///< アドレス転送
			data,		///< データ転送
		};

	private:
		static IICA iica_;

		uint16_t	speed_;

		uint8_t		error_;

		inline void sleep_()
		{
			asm("nop");
		} 

		// 「アクノリッジ」を検出したら「true」
		//  予定サイクル以内に検出しなかったら「false」
		bool probe_ack_()
		{
			auto n = speed_;
			while(iica_.IICS.ACKD() == 0) {
				if(n == 0) return false;
				--n;
			}
			return true;
		}

		// バスの開放を同期する。
		bool sync_open_(uint16_t cnt) {
			while(iica_.IICF.IICBSY() != 0) {
				if(cnt == 0) {
					sci_puts("I2C bus open sync error...\n");
					return false;
				}
				--cnt;
			}
			return true;
		}

	public:

		//-----------------------------------------------------------------//
		/*!
			@brief  割り込みタスク
		*/
		//-----------------------------------------------------------------//
		static __attribute__ ((interrupt)) void task() __attribute__ ((section (".lowtext")))
		{
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	動作開始
			@param[in]	spd_type	速度タイプ（メインクロックが 32MHz）
			@param[in]	intr_lvl	割り込みレベル（１、２）０の場合ポーリング
			@return 速度範囲エラーの場合「false」	
		 */
		//-----------------------------------------------------------------//
		bool start(speed spd_type, uint8_t intr_lvl)
		{
//			error_ = error::none;

			// ハードウェアーマニュアル１３．４．２により計算
			// tF: 立下り時間
			// tR: 立ち上がり時間
			// ※ 32MHz としての計算値
			uint8_t wl;
			uint8_t wh;
			uint8_t smc = 0x00;
			switch(spd_type) {
			// 100K b.p.s. 時 tF: 0.5us、tR: 0.5us
			case speed::standard:
				wl = 151;
				wh = 138;
				speed_ = 2560 * 2;
				break;

			// 400K b.p.s. 時 tF: 0.2us、tR: 0.2us
			case speed::fast:
				wl = 42;
				wh = 26;
				speed_ = 640 * 2;
				// ※デジタル・フィルター有効
				smc = iica_.IICCTL1.SMC.b(1) | iica_.IICCTL1.DFC.b(1);
				break;

			//   1M b.p.s. 時 tF: 0.1us、tR: 0.1us
			case speed::fast_plus:
				wl = 16;
				wh = 10;
				speed_ = 256 * 2;
				// ※デジタル・フィルター有効
				smc = iica_.IICCTL1.SMC.b(1) | iica_.IICCTL1.DFC.b(1);
				break;
			default:
				// error_ = error::start;
				return false;
			}

			// ユニットを有効にする
			if(iica_.get_unit_no() == 0) {
				PER0.IICA0EN = 1;
			} else {
				PER0.IICA1EN = 1;
			}

			// IICE(0) の時に設定
			iica_.IICCTL1 = iica_.IICCTL1.PRS.b(1) | smc;

			// 転送レート設定、IICE(0) の時に設定
			iica_.IICWL = wl / 2;
			iica_.IICWH = wh / 2;

			// iica_.IICCTL0.SPIE.b(1);  // 割り込み許可
			iica_.IICCTL0 = iica_.IICCTL0.IICE.b(1);  // 許可

			// ポート・モード設定、IICE(1) の時に設定
			if(iica_.get_unit_no() == 0) {
				PM6 &= 0b1111'1100;
				P6  &= 0b1111'1100;
			} else {
				PM6 &= 0b1111'0011;
				P6  &= 0b1111'0011;
			}

			// 「stop」コンディションを発行。
			iica_.IICCTL0.SPT = 1;

			// バスが開放されるまで待つ
			return sync_open_(speed_ / 8);
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	最終エラーの取得
			@return エラー・タイプ
		 */
		//-----------------------------------------------------------------//
		uint8_t get_last_error() const { return error_; }


		//-----------------------------------------------------------------//
		/*!
			@brief	受信
			@param[in]	adr	７ビットアドレス
			@param[out]	dst	転送先
			@param[in]	len	受信バイト数
			@return 受信が完了した場合「true」
		 */
		//-----------------------------------------------------------------//
		bool recv(uint8_t adr, uint8_t* dst, uint8_t len)
		{
			// error_ = error::none;

			iica_.IICCTL0.STT = 1;  // スタート・コンディション（開始）

			iica_.IICA = (adr << 1) | 0x01;  // アドレス転送と確認
			if(!probe_ack_()) {
				iica_.IICCTL0.SPT = 1;
				// error_ = error::address;
				return false;
			}

			// 受信データ転送
			iica_.IICCTL0.ACKE = 1;
			for(uint8_t i = 0; i < len; ++i) {
				if(!probe_ack_()) {
					iica_.IICCTL0.SPT = 1;
					// error_ = error::data;
					return false;
				}
				
				*dst = iica_.IICA();
				++dst;
			}
			// ストップ・コンディション
			iica_.IICCTL0.SPT = 1;

			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	送信
			@param[in]	adr	７ビットアドレス
			@param[in]	src	転送先
			@param[in]	len	受信バイト数
			@return 送信が完了した場合「true」
		 */
		//-----------------------------------------------------------------//
		bool send(uint8_t adr, const uint8_t* src, uint8_t len)
		{
			// error_ = error::none;
			error_ = 0;

			if(!sync_open_(speed_ / 8)) {
				return false;
			}

			iica_.IICCTL0.STT = 1;  // スタート・コンディション（開始）

			// スタート・コンディション検出を待つ
			while(iica_.IICF.IICBSY() == 0) sleep_();

			utils::delay::micro_second(5);

			iica_.IICA = adr << 1;  // アドレス転送とアクノリッジの確認
			if(!probe_ack_()) {
				iica_.IICCTL0.SPT = 1;
				// error_ = error::address;
				error_= 99;
				sci_puts("I2C error address\n");
				return false;
			}

			// 送信データ転送
			for(uint8_t i = 0; i < len; ++i) {
				iica_.IICA = *src;
				++src;
				if(!probe_ack_()) {
					iica_.IICCTL0.SPT = 1;
					error_ = 100;
					sci_puts("I2C error data\n");
					return false;
				}
			}
			// ストップ・コンディション
			iica_.IICCTL0.SPT = 1;

			sci_puts("I2C send ok\n");

			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	送信
			@param[in]	adr	７ビットアドレス
			@param[in]	first	第一データ
			@param[in]	src	転送先
			@param[in]	len	受信バイト数
			@return 送信が完了した場合「true」
		 */
		//-----------------------------------------------------------------//
		bool send(uint8_t adr, uint8_t first, const uint8_t* src, uint8_t len)
		{
			// error_ = error::none;

			iica_.IICCTL0.STT = 1;  // スタート・コンディション（開始）

			iica_.IICA = adr << 1;  // アドレス転送と確認
			if(!probe_ack_()) {
				iica_.IICCTL0.SPT = 1;
				// error_ = error::address;
				return false;
			}

			// 第一データ
			iica_.IICA = first;
			if(!probe_ack_()) {
				iica_.IICCTL0.SPT = 1;
				// error_ = error::data;
				return false;
			}

			// 送信データ転送
			for(uint8_t i = 0; i < len; ++i) {
				iica_.IICA = *src;
				++src;
				if(!probe_ack_()) {
					iica_.IICCTL0.SPT = 1;
					// error_ = error::data;
					return false;
				}
			}
			// ストップ・コンディション
			iica_.IICCTL0.SPT = 1;

			return true;
		}
	};
}
