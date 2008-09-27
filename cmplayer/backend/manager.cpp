#include "manager.h"
#include "info.h"
#include <QtCore/QStringList>
#include <QtCore/QDir>
#include <QtCore/QCoreApplication>
#include <QtCore/QPluginLoader>
#include <QtCore/QMap>
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>

namespace Backend {

struct Manager::Data {
	QDir dir;
	QMap<QString, int> idxes;
	BackendList objs;
};

Manager::Manager() {
	d = new Data;
	d->dir = QDir(qApp->applicationDirPath());
	d->dir.cd("backends");
}

Manager::~Manager() {
	delete d;
}

Manager *Manager::get() {
	static Manager self;
	return &self;
}

BackendObject *Manager::load(const QString &fileName) {
	QMap<QString, int>::const_iterator it = d->idxes.find(fileName);
	if (it != d->idxes.end())
		return d->objs[it.value()];
	BackendObject *obj = new BackendObject(d->dir.filePath(fileName));
	if (!obj->isValid()) {
		delete obj;
		return 0;
	}
	d->idxes[fileName] = d->objs.size();
	d->objs.append(obj);
	return obj;
}

const BackendList &Manager::loadAll() {
	static bool loaded = false;
	if (!loaded) {
		QStringList files = d->dir.entryList(QDir::Files);
		for (int i=0; i<files.size(); ++i)
			load(files[i]);
	}
	return d->objs;
}

}
