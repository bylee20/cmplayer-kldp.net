#ifndef XINEX11NATIVERENDERER_H
#define XINEX11NATIVERENDERER_H

#include <nativerenderer.h>

namespace Xine {

class X11NativeRenderer : public NativeRenderer {
	Q_OBJECT
public:
	X11NativeRenderer(Core::PlayEngine *engine, Xine::XineStream *stream);
	~X11NativeRenderer();
	int xineType() const;
	void *visual();
private:
	bool screenX11Event(XEvent *event);
	Display *m_display;
	x11_visual_t m_visual;
};

}

#endif
