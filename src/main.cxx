#include <FL/Fl.H>
#include "BihinWindow.hxx"

int main(int argc, char **argv) {
	BihinWindow *window = new BihinWindow();
	window->show(argc, argv);
	return Fl::run();
}