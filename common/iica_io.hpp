#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/ (G13/L1C) グループ IICA 制御 @n
			※マスター動作のみ実装 @n
			※割り込みに対応していない、ポーリングのみ動作可能
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016, 2017 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include "common/renesas.hpp"

/// F_CLK はボーレートパラメーター計算で必要、設定が無いとエラーにします。
#ifndef F_CLK
#  error "iica_io.hpp requires F_CLK to be defined"
#endif

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
			start,		///< スタート（初期化）
			bus_open,	///< バス・オープン
			address,	///< アドレス転送
			send_data,	///< 送信データ転送
			recv_data,	///< 受信データ転送
			stop,		///< ストップ・コンディション
		};

	private:
		static volatile uint8_t sync_;

		uint8_t		intr_lvl_;
		uint8_t		sadr_;
		uint8_t		speed_;
		error		error_;

		inline void sleep_() {
			asm("nop");
		} 


		bool sync_intr_(uint8_t loop)
		{
			// 最終クロック検出割り込み
			while(intr::get_request(IICA::get_peripheral()) == 0) {
				utils::delay::micro_second(1);
				if(loop == 0) return false;
				--loop;
			}
			intr::set_request(IICA::get_peripheral(), 0);
			return true;
		}

		// 「アクノリッジ」を検出したら「true」
		//  予定サイクル以内に検出しなかったら「false」
		bool probe_ack_()
		{
			sync_intr_(speed_);
			if(IICA::IICS.ACKD() == 0) {  // アクノリッジ確認
				return false;
			}
			return true;
		}

		// アドレスの転送
		bool send_adr_(uint8_t adr) {
			// バスが開放されているか確認（通信状態ならエラー）
			if(IICA::IICS.SPD() != 0 && IICA::IICF.IICBSY() == 0) ;
			else {
				error_ = error::bus_open;
				return false;
			}

			IICA::IICCTL0.STT = 1;  // スタート・コンディション（開始）

			utils::delay::micro_second(1);

			IICA::IICA = adr;  // アドレス
			if(!probe_ack_()) {  // アクノリッジの確認
				error_ = error::address;
				return false;
			}
			return true;
		}


		bool out_stop_() {
			IICA::IICCTL0.SPT = 1;   // ストップ・コンディション
			bool f = sync_intr_(speed_ / 2);
			if(!f) {
				error_ = error::stop;
			}
			return f;
		}

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief  割り込みタスク
		*/
		//-----------------------------------------------------------------//
		static void task() __attribute__ ((section (".lowtext")))
		{
			++sync_;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  コンストラクター
			@param[in]	sadr	スレーブ・アドレス
		*/
		//-----------------------------------------------------------------//
		iica_io(uint8_t sadr = 0x00) : intr_lvl_(0), sadr_(sadr), speed_(0), error_(error::none) { }


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
			IICA::IICCTL0.IICE = 0;  // Unit Disable

			intr_lvl_ = intr_lvl;

			error_ = error::none;

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
				speed_ = 90 * 2;  // 100K b.p.s. * 9 clock * 2 (us)
				break;

			// 400K b.p.s. 時 tF: 0.2us、tR: 0.2us
			case speed::fast:
				wl = 42;
				wh = 26;
				speed_ = 23 * 2;  // 400K b.p.s * 9 clock * 2 (us)
				// ※デジタル・フィルター有効
				smc = IICA::IICCTL1.SMC.b(1) | IICA::IICCTL1.DFC.b(1);
				break;

			//   1M b.p.s. 時 tF: 0.1us、tR: 0.1us
			case speed::fast_plus:
				wl = 16;
				wh = 10;
				speed_ = 9 * 3; // 1M b.p.s * 9 clock * 3 (us)
				// ※デジタル・フィルター有効
				smc = IICA::IICCTL1.SMC.b(1) | IICA::IICCTL1.DFC.b(1);
				break;
			default:
				// error_ = error::start;
				return false;
			}

			// ユニットを有効にする
			manage::enable(IICA::get_peripheral());

			// 転送レート設定、IICE(0) の時に設定
			IICA::IICWL = wl / 2;
			IICA::IICWH = wh / 2;

			IICA::SVA = sadr_;  // スレーブ時のアドレス設定

			IICA::IICF.IICRSV = 1;  // 通信予約（不許可）

			// IICE(0) の時に設定
			IICA::IICCTL1 = IICA::IICCTL1.PRS.b(1) | smc;

			// clock 9, stop condition interrupt.
			IICA::IICCTL0 = IICA::IICCTL0.WTIM.b(1) | IICA::IICCTL0.SPIE.b(1);

			IICA::IICCTL0.IICE = 1;  // ユニット許可

			// ポート・モード設定、IICE(1) の時に設定
			manage::set_iica_port(IICA::get_peripheral());

			// 割り込みフラグ・クリア
			intr::set_request(IICA::get_peripheral(), 0);

			return out_stop_();
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	最終エラーの取得
			@return エラー・タイプ
		 */
		//-----------------------------------------------------------------//
		error get_last_error() const { return error_; }


		//-----------------------------------------------------------------//
		/*!
			@brief	送信
			@param[in]	adr	７ビットアドレス
			@param[in]	src	転送先
			@param[in]	len	受信バイト数
			@return 送信が完了した場合「true」
		 */
		//-----------------------------------------------------------------//
		bool send(uint8_t adr, const void* src, uint8_t len)
		{
			error_ = error::none;

			if(!send_adr_(adr << 1)) {
				IICA::IICCTL0.WREL = 1;
				IICA::IICCTL0.SPT  = 1;
				return false;
			}

			// 送信データ転送
			const uint8_t* p = static_cast<const uint8_t*>(src);
			for(uint8_t i = 0; i < len; ++i) {
				IICA::IICA = *p;
				++p;
				if(!probe_ack_()) {
					IICA::IICCTL0.SPT = 1;
					error_ = error::send_data;
					return false;
				}
			}
			return out_stop_();
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
		bool send(uint8_t adr, uint8_t first, const void* src, uint8_t len)
		{
			error_ = error::none;

			if(!send_adr_(adr << 1)) {
				IICA::IICCTL0.WREL = 1;
				IICA::IICCTL0.SPT = 1;
				return false;
			}

			IICA::IICA = first;
			if(!probe_ack_()) {
				IICA::IICCTL0.SPT = 1;
				error_ = error::send_data;
				return false;
			}

			// 送信データ転送
			const uint8_t* p = static_cast<const uint8_t*>(src);
			for(uint8_t i = 0; i < len; ++i) {
				IICA::IICA = *p;
				++p;
				if(!probe_ack_()) {
					IICA::IICCTL0.SPT = 1;
					error_ = error::send_data;
					return false;
				}
			}
			return out_stop_();
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	送信
			@param[in]	adr	７ビットアドレス
			@param[in]	first	第一データ
			@param[in]	second	第二データ
			@param[in]	src	転送先
			@param[in]	len	受信バイト数
			@return 送信が完了した場合「true」
		 */
		//-----------------------------------------------------------------//
		bool send(uint8_t adr, uint8_t first, uint8_t second, const void* src, uint8_t len)
		{
			error_ = error::none;

			if(!send_adr_(adr << 1)) {
				IICA::IICCTL0.WREL = 1;
				IICA::IICCTL0.SPT = 1;
				return false;
			}

			IICA::IICA = first;
			if(!probe_ack_()) {
				IICA::IICCTL0.SPT = 1;
				error_ = error::send_data;
				return false;
			}

			IICA::IICA = second;
			if(!probe_ack_()) {
				IICA::IICCTL0.SPT = 1;
				error_ = error::send_data;
				return false;
			}

			// 送信データ転送
			const uint8_t* p = static_cast<const uint8_t*>(src);
			for(uint8_t i = 0; i < len; ++i) {
				IICA::IICA = *p;
				++p;
				if(!probe_ack_()) {
					IICA::IICCTL0.SPT = 1;
					error_ = error::send_data;
					return false;
				}
			}
			return out_stop_();
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	受信
			@param[in]	adr	７ビットアドレス
			@param[out]	dst	転送先
			@param[in]	len	受信バイト数
			@return 受信が完了した場合「true」
		 */
		//-----------------------------------------------------------------//
		bool recv(uint8_t adr, void* dst, uint8_t len)
		{
			error_ = error::none;

			if(!send_adr_((adr << 1) | 1)) {
				IICA::IICCTL0.WREL = 1;
				IICA::IICCTL0.SPT = 1;
				// utils::format("recv address\n");
				return false;
			}

			if(len > 1) {
				IICA::IICCTL0.ACKE = 1;  // ACK 自動生成
			}
			IICA::IICCTL0.WREL = 1;  // Wait 削除

			// 受信データ転送
			uint8_t* p = static_cast<uint8_t*>(dst);
			for(uint8_t i = 0; i < len; ++i) {
				if(i == (len - 1)) {  // last data..
					IICA::IICCTL0.ACKE = 0;
				}
				if(!sync_intr_(speed_)) {
					error_ = error::recv_data;
					IICA::IICCTL0.WREL = 1;
					IICA::IICCTL0.SPT = 1;
					// utils::format("idx: %d\n") % static_cast<uint32_t>(i);
					return false;
				}
				*p = IICA::IICA();
				++p;
				if(i != (len - 1)) {
					IICA::IICCTL0.WREL = 1;  // Wait 削除
				}
			}

			return out_stop_();
		}
	};

	template <class IICA> volatile uint8_t iica_io<IICA>::sync_ = 0;
}
