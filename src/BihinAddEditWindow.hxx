#ifndef _BIHIN_ADD_EDIT_WINDOW_H_
#define _BIHIN_ADD_EDIT_WINDOW_H_

#include <string>
#include <iostream>
#include <vector>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Browser.H>
#include <FL/Fl_Hold_Browser.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Box.H>
#include "BihinModel.hxx"
#include "common.hxx"
using namespace std;

class BihinAddEditWindow: public Fl_Window {

	public:
		//! コンストラクタ
		//! bihinNameがNULLでは無い場合->エディットモード
		BihinAddEditWindow(int x_pos, int y_pos, BihinModel *model, const char *bihinName = "", int w = 360, int h = 340, const char *t = "備品追加/備品サイズ変更");
		//! デストラクタ
		~BihinAddEditWindow() {};
	
	public:
		//! クローズボタン用コールバック
		static void closeCallback(Fl_Widget *w, void *data) {
			BihinAddEditWindow *baew = static_cast<BihinAddEditWindow *>(data);
			baew->hide();
		}
		//! サイズ追加用コールバック
		static void addSizeCallback(Fl_Widget *w, void *data) {
			BihinAddEditWindow *baew = static_cast<BihinAddEditWindow *>(data);
			baew->addSize();
		}
		//! サイズ削除用コールバック
		static void deleteSizeCallback(Fl_Widget *w, void *data) {
			BihinAddEditWindow *baew = static_cast<BihinAddEditWindow *>(data);
			baew->deleteSize();
		}
		//! 備品追加用コールバック
		static void addBihinCallback(Fl_Widget *w, void *data) {
			BihinAddEditWindow *baew = static_cast<BihinAddEditWindow *>(data);
			baew->addBihin();
		}
		
	private:
		//! サイズ追加
		void addSize();
		//! サイズ削除
		void deleteSize();
		//! 備品追加
		void addBihin();
		
	private:
		string name;
		BihinModel *model;
		Fl_Box *nameLabel;
		Fl_Input *nameInput;
		
		Fl_Box *sizeLabel;
		Fl_Input *sizeInput;
		Fl_Button *addSizeButton;
		Fl_Button *deleteSizeButton;
		
		Fl_Box *sizeListLabel;
		Fl_Browser *sizeBrowser;
		
		Fl_Button *addBihinButton;
		Fl_Button *cancelButton;
		
};

#endif