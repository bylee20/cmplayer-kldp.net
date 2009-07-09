#ifndef XINE_VIDEOWIDGET_H
#define XINE_VIDEOWIDGET_H

#include "xinevideoiface.h"
#include "xinestream.h"
#include <core/nativerenderer.h>
#include <core/playengine.h>
#include <xine.h>

namespace Core {class PlayEngine;}

namespace Xine {

class XineStream;				class XineOsd;

class NativeRenderer : public Core::NativeRenderer, public XineVideoIface {
	Q_OBJECT
public:
	NativeRenderer(Core::PlayEngine *engine, XineStream *stream);
	~NativeRenderer();
	Core::VideoType type() const {return Core::NativeVideo;}
	void rerender();
	virtual void setVideoSize(const QSize &size);
	Core::AbstractOsdRenderer *createOsd();
	const Core::PlayEngine *engine() const;
	QImage grabCurrentFrame() const;
private slots:
	void updateOsdRect(const QRect &rect);
protected:
	bool eventFilter(QObject *obj, QEvent *event);
	static void cbDestSize(void*, int, int, double,	int*, int*, double*);
	static void cbFrameOutput(void*, int, int, double, int*, int*, int*, int*, double*, int*, int*);
private:
	void customEvent(QEvent *event);
	struct Data;
	Data *d;
};

}

#endif
