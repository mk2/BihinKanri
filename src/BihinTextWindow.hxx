#ifndef _BIHIN_TEXT_WINDOW_H_
#define _BIHIN_TEXT_WINDOW_H_

#include <string>
#include <vector>
#include <iostream>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Text_Display.H>
#include "common.hxx"
using namespace std;

/*!
 * \brief 備品情報を一覧表示するためのウィンドウ
 * \author OKA
 */
class BihinTextWindow: public Fl_Window {

	public:
		//! コンストラクタ
		BihinTextWindow(const string &content,int x, int y, int w = 400, int h = 300, const char *t = "備品情報");
		//! デストラクタ
		~BihinTextWindow() {
			delete textDisplay;
			delete textBuffer;
			delete closeButton;
		};
	
	public:
		//! closeButton用コールバック
		static void closeCallback(Fl_Widget *w, void *data) {
			BihinTextWindow *btw = static_cast<BihinTextWindow *>(data);
			btw->hide();
		}
		
	private:
		//! テキスト表示エリア
		Fl_Text_Display *textDisplay;
		//! テキストバッファ
		Fl_Text_Buffer *textBuffer;
		//! 閉じるボタン
		Fl_Button *closeButton;

};

#endif