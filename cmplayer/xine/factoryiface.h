#ifndef BACKEND_XINE_FACTORYIFACE_H
#define BACKEND_XINE_FACTORYIFACE_H

#include <QtCore/QObject>
#include <backend/factoryiface.h>

namespace Backend {

class Info;						class PlayEngine;

namespace Xine {

class FactoryIface : public QObject, public Backend::FactoryIface {
	Q_OBJECT
	Q_INTERFACES(Backend::FactoryIface)
public:
	PlayEngine *createPlayEngine(QObject *parent) const;
	Info *info() const;
	Config *config() const;
};

}

}

#endif
