#include "backendobject.h"
#include "playengine.h"
#include "info.h"
#include "config.h"
#include "private/backendiface.h"
#include <QtCore/QPluginLoader>
#include <QtCore/QFileInfo>
#include <QtCore/QLibrary>

namespace Backend {

struct BackendObject::Data {
	Data():iface(0), info(0), config(0) {}
	~Data() {if (iface) delete iface;}
	BackendIface *iface;
	Info *info;
	Config *config;
	QString filePath, fileName;
};

BackendObject *BackendObject::current = 0;

BackendObject::BackendObject(const QString &filePath)
: d(new Data) {
	QFileInfo file(filePath);
	if (!file.exists() || !QLibrary::isLibrary(filePath))
		return;
	d->filePath = file.absoluteFilePath();
	d->fileName = file.fileName();
	QPluginLoader loader(d->filePath);
	if (!loader.load()) {
		qFatal("%s", qPrintable(loader.errorString()));
		return;
	}
	d->iface = qobject_cast<BackendIface*>(loader.instance());
	if (d->iface) {
		d->info = d->iface->info();
		d->config = d->iface->config();
	}
}

BackendObject::~BackendObject() {
	delete d;
}

Info *BackendObject::info() const {
	return d->info;
}

Config *BackendObject::config() const {
	return d->config;
}

PlayEngine *BackendObject::createPlayEngine(QObject *parent) const {
	return d->iface->createPlayEngine(parent);
}

const QString &BackendObject::name() const {
	return d->info->name();
}

const QString &BackendObject::fileName() const {
	return d->fileName;
}

const QString &BackendObject::filePath() const {
	return d->filePath;
}

const QString &BackendObject::description() const {
	return d->info->description();
}

bool BackendObject::isValid() const {
	return d->iface != 0;
}

}
