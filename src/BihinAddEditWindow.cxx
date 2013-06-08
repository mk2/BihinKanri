#include "BihinAddEditWindow.hxx"

BihinAddEditWindow::BihinAddEditWindow(int x_pos, int y_pos, BihinModel *model, const char *bihinName, int w, int h, const char *t): Fl_Window(x_pos, y_pos, w, h, t), name(bihinName) {
	this->model = model;

	int x = 0, y = 0;
		
	//! row 1
	x = 10; y += 10;
	nameLabel = new Fl_Box(x, y, 80, 30, "備品名称: ");
	nameLabel->align(FL_ALIGN_RIGHT | FL_ALIGN_INSIDE);
	x += 90;
	nameInput = new Fl_Input(x, y, 250, 30);
	nameInput->box(APP_UP_BOX);
	if (name != "") {
		nameInput->value(bihinName);
		nameInput->deactivate();
	}
		
	//! row 2
	x = 10; y += 40;
	sizeLabel = new Fl_Box(x, y, 80, 30, "サイズ: ");
	sizeLabel->align(FL_ALIGN_RIGHT | FL_ALIGN_INSIDE);
	x += 90;
	sizeInput = new Fl_Input(x, y, 90, 30);
	sizeInput->box(APP_UP_BOX);
	x += 100;
	addSizeButton = new Fl_Button(x, y, 70, 30, "追加");
	addSizeButton->box(APP_UP_BOX);
	addSizeButton->callback((Fl_Callback *)addSizeCallback, (void *)this);
	x += 80;
	deleteSizeButton = new Fl_Button(x, y, 70, 30, "削除");
	deleteSizeButton->box(APP_UP_BOX);
	deleteSizeButton->callback((Fl_Callback *)deleteSizeCallback, (void *)this);
	
	//! row 3
	x = 10; y += 40;
	sizeListLabel = new Fl_Box(x, y, 80, 30, "サイズ一覧: ");
	sizeListLabel->align(FL_ALIGN_RIGHT | FL_ALIGN_INSIDE);
	x += 90;
	sizeBrowser = new Fl_Hold_Browser(x, y, 250, 200);
	sizeBrowser->box(APP_UP_BOX);
	resizable(sizeBrowser);
	if (name != "" && model != NULL) {
		vector<string> &sizes = model->getBihinSizes(name);
		for (vector<string>::iterator itr = sizes.begin(); itr < sizes.end(); ++itr) {
			sizeBrowser->add((*itr).c_str());
		}
	}
	
	//! row 4
	x = 180; y += 210;
	addBihinButton = new Fl_Button(x, y, 80, 30, "追加・編集");
	addBihinButton->box(APP_UP_BOX);
	addBihinButton->callback((Fl_Callback *)addBihinCallback, (void *)this);
	x += 90;
	cancelButton = new Fl_Button(x, y, 80, 30, "キャンセル");
	cancelButton->box(APP_UP_BOX);
	cancelButton->callback((Fl_Callback *)closeCallback, (void *)this);
	
	end();
}

void BihinAddEditWindow::addSize() {
	string bihinSize = sizeInput->value();
	if (bihinSize != "") {
		sizeBrowser->add(bihinSize.c_str());
		sizeInput->value("");
	}
}

void BihinAddEditWindow::deleteSize() {
	for (int i = 0; i < sizeBrowser->size(); ++i) {
		if (sizeBrowser->selected(i+1) > 0)
			sizeBrowser->remove(i+1);
	}
}

void BihinAddEditWindow::addBihin() {
	vector<string> sizes;
	for (int i = 0; i < sizeBrowser->size(); ++i) {
		string size = sizeBrowser->text(i+1);
		if (size != "") {
			sizes.push_back(size);
		}
	}
	
	if (name == "") {
		name = nameInput->value();
		//! 追加モード
		model->deleteBihin(name);
		model->addBihin(name, sizes);
	} else {
		//! エディットモード
		model->addBihin(name, sizes);
	}
	hide();
}