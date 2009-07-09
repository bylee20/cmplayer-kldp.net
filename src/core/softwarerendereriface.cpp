#include "softwarerendereriface.h"
#include "softwarerenderercreator.h"
#include <QtCore/QStringList>
#include "info.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QPluginLoader>

namespace Core {

class SoftwareRendererIface::Loader {
public:
	static Loader &get();
	~Loader() {delete opengl; delete xvideo;}
	SoftwareRendererCreator *creator(SoftwareRendererType type) {
		if (type == OpenGLRenderer)
			return opengl;
		else if (type == XVideoRenderer)
			return xvideo;
		return 0;
	}
private:
	Loader() {
		opengl = qobject_cast<SoftwareRendererCreator*>(load("libcmplayer_opengl.so"));
		xvideo = qobject_cast<SoftwareRendererCreator*>(load("libcmplayer_xvideo.so"));
	}
	QObject *load(const QString &fileName) {
		static const QStringList list = QStringList() << Core::Info::pluginPath()
				 << QCoreApplication::applicationDirPath();
		for (int i=0; i<list.size(); ++i) {
			QDir dir(list[i]);
			if (!dir.exists() || !dir.exists(fileName))
				continue;
			const QString file = dir.absoluteFilePath(fileName);
			QPluginLoader loader(file);
			if (loader.load())
				return loader.instance();
			else
				qFatal("%s", qPrintable(loader.errorString()));
		}
		return 0;
	}
	SoftwareRendererCreator *opengl, *xvideo;
};

SoftwareRendererIface::Loader &SoftwareRendererIface::Loader::get() {
	static Loader self;
	return self;
}

SoftwareRendererIface *SoftwareRendererIface::create(SoftwareRendererType type) {
	SoftwareRendererCreator *c = Loader::get().creator(type);
	if (c)
		return c->create();
	return 0;
}

bool SoftwareRendererIface::isAvailable(SoftwareRendererType type) {
	return Loader::get().creator(type) != 0;
}

SoftwareRendererIface::SoftwareRendererIface(QWidget *widget)
: VideoRendererIface(widget), m_vscale(1.0), m_hscale(1.0), m_vmargin(0.0), m_hmargin(0.0), m_obj(0) {
}

SoftwareRendererIface::~SoftwareRendererIface() {
}

void SoftwareRendererIface::calculate() {
	QSizeF video(aspectRatioF(), 1.0);
	QSizeF visual = video;
	if (cropRatio() > 0.0) {
		visual.setWidth(cropRatio());
		visual.setHeight(1.0);
	}
	visual.scale(m_widget, Qt::KeepAspectRatio);
	video.scale(visual, Qt::KeepAspectRatioByExpanding);
	const double x = (m_widget.width() - video.width())/2.0;
	const double y = (m_widget.height() -video.height())/2.0;
	setImageRect(x, y, video);
	m_hmargin = (m_widget.width() - visual.width())/2.0;
	m_vmargin = (m_widget.height() - visual.height())/2.0;
	hscale() = video.width() / double(videoSize().width());
	vscale() = video.height() / double(videoSize().height());
	const double pixel = frameInfo().pixelAspectRatio;
	if (pixel > 1.)
		hscale() *= pixel;
	else
		vscale() /= pixel;
	m_visual = visual;
}

}
