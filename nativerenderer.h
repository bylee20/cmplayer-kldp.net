#ifndef NATIVERENDERER_H
#define NATIVERENDERER_H

#include <core/nativerenderer.h>
#include <gst/gst.h>
#include "gstvideoiface.h"

namespace Gst {

class NativeRenderer : public Core::NativeRenderer, public GstVideoIface {
	Q_OBJECT
public:
	NativeRenderer(Core::PlayEngine *engine);
	~NativeRenderer();
	void setOverlay();
private:
	bool eventFilter(QObject *obj, QEvent *event);
// 	void paintEvent(QPaintEvent *event);
	void windowExposed();
	struct Data {

	};
	Data *d;
};


} // namespace Gst
#endif // NATIVERENDERER_H
