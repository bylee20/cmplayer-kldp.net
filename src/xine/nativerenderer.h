#ifndef XINE_VIDEOWIDGET_H
#define XINE_VIDEOWIDGET_H

#include "xinevideoiface.h"
#include <core/nativerenderer.h>
#include <xcb/xcb.h>
#include <xine.h>

namespace Core {class PlayEngine;}

namespace Xine {

class XineStream;				class XineOsd;

class NativeRenderer : public Core::NativeRenderer, public XineVideoIface {
	Q_OBJECT
public:
	NativeRenderer(Core::PlayEngine *engine, XineStream *stream);
	~NativeRenderer();
	Core::VideoType type() const {return Core::Native;}
	int xineType() const {return XINE_VISUAL_TYPE_XCB;}
	void *visual() {return &m_visual;}
	void rerender();
	virtual void setVideoSize(const QSize &size);
	Core::AbstractOsdRenderer *createOsd();
private slots:
	void updateOsdRect(const QRect &rect);
private:
	void customEvent(QEvent *event);
	bool eventFilter(QObject *obj, QEvent *event);
	static void cbDestSize(void*, int, int, double,	int*, int*, double*);
	static void cbFrameOutput(void*, int, int, double, int*, int*, int*, int*, double*, int*, int*);
	xcb_visual_t m_visual;
	xcb_connection_t *m_connection;
	struct Data;
	Data *d;
};

}

#endif
