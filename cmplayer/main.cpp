#include "mainwindow.h"
#include "helper.h"
#include <QApplication>

int main(int argc, char **argv) {
	QApplication app(argc, argv);
	MainWindow mw;
	mw.show();
	return app.exec();
}
