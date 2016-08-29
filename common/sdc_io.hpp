#pragma once
//=====================================================================//
/*!	@file
	@brief	RL78/G13 グループ SD カード・アクセス制御 @n
			Copyright 2016 Kunihito Hiramatsu
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include <cstring>
#include "common/port_utils.hpp"
#include "ff12a/mmc_io.hpp"
#include "common/format.hpp"
#include "common/time.h"

namespace utils {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  １６進数ダンプ
		@param[in]	src	ソース
		@param[in]	num	数
		@param[in]	lin	１行辺りの表示数
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	static void hex_dump(char* src, uint16_t num, uint8_t lin)
	{
		uint8_t l = 0;
		for(uint16_t i = 0; i < num; ++i) {
			auto n = *src++;
			utils::format("%02X") % static_cast<uint16_t>(n);
			++l;
			if(l == lin) {
				utils::format("\n");
				l = 0;
			} else {
				utils::format(" ");
			}
		}
	}


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  UTF-8 から UTF-16 への変換
		@param[in]	src	ソース
		@param[in]	dst	変換先
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	static void utf8_to_utf16(const char* src, WCHAR* dst)
	{
		uint8_t cnt = 0;
		uint16_t code = 0;
		char tc;
		while((tc = *src++) != 0) {
			uint8_t c = static_cast<uint8_t>(tc);
			if(c < 0x80) { code = c; cnt = 0; }
			else if((c & 0xf0) == 0xe0) { code = (c & 0x0f); cnt = 2; }
			else if((c & 0xe0) == 0xc0) { code = (c & 0x1f); cnt = 1; }
			else if((c & 0xc0) == 0x80) {
				code <<= 6;
				code |= c & 0x3f;
				cnt--;
				if(cnt == 0 && code < 0x80) {
					code = 0;	// 不正なコードとして無視
					break;
				} else if(cnt < 0) {
					code = 0;
				}
			}
			if(cnt == 0 && code != 0) {
				*dst++ = code;
				code = 0;
			}
		}
		*dst = 0;
	}


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  UTF-16 から UTF-8 への変換
		@param[in]	code	UTF-16 コード
		@param[in]	dst	変換先
		@return 変換先の最終ポインター
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	static char* utf16_to_utf8(uint16_t code, char* dst)
	{
		if(code < 0x0080) {
			*dst++ = code;
		} else if(code >= 0x0080 && code <= 0x07ff) {
			*dst++ = 0xc0 | ((code >> 6) & 0x1f);
			*dst++ = 0x80 | (code & 0x3f);
		} else if(code >= 0x0800) {
			*dst++ = 0xe0 | ((code >> 12) & 0x0f);
			*dst++ = 0x80 | ((code >> 6) & 0x3f);
			*dst++ = 0x80 | (code & 0x3f);
		}
		return dst;
	}


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  UTF-16 から UTF-8 への変換
		@param[in]	src	ソース
		@param[in]	dst	変換先
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	static void utf16_to_utf8(const WCHAR* src, char* dst) 
	{
		uint16_t code;
		while((code = static_cast<uint16_t>(*src++)) != 0) {
			dst = utf16_to_utf8(code, dst);
		}
		*dst = 0;
	}

#if _USE_LFN != 0
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  sjis から UTF-8 への変換
		@param[in]	src	ソース
		@param[in]	dst	変換先
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	static void sjis_to_utf8(const char* src, char* dst)
	{
		if(src == nullptr) return;
		uint16_t wc = 0;
		char ch;
		while((ch = *src++) != 0) {
			uint8_t c = static_cast<uint8_t>(ch);
			if(wc) {
				if(0x40 <= c && c <= 0x7e) {
					wc <<= 8;
					wc |= c;
					dst = utf16_to_utf8(ff_convert(wc, 1), dst);
				} else if(0x80 <= c && c <= 0xfc) {
					wc <<= 8;
					wc |= c;
					dst = utf16_to_utf8(ff_convert(wc, 1), dst);
				}
				wc = 0;
			} else {
				if(0x81 <= c && c <= 0x9f) wc = c;
				else if(0xe0 <= c && c <= 0xfc) wc = c;
				else {
					dst = utf16_to_utf8(ff_convert(c, 1), dst);
				}
			}
		}
		*dst = 0;
	}


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  UTF-8 から sjis への変換
		@param[in]	src	ソース
		@param[in]	dst	変換先
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	static void utf8_to_sjis(const char* src, char* dst)
	{
		int8_t cnt = 0;
		uint16_t code = 0;
		char tc;
		while((tc = *src++) != 0) {
			uint8_t c = static_cast<uint8_t>(tc);
			if(c < 0x80) { *dst++ = tc; code = 0; }
			else if((c & 0xf0) == 0xe0) { code = (c & 0x0f); cnt = 2; }
			else if((c & 0xe0) == 0xc0) { code = (c & 0x1f); cnt = 1; }
			else if((c & 0xc0) == 0x80) {
				code <<= 6;
				code |= c & 0x3f;
				cnt--;
				if(cnt == 0 && code < 0x80) {
					code = 0;	// 不正なコードとして無視
					break;
				} else if(cnt < 0) {
					code = 0;
				}
			}
			if(cnt == 0 && code != 0) {
				auto wc = ff_convert(code, 0);
				*dst++ = static_cast<char>(wc >> 8);
				*dst++ = static_cast<char>(wc & 0xff);
				code = 0;
			}			
		}
		*dst = 0;
	}
#endif

	//-----------------------------------------------------------------//
	/*!
		@brief	FatFS が使う時間取得関数
		@return	FatFS 形式の時刻ビットフィールドを返す。@n
		        現在のローカル・タイムがDWORD値にパックされて返されます。@n
				ビット・フィールドは次に示すようになります。@n
				bit31:25 ---> 1980年を起点とした年が 0..127 で入ります。@n
				bit24:21 ---> 月が 1..12 の値で入ります。@n
				bit20:16 ---> 日が 1..31 の値で入ります。@n
				bit15:11 ---> 時が 0..23 の値で入ります。@n
				bit10:5 ---> 分が 0..59 の値で入ります。@n
				bit4:0 ---> 秒/2が 0..29 の値で入ります。
	*/
	//-----------------------------------------------------------------//
	inline DWORD get_fattime(time_t t)
	{
//		t = 0;	/* 1970 01-01 00:00:00 */
		struct tm *tp = localtime(&t);

#ifdef _DEBUG
		format("get_fattime: (source) %4d/%d/%d/ %02d:%02d:%02d\n")
			% (tp->tm_year + 1900) % (tp->tm_mon + 1) % tp->tm_mday
			% tp->tm_hour % tp->tm_min % tp->tm_sec;
#endif

		DWORD tt = 0;
		tt |= (DWORD)(tp->tm_sec) >> 1;
		tt |= (DWORD)tp->tm_min  << 5;
		tt |= (DWORD)tp->tm_hour << 11;

		tt |= (DWORD)tp->tm_mday << 16;
		tt |= ((DWORD)(tp->tm_mon) + 1) << 21;
		if(tp->tm_year >= 80) {
			tt |= ((DWORD)(tp->tm_year) - 80) << 25;
		}

		return tt;
	}


	//-----------------------------------------------------------------//
	/*!
		@brief	FatFS が扱う時間値から、time_t への変換
		@param[in]	date	日付
		@param[in]	time	時間
		@return 標準時間値
	*/
	//-----------------------------------------------------------------//
	inline time_t fatfs_time_to(WORD date, WORD time)
	{
		struct tm ttm;
/* ftime
		ファイルの変更された時刻、またはディレクトリの作成された時刻が格納されます。
		bit15:11 ---> 時が 0..23 の値で入ります。
		bit10:5  ---> 分が 0..59 の値で入ります。
		bit4:0   ---> 秒/2が 0..29 の値で入ります。
*/
		ttm.tm_sec  =  time & 0x1f;
		ttm.tm_min  = (time >> 5) & 0x3f;
		ttm.tm_hour = (time >> 11) & 0x1f;

/* fdate
		ファイルの変更された日付、またはディレクトリの作成された日付が格納されます。
		bit15:9 ---> 1980年を起点とした年が 0..127 で入ります。
		bit8:5  ---> 月が 1..12 の値で入ります。
		bit4:0  ---> 日が 1..31 の値で入ります。
*/
		ttm.tm_mday =  date & 0x1f;
		ttm.tm_mon  = ((date >> 5) & 0xf) - 1;
		ttm.tm_year = ((date >> 9) & 0x7f) + 1980 - 1900;

		return mktime(&ttm);
	}


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
		uint8_t	select_wait_;
		bool	cd_;
		bool	mount_;

		char	current_[path_buff_size_];

		static void dir_list_func_(const char* name, const FILINFO* fi, bool dir) {
			if(fi == nullptr) return;

			time_t t = fatfs_time_to(fi->fdate, fi->ftime);
			if(dir) {
				format("          /%s\n") % name;
			} else {
				format("%8d   %s\n") % fi->fsize % name;
			}
		}

		static const char* match_key_;
		static char* match_dst_;
		static uint8_t match_cnt_;
		static uint8_t match_no_;
		static void match_func_(const char* name, const FILINFO* fi, bool dir) {
			if(std::strncmp(name, match_key_, std::strlen(match_key_)) == 0) {
				if(match_dst_ != nullptr && match_cnt_ == match_no_) {
					std::strcpy(match_dst_, name);
				}
				++match_cnt_;
			}
		}

		void create_full_path_(const char* path, char* full) {
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
		typedef void (*dir_loop_func)(const char* name, const FILINFO* fi, bool dir);


		//-----------------------------------------------------------------//
		/*!
			@brief	コンストラクター
		 */
		//-----------------------------------------------------------------//
		sdc_io(CSI& csi) : csi_(csi), mmc_(csi_),
			mount_delay_(0), select_wait_(0), cd_(false), mount_(false) { }


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
			uint8_t intr_level = 0;
			if(!csi_.start(4000000, CSI::PHASE::TYPE4, intr_level)) {
				format("CSI Start fail ! (Clock spped over range)\n");
			}
			csi_.destroy();

			mount_ = false;

			current_[0] = 0;
			select_wait_ = 0;
			mount_delay_ = 0;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	カレント・ファイルのオープン
			@param[in]	fp		ファイル構造体ポインター
			@param[in]	path	ファイル名
			@param[in]	mode	オープン・モード
			@return 移動成功なら「true」
		 */
		//-----------------------------------------------------------------//
		bool open(FIL* fp, const char* path, BYTE mode)
		{
			char full[path_buff_size_];
			create_full_path_(path, full);

#if _USE_LFN != 0
			utf8_to_sjis(full, full);
#endif
			if(f_open(fp, full, mode) != FR_OK) {
				return false;
			}
			return true;
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
			create_full_path_(path, full);
#if _USE_LFN != 0
			char oem[path_buff_size_];
			utf8_to_sjis(full, oem);
			DIR dir;
			auto st = f_opendir(&dir, oem);
#else
			DIR dir;
			auto st = f_opendir(&dir, full);
#endif
			if(st != FR_OK) {
				format("Can't open dir(%d): '%s'\n") % static_cast<uint32_t>(st) % full;
				return false;
			}
			std::strcpy(current_, full);

			f_closedir(&dir);
			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	SD カードのディレクトリーリストでタスクを実行する
			@param[in]	root	ルート・パス
			@param[in]	func	実行関数
			@param[in]	recursive  「true」にすると再帰的にループを回す（スタックの消費に注意）
			@return ファイル数
		 */
		//-----------------------------------------------------------------//
		uint16_t dir_loop(const char* root, dir_loop_func func, bool recursive = false)
		{
			char full[path_buff_size_];
#if _USE_LFN != 0
			char fn[64];
#endif
			DIR dir;
			FILINFO fi;

			if(!mount_) return 0;

			create_full_path_(root, full);
#if _USE_LFN != 0
			utf8_to_sjis(full, full);
#endif
			uint16_t num = 0;
			auto st = f_opendir(&dir, full);
			if(st != FR_OK) {
				format("Can't open dir(%d): '%s'\n") % static_cast<uint32_t>(st) % full;
			} else {
				std::strcat(full, "/");
				char* p = &full[std::strlen(full)];
				for(;;) {
					// Read a directory item
					if(f_readdir(&dir, &fi) != FR_OK) {
						format("Can't read dir\n");
						break;
					}
					if(!fi.fname[0]) break;
#if _USE_LFN != 0
					sjis_to_utf8(fi.fname, fn);
					std::strcpy(p, fn);
#else
					std::strcpy(p, fi.fname);
#endif
					if(fi.fattrib & AM_DIR) {
						if(recursive) {
							func(p, &fi, true);
						}
					} else {
						func(p, &fi, false);
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
			return dir_loop(root, dir_list_func_, true);
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	ファイル名候補の取得
			@param[in]	name	候補のキー
			@param[in]	no		候補の順番
			@param[out]	dst		候補の格納先
			@return 候補の数
		 */
		//-----------------------------------------------------------------//
		uint8_t match(const char* key, uint8_t no, char* dst)
		{
			match_key_ = key;
			match_dst_ = dst;
			match_cnt_ = 0;
			match_no_ = no;
			dir_loop(current_, match_func_, false);
			return match_cnt_;
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
			if(st) {
				if(select_wait_ < 255) {
					++select_wait_;
				}
			} else {
				select_wait_ = 0;
			}
			if(!cd_ && select_wait_ >= 10) {
				mount_delay_ = 30;  // 30 フレーム後にマウントする
				POWER::P = 0;
				SELECT::P = 1;
//				format("Card ditect\n");
			} else if(cd_ && select_wait_ == 0) {
				f_mount(&fatfs_, "", 0);
				csi_.destroy();
				POWER::P = 1;
				SELECT::P = 0;
				mount_ = false;
//				format("Card unditect\n");
			}
			if(select_wait_ >= 10) cd_ = true;
			else cd_ = false;

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
			@brief	フル・パスを生成
			@param[in]	name	ファイル名
			@param[out]	dst		生成先（オーバーランに注意）
		 */
		//-----------------------------------------------------------------//
		void make_full_path(const char* name, char* dst) const {
			if(name == nullptr || dst == nullptr) return;
			std::strcpy(dst, current_);
			if(dst[std::strlen(dst)] != '/') {
				std::strcat(dst, "/");
			}
			std::strcat(dst, name);
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

	template <class CSI, class SELECT, class POWER, class DETECT>
		const char* sdc_io<CSI, SELECT, POWER, DETECT>::match_key_ = nullptr; 

	template <class CSI, class SELECT, class POWER, class DETECT>
		char* sdc_io<CSI, SELECT, POWER, DETECT>::match_dst_ = nullptr; 

	template <class CSI, class SELECT, class POWER, class DETECT>
		uint8_t sdc_io<CSI, SELECT, POWER, DETECT>::match_cnt_ = 0;

	template <class CSI, class SELECT, class POWER, class DETECT>
		uint8_t sdc_io<CSI, SELECT, POWER, DETECT>::match_no_ = 0;
}
