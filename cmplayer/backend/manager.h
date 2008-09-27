#ifndef BACKENDMANAGER_H
#define BACKENDMANAGER_H

#include "backendobject.h"
#include <QtCore/QMap>

class QStringList;				class QString;

namespace Backend {

typedef QList<BackendObject*> BackendList;

class Manager {
public:
	~Manager();
	static Manager *get();
	BackendObject *load(const QString &fileName);
	const BackendList &loadAll();
private:
	void getFileNames();
	Manager();
	struct Data;
	Data *d;
};

}

#endif
