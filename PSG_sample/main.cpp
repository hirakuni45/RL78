//=====================================================================//
/*!	@file
	@brief	PSG エミュレーターを使った音楽演奏をするサンプル @n
			P16/TO01(40) から、左チャネル @n
			P17/TO02(39) から、右チャネル
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2021 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RL78/blob/master/LICENSE
*/
//=====================================================================//
#include "common/renesas.hpp"
#include "common/port_utils.hpp"
#include "common/tau_io.hpp"
#include "common/fifo.hpp"
#include "common/uart_io.hpp"
#include "common/itimer.hpp"
#include "common/format.hpp"
#include "common/delay.hpp"
#include "common/csi_io.hpp"
#include "common/command.hpp"

#include "common/psg_mng.hpp"

namespace pwm {

	static constexpr uint16_t BUFF_NUM = 1024;

	// インターバル・タイマー割り込み制御クラス
	// ※PWMコンペアレジスターに、直接書き込んでいるので、PWMチャネルを変更する場合は注意
	class interval_master {
		int8_t buff_[BUFF_NUM];
		volatile uint16_t	pos_;

	public:
		interval_master() : buff_{ 0 }, pos_(0) { }

		// バッファを取得
		int8_t* get_buff() { return buff_; }

		// 波形位置を取得
		uint16_t get_pos() const { return pos_; }

		// 割り込み、functor
		void operator() () {
			device::TAU01::TDRL = buff_[pos_];
			device::TAU02::TDRL = buff_[pos_];
			++pos_;
			if(pos_ >= BUFF_NUM) pos_ = 0;
		}
	};
}

namespace {

	// 吸い込みなので、三番目のパラメーターを「false」とする。
	// LED::P = 1 で、実際のポートは、０になり、LED が点灯する。
	typedef device::PORT<device::port_no::P4, device::bitpos::B3, false> LED;

	// 送信、受信バッファの定義
	typedef utils::fifo<uint32_t, 32> buffer;
	// UART の定義（SAU02、SAU03）
	device::uart_io<device::SAU02, device::SAU03, buffer, buffer> uart_;

	// インターバル・タイマー
	device::itimer<uint8_t> itm_;

	typedef device::tau_io<device::TAU00, pwm::interval_master> MASTER;
	MASTER		master_;
	device::tau_io<device::TAU01> pwm1_;
	device::tau_io<device::TAU02> pwm2_;

	static constexpr uint16_t SAMPLE = 31'250;  // サンプリング周期
	static constexpr uint16_t TICK = 100;		// サンプルの楽曲では、１００を前提にしている。
	static constexpr uint16_t CNUM = 3;			// 同時発音数（大きくすると処理負荷が増えるので注意）
	typedef utils::psg_base PSG;
	typedef utils::psg_mng<SAMPLE, TICK, CNUM> PSG_MNG;
	PSG_MNG		psg_mng_;

	utils::command<64> command_;

	bool init_pwm_()
	{
		// 31.25 KHz (32MHz(F_CLK) / 4 / 256)
		uint8_t intr_level = 3;
		if(!master_.start_interval_direct(MASTER::DIVIDE::F4, 256 - 1, intr_level)) {
			return false;
		}
		intr_level = 0;
		if(!pwm1_.start_pwm<MASTER::tau_type>(0, intr_level)) {
			return false;
		}
		if(!pwm2_.start_pwm<MASTER::tau_type>(0, intr_level)) {
			return false;
		}

		return true;
	}


	// ドラゴンクエスト１・ラダトーム城（Dragon Quest 1 Chateau Ladutorm）
	constexpr PSG::SCORE score0_[] = {
		PSG::CTRL::VOLUME, 128,
		PSG::CTRL::SQ50,
		PSG::CTRL::TEMPO, 80,
		PSG::CTRL::ATTACK, 175,
		// 1
		PSG::KEY::Q,   8,
		PSG::KEY::E_5, 8,
		PSG::KEY::D_5, 8,
		PSG::KEY::E_5, 8,
		PSG::KEY::C_5, 8,
		PSG::KEY::E_5, 8,
		PSG::KEY::B_4, 8,
		PSG::KEY::E_5, 8,
		// 2
		PSG::KEY::A_4, 8*3,
		PSG::KEY::Q,   8*5,
		// 3
		PSG::KEY::Q,   8,
		PSG::KEY::F_5, 8,
		PSG::KEY::E_5, 8,
		PSG::KEY::F_5, 8,
		PSG::KEY::D_5, 8,
		PSG::KEY::F_5, 8,
		PSG::KEY::C_5, 8,
		PSG::KEY::F_5, 8,
		// 4
		PSG::KEY::B_4, 8*3,
		PSG::KEY::Q,   8*5,
		// 5
		PSG::KEY::Q,   8,
		PSG::KEY::G_5, 8,
		PSG::KEY::F_5, 8,
		PSG::KEY::G_5, 8,
		PSG::KEY::E_5, 8,
		PSG::KEY::G_5, 8,
		PSG::KEY::Cs5, 8,
		PSG::KEY::G_5, 8,
		// 6
		PSG::KEY::F_5, 16,
		PSG::KEY::G_5, 16,
		PSG::KEY::A_5, 16,
		PSG::KEY::G_5, 8,
		PSG::KEY::F_5, 8,
		// 7
		PSG::KEY::E_5, 16,
		PSG::KEY::C_5, 8,
		PSG::KEY::E_5, 8,
		PSG::KEY::D_5, 16,
		PSG::KEY::Eb5, 16,
		// 8
		PSG::KEY::E_5, 8*8,
		// 9
		PSG::KEY::Q,   8,
		PSG::KEY::A_5, 4,
		PSG::KEY::Gs5, 4,
		PSG::KEY::A_5, 8,
		PSG::KEY::E_5, 4,
		PSG::KEY::Eb5, 4,
		PSG::KEY::E_5, 8,
		PSG::KEY::C_5, 4,
		PSG::KEY::B_4, 4,
		PSG::KEY::C_5, 8,
		PSG::KEY::A_4, 8,
		// 10
		PSG::KEY::Q,   8,
		PSG::KEY::A_5, 4,
		PSG::KEY::Gs5, 4,
		PSG::KEY::A_5, 8,
		PSG::KEY::E_5, 4,
		PSG::KEY::Eb5, 4,
		PSG::KEY::E_5, 8,
		PSG::KEY::C_5, 4,
		PSG::KEY::B_4, 4,
		PSG::KEY::C_5, 8,
		PSG::KEY::A_4, 8,
		// 11
		PSG::KEY::B_4, 8,
		PSG::KEY::G_4, 8,
		PSG::KEY::C_5, 8,
		PSG::KEY::G_4, 8,
		PSG::KEY::F_5, 8,
		PSG::KEY::A_4, 8,
		PSG::KEY::C_5, 8,
		PSG::KEY::A_4, 8,
		// 12
		PSG::KEY::E_5, 8,
		PSG::KEY::E_4, 8,
		PSG::KEY::Gs4, 8,
		PSG::KEY::B_4, 8,
		PSG::KEY::E_5, 8,
		PSG::KEY::G_4, 8,
		PSG::KEY::Cs5, 8,
		PSG::KEY::E_5, 8,
		// 13
		PSG::KEY::Q,   8,
		PSG::KEY::A_5, 4,
		PSG::KEY::Gs5, 4,
		PSG::KEY::A_5, 8,
		PSG::KEY::F_5, 4,
		PSG::KEY::E_5, 4,
		PSG::KEY::F_5, 8,
		PSG::KEY::D_5, 4,
		PSG::KEY::Cs5, 4,
		PSG::KEY::D_5, 8,
		PSG::KEY::A_4, 8,
		// 14
		PSG::KEY::Q,   8,
		PSG::KEY::A_5, 4,
		PSG::KEY::Gs5, 4,
		PSG::KEY::A_5, 8,
		PSG::KEY::F_5, 4,
		PSG::KEY::E_5, 4,
		PSG::KEY::F_5, 8,
		PSG::KEY::D_5, 4,
		PSG::KEY::Cs5, 4,
		PSG::KEY::D_5, 8,
		PSG::KEY::A_4, 8,
		// 15
		PSG::KEY::B_4, 8,
		PSG::KEY::G_4, 8,
		PSG::KEY::C_5, 8,
		PSG::KEY::G_4, 8,
		PSG::KEY::F_5, 8,
		PSG::KEY::A_4, 8,
		PSG::KEY::C_5, 8,
		PSG::KEY::F_5, 8,
		// 16
		PSG::KEY::E_5, 8,
		PSG::KEY::A_4, 8,
		PSG::KEY::Gs4, 8,
		PSG::KEY::F_4, 8,
		PSG::KEY::E_4, 8,
		PSG::KEY::D_4, 8,
		PSG::KEY::C_4, 8,
		PSG::KEY::B_3, 8,
		// 17
		PSG::KEY::Q,   8,
		PSG::KEY::E_5, 8,
		PSG::KEY::D_5, 8,
		PSG::KEY::E_5, 8,
		PSG::KEY::C_5, 8,
		PSG::KEY::E_5, 8,
		PSG::KEY::B_4, 8,
		PSG::KEY::E_5, 8,
		// 18
		PSG::KEY::A_4, 8*3,
		PSG::KEY::Q,   8*5,
		// 19
		PSG::KEY::Q,   8,
		PSG::KEY::F_5, 8,
		PSG::KEY::E_5, 8,
		PSG::KEY::F_5, 8,
		PSG::KEY::D_5, 8,
		PSG::KEY::F_5, 8,
		PSG::KEY::C_5, 8,
		PSG::KEY::F_5, 8,
		// 20
		PSG::KEY::B_4, 8*3,
		PSG::KEY::Q,   8*5,
		// 21
		PSG::KEY::Q,   8,
		PSG::KEY::G_5, 8,
		PSG::KEY::F_5, 8,
		PSG::KEY::G_5, 8,
		PSG::KEY::E_5, 8,
		PSG::KEY::G_5, 8,
		PSG::KEY::Cs5, 8,
		PSG::KEY::G_5, 8,
		// 22
		PSG::KEY::F_5, 16,
		PSG::KEY::G_5, 16,
		PSG::KEY::A_5, 16,
		PSG::KEY::G_5, 8,
		PSG::KEY::F_5, 8,
		// 23
		PSG::KEY::E_5, 16,
		PSG::KEY::C_5, 8,
		PSG::KEY::E_5, 8,
		PSG::KEY::D_5, 16,
		PSG::KEY::Eb5, 16,
		// 24
		PSG::KEY::E_5, 8*8,
		// 25
		PSG::KEY::Q   ,8,
		PSG::KEY::A_5, 4,
		PSG::KEY::Gs5, 4,
		PSG::KEY::A_5, 8,
		PSG::KEY::E_5, 4,
		PSG::KEY::Eb5, 4,
		PSG::KEY::E_5, 8,
		PSG::KEY::C_5, 4,
		PSG::KEY::B_4, 4,
		PSG::KEY::C_5, 8,
		PSG::KEY::A_4, 8,
		// 26
		PSG::KEY::Q   ,8,
		PSG::KEY::A_5, 4,
		PSG::KEY::Gs5, 4,
		PSG::KEY::A_5, 8,
		PSG::KEY::E_5, 4,
		PSG::KEY::Eb5, 4,
		PSG::KEY::E_5, 8,
		PSG::KEY::C_5, 4,
		PSG::KEY::B_4, 4,
		PSG::KEY::C_5, 8,
		PSG::KEY::A_4, 8,
		// 27
		PSG::KEY::B_4, 8,
		PSG::KEY::G_4, 8,
		PSG::KEY::C_5, 8,
		PSG::KEY::G_4, 8,
		PSG::KEY::F_5, 8,
		PSG::KEY::A_4, 8,
		PSG::KEY::C_5, 8,
		PSG::KEY::A_4, 8,
		// 28
		PSG::KEY::E_5, 8,
		PSG::KEY::E_4, 8,
		PSG::KEY::Gs4, 8,
		PSG::KEY::B_4, 8,
		PSG::KEY::E_5, 8,
		PSG::KEY::G_4, 8,
		PSG::KEY::C_5, 8,
		PSG::KEY::E_5, 8,
		// 29
		PSG::KEY::Q,   8,
		PSG::KEY::A_5, 4,
		PSG::KEY::Gs5, 4,
		PSG::KEY::A_5, 8,
		PSG::KEY::F_5, 4,
		PSG::KEY::E_5, 4,
		PSG::KEY::F_5, 8,
		PSG::KEY::D_5, 4,
		PSG::KEY::Cs5, 4,
		PSG::KEY::D_5, 8,
		PSG::KEY::A_4, 8,
		// 30
		PSG::KEY::Q,   8,
		PSG::KEY::A_5, 4,
		PSG::KEY::Gs5, 4,
		PSG::KEY::A_5, 8,
		PSG::KEY::F_5, 4,
		PSG::KEY::E_5, 4,
		PSG::KEY::F_5, 8,
		PSG::KEY::D_5, 4,
		PSG::KEY::Cs5, 4,
		PSG::KEY::D_5, 8,
		PSG::KEY::A_4, 8,
		// 31
		PSG::KEY::B_4, 8,
		PSG::KEY::G_4, 8,
		PSG::KEY::C_5, 8,
		PSG::KEY::G_4, 8,
		PSG::KEY::F_5, 8,
		PSG::KEY::A_4, 8,
		PSG::KEY::C_5, 8,
		PSG::KEY::F_5, 8,
		// 32
		PSG::KEY::E_5, 8,
		PSG::KEY::A_4, 8,  // F_4, 8
		PSG::KEY::Gs4, 8,  // E_4, 8
		PSG::KEY::F_4, 8,  // D_4, 8
		PSG::KEY::E_4, 8,  // C_4, 8
		PSG::KEY::D_4, 8,  // B_3, 8
		PSG::KEY::C_4, 8,  // A_3, 8
		PSG::KEY::B_3, 8,  // G_3, 8
		// 33
		PSG::KEY::Q,   8,
		PSG::KEY::E_5, 8,
		PSG::KEY::D_5, 8,
		PSG::KEY::E_5, 8,
		PSG::KEY::C_5, 8,
		PSG::KEY::E_5, 8,
		PSG::KEY::B_4, 8,
		PSG::KEY::E_5, 8,
		// 34
		PSG::KEY::A_4, 8,
		PSG::KEY::Gs4, 8,
		PSG::KEY::A_4, 8,
		PSG::KEY::B_4, 8,
		PSG::KEY::Cs5, 8,
		PSG::KEY::D_5, 8,
		PSG::KEY::E_5, 8,
		PSG::KEY::Cs5, 8,
		// 35
		PSG::KEY::D_5, 8,
		PSG::KEY::F_5, 8,
		PSG::KEY::E_5, 8,  // Cs5, 8 
		PSG::KEY::F_5, 8,
		PSG::KEY::D_5, 8,
		PSG::KEY::F_5, 8,
		PSG::KEY::A_5, 8,  // C_5, 8
		PSG::KEY::F_5, 8,
		// 36
		PSG::KEY::Gs5, 8,
		PSG::KEY::Fs5, 8,
		PSG::KEY::F_5, 8,
		PSG::KEY::Fs5, 8,
		PSG::KEY::Gs5, 8,
		PSG::KEY::A_5, 8,
		PSG::KEY::B_5, 8,
		PSG::KEY::Gs5, 8,
		// 37
		PSG::KEY::Bb5, 8,
		PSG::KEY::Fs5, 8,
		PSG::KEY::Cs5, 8,
		PSG::KEY::Fs5, 8,
		PSG::KEY::Bb4, 8,
		PSG::KEY::Cs5, 8,
		PSG::KEY::Fs5, 8,
		PSG::KEY::Cs5, 8,
		// 38
		PSG::KEY::Gs5, 8,
		PSG::KEY::F_5, 8,
		PSG::KEY::D_5, 8,
		PSG::KEY::F_5, 8,
		PSG::KEY::Gs4, 8,
		PSG::KEY::D_5, 8,
		PSG::KEY::F_5, 8,
		PSG::KEY::D_5, 8,
		// 39
		PSG::KEY::Fs5, 8,
		PSG::KEY::Eb5, 8,
		PSG::KEY::Bb4, 8,
		PSG::KEY::Eb5, 8,
		PSG::KEY::Fs4, 8,
		PSG::KEY::Bb4, 8,
		PSG::KEY::Eb5, 8,
		PSG::KEY::Bb4, 8,
		// 40
		PSG::KEY::E_5, 8,
		PSG::KEY::Bb4, 8,
		PSG::KEY::Fs4, 8,
		PSG::KEY::Bb4, 8,
		PSG::KEY::E_4, 8,
		PSG::KEY::Fs4, 8,
		PSG::KEY::Bb4, 8,
		PSG::KEY::Fs4, 8,
		// 41
		PSG::KEY::B_4, 16,
		PSG::KEY::Bb4, 16,
		PSG::KEY::B_4, 16,
		PSG::KEY::Fs5, 16,  // PSG::KEY::Fs4, 16,
		// 42
		PSG::KEY::Eb4, 8,
		PSG::KEY::Cs4, 8,
		PSG::KEY::Eb4, 8,
		PSG::KEY::E_4, 8,
		PSG::KEY::Fs4, 8,
		PSG::KEY::Gs4, 8,
		PSG::KEY::A_4, 8,
		PSG::KEY::Fs4, 8,
		// 43
		PSG::KEY::Q,   8,
		PSG::KEY::Gs4, 8,
		PSG::KEY::Fs4, 8,
		PSG::KEY::Gs4, 8,
		PSG::KEY::E_4, 8,
		PSG::KEY::Gs4, 8,
		PSG::KEY::Eb4, 8,
		PSG::KEY::Fs4, 8,
		// 44
		PSG::KEY::E_4, 8,
		PSG::KEY::Eb4, 8,
		PSG::KEY::E_4, 8,
		PSG::KEY::Fs4, 8,
		PSG::KEY::Gs4, 8,
		PSG::KEY::A_4, 8,
		PSG::KEY::B_4, 8,
		PSG::KEY::Gs4, 8,
		// 45
		PSG::KEY::Cs5, 8,
		PSG::KEY::A_4, 8,
		PSG::KEY::E_4, 8,
		PSG::KEY::A_4, 8,
		PSG::KEY::Cs5, 8,
		PSG::KEY::A_4, 8,
		PSG::KEY::E_4, 8,
		PSG::KEY::A_4, 8,
		// 46
		PSG::KEY::Eb5, 8,
		PSG::KEY::A_4, 8,
		PSG::KEY::Fs4, 8,
		PSG::KEY::A_4, 8,
		PSG::KEY::Eb5, 8,
		PSG::KEY::A_4, 8,
		PSG::KEY::Fs4, 8,
		PSG::KEY::A_4, 8,
		// 47
		PSG::KEY::E_5, 8,
		PSG::KEY::B_4, 8,
		PSG::KEY::Gs4, 8,
		PSG::KEY::B_4, 8,
		PSG::KEY::E_5, 8,
		PSG::KEY::B_4, 8,
		PSG::KEY::Gs4, 8,
		PSG::KEY::B_4, 8,
		// 48
		PSG::KEY::E_5, 8,
		PSG::KEY::E_6, 8,  // PSG::KEY::Gs5, 8,
		PSG::KEY::D_6, 8,  // PSG::KEY::F_5, 8,
		PSG::KEY::C_6, 8,  // PSG::KEY::E_5, 8,
		PSG::KEY::B_5, 8,  // PSG::KEY::D_5, 8,
		PSG::KEY::A_5, 8,  // PSG::KEY::C_5, 8,
		PSG::KEY::Gs5, 8,  // PSG::KEY::B_4, 8,
		PSG::KEY::E_5, 8,  // PSG::KEY::Gs4, 8,
		// 49
		PSG::KEY::Cs5, 8,
		PSG::KEY::Bb4, 8,
		PSG::KEY::G_4, 8,
		PSG::KEY::Eb4, 8,
		PSG::KEY::Cs5, 8,
		PSG::KEY::Eb4, 8,
		PSG::KEY::G_4, 8,
		PSG::KEY::Bb4, 8,
		// 50
		PSG::KEY::D_5, 8,
		PSG::KEY::A_4, 8,
		PSG::KEY::F_4, 8,
		PSG::KEY::A_4, 8,
		PSG::KEY::D_5, 8,
		PSG::KEY::A_4, 8,
		PSG::KEY::F_4, 8,
		PSG::KEY::A_4, 8,
		// 51
		PSG::KEY::Eb5, 8,
		PSG::KEY::A_4, 8,
		PSG::KEY::Fs4, 8,
		PSG::KEY::A_4, 8,
		PSG::KEY::Eb5, 8,
		PSG::KEY::A_4, 8,
		PSG::KEY::Fs4, 8,
		PSG::KEY::A_4, 8,
		// 52
		PSG::KEY::E_5, 8,
		PSG::KEY::B_4, 8,
		PSG::KEY::Gs4, 8,
		PSG::KEY::B_4, 8,
		PSG::KEY::E_5, 8,
		PSG::KEY::B_4, 8,
		PSG::KEY::Gs4, 8,
		PSG::KEY::B_4, 8,
		// 53
		PSG::KEY::F_5, 8,
		PSG::KEY::Cs5, 8,
		PSG::KEY::Gs4, 8,
		PSG::KEY::Cs5, 8,
		PSG::KEY::F_5, 8,
		PSG::KEY::Cs5, 8,
		PSG::KEY::Gs4, 8,
		PSG::KEY::Cs5, 8,
		// 54
		PSG::KEY::Fs5, 8,
		PSG::KEY::Cs5, 8,
		PSG::KEY::Fs4, 8,
		PSG::KEY::Cs5, 8,
		PSG::KEY::Fs5, 8,
		PSG::KEY::Cs5, 8,
		PSG::KEY::Fs4, 8,
		PSG::KEY::Cs5, 8,
		// 55
		PSG::KEY::G_5, 8,  // KEY::Bb4, 8,
		PSG::KEY::Cs5, 8,
		PSG::KEY::Bb4, 8,
		PSG::KEY::Cs5, 8,
		PSG::KEY::G_5, 8,  // KEY::Bb4, 8,
		PSG::KEY::Cs5, 8,
		PSG::KEY::Bb4, 8,
		PSG::KEY::Cs5, 8,
		// 56
		PSG::KEY::Gs5, 8,  // KEY::C_5, 8,
		PSG::KEY::Gs4, 8,
		PSG::KEY::Fs4, 8,
		PSG::KEY::E_4, 8,
		PSG::KEY::Eb4, 8,
		PSG::KEY::Cs4, 8,
		PSG::KEY::C_4, 8,
		PSG::KEY::Gs3, 8,
		// 57
		PSG::KEY::Q,   8,
		PSG::KEY::Gs5, 8,
		PSG::KEY::Fs5, 8,
		PSG::KEY::Gs5, 8,
		PSG::KEY::E_5, 8,
		PSG::KEY::Gs5, 8,
		PSG::KEY::Eb5, 8,
		PSG::KEY::Gs5, 8,
		// 58
		PSG::KEY::Cs5, 8,
		PSG::KEY::Bb5, 8,
		PSG::KEY::Gs5, 8,
		PSG::KEY::Bb5, 8,
		PSG::KEY::Fs5, 8,
		PSG::KEY::Bb5, 8,
		PSG::KEY::E_5, 8,
		PSG::KEY::Bb5, 8,
		// 59
		PSG::KEY::Eb5, 8,
		PSG::KEY::B_5, 8,
		PSG::KEY::A_5, 8,
		PSG::KEY::B_5, 8,
		PSG::KEY::Gb5, 8,
		PSG::KEY::B_5, 8,
		PSG::KEY::Fs5, 8,
		PSG::KEY::B_5, 8,
		// 60
		PSG::KEY::E_5, 8,
		PSG::KEY::Eb5, 8,
		PSG::KEY::E_5, 8,
		PSG::KEY::Fs5, 8,
		PSG::KEY::Gs5, 8,
		PSG::KEY::A_5, 8,
		PSG::KEY::B_5, 8,
		PSG::KEY::Gs5, 8,






		PSG::CTRL::END
	};

	constexpr PSG::SCORE score1_[] = {
		PSG::CTRL::VOLUME, 128,
		PSG::CTRL::SQ50,
		PSG::CTRL::TEMPO, 80,
		PSG::CTRL::ATTACK, 175,
		// 1
		PSG::KEY::A_2, 8,
		PSG::KEY::Q,   8*7,
		// 2
		PSG::KEY::Q,   8,
		PSG::KEY::A_2, 8,
		PSG::KEY::C_3, 8,
		PSG::KEY::E_3, 8,
		PSG::KEY::A_3, 8,
		PSG::KEY::G_3, 8,
		PSG::KEY::F_3, 8,
		PSG::KEY::E_3, 8,
		// 3
		PSG::KEY::D_3, 8,
		PSG::KEY::D_4, 8,
		PSG::KEY::C_4, 8,
		PSG::KEY::D_4, 8,
		PSG::KEY::B_3, 8,
		PSG::KEY::D_4, 8,
		PSG::KEY::A_3, 8,
		PSG::KEY::D_4, 8,
		// 4
		PSG::KEY::Gs3, 8,
		PSG::KEY::E_3, 8,
		PSG::KEY::Gs3, 8,
		PSG::KEY::B_3, 8,
		PSG::KEY::E_4, 8,
		PSG::KEY::E_3, 8,
		PSG::KEY::Fs3, 8,
		PSG::KEY::Gs3, 8,
		// 5
		PSG::KEY::A_3, 8,
		PSG::KEY::E_4, 8,
		PSG::KEY::D_4, 8,
		PSG::KEY::E_4, 8,
		PSG::KEY::Cs4, 8,
		PSG::KEY::E_4, 8,
		PSG::KEY::A_3, 8,
		PSG::KEY::E_4, 8,
		// 6
		PSG::KEY::D_4, 8,
		PSG::KEY::A_3, 8,
		PSG::KEY::E_4, 8,
		PSG::KEY::A_3, 8,
		PSG::KEY::F_4, 8,
		PSG::KEY::A_3, 8,
		PSG::KEY::E_4, 8,
		PSG::KEY::D_4, 8,
		// 7
		PSG::KEY::C_4, 8,
		PSG::KEY::G_3, 8,
		PSG::KEY::E_4, 8,
		PSG::KEY::G_3, 8,
		PSG::KEY::B_3, 8,
		PSG::KEY::F_4, 8,
		PSG::KEY::A_3, 8,
		PSG::KEY::Fs4, 8,
		// 8
		PSG::KEY::Gs4, 8,
		PSG::KEY::E_3, 8,
		PSG::KEY::Gs3, 8,
		PSG::KEY::B_3, 8,
		PSG::KEY::E_4, 8,
		PSG::KEY::D_4, 8,
		PSG::KEY::C_4, 8,
		PSG::KEY::B_3, 8,
		// 9
		PSG::KEY::A_3, 8,
		PSG::KEY::C_4, 4,
		PSG::KEY::B_3, 4,
		PSG::KEY::C_4, 8,
		PSG::KEY::C_4, 4,
		PSG::KEY::B_3, 4,
		PSG::KEY::C_4, 8,
		PSG::KEY::Eb4, 4,
		PSG::KEY::E_4, 4,
		PSG::KEY::E_4, 8,
		PSG::KEY::C_4, 8,
		// 10
		PSG::KEY::G_3, 8,
		PSG::KEY::C_4, 4,
		PSG::KEY::B_3, 4,
		PSG::KEY::C_4, 8,
		PSG::KEY::C_4, 4,
		PSG::KEY::B_3, 4,
		PSG::KEY::E_4, 8, // PSG::KEY::Fs3, 8,
		PSG::KEY::Eb4, 4,
		PSG::KEY::E_4, 4,
		PSG::KEY::E_4, 8,
		PSG::KEY::C_4, 8,
		// 11
		PSG::KEY::F_3, 16,
		PSG::KEY::E_3, 16,
		PSG::KEY::D_3, 16,
		PSG::KEY::Eb3, 16,
		// 12
		PSG::KEY::E_3, 16*2,
		PSG::KEY::A_3, 16*2,
		// 13
		PSG::KEY::D_3, 8,
		PSG::KEY::F_4, 4,
		PSG::KEY::E_4, 4,
		PSG::KEY::F_4, 8,
		PSG::KEY::D_4, 4,
		PSG::KEY::Cs4, 4,
		PSG::KEY::Ds4, 8,
		PSG::KEY::F_4, 4,
		PSG::KEY::E_4, 4,
		PSG::KEY::F_4, 8,
		PSG::KEY::D_4, 8,
		// 14
		PSG::KEY::C_4, 8,
		PSG::KEY::F_4, 4,
		PSG::KEY::E_4, 4,
		PSG::KEY::F_4, 8,
		PSG::KEY::D_4, 4,
		PSG::KEY::Cs4, 4,
		PSG::KEY::D_4, 8,  // B_3, 8
		PSG::KEY::F_4, 4,
		PSG::KEY::E_4, 4,
		PSG::KEY::F_4, 8,
		PSG::KEY::D_4, 8,
		// 15
		PSG::KEY::F_3, 16,
		PSG::KEY::E_3, 16,
		PSG::KEY::D_3, 32,
		// 16
		PSG::KEY::E_3, 16,
		PSG::KEY::Q,   8*6,
		// 17
		PSG::KEY::A_2, 16,
		PSG::KEY::Q,   8*6,
		// 18
		PSG::KEY::Q,   8,
		PSG::KEY::A_2, 8,
		PSG::KEY::C_3, 8,
		PSG::KEY::E_3, 8,
		PSG::KEY::A_3, 8,
		PSG::KEY::G_3, 8,
		PSG::KEY::F_3, 8,
		PSG::KEY::E_3, 8,
		// 19
		PSG::KEY::D_3, 8,
		PSG::KEY::D_4, 8,
		PSG::KEY::C_4, 8,
		PSG::KEY::D_4, 8,
		PSG::KEY::B_3, 8,
		PSG::KEY::D_4, 8,
		PSG::KEY::A_3, 8,
		PSG::KEY::D_4, 8,
		// 20
		PSG::KEY::Gs3, 8,
		PSG::KEY::E_3, 8,
		PSG::KEY::Gs3, 8,
		PSG::KEY::B_3, 8,
		PSG::KEY::E_4, 8,
		PSG::KEY::E_3, 8,
		PSG::KEY::Fs3, 8,
		PSG::KEY::Gs3, 8,
		// 21
		PSG::KEY::A_3, 8,
		PSG::KEY::E_4, 8,
		PSG::KEY::D_4, 8,
		PSG::KEY::E_4, 8,
		PSG::KEY::Cs4, 8,
		PSG::KEY::E_4, 8,
		PSG::KEY::A_3, 8,
		PSG::KEY::E_4, 8,
		// 22
		PSG::KEY::D_4, 8,
		PSG::KEY::A_3, 8,
		PSG::KEY::E_4, 8,
		PSG::KEY::A_3, 8,
		PSG::KEY::F_4, 8,
		PSG::KEY::A_3, 8,
		PSG::KEY::E_4, 8,
		PSG::KEY::D_4, 8,
		// 23
		PSG::KEY::C_4, 8,
		PSG::KEY::G_3, 8,
		PSG::KEY::E_4, 8,
		PSG::KEY::G_3, 8,
		PSG::KEY::B_3, 8,
		PSG::KEY::F_4, 8,
		PSG::KEY::A_3, 8,
		PSG::KEY::Fs4, 8,
		// 24
		PSG::KEY::Gs4, 8,
		PSG::KEY::E_3, 8,
		PSG::KEY::Gs3, 8,
		PSG::KEY::B_3, 8,
		PSG::KEY::E_4, 8,
		PSG::KEY::D_4, 8,
		PSG::KEY::C_4, 8,
		PSG::KEY::B_3, 8,
		// 25
		PSG::KEY::A_3, 8,
		PSG::KEY::C_4, 4,
		PSG::KEY::B_3, 4,
		PSG::KEY::C_4, 8,
		PSG::KEY::C_4, 4,
		PSG::KEY::B_3, 4,
		PSG::KEY::C_4, 8,
		PSG::KEY::Eb4, 4,
		PSG::KEY::E_4, 4,
		PSG::KEY::E_4, 8,
		PSG::KEY::C_3, 8,
		// 26
		PSG::KEY::A_3, 8,
		PSG::KEY::C_4, 4,
		PSG::KEY::B_3, 4,
		PSG::KEY::C_4, 8,
		PSG::KEY::C_4, 4,
		PSG::KEY::B_3, 4,
		PSG::KEY::C_4, 8,  // Fs3, 8
		PSG::KEY::Eb4, 4,
		PSG::KEY::E_4, 4,
		PSG::KEY::E_4, 8,
		PSG::KEY::C_3, 8,
		// 27
		PSG::KEY::F_3, 16,
		PSG::KEY::E_3, 16,
		PSG::KEY::D_3, 16,
		PSG::KEY::Eb3, 16,
		// 28
		PSG::KEY::E_3, 32,
		PSG::KEY::A_3, 32,
		// 29
		PSG::KEY::D_3, 8,
		PSG::KEY::F_4, 4,
		PSG::KEY::E_4, 4,
		PSG::KEY::F_4, 8,
		PSG::KEY::D_4, 4,
		PSG::KEY::Cs4, 4,
		PSG::KEY::D_4, 8,
		PSG::KEY::F_4, 4,
		PSG::KEY::E_4, 4,
		PSG::KEY::F_4, 8,
		PSG::KEY::D_4, 8,
		// 30
		PSG::KEY::C_4, 8,
		PSG::KEY::F_4, 4,
		PSG::KEY::E_4, 4,
		PSG::KEY::F_4, 8,
		PSG::KEY::D_4, 4,
		PSG::KEY::Cs4, 4,
		PSG::KEY::D_4, 8,  // B_3, 8
		PSG::KEY::F_4, 4,
		PSG::KEY::E_4, 4,
		PSG::KEY::F_4, 8,
		PSG::KEY::D_4, 8,
		// 31
		PSG::KEY::F_3, 16,
		PSG::KEY::E_3, 16,
		PSG::KEY::D_3, 32,
		// 32
		PSG::KEY::E_3, 16,
		PSG::KEY::Q,   16*3,
		// 33
		PSG::KEY::A_3, 16,
		PSG::KEY::Q,   16*3,
		// 34
		PSG::KEY::Q,   8*8,
		// 35
		PSG::KEY::Q,   8*8,
		// 36
		PSG::KEY::B_4, 8,
		PSG::KEY::A_4, 8,
		PSG::KEY::Gs4, 8,
		PSG::KEY::A_4, 8,
		PSG::KEY::B_4, 8,
		PSG::KEY::C_5, 8,
		PSG::KEY::D_5, 8,
		PSG::KEY::B_4, 8,
		// 37
		PSG::KEY::Cs5, 8,
		PSG::KEY::Bb4, 8,
		PSG::KEY::Fs4, 8,
		PSG::KEY::Bb4, 8,
		PSG::KEY::Cs4, 8,
		PSG::KEY::Bb4, 8,
		PSG::KEY::Fs4, 8,
		PSG::KEY::Bb4, 8,
		// 38
		PSG::KEY::F_4, 8,
		PSG::KEY::D_5, 8,
		PSG::KEY::Bb4, 8,
		PSG::KEY::D_5, 8,
		PSG::KEY::F_4, 8,
		PSG::KEY::Bb4, 8,
		PSG::KEY::D_5, 8,
		PSG::KEY::Bb4, 8,
		// 39
		PSG::KEY::Eb4, 8,
		PSG::KEY::Bb4, 8,
		PSG::KEY::Fs4, 8,
		PSG::KEY::Bb4, 8,
		PSG::KEY::Eb4, 8,
		PSG::KEY::Bb4, 8,
		PSG::KEY::Fs4, 8,
		PSG::KEY::Bb4, 8,
		// 40
		PSG::KEY::Cs4, 8,
		PSG::KEY::Fs4, 8,
		PSG::KEY::Bb3, 8,
		PSG::KEY::Fs4, 8,
		PSG::KEY::Cs4, 8,
		PSG::KEY::Bb3, 8,
		PSG::KEY::Fs4, 8,
		PSG::KEY::Bb3, 8,
		// 41
		PSG::KEY::B_3, 8,
		PSG::KEY::Fs3, 8,
		PSG::KEY::E_3, 8,
		PSG::KEY::Fs3, 8,
		PSG::KEY::Eb3, 8,
		PSG::KEY::Fs3, 8,
		PSG::KEY::Cs3, 8,
		PSG::KEY::Fs3, 8,
		// 42
		PSG::KEY::B_2, 8,
		PSG::KEY::Bb2, 8,
		PSG::KEY::B_2, 8,
		PSG::KEY::C_3, 8,
		PSG::KEY::Eb3, 8,
		PSG::KEY::E_3, 8,
		PSG::KEY::Fs3, 8,
		PSG::KEY::E_3, 8,
		// 43
		PSG::KEY::E_3, 8,
		PSG::KEY::B_3, 8,
		PSG::KEY::A_3, 8,
		PSG::KEY::B_3, 8,
		PSG::KEY::Gs3, 8,  // PSG::KEY::E_3, 8,
		PSG::KEY::B_3, 8,
		PSG::KEY::Fs3, 8,  // PSG::KEY::B_2, 8,
		PSG::KEY::B_3, 8,
		// 44
		PSG::KEY::Gs3, 8,  // PSG::KEY::E_3, 8,
		PSG::KEY::Fs3, 8,
		PSG::KEY::Gs3, 8,
		PSG::KEY::A_3, 8,
		PSG::KEY::B_3, 8,
		PSG::KEY::Cs4, 8,
		PSG::KEY::D_4, 8,
		PSG::KEY::B_3, 8,
		// 45
		PSG::KEY::A_3, 16,
		PSG::KEY::A_2, 16,
		PSG::KEY::A_3, 16,
		PSG::KEY::A_2, 16,
		// 46
		PSG::KEY::Fs3, 16,
		PSG::KEY::Fs2, 16,
		PSG::KEY::B_2, 16,
		PSG::KEY::B_2, 16,
		// 47
		PSG::KEY::E_3, 16,
		PSG::KEY::E_2, 16,
		PSG::KEY::E_3, 16,
		PSG::KEY::E_2, 16,
		// 48
		PSG::KEY::E_3, 16,
		PSG::KEY::Q,   16*3,
		// 49
		PSG::KEY::E_4, 16,
		PSG::KEY::E_4, 16,
		PSG::KEY::E_4, 16,
		PSG::KEY::E_4, 16,
		// 50
		PSG::KEY::D_4, 16,
		PSG::KEY::D_4, 16,
		PSG::KEY::D_4, 16,
		PSG::KEY::D_4, 16,
		// 51
		PSG::KEY::C_4, 16,
		PSG::KEY::C_4, 16,
		PSG::KEY::C_4, 16,
		PSG::KEY::C_4, 16,
		// 52
		PSG::KEY::B_3, 16,
		PSG::KEY::B_3, 16,
		PSG::KEY::B_3, 16,
		PSG::KEY::B_3, 16,
		// 53
		PSG::KEY::B_3, 16,
		PSG::KEY::B_3, 16,
		PSG::KEY::B_3, 16,
		PSG::KEY::B_3, 16,
		// 54
		PSG::KEY::A_3, 16,
		PSG::KEY::A_3, 16,
		PSG::KEY::A_3, 16,
		PSG::KEY::A_3, 16,
		// 55
		PSG::KEY::Eb3, 16,
		PSG::KEY::Eb3, 16,  //  PSG::KEY::Eb2, 16, 
		PSG::KEY::Eb3, 16,
		PSG::KEY::Eb3, 16,  //  PSG::KEY::Eb2, 16, 
		// 56
		PSG::KEY::Gs2, 8,
		PSG::KEY::Gs3, 8,
		PSG::KEY::Fs3, 8,
		PSG::KEY::E_3, 8,
		PSG::KEY::Eb3, 8,
		PSG::KEY::Cs3, 8,
		PSG::KEY::C_3, 8,
		PSG::KEY::Gs2, 8,
		// 57
		PSG::KEY::Cs3, 16,
		PSG::KEY::Q,   16*3,
		// 58
		PSG::KEY::Q,   16*4,
		// 59
		PSG::KEY::Q,   16*4,
		// 60
		PSG::KEY::Q,   16*4,



		PSG::CTRL::END
	};
}


extern "C" {

	void sci_putch(char ch)
	{
		uart_.putch(ch);
	}


	void sci_puts(const char* str)
	{
		uart_.puts(str);
	}


	char sci_getch(void)
	{
		return uart_.getch();
	}


	uint16_t sci_length()
	{
		return uart_.recv_length();
	}

	void UART1_TX_intr(void)
	{
		uart_.send_task();
	}


	void UART1_RX_intr(void)
	{
		uart_.recv_task();
	}


	void UART1_ER_intr(void)
	{
		uart_.error_task();
	}

	void ITM_intr(void)
	{
		itm_.task();
	}


	void TM00_intr(void)
	{
		master_.task();
	}
};


int main(int argc, char* argv[])
{
	utils::port::pullup_all();  ///< 安全の為、全ての入力をプルアップ

	LED::DIR = 1;

	// インターバル・タイマー開始
	{
		uint8_t intr_level = 1;
		itm_.start(TICK, intr_level);
	}

	// UART 開始
	{
		uint8_t intr_level = 1;
		uart_.start(115200, intr_level);
	}

	uart_.puts("Start RL78/G13 PSG sample\n");

	if(!init_pwm_()) {
		uart_.puts("PWM initialization fail\n");
	}

	command_.set_prompt("# ");

	psg_mng_.set_score(0, score0_);
	psg_mng_.set_score(1, score1_);

	itm_.sync();
	uint16_t pos = master_.at_task().get_pos();
	uint8_t n = 0;
	uint8_t delay = 200;
	while(1) {
		itm_.sync();

		{
			auto newpos = master_.at_task().get_pos();
			auto n = newpos - pos;
			n &= pwm::BUFF_NUM - 1;
			if(n > (SAMPLE / TICK + 8)) n = SAMPLE / TICK + 8;
			int8_t tmp[n];
			psg_mng_.render(n, tmp);
			auto p = master_.at_task().get_buff();
			pos -= n;
			pos &= pwm::BUFF_NUM - 1;
			for(uint32_t i = 0; i < n; ++i) {
				p[pos] = tmp[i] + 0x80;
				++pos;
				if(pos >= pwm::BUFF_NUM) pos = 0;
			}
			pos = newpos;

			if(delay > 0) {
				delay--;
			} else {
				psg_mng_.service();
			}
		}

		// コマンド入力と、コマンド解析
		if(command_.service()) {
			auto cmdn = command_.get_words();
			if(cmdn >= 1) {
			}
		}

		++n;
		if(n >= 50) n = 0;
		LED::P = n < 20 ? 1 : 0;
	}
}
