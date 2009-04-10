#ifndef GSTVIDEOIFACE_H
#define GSTVIDEOIFACE_H

#include <gst/gst.h>

namespace Core {
class VideoRendererIface;
}

namespace Gst {

class GstVideoIface {
public:
	GstVideoIface(Core::VideoRendererIface *renderer);
	virtual ~GstVideoIface();
	GstElement *sink() {return d->sink;}
	Core::VideoRendererIface *renderer() {return d->renderer;}
protected:
	void setSink(GstElement *sink);
private:
	struct Data {
		GstElement *sink;
		Core::VideoRendererIface *renderer;
	};
	Data *d;
};

}

#endif
