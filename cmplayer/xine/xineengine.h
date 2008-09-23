#ifndef XINEENGINE_H
#define XINEENGINE_H

#include <QtCore/QList>
#include <xine.h>

namespace Backend {

namespace Xine {

class XineStream;					class PlayEngine;

class XineEngine {
public:
	~XineEngine();
	xine_t *xine() {return m_xine;}
	XineStream *createStream(PlayEngine *engine);
	static XineEngine *get();
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

}

#endif
