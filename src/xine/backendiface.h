#ifndef XINE_BACKENDIFACE_H
#define XINE_BACKENDIFACE_H

#include <core/backendiface.h>

namespace Xine {

class BackendIface : public QObject, public Core::BackendIface {
	Q_OBJECT
	Q_INTERFACES(Core::BackendIface)
public:
	Core::PlayEngine *createPlayEngine(QObject *parent) const;
	Core::Info *info() const;
};

}

#endif
