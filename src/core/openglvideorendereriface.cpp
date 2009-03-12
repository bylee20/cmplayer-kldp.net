#include "openglvideorendereriface.h"

namespace Core {

OpenGLVideoRendererIface::OpenGLVideoRendererIface(QWidget *widget)
: VideoRendererIface(widget), m_vscale(1.0), m_hscale(1.0), m_obj(0) {
}


OpenGLVideoRendererIface::~OpenGLVideoRendererIface() {
}


}
