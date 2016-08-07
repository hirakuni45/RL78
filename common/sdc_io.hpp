#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/G13 グループ SD カード・アクセス制御 @n
			Copyright 2016 Kunihito Hiramatsu
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include "common/port_utils.hpp"
#include "ff12a/mmc_io.hpp"
#include "common/format.hpp"

namespace utils {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  SD カード・アクセス制御テンプレート
		@param[in]	CSI		CSI 制御クラス
		@param[in]	SELECT	SD カード選択 I/O ポートクラス
		@param[in]	POWER	SD カード電源 I/O ポートクラス
		@param[in]	DETECT	SD カード検出 I/O ポートクラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class CSI, class SELECT, class POWER, class DETECT>
	class sdc_io {
	public:
		typedef CSI csi_type;
		typedef fatfs::mmc_io<CSI, SELECT> mmc_type;

	private:
		CSI&	csi_;

		fatfs::mmc_io<CSI, SELECT> mmc_;

		FATFS	fatfs_;  ///< FatFS コンテキスト

		uint8_t	mount_delay_ = 0;
		bool	cd_ = false;
		bool	mount_ = false;

		// CSI 開始
		// ※SD カードのアクセスでは、「PHASE::TYPE4」を選択する。
		void start_csi_()
		{
			uint8_t intr_level = 0;
			if(!csi_.start(16000000, CSI::PHASE::TYPE4, intr_level)) {
				format("CSI Start fail ! (Clock spped over range)\n");
			}
		}

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief	コンストラクター
		 */
		//-----------------------------------------------------------------//
		sdc_io(CSI& csi) : csi_(csi), mmc_(csi_) { }


		//-----------------------------------------------------------------//
		/*!
			@brief	初期化
		 */
		//-----------------------------------------------------------------//
		void initialize()
		{
			SELECT::PMC = 0;
			SELECT::PM = 0;
			SELECT::P = 0;  // 電源ＯＦＦ時、「０」にしておかないと電流が回り込む

			POWER::PMC = 0;
			POWER::PM = 0;
			POWER::P = 1; 

			DETECT::PM = 1;  // input
			DETECT::POM = 0;
			DETECT::PU = 1;  // pull-up

			// CSI を初期化後、廃棄する事で関係ポートを初期化する。
			start_csi_();
			csi_.destroy();

			mount_ = false;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	SD カードのディレクトリーをリストする
			@param[in]	root	ルート・パス
			@return ファイル数
		 */
		//-----------------------------------------------------------------//
		uint16_t dir(const char* root)
		{
			uint16_t num = 0;
			DIR dir;
			auto st = f_opendir(&dir, root);
			if(st != FR_OK) {
				format("Can't open dir(%d): '%s'\n") % static_cast<uint32_t>(st) % root;
			} else {
				for(;;) {
					FILINFO fno;
					// Read a directory item
					if(f_readdir(&dir, &fno) != FR_OK) {
						format("Can't read dir\n");
						break;
					}
					if(!fno.fname[0]) break;
						if(fno.fattrib & AM_DIR) {
						format("          /%s\n") % fno.fname;
					} else {
						format("%8d  %s\n") % static_cast<uint32_t>(fno.fsize) % fno.fname;
					}
					++num;
				}
			}
			return num;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	SD カードアクセス・サービス（毎フレーム呼ぶ）
			@return マウントしている場合「true」
		 */
		//-----------------------------------------------------------------//
		bool service()
		{
			auto st = !DETECT::P();
			if(!cd_ && st) {
				start_csi_();
				mount_delay_ = 10;  // 10 フレーム後にマウントする
				POWER::P = 0;
				SELECT::P = 1;
//				format("Card ditect\n");
			} else if(cd_ && !st) {
				f_mount(&fatfs_, "", 0);
				csi_.destroy();
				POWER::P = 1;
				SELECT::P = 0;
//				format("Card unditect\n");
			}
			cd_ = st;

			if(mount_delay_) {
				--mount_delay_;
				if(mount_delay_ == 0) {
					auto st = f_mount(&fatfs_, "", 1);
					if(st != FR_OK) {
						format("Mount fail: %d\n") % static_cast<uint32_t>(st);
						csi_.destroy();
						POWER::P = 1;
						SELECT::P = 0;
						mount_ = false;
					} else {
						mount_ = true;
					}
				}
			}
			return mount_;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	MMC クラスを参照で返す
			@return MMC クラス
		 */
		//-----------------------------------------------------------------//
		mmc_type& at_mmc() { return mmc_; }
	};
}
