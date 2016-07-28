#pragma once
//=====================================================================//
/*!	@file
	@brief	モノクロ・グラフィックス・クラス
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include <cstdint>

// LCD 128x64 の場合
#define LCD128X64

// LCD 128x32 の場合
//#define LCD128X32

// 16x16 dot matrix LED
//#define LED16X16

// 漢字フォントを表示させる場合有効にする
// #define KANJI_FONTS

namespace graphics {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	ビットマップ描画クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	class monograph {

#ifdef LCD128X64
		static const uint16_t fb_width_  = 128;
		static const uint16_t fb_height_ = 64;
#endif
#ifdef LCD128X32
		static const uint16_t fb_width_  = 128;
		static const uint16_t fb_height_ = 32;
#endif
#ifdef LED16X16
		static const uint16_t fb_width_  = 16;
		static const uint16_t fb_height_ = 16;
#endif
		uint8_t	fb_[fb_width_ * fb_height_ / 8];
		uint8_t	multi_byte_hi_;
		int8_t	font_width_;
		int8_t	font_height_;

#ifdef KANJI_FONTS
		// KANJI フォントをキャッシュする数
		static const int kanji_cash_size_ = 16;

		struct KANJI_CASH {
			uint8_t	sjis_hi;
			uint8_t	sjis_lo;
			uint8_t	bitmap[18];
		};
		KANJI_CASH kanji_cash_[kanji_cash_size_];
		uint8_t cash_first_;
#endif

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief	コンストラクター
		*/
		//-----------------------------------------------------------------//
		monograph() { }


		//-----------------------------------------------------------------//
		/*!
			@brief	初期化
		*/
		//-----------------------------------------------------------------//
		void init() {
#ifdef KANJI_FONTS
			for(uint8_t i = 0; i < KANJI_CASH_SIZE; ++i) {
				kanji_cash_[i].sjis_lo = 0;
				kanji_cash_[i].sjis_hi = 0;
			}
			cash_first_ = 0;
#endif
#ifdef LCD128X64
			font_width_ = 6;
			font_height_ = 12;
#endif
			multi_byte_hi_ = 0;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	横幅の取得
			@return 横幅
		*/
		//-----------------------------------------------------------------//
		uint16_t get_width() const { return fb_width_; }


		//-----------------------------------------------------------------//
		/*!
			@brief	高さの取得
			@return 高さ
		*/
		//-----------------------------------------------------------------//
		uint16_t get_height() const { return fb_height_; }


		//-----------------------------------------------------------------//
		/*!
			@brief	フレームバッファのアドレスを返す
			@return フレーム・バッファ・アドレス
		*/
		//-----------------------------------------------------------------//
		const uint8_t* fb() const { return fb_; }


		//-----------------------------------------------------------------//
		/*!
			@brief	点を描画する
			@param[in]	x	開始点Ｘ軸を指定
			@param[in]	y	開始点Ｙ軸を指定
		*/
		//-----------------------------------------------------------------//
		void point_set(int16_t x, int16_t y) {
			if(static_cast<uint16_t>(x) >= fb_width_) return;
			if(static_cast<uint16_t>(y) >= fb_height_) return;
#ifdef LED16X16
			fb_[((x & 8) >> 3) + (y << 1)] |= (1 << (x & 7));
#endif
#ifdef LCD128X64
			fb_[((y & 0xf8) << 4) + x] |= (1 << (y & 7));
#endif
#ifdef LCD128X32
			fb_[((y & 0xf8) << 4) + x] |= (1 << (y & 7));
#endif
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	点を消去する
			@param[in]	x	開始点Ｘ軸を指定
			@param[in]	y	開始点Ｙ軸を指定
		*/
		//-----------------------------------------------------------------//
		void point_reset(int16_t x, int16_t y) {
			if(static_cast<uint16_t>(x) >= fb_width_) return;
			if(static_cast<uint16_t>(y) >= fb_height_) return;
#ifdef LED16X16
			fb_[((x & 8) >> 3) + (y << 1)] &= ~(1 << (x & 7));
#endif
#ifdef LCD128X64
			fb_[((y & 0xf8) << 4) + x] &= ~(1 << (y & 7));
#endif
#ifdef LCD128X32
			fb_[((y & 0xf8) << 4) + x] &= ~(1 << (y & 7));
#endif
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	四角を塗りつぶす
			@param[in]	x	開始位置 X
			@param[in]	y	開始位置 Y
			@param[in]	w	横幅 
			@param[in]	h	高さ
			@param[in]	c	カラー
		*/
		//-----------------------------------------------------------------//
		void fill(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t c) {
			if(c) {
				for(int16_t i = y; i < (y + h); ++i) {
					for(int16_t j = x; j < (x + w); ++j) {
						point_set(j, i);
					}
				}
			} else {
				for(int16_t i = y; i < (y + h); ++i) {
					for(int16_t j = x; j < (x + w); ++j) {
						point_reset(j, i);
					}
				}
			}
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	全画面クリアをする
			@param[in]	c	クリアカラー 0:クリア 0!=:塗る
		*/
		//-----------------------------------------------------------------//
		void clear(uint8_t c) {
			fill(0, 0, fb_width_, fb_height_, c);
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	線を描画する
			@param[in]	x1	開始点Ｘ軸を指定
			@param[in]	y1	開始点Ｙ軸を指定
			@param[in]	x2	終了点Ｘ軸を指定
			@param[in]	y2	終了点Ｙ軸を指定
			@param[in]	c	描画色
		*/
		//-----------------------------------------------------------------//
		void line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t c) {
			int16_t dx;
			int16_t dy;
			int16_t sx;
			int16_t sy;
			if(x2 >= x1) { dx = x2 - x1; sx = 1; } else { dx = x1 - x2; sx = -1; }
			if(y2 >= y1) { dy = y2 - y1; sy = 1; } else { dy = y1 - y2; sy = -1; }

			int16_t m = 0;
			int16_t x = x1;
			int16_t y = y1;
			if(dx > dy) {
				for(int16_t i = 0; i <= dx; i++) {
					if(c) point_set(x, y);
					else point_reset(x, y);
					m += dy;
					if(m >= dx) {
						m -= dx;
						y += sy;
					}
					x += sx;
				}
			} else {
				for(int16_t i = 0; i <= dy; i++) {
					if(c) point_set(x, y);
					else point_reset(x, y);
					m += dx;
					if(m >= dy) {
						m -= dy;
						x += sx;
					}
					y += sy;
				}
			}
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	フレームを描画する
			@param[in]	x	開始点Ｘ軸を指定
			@param[in]	y	開始点Ｙ軸を指定
			@param[in]	w	横幅
			@param[in]	h	高さ
			@param[in]	c	描画色
		*/
		//-----------------------------------------------------------------//
		void frame(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t c) {
			for(int16_t i = 0; i < w; ++i) {
				if(c) {
					point_set(x + i, y);
					point_set(x + i, y + h - 1);
				} else {
					point_reset(x + i, y);
					point_reset(x + i, y + h - 1);
				}
			}
			for(int16_t i = 0; i < h; ++i) {
				if(c) {
					point_set(x, y + i);
					point_set(x + w - 1, y + i);
				} else {
					point_reset(x, y + i);
					point_reset(x + w - 1, y + i);
				}
			}
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	ビットマップイメージを描画する
			@param[in]	x	開始点Ｘ軸を指定
			@param[in]	y	開始点Ｙ軸を指定
			@param[in]	img	描画ソースのポインター
			@param[in]	w	描画ソースの幅
			@param[in]	h	描画ソースの高さ
		*/
		//-----------------------------------------------------------------//
		void draw_image(int16_t x, int16_t y, const uint8_t* img, uint8_t w, uint8_t h) {
			uint8_t k = 1;
			uint8_t c = *img++;

			for(uint8_t i = 0; i < h; ++i) {
				int16_t xx = x;
				for(uint8_t j = 0; j < w; ++j) {
					if(c & k) point_set(xx, y);
					k <<= 1;
					if(k == 0) {
						k = 1;
						c = *img++;
					}
					++xx;
				}
				++y;
			}
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	モーションオブジェクトを描画する
			@param[in]	x	開始点Ｘ軸を指定
			@param[in]	y	開始点Ｙ軸を指定
			@param[in]	img	描画ソースのポインター
		*/
		//-----------------------------------------------------------------//
		void draw_mobj(int16_t x, int16_t y, const uint8_t* img) {
			uint8_t w = *img++;
			uint8_t h = *img++;
			draw_image(x, y, img, w, h);
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	ascii フォントを描画する。
			@param[in]	x	開始点Ｘ軸を指定
			@param[in]	y	開始点Ｙ軸を指定
			@param[in]	code	ASCII コード（0x00 to 0x7F)
		*/
		//-----------------------------------------------------------------//
		void draw_font(int16_t x, int16_t y, char code) {
#ifdef KANJI_FONTS
			if(multi_byte_hi_) {
				uint8_t hi = multi_byte_hi_;
				multi_byte_hi_ = 0;
				if(-KANJI_FONT_WIDTH >= x || x >= fb_width_) return;

				if(kanji_mode_ == 0) {
					draw_image(x - FONT_WIDTH, y, &font6x12_[9 * 18], 6, 12);
					draw_image(x, y, &font6x12_[9 * 19], 6, 12);
					return;
				}

				const uint8_t* bitmap = scan_kanji_bitmap(hi, static_cast<uint8_t>(code));
				if(bitmap != 0) {
					draw_image(x - FONT_WIDTH, y, bitmap, KANJI_FONT_WIDTH, KANJI_FONT_HWIGHT);
				} else {
					uint8_t* bitmap = alloc_kanji_bitmap(hi, static_cast<uint8_t>(code));
					uint16_t pos = sjis_to_liner(hi, static_cast<uint8_t>(code));
					if(read_kanji_bitmap(pos, bitmap)) {
						draw_image(x - FONT_WIDTH, y, bitmap, KANJI_FONT_WIDTH, KANJI_FONT_HEIGHT);
					} else {
						draw_image(x - FONT_WIDTH, y, &font6x12_[9 * 18], FONT_WIDTH, FONT_HEIGHT);
						draw_image(x, y, &font6x12_[9 * 19], FONT_WIDTH, FONT_HEIGHT);
					}
				}
			} else
#endif
			{
#ifdef LCD128X64
				if(code >= 0) {
					if(-FONT_WIDTH >= x || static_cast<uint16_t>(x) >= fb_width_) {
						return;
					}
					draw_image(x, y, &font6x12_[(code << 3) + code], FONT_WIDTH, FONT_HEIGHT);
				} else if(static_cast<uint8_t>(code) >= 0x81
					   && static_cast<uint8_t>(code) <= 0x9f) {
					multi_byte_hi_ = code;
				} else if(static_cast<uint8_t>(code) >= 0xe0
					   && static_cast<uint8_t>(code) <= 0xef) {
					multi_byte_hi_ = code;
				} else {
					// 無効キャラクターの意味として
					multi_byte_hi_ = 0;
					if(-FONT_WIDTH >= x || static_cast<uint16_t>(x) >= fb_width_) return;
					draw_image(x, y, &font6x12_[(1 << 3)], FONT_WIDTH, FONT_HEIGHT);
				}
#endif
			}
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	ascii テキストを描画する。
			@param[in]	x	開始点Ｘ軸を指定
			@param[in]	y	開始点Ｙ軸を指定
			@param[in]	text	テキストのポインター
			@return 文字の終端座標（Ｘ）
		*/
		//-----------------------------------------------------------------//
		int16_t draw_string(int16_t x, int16_t y, const char* text) {
			char code;
			while((code = *text++) != 0) {
				draw_font(x, y, code);
				x += font_width_;
			}
			multi_byte_hi_ = 0;
			return x;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	水平レベルを表示
			@param[in]	x	開始点Ｘ軸を指定
			@param[in]	y	開始点Ｙ軸を指定
			@param[in]	w	横幅
			@param[in]	h	高さ
			@param[in]	l	レベル
		*/
		//-----------------------------------------------------------------//
		void draw_holizontal_level(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t l) {
			frame(x, y, w, h, 1);
		  	if(w <= 2 || h <= 2) return;
			++x;
			h -= 2;
			++y;
			w -= 2;
			for(uint8_t j = 0; j < h; ++j) {
				for(uint8_t i = 0; i < w; ++i) {
					if(i < l) {
						if((i ^ j) & 1) {
							point_set(x + i, y + j);
						} else {
							point_reset(x + i, y + j);
						}
					} else if(i == l && i != 0) {
						point_set(x + i, y + j);
					} else {
						point_reset(x + i, y + j);
					}
				}
			}
		}
	};
}
