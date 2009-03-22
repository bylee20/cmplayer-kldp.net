#include "backendiface.h"
#include "playengine.h"
#include "info.h"

namespace Xine {

Core::PlayEngine *BackendIface::createPlayEngine(QObject *parent) const {
	return new Xine::PlayEngine(parent);
}

Core::Info *BackendIface::info() const {
	static Info info;
	return &info;
}

}

Q_EXPORT_PLUGIN2(cmplayer_engine_xine, Xine::BackendIface);
