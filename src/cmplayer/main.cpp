#include <QtGui/QApplication>
#include <QtCore/QDir>
#include "mainwindow.h"
 
int main (int argc, char **argv) {
	QApplication app(argc, argv);
	MainWindow mw;
	mw.show();
	return app.exec();
}

