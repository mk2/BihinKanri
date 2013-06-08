#include "BihinTextWindow.hxx"

BihinTextWindow::BihinTextWindow(const string &content,int x, int y, int w, int h, const char *t): Fl_Window(x, y, w, h, t) {
	textDisplay = new Fl_Text_Display(10, 10, w-20, h-60);
	textDisplay->box(APP_UP_BOX);
	textBuffer = new Fl_Text_Buffer();
	textDisplay->buffer(textBuffer);
	textBuffer->text(content.c_str());
	closeButton = new Fl_Button(300, 260, 90, 30, "閉じる");
	closeButton->callback((Fl_Callback *)closeCallback, (void *)this);
	closeButton->box(APP_UP_BOX);
	
	resizable(textDisplay);
	end();
}