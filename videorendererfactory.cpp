#include "videorendererfactory.h"
#include "nativerenderer.h"
#include "glrenderer.h"
#include <core/openglfactory.h>
#include <QtCore/QStringList>
#include <QtCore/QMap>

namespace Gst {

struct VideoRendererFactory::Data {
	QStringList keys;
};

VideoRendererFactory::VideoRendererFactory() {
}

VideoRendererFactory::~VideoRendererFactory() {
// 	delete d;
}

QStringList VideoRendererFactory::keys() {
	return data().keys;
}

VideoRendererFactory::Data &VideoRendererFactory::data() {
	static Data data;
	data.keys.push_back("auto");
	if (NativeRenderer::isAvaiable(NativeRenderer::Xv))
		data.keys.push_back("xv");
	if (NativeRenderer::isAvaiable(NativeRenderer::X11))
		data.keys.push_back("x11");
	if (Core::OpenGLFactory::isAvailable())
		data.keys.push_back("software");
	return data;
}

GstVideoIface *VideoRendererFactory::create(const QString &key, PlayEngine *engine) {
	if (data().keys.size() < 2 || !data().keys.contains(key))
		return 0;
	const QString k = (key == "auto") ? data().keys[1] : key;
	if (k == "xv")
		return new NativeRenderer(engine, NativeRenderer::Xv);
	if (k == "x11")
		return new NativeRenderer(engine, NativeRenderer::X11);
	if (k == "software")
		return new GLRenderer;
	return 0;
}

}
