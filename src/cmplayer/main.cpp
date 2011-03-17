#include "application.hpp"
#include <QtCore/QMetaType>
//#include "libvlc.h"

int main(int argc, char **argv) {
	Application app(argc, argv);
//	LibVlc::init();
	const int ret = app.exec();
//	LibVlc::release();
	return ret;
}
