#ifndef BACKENDMANAGER_H
#define BACKENDMANAGER_H

#include "factoryiface.h"
#include <QtCore/QMap>

class QStringList;				class QString;

namespace Backend {

class FactoryIface;
typedef QMap<QString, FactoryIface*> FactoryMap;

class Manager {
public:
	~Manager();
	static Manager *get();
	const QStringList &fileNameList() const;
	FactoryIface *load(const QString &fileName);
	const FactoryMap &loadAll();
private:
	void getFileNames();
	Manager();
	struct Data;
	Data *d;
};

}

#endif
