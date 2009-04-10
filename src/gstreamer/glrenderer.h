#ifndef GST_GLRENDERER_H
#define GST_GLRENDERER_H

#include <core/glrenderer.h>
#include <gst/gst.h>

namespace Gst {

class PlayEngine;

class GLRenderer : public Core::GLRenderer {
	Q_OBJECT
public:
	GLRenderer(PlayEngine *engine);
	~GLRenderer();
	void setVideoBin(GstElement *bin);
protected:
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
private:
	void navEvent(const char *name, const int button, const QPoint &pos);
	struct Data;
	Data *d;
};

}

#endif
