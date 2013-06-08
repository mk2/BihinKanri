#ifndef _BIHIN_WINDOW_H_
#define _BIHIN_WINDOW_H_

#include <string>
#include <iostream>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Sys_Menu_Bar.H>
#include <FL/Fl_Menu_Item.H>
#include <cstdlib>
#include "common.hxx"
#include "BihinModel.hxx"
#include "BihinTextWindow.hxx"
#include "BihinAddEditWindow.hxx"
using namespace std;

/*!
 * \brief メインウィンドウのクラス。コントローラーの役割も備えている
 * \author OKA
 */
class BihinWindow: public Fl_Window {
	public:
		//! コンストラクタ 親のコンストラクタも呼び出している
		BihinWindow(int w = 520, int h = 440, const char *t = "備品管理");
		//! デストラクタ 今回ウィンドウは1度の起動において1回しかnewしないので、メンバ変数の開放などは行わない
		~BihinWindow() {};

	public:
		//! rightChangingBihinButton用コールバック
		static void setNextBihinCallback(Fl_Widget *w, void *data) {
			BihinWindow *bw = static_cast<BihinWindow *>(data);
			if (bw->model != NULL) {
				bw->setNextBihin();
			} else {
				fl_alert("データファイルを開いてください");
			}
		}
		//! leftChangingBihinButton用コールバック
		static void setPrevBihinCallback(Fl_Widget *w, void *data) {
			BihinWindow *bw = static_cast<BihinWindow *>(data);
			if (bw->model != NULL) {
				bw->setPrevBihin();
			} else {
				fl_alert("データファイルを開いてください");
			}
		}
		//! rightChangingSizeButton用コールバック
		static void setNextSizeCallback(Fl_Widget *w, void *data) {
			BihinWindow *bw = static_cast<BihinWindow *>(data);
			if (bw->model != NULL) {
				bw->setNextSize();
			} else {
				fl_alert("データファイルを開いてください");
			}
		}
		//! leftChangingSizeButton用コールバック
		static void setPrevSizeCallback(Fl_Widget *w, void *data) {
			BihinWindow *bw = static_cast<BihinWindow *>(data);
			if (bw->model != NULL) {
				bw->setPrevSize();
			} else {
				fl_alert("データファイルを開いてください");
			}
		}
		//! updateButton用コールバック
		static void updateValuesCallback(Fl_Widget *w, void *data) {
			BihinWindow *bw = static_cast<BihinWindow *>(data);
			if (bw->model != NULL) {
				bw->updateValues();
			} else {
				fl_alert("データファイルを開いてください");
			}
		}
		//! openButton用コールバック
		static void openFileCallback(Fl_Widget *w, void *data) {
			(static_cast<BihinWindow *>(data))->openFile();
		}
		//! overwriteButton用コールバック
		static void overwriteFileCallback(Fl_Widget *w, void *data) {
			BihinWindow *bw = static_cast<BihinWindow *>(data);
			if (bw->model != NULL) {
				bw->overwriteFile();
			} else {
				fl_alert("データファイルを開いてください");
			}
		}
		//! saveButton用コールバック
		static void saveFileCallback(Fl_Widget *w, void *data) {
			BihinWindow *bw = static_cast<BihinWindow *>(data);
			if (bw->model != NULL) {
				bw->saveFile();
			} else {
				fl_alert("データファイルを開いてください");
			}
		}
		//! Quitメニュー用コールバック
		static void quitCallback(Fl_Widget *w, void *data) {
			BihinWindow *bw = static_cast<BihinWindow *>(data);
			bw->quit();
		}
		//! 備品情報一覧表示メニュー用コールバック
		static void showBihinTextViewCallback(Fl_Widget *w, void *data) {
			BihinWindow *bw = static_cast<BihinWindow *>(data);
			if (bw->model != NULL) {
				bw->showBihinTextView();
			} else {
				fl_alert("データファイルを開いてください");
			}
		}
		//! 備品追加メニュー用コールバック
		static void addNewBihinCallback(Fl_Widget *w, void *data) {
			BihinWindow *bw = static_cast<BihinWindow *>(data);
			bw->addNewBihin();
		}
		//! 備品編集メニュー用コールバック
		static void editCurrentBihinCallback(Fl_Widget *w, void *data) {
			BihinWindow *bw = static_cast<BihinWindow *>(data);
			bw->editCurrentBihin();
		}
		//! 備品削除メニュー用コールバック
		static void deleteCurrentBihinCallback(Fl_Widget *w, void *data) {
			BihinWindow *bw = static_cast<BihinWindow *>(data);
			bw->deleteCurrentBihin();
		}
	
	private:
		//! 注目している備品をひとつ先に進める
		void setNextBihin();
		//! 注目している備品をひとつ前に戻す
		void setPrevBihin();
		//! 注目しているサイズをひとつ先に進める
		void setNextSize();
		//! 注目しているサイズをひとつ前に戻す
		void setPrevSize();
		//! 入力欄の値をmodelに反映する
		void updateValues();
		//! データファイルを開いてmodelとしてセットする
		void openFile();
		//! 既存のデータファイルに上書き保存する
		void overwriteFile();
		//! 新しいデータファイルに保存する
		void saveFile();
		//! プログラムを終了する
		void quit();
		//! 備品情報を一覧表示
		void showBihinTextView();
		//! 現在の備品を削除
		void deleteCurrentBihin();
		//! 備品を追加
		void addNewBihin();
		//! 現在の備品を編集
		void editCurrentBihin();
	
	private:
		//! モデルオブジェクトをセット
		void setModel(BihinModel *model);
		//! 入力欄の値（数、保管場所など）をmodelの各変数にセットする
		void setValues();
		//! 各入力欄の値を消去する
		void cleanInputs();
	
	private:
		BihinModel *model;
		int bihinNameIndex;
		int bihinSizeIndex;
		BihinTextWindow *bihinTextWindow;
		BihinAddEditWindow *bihinAddEditWindow;
	
	private:
		Fl_Menu_Bar *menuBar;
		
		Fl_Box *bihinLabelBox;
		Fl_Box *sizeLabelBox;
		
		Fl_Button *leftChangingBihinButton;
		Fl_Button *rightChangingBihinButton;
		
		Fl_Button *leftChangingSizeButton;
		Fl_Button *rightChangingSizeButton;
		
		Fl_Box *bihinNameBox;
		Fl_Box *sizeBox;
		
		Fl_Box *titleCurrentBox;
		Fl_Box *titleChangingBox;
		
		Fl_Box *titleCountBox;
		Fl_Box *titleCleaningBox;
		Fl_Box *titleCleanedBox;
		Fl_Box *titleWhenCleanedBox;
		Fl_Box *titleWhoCleanedBox;
		Fl_Box *titleAllCountBox;
		Fl_Box *titleStoredPlaceBox;
		
		Fl_Box *currentCountBox;
		Fl_Box *currentCleaningBox;
		Fl_Box *currentCleanedBox;
		Fl_Box *currentWhenCleanedBox;
		Fl_Box *currentWhoCleanedBox;
		Fl_Box *currentAllCountBox;
		Fl_Box *currentStoredPlaceBox;
		
		Fl_Input *changingCountInput;
		Fl_Input *changingCleaningInput;
		Fl_Input *changingCleanedInput;
		Fl_Input *changingWhenCleanedInput;
		Fl_Input *changingWhoCleanedInput;
		Fl_Input *changingAllCountInput;
		Fl_Input *changingStoredPlaceInput;
		
		Fl_Box *dataLabelBox;
		Fl_Button *openButton;
		Fl_Button *overwriteButton;
		Fl_Button *saveButton;
		Fl_Button *updateButton;
};

#endif