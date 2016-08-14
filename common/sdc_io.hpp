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
		static const int path_buff_size_ = 256;

		CSI&	csi_;

		fatfs::mmc_io<CSI, SELECT> mmc_;

		FATFS	fatfs_;  ///< FatFS コンテキスト

		uint8_t	mount_delay_;
		bool	cd_;
		bool	mount_;

		char	current_[path_buff_size_];

		// CSI 開始
		// ※SD カードのアクセスでは、「PHASE::TYPE4」を選択する。
		void start_csi_()
		{
			uint8_t intr_level = 0;
			if(!csi_.start(16000000, CSI::PHASE::TYPE4, intr_level)) {
				format("CSI Start fail ! (Clock spped over range)\n");
			}
		}

		
		static void dir_list_func_(const char* name, uint32_t size, bool dir) {
			if(dir) {
				format("          /%s\n") % name;
			} else {
				format("%8d  %s\n") % size % name;
			}
		}


		void create_full_path_(char* full, const char* path) {
			std::strcpy(full, current_);
			if(path == nullptr || path[0] == 0) {
				if(full[0] == 0) {
					std::strcpy(full, "/");
				}
			} else if(std::strcmp(path, "..") == 0) {
				char* p = std::strrchr(full, '/');
				if(p != nullptr) {
					*p = 0;
				}
			} else if(path[0] == '/') {
				std::strcpy(full, path);
			} else {
				std::strcat(full, "/");
				std::strcat(full, path);
			}
		}

	public:
///		typedef std::function<void (const char* name, uint32_t size, bool dir)> dir_loop_func;
		typedef void (*dir_loop_func)(const char* name, uint32_t size, bool dir);


		//-----------------------------------------------------------------//
		/*!
			@brief	コンストラクター
		 */
		//-----------------------------------------------------------------//
		sdc_io(CSI& csi) : csi_(csi), mmc_(csi_),
			mount_delay_(0), cd_(false), mount_(false) { }


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

			current_[0] = 0;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	カレント・パスの移動
			@param[in]	path	相対パス、又は、絶対パス
			@return 移動成功なら「true」
		 */
		//-----------------------------------------------------------------//
		bool cd(const char* path)
		{
			if(!mount_) return false;

			if(path == nullptr) return false;

			char full[path_buff_size_];
			create_full_path_(full, path);

			DIR dir;
			auto st = f_opendir(&dir, full);
			if(st != FR_OK) {
				return false;
			}
			std::strcpy(current_, full);

			f_closedir(&dir);
			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	SD カードのディレクトリーを取り、タスクを実行する
			@param[in]	root	ルート・パス
			@param[in]	func	実行関数
			@return ファイル数
		 */
		//-----------------------------------------------------------------//
		uint16_t dir_loop(const char* root, dir_loop_func func)
		{
			if(!mount_) return 0;

			char full[path_buff_size_];
			create_full_path_(full, root);			

			uint16_t num = 0;
			DIR dir;
			auto st = f_opendir(&dir, full);
			if(st != FR_OK) {
				format("Can't open dir(%d): '%s'\n") % static_cast<uint32_t>(st) % root;
			} else {
				std::strcat(full, "/");
				char* p = &full[std::strlen(full)];
				for(;;) {
					FILINFO fno;
					// Read a directory item
					if(f_readdir(&dir, &fno) != FR_OK) {
						format("Can't read dir\n");
						break;
					}
					if(!fno.fname[0]) break;
					std::strcpy(p, fno.fname);
					if(fno.fattrib & AM_DIR) {
						func(p, static_cast<uint32_t>(fno.fsize), true);
					} else {
						func(p, static_cast<uint32_t>(fno.fsize), false);
					}
					++num;
				}
				f_closedir(&dir);
			}
			return num;
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
			return dir_loop(root, dir_list_func_);
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
				mount_ = false;
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
						current_[0] = 0;
						mount_ = true;
					}
				}
			}
			return mount_;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	カレント・パスを取得
			@return カレント。パス
		 */
		//-----------------------------------------------------------------//
		const char* get_current() const { return current_; }


		//-----------------------------------------------------------------//
		/*!
			@brief	カードのマウント状態を取得
			@return 「true」ならマウント状態
		 */
		//-----------------------------------------------------------------//
		bool get_mount() const { return mount_; }


		//-----------------------------------------------------------------//
		/*!
			@brief	MMC クラスを参照で返す
			@return MMC クラス
		 */
		//-----------------------------------------------------------------//
		mmc_type& at_mmc() { return mmc_; }
	};
}
