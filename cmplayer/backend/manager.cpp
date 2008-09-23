#include "manager.h"
#include "info.h"
#include <QtCore/QStringList>
#include <QtCore/QDir>
#include <QtCore/QCoreApplication>
#include <QtCore/QPluginLoader>
#include <QtCore/QMap>
#include <QtCore/QDebug>

namespace Backend {

struct Manager::Data {
	QStringList backends;
	QDir dir;
	QStringList files;
	FactoryMap factories;
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

void Manager::getFileNames() {
	if (d->files.isEmpty()) {
		d->files = d->dir.entryList(QDir::Files);
		foreach(QString fileName, d->files)
			d->factories[fileName] = 0;
	}
}

const QStringList &Manager::fileNameList() const {
	if (d->files.isEmpty())
		const_cast<Manager*>(this)->getFileNames();
	return d->files;
}

FactoryIface *Manager::load(const QString &fileName) {
	if (d->files.isEmpty())
		getFileNames();
	QMap<QString, FactoryIface*>::iterator it = d->factories.find(fileName);
	if (it == d->factories.end())
		return 0;
	if (it.value())
		return it.value();
	QPluginLoader loader(d->dir.absoluteFilePath(it.key()));
	if (!loader.load()) {
		qFatal("%s", qPrintable(loader.errorString()));
		return 0;
	}
	return it.value() = qobject_cast<FactoryIface*>(loader.instance());
}

const FactoryMap &Manager::loadAll() {
	if (d->files.isEmpty())
		getFileNames();
	for (int i=0; i<d->files.size(); ++i)
		load(d->files[i]);
	return d->factories;
}

}
