#include "application.hpp"
#include <gst/gst.h>
#include <gst/controller/gstcontroller.h>
#include <liboil/liboil.h>
#include "playengine.hpp"
#include "nativevideorenderer.hpp"

int main(int argc, char **argv) {
	gst_init(NULL, NULL);
	gst_controller_init(NULL, NULL);
	oil_init ();

	QApplication app(argc, argv);

	PlayEngine engine;
	engine.setMrl(QUrl::fromLocalFile("/media/sda2/videos/ww.avi"));
	engine.play();
//	engine.renderer()->show();
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
