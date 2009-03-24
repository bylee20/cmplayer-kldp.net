#ifndef XINEXCBNATIVERENDERER_H
#define XINEXCBNATIVERENDERER_H

#include "nativerenderer.h"
#include <xcb/xcb.h>

namespace Xine {

class XcbNativeRenderer : public NativeRenderer {
Q_OBJECT
public:
	XcbNativeRenderer(Core::PlayEngine *engine, Xine::XineStream *stream);
	~XcbNativeRenderer();
	int xineType() const;
	void *visual();
private:
	bool eventFilter(QObject *obj, QEvent *event);
	xcb_visual_t m_visual;
	xcb_connection_t *m_connection;
};

}

#endif
