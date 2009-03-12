#include "xineengine.h"

namespace Xine {

xine_t *XineEngine::xine() {
	static XineEngine self;
	return self.m_xine;
}
	
XineEngine::XineEngine() {
	m_xine = xine_new();
	if (m_xine)
		xine_init(m_xine);
}

XineEngine::~XineEngine() {
	if (m_xine) {
		xine_exit(m_xine);
		m_xine = 0;
	}
}

}
