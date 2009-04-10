#ifndef BACKENDIFACE_H
#define BACKENDIFACE_H

#include <core/backendiface.h>

namespace Gst {

class BackendIface : public QObject, public Core::BackendIface {
	Q_OBJECT
	Q_INTERFACES(Core::BackendIface)
public:
	Core::PlayEngine *createPlayEngine(QObject *parent) const;
	Core::Info *info() const;
};

}

#endif
