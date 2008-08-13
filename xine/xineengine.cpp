#include "xineengine.h"
#include "xinestream.h"

namespace Xine {

XineEngine::XineEngine() {
	m_xine = 0;
}

XineEngine::~XineEngine() {
	exit();
}

void XineEngine::initialize() {
	if (!m_xine)
		xine_init(m_xine = xine_new());
}

void XineEngine::exit() {
	if (!m_xine)
		return;
	for (int i=0; i<m_streams.size(); ++i)
		delete m_streams[i];
	m_streams.clear();
	xine_exit(m_xine);
	m_xine = 0;
}

void XineEngine::unregister(XineStream *stream) {
	int idx = m_streams.indexOf(stream);
	if (idx != -1)
		m_streams.removeAt(idx);
}

XineStream *XineEngine::createStream() {
	if (!m_xine)
		initialize();
	XineStream *stream = new XineStream(0);
	m_streams.append(stream);
	return stream;
}

}
