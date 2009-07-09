#include "creator.h"
#include "videorenderer.h"

namespace XVideo {

Core::SoftwareRendererIface *Creator::create() const {
	return new VideoRenderer;
}

} // namespace XVideo

Q_EXPORT_PLUGIN2(cmplayer_xvideo, XVideo::Creator);
