#ifndef NATIVERENDERER_H
#define NATIVERENDERER_H

#include <core/nativerenderer.h>
#include <gst/gst.h>
#include "gstvideoiface.h"

namespace Gst {

class PlayEngine;

class NativeRenderer : public Core::NativeRenderer, public GstVideoIface {
	Q_OBJECT
public:
	enum XType {Xv, X11};
	NativeRenderer(PlayEngine *engine, XType type);
	~NativeRenderer();
	XType xType() const {return d->type;}
	static QString sinkName(XType type);
	static bool isAvaiable(XType type);
public slots:
	void setOverlay();
private:
	static GstElement *makeSink(const QString &name);
	bool eventFilter(QObject *obj, QEvent *event);
// 	void paintEvent(QPaintEvent *event);
	void windowExposed();
	struct Data {
		XType type;
	};
	Data *d;
};


} // namespace Gst
#endif // NATIVERENDERER_H
