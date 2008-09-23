#include "mainwindow.h"
#include "helper.h"
#include <QtGui/QApplication>
#include <QtCore/QDebug>
#include <backend/info.h>

int main(int argc, char **argv) {
	QApplication app(argc, argv);
	Backend::Info::setPrivatePath(Helper::privatePath());
	MainWindow mw;
	mw.show();
	return app.exec();
}
