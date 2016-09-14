#pragma once
//=====================================================================//
/*!	@file
	@brief	ビットマップ・グラフィックス用ファイル選択クラス
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include "common/monograph.hpp"
#include "common/sdc_io.hpp"

namespace graphics {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	ファイラー・クラス
		@param[in]	SDC_IO	SDC I/O クラスの型
		@param[in]	BITMAP	ビットマップの型
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class SDC_IO, class BITMAP>
	class filer {

		SDC_IO& sdc_;
		BITMAP&	bitmap_;

		const char* root_;
		uint16_t	file_num_;
		uint16_t	file_pos_;

		bool		enable_;

		struct option_t {
			BITMAP& bmp;
			int16_t y;
			option_t(BITMAP& bitmap) : bmp(bitmap), y(0) { }
		};

		static void dir_(const char* fname, const FILINFO* fi, bool dir, void* option) {
			option_t* t = reinterpret_cast<option_t*>(option);
			if(t->y < 0 || t->y >= static_cast<int16_t>(t->bmp.get_height())) return;
			t->bmp.draw_text(0, t->y, fname);
			t->y += t->bmp.get_kfont_height();
		}

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief	コンストラクター
			@param[in]	sdc	sdc_io クラス参照
			@param[in]	bitmap	ビットマップ・クラス参照
		*/
		//-----------------------------------------------------------------//
		filer(SDC_IO& sdc, BITMAP& bitmap) : sdc_(sdc), bitmap_(bitmap),
			root_(nullptr), file_num_(0), file_pos_(0),
			enable_(false) { }


		//-----------------------------------------------------------------//
		/*!
			@brief	表示許可の取得
			@return 表示許可状態
		*/
		//-----------------------------------------------------------------//
		bool get_enable() const { return enable_; }


		//-----------------------------------------------------------------//
		/*!
			@brief	表示許可
			@param[in]	ena	不許可の場合「false」
		*/
		//-----------------------------------------------------------------//
		void enable(bool ena = true) { enable_ = ena; }


		//-----------------------------------------------------------------//
		/*!
			@brief	ルート・ディレクトリーを設定
			@param[in]	root	ルート・パス
		*/
		//-----------------------------------------------------------------//
		void set_root(const char* root) {
			root_ = root;
			file_num_ = sdc_.dir_loop(root, nullptr, true);
			file_pos_ = 0;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	サービス 表示、毎フレーム呼ぶ
		*/
		//-----------------------------------------------------------------//
		void service() {
			option_t opt(bitmap_);

			if(!enable_) return;
			if(root_ == nullptr) return;
			if(file_num_ == 0) return;

			sdc_.dir_loop(root_, dir_, true, &opt);
		}

	};
}
