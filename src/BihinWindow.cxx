#include "BihinWindow.hxx"

BihinWindow::BihinWindow(int w, int h, const char *t): Fl_Window(w, h, t) {
	
	int x = 0;
	int y = 0;
	
	model = new BihinModel();
	bihinNameIndex = 0;
	bihinSizeIndex = 0;
	bihinTextWindow = NULL;
	bihinAddEditWindow = NULL;
	
	box(APP_UP_BOX);
	
	//! Fl_Sys_Menu_Barがうまく動かない…
	const Fl_Menu_Item mitms[] = {
		{"ファイル/データファイルを開く", FL_COMMAND + 'o', (Fl_Callback *)openFileCallback, (void *)this},
		{"ファイル/データファイルを上書き保存", FL_COMMAND + 's', (Fl_Callback *)overwriteFileCallback, (void *)this},
		{"ファイル/データファイルを新規保存", FL_SHIFT + FL_COMMAND + 's', (Fl_Callback *)saveFileCallback, (void *)this},
		{"ファイル/プログラムを終了", FL_COMMAND + 'q', (Fl_Callback *)quitCallback, (void *)this},
		{"編集/新規備品を追加", FL_COMMAND + 'a', 0},
		{"編集/既存の備品のサイズ情報を編集", FL_COMMAND + 'e', 0},
		{"編集/既存の備品を削除", FL_COMMAND + 'd', 0},
		{"編集/現在の備品情報を一覧表示", FL_COMMAND + 'v', (Fl_Callback *)showBihinTextViewCallback, (void *)this}
	};
	
	// row 0
	x = 0; y = 0;
	menuBar = new Fl_Menu_Bar(x, y, w, 30);
	menuBar->box(APP_UP_BOX);
	menuBar->add("ファイル/データファイルを開く", FL_COMMAND + 'o', (Fl_Callback *)openFileCallback, (void *)this);
	menuBar->add("ファイル/データファイルを上書き保存", FL_COMMAND + 's', (Fl_Callback *)overwriteFileCallback, (void *)this);
	menuBar->add("ファイル/データファイルを新規保存", FL_SHIFT + FL_COMMAND + 's', (Fl_Callback *)saveFileCallback, (void *)this);
	menuBar->add("ファイル/プログラムを終了", FL_COMMAND + 'q', (Fl_Callback *)quitCallback, (void *)this);
	menuBar->add("編集/新規備品を追加", FL_COMMAND + 'a', (Fl_Callback *)addNewBihinCallback, (void *)this);
	menuBar->add("編集/既存の備品のサイズ情報を編集", FL_COMMAND + 'e', (Fl_Callback *)editCurrentBihinCallback, (void *)this);
	menuBar->add("編集/既存の備品を削除", FL_COMMAND + 'd', (Fl_Callback *)deleteCurrentBihinCallback, (void *)this);
	menuBar->add("編集/現在の備品情報を一覧表示", FL_COMMAND + 'v', (Fl_Callback *)showBihinTextViewCallback, (void *)this);
	
	// row 1
	x = 10; y += 40;
	bihinLabelBox = new Fl_Box(x, y, 50, 30, "名称: ");
	bihinLabelBox->box(APP_NO_BOX);
	bihinLabelBox->align(FL_ALIGN_RIGHT | FL_ALIGN_INSIDE);
	x += 60;
	leftChangingBihinButton = new Fl_Button(x, y, 30, 30, "");
	leftChangingBihinButton->label("@<");
	leftChangingBihinButton->callback((Fl_Callback *)setPrevBihinCallback, (void *)this);
	leftChangingBihinButton->box(APP_UP_BOX);
	x += 30;
	bihinNameBox = new Fl_Box(x, y, 380, 30, "");
	bihinNameBox->box(APP_UP_BOX);
	resizable(this);
	x += 380;
	rightChangingBihinButton = new Fl_Button(x, y, 30, 30, "");
	rightChangingBihinButton->label("@>");
	rightChangingBihinButton->callback((Fl_Callback *)setNextBihinCallback, (void *)this);
	rightChangingBihinButton->box(APP_UP_BOX);
	
	// row 2
	x = 10; y += 35;
	sizeLabelBox = new Fl_Box(x, y, 50, 30, "寸法: ");
	sizeLabelBox->box(APP_NO_BOX);
	sizeLabelBox->align(FL_ALIGN_RIGHT | FL_ALIGN_INSIDE);
	x += 60;
	leftChangingSizeButton = new Fl_Button(x, y, 30, 30, "");
	leftChangingSizeButton->label("@<");
	leftChangingSizeButton->callback((Fl_Callback *)setPrevSizeCallback, (void *)this);
	leftChangingSizeButton->box(APP_UP_BOX);
	x += 30;
	sizeBox = new Fl_Box(x, y, 380, 30, "");
	sizeBox->box(APP_UP_BOX);
	x += 380;
	rightChangingSizeButton = new Fl_Button(x, y, 30, 30, "");
	rightChangingSizeButton->label("@>");
	rightChangingSizeButton->callback((Fl_Callback *)setNextSizeCallback, (void *)this);
	rightChangingSizeButton->box(APP_UP_BOX);
	
	// row 3
	x = 10; y += 40;
	x += 190;
	titleCurrentBox = new Fl_Box(x, y, 150, 30, "現在");
	titleCurrentBox->box(APP_ROUND_UP_BOX);
	x += 160;
	titleChangingBox = new Fl_Box(x, y, 150, 30, "変更");
	titleChangingBox->box(APP_ROUND_UP_BOX);
	
	// row 4
	x = 10; y += 35;
	titleCountBox = new Fl_Box(x, y, 180, 30, "現在数: ");
	titleCountBox->box(APP_NO_BOX);
	titleCountBox->align(FL_ALIGN_RIGHT | FL_ALIGN_INSIDE);
	x += 190;
	currentCountBox = new Fl_Box(x, y, 150, 30, "");
	currentCountBox->box(APP_THIN_UP_BOX);
	x += 160;
	changingCountInput = new Fl_Int_Input(x, y, 150, 30, "");
	changingCountInput->box(APP_UP_BOX);
	
	// row 5
	x = 10; y += 35;
	titleCleaningBox = new Fl_Box(x, y, 180, 30, "クリーニング中: ");
	titleCleaningBox->box(APP_NO_BOX);
	titleCleaningBox->align(FL_ALIGN_RIGHT | FL_ALIGN_INSIDE);
	x += 190;
	currentCleaningBox = new Fl_Box(x, y, 150, 30, "");
	currentCleaningBox->box(APP_THIN_UP_BOX);
	x += 160;
	changingCleaningInput = new Fl_Input(x, y, 150, 30, "");
	changingCleaningInput->box(APP_UP_BOX);
	
	// row 6
	x = 10; y += 35;
	titleCleanedBox = new Fl_Box(x, y, 180, 30, "クリーニング: ");
	titleCleanedBox->box(APP_NO_BOX);
	titleCleanedBox->align(FL_ALIGN_RIGHT | FL_ALIGN_INSIDE);
	x += 190;
	currentCleanedBox = new Fl_Box(x, y, 150, 30, "");
	currentCleanedBox->box(APP_THIN_UP_BOX);
	x += 160;
	changingCleanedInput = new Fl_Input(x, y, 150, 30, "");
	changingCleanedInput->box(APP_UP_BOX);
	
	// row 7
	x = 10; y += 35;
	titleWhenCleanedBox = new Fl_Box(x, y, 180, 30, "クリーニング実施日: ");
	titleWhenCleanedBox->box(APP_NO_BOX);
	titleWhenCleanedBox->align(FL_ALIGN_RIGHT | FL_ALIGN_INSIDE);
	x += 190;
	currentWhenCleanedBox = new Fl_Box(x, y, 150, 30, "");
	currentWhenCleanedBox->box(APP_THIN_UP_BOX);
	x += 160;
	changingWhenCleanedInput = new Fl_Input(x, y, 150, 30, "");
	changingWhenCleanedInput->box(APP_UP_BOX);
		
	// row 8
	x = 10; y += 35;
	titleWhoCleanedBox = new Fl_Box(x, y, 180, 30, "クリーニング実施責任者: ");
	titleWhoCleanedBox->box(APP_NO_BOX);
	titleWhoCleanedBox->align(FL_ALIGN_RIGHT | FL_ALIGN_INSIDE);
	x += 190;
	currentWhoCleanedBox = new Fl_Box(x, y, 150, 30, "");
	currentWhoCleanedBox->box(APP_THIN_UP_BOX);
	x += 160;
	changingWhoCleanedInput = new Fl_Input(x, y, 150, 30, "");
	changingWhoCleanedInput->box(APP_UP_BOX);
	
	// row 9
	x = 10; y += 35;
	titleAllCountBox = new Fl_Box(x, y, 180, 30, "総数: ");
	titleAllCountBox->box(APP_NO_BOX);
	titleAllCountBox->align(FL_ALIGN_RIGHT | FL_ALIGN_INSIDE);
	x += 190;
	currentAllCountBox = new Fl_Box(x, y, 150, 30, "");
	currentAllCountBox->box(APP_THIN_UP_BOX);
	x += 160;
	changingAllCountInput = new Fl_Int_Input(x, y, 150, 30, "");
	changingAllCountInput->box(APP_UP_BOX);
	
	// row 10
	x = 10; y += 35;
	titleStoredPlaceBox = new Fl_Box(x, y, 180, 30, "保存場所: ");
	titleStoredPlaceBox->box(APP_NO_BOX);
	titleStoredPlaceBox->align(FL_ALIGN_RIGHT | FL_ALIGN_INSIDE);
	x += 190;
	currentStoredPlaceBox = new Fl_Box(x, y, 150, 30, "");
	currentStoredPlaceBox->box(APP_THIN_UP_BOX);
	x += 160;
	changingStoredPlaceInput = new Fl_Input(x, y, 150, 30, "");
	changingStoredPlaceInput->box(APP_UP_BOX);
	
	// row 10
	x = 360; y += 40;
	updateButton = new Fl_Button(x, y, 150, 30, "更新");
	updateButton->callback((Fl_Callback *)updateValuesCallback, (void *)this);
	updateButton->box(APP_UP_BOX);
	
	//! row 11はコメントアウト メニューの操作とかぶっているため
	// row 11
	// 	x = 0; y += 30;
	// 	dataLabelBox = new Fl_Box(x, y, 120, 30, "データファイル: ");
	// 	dataLabelBox->box(APP_NO_BOX);
	// 	x += 120;
	// 	openButton = new Fl_Button(x, y, 120, 30, "開く");
	// 	openButton->callback((Fl_Callback *)openFileCallback, (void *)this);
	// 	x += 120;
	// 	overwriteButton = new Fl_Button(x, y, 	120, 30, "上書き保存");
	// 	overwriteButton->callback((Fl_Callback *)overwriteFileCallback, (void *)this);
	// 	x += 120;
	// 	saveButton = new Fl_Button(x, y, 120, 30, "新規保存");
	// 	saveButton->callback((Fl_Callback *)saveFileCallback, (void *)this);
	
	end();
}

void BihinWindow::setModel(BihinModel *model) {
	vector<string>::iterator itr;
	this->model = model;
	bihinNameIndex = 0; bihinSizeIndex = 0;
	string &bihinName = model->getBihinNames()[bihinNameIndex];
	vector<string> &bihinSizes = model->getBihinSizes(bihinName);
	string bihinSize = bihinSizes[bihinSizeIndex];
	bihinNameBox->label(bihinName.c_str());
	sizeBox->label(bihinSizes[bihinSizeIndex].c_str());
}

void BihinWindow::setNextBihin() {
	if (model->getBihinNames().size() <= 0)
		return;
	
	++bihinNameIndex;
	if (bihinNameIndex >= model->getBihinNames().size()) {
		bihinNameIndex = 0;
	}
	string &bihinName = model->getBihinNames()[bihinNameIndex];
	vector<string> &bihinSizes = model->getBihinSizes(bihinName);
	bihinNameBox->label(bihinName.c_str());
	bihinSizeIndex = 0;
	sizeBox->label(bihinSizes[bihinSizeIndex].c_str());
	setValues();
}

void BihinWindow::setPrevBihin() {
	if (model->getBihinNames().size() <= 0)
		return;
	
	--bihinNameIndex;
	if (bihinNameIndex < 0) {
		bihinNameIndex = model->getBihinNames().size() - 1;
	}
	string &bihinName = model->getBihinNames()[bihinNameIndex];
	vector<string> &bihinSizes = model->getBihinSizes(bihinName);
	bihinNameBox->label(bihinName.c_str());
	bihinSizeIndex = 0;
	sizeBox->label(bihinSizes[bihinSizeIndex].c_str());
	setValues();
}

void BihinWindow::setNextSize() {
	if (model->getBihinNames().size() <= 0)
		return;
	
	string &bihinName = model->getBihinNames()[bihinNameIndex];
	vector<string> &bihinSizes = model->getBihinSizes(bihinName);
	
	if (bihinSizes.size() <= 0)
		return;
	
	++bihinSizeIndex;
	if (bihinSizeIndex >= bihinSizes.size()) {
		bihinSizeIndex = 0;
	}
	sizeBox->label(bihinSizes[bihinSizeIndex].c_str());
	setValues();
}

void BihinWindow::setPrevSize() {
	if (model->getBihinNames().size() <= 0)
		return;
	
	string &bihinName = model->getBihinNames()[bihinNameIndex];
	vector<string> &bihinSizes = model->getBihinSizes(bihinName);
	
	if (bihinSizes.size() <= 0)
		return;
	
	--bihinSizeIndex;
	if (bihinSizeIndex < 0) {
		bihinSizeIndex = bihinSizes.size() - 1;
	}
	sizeBox->label(bihinSizes[bihinSizeIndex].c_str());
	setValues();
}

void BihinWindow::updateValues() {
	string &bihinName = model->getBihinNames()[bihinNameIndex];
	vector<string> &bihinSizes = model->getBihinSizes(bihinName);
	
	if (model->getBihinNames().size() <= 0)
		return;
	
	string itemName = model->getBihinNames()[bihinNameIndex];
	string itemSize = model->getBihinSizes(itemName)[bihinSizeIndex];
	
	string count = changingCountInput->value();
	string cleaning = changingCleaningInput->value();
	string cleaned = changingCleanedInput->value();
	string whenCleaned = changingWhenCleanedInput->value();
	string whoCleaned = changingWhoCleanedInput->value();
	string allCount = changingAllCountInput->value();
	string storedPlace = changingStoredPlaceInput->value();
	
	if (count != "")
		model->setValue(itemName, itemSize, BihinModel::KEY_COUNT, count);
	
	if (cleaning != "")
		model->setValue(itemName, itemSize, BihinModel::KEY_CLEANING, cleaning);
	
	if (cleaned != "")
		model->setValue(itemName, itemSize, BihinModel::KEY_CLEANED, cleaned);
	
	if (whenCleaned != "")
		model->setValue(itemName, itemSize, BihinModel::KEY_WHEN_CLEANED, whenCleaned);
	
	if (whoCleaned != "")
		model->setValue(itemName, itemSize, BihinModel::KEY_WHO_CLEANED, whoCleaned);
	
	if (allCount != "")
		model->setValue(itemName, itemSize, BihinModel::KEY_ALL_COUNT, allCount);
	
	if (storedPlace != "")
		model->setValue(itemName, itemSize, BihinModel::KEY_STORED_PLACE, storedPlace);
	
	setValues();
	cleanInputs();
}

void BihinWindow::setValues() {
	string &bihinName = model->getBihinNames()[bihinNameIndex];
	vector<string> &bihinSizes = model->getBihinSizes(bihinName);
	string bihinSize = bihinSizes[bihinSizeIndex];
	
	currentCountBox->label(model->getValue(bihinName, bihinSize, BihinModel::KEY_COUNT).c_str());
	currentCleaningBox->label(model->getValue(bihinName, bihinSize, BihinModel::KEY_CLEANING).c_str());
	currentCleanedBox->label(model->getValue(bihinName, bihinSize, BihinModel::KEY_CLEANED).c_str());
	currentWhenCleanedBox->label(model->getValue(bihinName, bihinSize, BihinModel::KEY_WHEN_CLEANED).c_str());
	currentWhoCleanedBox->label(model->getValue(bihinName, bihinSize, BihinModel::KEY_WHO_CLEANED).c_str());
	currentAllCountBox->label(model->getValue(bihinName, bihinSize, BihinModel::KEY_ALL_COUNT).c_str());
	currentStoredPlaceBox->label(model->getValue(bihinName, bihinSize, BihinModel::KEY_STORED_PLACE).c_str());
}


void BihinWindow::openFile() {
	char *filename;
	filename = fl_file_chooser("読み込むデータファイルを選択してください", "", "", 0);
	if (filename != NULL) {
		cout << "Chose data file is " << filename << endl;
		BihinModel *prevModel = model;
		BihinModel *nextModel = new BihinModel(string(filename));
		if (nextModel != NULL && nextModel->read()) {
			setModel(nextModel);
			setValues();
			if (prevModel != NULL)
				delete(prevModel);
		} else {
			fl_alert("適切なデータファイルを選択してください");
		}
	}
}

void BihinWindow::cleanInputs() {
	changingCountInput->value("");
	changingCleaningInput->value("");
	changingCleanedInput->value("");
	changingWhenCleanedInput->value("");
	changingWhoCleanedInput->value("");
	changingAllCountInput->value("");
	changingStoredPlaceInput->value("");
}

void BihinWindow::saveFile() {
	char *filename = fl_file_chooser("新しく保存するデータファイルを選択してください", "", "", 0);
	if (filename != NULL) {
		if (model->save(filename))
			fl_alert("保存しました");
	}
}

void BihinWindow::overwriteFile() {
	if (model->save())
		fl_alert("上書きしました");
}

void BihinWindow::quit() {
	exit(0);
}

void BihinWindow::showBihinTextView() {
	bool isWritable = false;
	string content = model->getBihinText(isWritable);
	if (isWritable) {
		if (bihinTextWindow != NULL)
			delete bihinTextWindow;
		bihinTextWindow = new BihinTextWindow(content, x_root(), y_root());
		bihinTextWindow->set_modal();
		bihinTextWindow->show();
	} else {
		fl_alert("エラーが発生しました");
	}
}

void BihinWindow::addNewBihin() {
	if (bihinAddEditWindow != NULL)
		delete bihinAddEditWindow;
	bihinAddEditWindow = new BihinAddEditWindow(x_root(), y_root(), model);
	bihinAddEditWindow->set_modal();
	bihinAddEditWindow->show();
}

void BihinWindow::deleteCurrentBihin() {
	if (model->getBihinNames().size() <= 0)
		return;
	string itemName = model->getBihinNames()[bihinNameIndex];
	model->deleteBihin(itemName);
}

void BihinWindow::editCurrentBihin() {
	if (model->getBihinNames().size() <= 0)
		return;
	string itemName = model->getBihinNames()[bihinNameIndex];
	if (bihinAddEditWindow != NULL)
		delete bihinAddEditWindow;
	bihinAddEditWindow = new BihinAddEditWindow(x_root(), y_root(), model, itemName.c_str());
	bihinAddEditWindow->set_modal();
	bihinAddEditWindow->show();
}