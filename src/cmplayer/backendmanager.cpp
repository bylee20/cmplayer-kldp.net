#include "backendmanager.h"
#include <QtCore/QDir>
#include <core/info.h>
#include <QtCore/QPluginLoader>

struct BackendManager::Data {
	static BackendMap map;
};

BackendMap BackendManager::Data::map;

BackendManager::BackendManager() {
}

const BackendMap &BackendManager::load() {
	QDir dir(Core::Info::pluginPath());
	if (dir.exists()) {
		static QRegExp rxFilter("^libcmplayer_engine_(.+).so$");
		const QFileInfoList file = dir.entryInfoList(QDir::Files);
		for (int i=0; i<file.size(); ++i) {
			if (rxFilter.indexIn(file[i].fileName()) == -1)
				continue;
			QPluginLoader loader(file[i].absoluteFilePath());
			if (!loader.load()) {
				qFatal("%s", qPrintable(loader.errorString()));
				continue;
			}
			Core::BackendIface *backend = 0;
			backend = qobject_cast<Core::BackendIface*>(loader.instance());
			if (!backend)
				continue;
			const QString name = backend->info()->name();
			if (!Data::map.contains(name))
				Data::map[name] = backend;
		}
	}
	return Data::map;
}

const BackendMap &BackendManager::map() {
	return Data::map;
}
