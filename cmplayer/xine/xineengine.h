#ifndef XINEENGINE_H
#define XINEENGINE_H

#include <QList>
#include <xine.h>
#include "xinenamespace.h"

namespace Xine {

class XineStream;

class XineEngine {
public:
	~XineEngine();
	xine_t *xine() {return m_xine;}
	XineStream *createStream();
	static XineEngine *get() {static XineEngine e; return &e;}
	bool isInitialized() {return m_xine != 0;}
	void initialize();
	void exit();
	void unregister(XineStream *stream);
private:
	XineEngine();
	xine_t *m_xine;
	QList<XineStream*> m_streams;
};

}

#endif
