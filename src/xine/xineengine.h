#ifndef XINE_XINEENGINE_H
#define XINE_XINEENGINE_H

#include <xine.h>

namespace Xine {

class XineStream;

class XineEngine {
public:
	~XineEngine();
	static xine_t *xine();
private:
	XineEngine();
	xine_t *m_xine;
};

}

#endif
