#include "mainwindow.h"
#include "helper.h"
#include "pref/wizard.h"
#include <backend/info.h>
#include <QtGui/QApplication>
#include "encodingfiledialog.h"

int main(int argc, char **argv) {
	QApplication app(argc, argv);
	Backend::Info::setPrivatePath(Helper::privatePath());
// 	Pref::Wizard wizard;
// 	wizard.exec();
	MainWindow mw;
	mw.show();
	return app.exec();
}
