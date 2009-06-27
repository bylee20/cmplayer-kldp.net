#ifndef BACKENDMANAGER_H
#define BACKENDMANAGER_H

#include <QtCore/QMap>
#include <core/backendiface.h>

namespace Core {class BackendIface;}

typedef QMap<QString, Core::BackendIface*> BackendMap;

class BackendManager {
public:
	static const BackendMap &load();
	static const BackendMap &map();
private:
	BackendManager();
	struct Data;
};

#endif // BACKENDMANAGER_H
