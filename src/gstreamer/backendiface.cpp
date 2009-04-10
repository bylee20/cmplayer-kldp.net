#include "backendiface.h"
#include "playengine.h"
#include "info.h"

namespace Gst {

Core::PlayEngine *BackendIface::createPlayEngine(QObject *parent) const {
	return new Gst::PlayEngine(parent);
}

Core::Info *BackendIface::info() const {
	static Info info;
	return &info;
}

}

Q_EXPORT_PLUGIN2(cmplayer_gstreamer, Gst::BackendIface);
