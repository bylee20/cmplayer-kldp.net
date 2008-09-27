#ifndef BACKEND_XINE_FACTORYIFACE_H
#define BACKEND_XINE_FACTORYIFACE_H

#include <QtCore/QObject>
#include <backend/private/backendiface.h>

namespace Backend {

class Info;						class PlayEngine;

namespace Xine {

class BackendIface : public QObject, public Backend::BackendIface {
	Q_OBJECT
	Q_INTERFACES(Backend::BackendIface)
public:
	PlayEngine *createPlayEngine(QObject *parent) const;
	Info *info() const;
	Config *config() const;
};

}

}

#endif
