#include "info.h"

namespace Gst {

Info::Data Info::d;

Info::Data::Data() {
	videoRenderers.append("OpenGL Software Renderer");
	audioRenderers.append("Auto Selected Renderer");
}

}
