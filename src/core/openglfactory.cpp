#include "openglfactory.h"
#include "opengliface.h"
#include "info.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QPluginLoader>

namespace Core {

OpenGLFactory::OpenGLFactory() {
	iface = 0;
	const QStringList list = QStringList() << Core::Info::pluginPath()
			 << QCoreApplication::applicationDirPath();
	const QString fileName = "libcmplayer_opengl.so";
	for (int i=0; i<list.size(); ++i) {
		QDir dir(list[i]);
		if (!dir.exists() || !dir.exists(fileName))
			continue;
		const QString file = dir.absoluteFilePath(fileName);
		QPluginLoader loader(file);
		if (loader.load())
			iface = qobject_cast<OpenGLIface*>(loader.instance());
		else
			qFatal("%s", qPrintable(loader.errorString()));
	}
}

OpenGLFactory::~OpenGLFactory() {
	delete iface;
}

OpenGLFactory &OpenGLFactory::get() {
	static OpenGLFactory self;
	return self;
}

OpenGLVideoRendererIface *OpenGLFactory::createVideoRenderer() {
	return get().iface ? get().iface->createVideoRenderer() : 0;
}

AbstractOsdRenderer *OpenGLFactory::createOsdRenderer() {
	return get().iface ? get().iface->createOsdRenderer() : 0;
}

bool OpenGLFactory::isAvailable() {
	return get().iface != 0;
}

}
