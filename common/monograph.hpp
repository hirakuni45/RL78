#pragma once
//=====================================================================//
/*!	@file
	@brief	モノクロ・グラフィックス・クラス
	@author	平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include <cstdint>

namespace graphics {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	ASCII 無効フォント定義
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	class afont_null {
	public:
		static const uint8_t width = 0;
		static const uint8_t height = 0;
		static const uint8_t* get(uint8_t code) { return nullptr; }
		static const int8_t get_width(uint8_t code) { return 0; }
	};


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	漢字 無効フォント定義
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	class kfont_null {
	public:
		static const uint8_t width = 0;
		static const uint8_t height = 0;
		const uint8_t* get(uint16_t code) { return nullptr; }
	};


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	ビットマップ描画クラス
		@param[in]	WIDTH	横幅
		@param[in]	HEIGHT	高さ
		@param[in]	AFONT	ASCII フォント・クラス
		@param[in]	KFONT	漢字フォントクラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <uint16_t WIDTH, uint16_t HEIGHT, class AFONT = afont_null, class KFONT = kfont_null>
	class monograph {

		uint8_t	fb_[WIDTH * HEIGHT / 8];

		KFONT& kfont_;
	public:
		//-----------------------------------------------------------------//
		/*!
			@brief	コンストラクター
		*/
		//-----------------------------------------------------------------//
		monograph(KFONT& kf) : kfont_(kf) { }


		//-----------------------------------------------------------------//
		/*!
			@brief	横幅の取得
			@return 横幅
		*/
		//-----------------------------------------------------------------//
		uint16_t get_width() const { return WIDTH; }


		//-----------------------------------------------------------------//
		/*!
			@brief	高さの取得
			@return 高さ
		*/
		//-----------------------------------------------------------------//
		uint16_t get_height() const { return HEIGHT; }


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
			if(static_cast<uint16_t>(x) >= WIDTH) return;
			if(static_cast<uint16_t>(y) >= HEIGHT) return;
#ifdef LED16X16
			fb_[((x & 8) >> 3) + (y << 1)] |= (1 << (x & 7));
#else
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
			if(static_cast<uint16_t>(x) >= WIDTH) return;
			if(static_cast<uint16_t>(y) >= HEIGHT) return;
#ifdef LED16X16
			fb_[((x & 8) >> 3) + (y << 1)] &= ~(1 << (x & 7));
#else
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
		void fill(int16_t x, int16_t y, int16_t w, int16_t h, bool c) {
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
			@brief	全画面クリアをする（高速）
			@param[in]	c	クリアカラー
		*/
		//-----------------------------------------------------------------//
		void flash(uint8_t c) {
			for(uint16_t i = 0; i < (WIDTH * HEIGHT / 8); ++i) {
				fb_[i] = c;
			}
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	全画面クリアをする
			@param[in]	c	クリアカラー
		*/
		//-----------------------------------------------------------------//
		void clear(bool c) {
			fill(0, 0, WIDTH, HEIGHT, c);
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
		void line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, bool c) {
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
		void frame(int16_t x, int16_t y, int16_t w, int16_t h, bool c) {
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
			if(img == nullptr) return;
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
			@brief	フォントを描画する。
			@param[in]	x	開始点Ｘ軸を指定
			@param[in]	y	開始点Ｙ軸を指定
			@param[in]	code	コード
		*/
		//-----------------------------------------------------------------//
		void draw_font(int16_t x, int16_t y, uint16_t code) {
			if(code < 0x80) {
				if(-AFONT::width >= x || static_cast<uint16_t>(x) >= WIDTH) {
					return;
				}
				draw_image(x, y, AFONT::get(code), AFONT::width, AFONT::height);
			} else {
				if(-KFONT::width >= x || static_cast<uint16_t>(x) >= WIDTH) {
					return;
				}
				draw_image(x, y, kfont_.get(code), KFONT::width, KFONT::height);
			}
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	ascii テキストを描画する。
			@param[in]	x	開始点Ｘ軸を指定
			@param[in]	y	開始点Ｙ軸を指定
			@param[in]	text	テキストのポインター
			@param[in]	prop	プロポーショナルの場合「true」
			@return 文字の終端座標（Ｘ）
		*/
		//-----------------------------------------------------------------//
		int16_t draw_text(int16_t x, int16_t y, const char* text, bool prop = false) {
			int8_t cnt = 0;
			uint16_t code = 0;
			char tc;
			while((tc = *text++) != 0) {
				uint8_t c = static_cast<uint8_t>(tc);
				if(c < 0x80) {
					draw_font(x, y, tc);
					if(prop) x += AFONT::get_width(code);
					else x += AFONT::width;
					code = 0;
				} else if((c & 0xf0) == 0xe0) {
					code = (c & 0x0f);
					cnt = 2;
				} else if((c & 0xe0) == 0xc0) {
					code = (c & 0x1f);
					cnt = 1;
				} else if((c & 0xc0) == 0x80) {
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
					draw_font(x, y, code);
					x += KFONT::width;
					code = 0;
				}			
			}
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
