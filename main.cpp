#include "application.hpp"
#include <gst/gst.h>

int main(int argc, char **argv) {
	gst_init(NULL, NULL);
	Application app(argc, argv);
//	QApplication app(argc, argv);
//	PlayEngine engine;
//	engine.setMrl(QUrl::fromLocalFile("/media/sda2/videos/ww.avi"));
//	engine.play();
//	QDialog dlg;
//	dlg.show();

//	MainWindow mw;
//	mw.show();
//	Gst::MediaPlayerControl con;
//	con.setMedia(QUrl::fromLocalFile("/media/sda2/videos/CSI/01/csi_lv_01x01.avi"), 0);
//	con.play();
	return app.exec();
}
