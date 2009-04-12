#include "info.h"
#include "videorendererfactory.h"
#include <core/openglfactory.h>

namespace Gst {

Info::Data Info::d;

Info::Data::Data() {
}

const QStringList &Info::videoRenderer() const {
	if (d.videoRenderers.isEmpty())
		d.videoRenderers = VideoRendererFactory::keys();
	return d.videoRenderers;
}

const QStringList &Info::audioRenderer() const {
	if (d.audioRenderers.isEmpty()) {
		d.audioRenderers.push_back("auto");
		d.audioRenderers.append("alsa");
	}
	return d.audioRenderers;
}

}
