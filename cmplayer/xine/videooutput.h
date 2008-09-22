#ifndef XINEVIDEOOUTPUT_H
#define XINEVIDEOOUTPUT_H

#include <QObject>
#include <QSize>
#include <xine.h>
#include <xcb/xcb.h>
#include <QRectF>
#include <backend/videooutput.h>
#include <QImage>

class QRect;

namespace Backend {

namespace Xine {

class XineStream;					class VideoWidget;
class PlayEngine;

class VideoOutput : public Backend::VideoOutput {
	Q_OBJECT
public:
	VideoOutput(PlayEngine *engine, XineStream *stream);
	~VideoOutput();
	void *visual();
	XineStream *stream() {return m_stream;}
	xine_video_port_t *&port() {return m_port;}
	QRect osdRect(bool forScaled) const;
	void updateSizeInfo();
	virtual bool supportsExpansion() const {return true;}
protected:
	virtual bool eventFilter(QObject *obj, QEvent *event);
private:
	static void cbDestSize(void*, int, int, double,	int*, int*, double*);
	static void cbFrameOutput(void*, int, int, double, int*, int*, int*, int*, double*, int*, int*);
	static int eq(double value) {
		return isSame(value, 0.0)
				? 32768 : qBound(0, qRound((value + 1.0)/2.0*65535), 65535);
	}
	QRectF videoRect() const;
	struct Video {
		xcb_visual_t visual;
		xcb_connection_t *connection;
	};
	virtual bool updateExpand(bool expand);
	virtual void updateHue(double value);
	virtual void updateContrast(double value);
	virtual void updateSaturation(double value);
	virtual void updateBrightness(double value);
	friend class XineStream;
	QSize visualSize(bool scaled) const;
	PlayEngine *m_engine;
	double m_pixelAspect;
	XineStream *m_stream;
	Video m_video;
	xine_video_port_t *m_port;
	QImage m_image;
};

}

}

#endif

