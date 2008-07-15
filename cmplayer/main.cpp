#include "mainwindow.h"
#include "helper.h"
//#include <mplayer/informations.h>
#include <QApplication>

int main(int argc, char **argv) {
	QApplication app(argc, argv);
//	MPlayer::Informations::get()->setPrivatePath(Helper::privatePath());
	MainWindow mw;
	mw.show();
	return app.exec();
}
