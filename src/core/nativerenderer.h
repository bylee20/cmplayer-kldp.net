#ifndef BACKEND_NATIVERENDERER_H
#define BACKEND_NATIVERENDERER_H

#include <QtGui/QWidget>
#include "videorendereriface.h"

namespace Core {

class PlayEngine;

class NativeRenderer : public QWidget, public VideoRendererIface {
	Q_OBJECT
public:
	NativeRenderer(PlayEngine *engine);
	~NativeRenderer();
	VideoType type() const {return NativeVideo;}
	void rerender();
	virtual void setVideoSize(const QSize &size);
	int screenWinId() const;
	void setFrameInfo(const VideoFrame::Info &frame);
	void setFrameSize(const QSize &size);
	const QRect &osdRect() const {return d->osdRect;}
	QSize sizeHint() const {return widgetSizeHint();}
signals:
	void osdRectChanged(const QRect &rect);
protected:
	QWidget *video() {return d->video;}
	QWidget *screen() {return d->screen;}
	bool eventFilter(QObject *obj, QEvent *event);
	void resizeEvent(QResizeEvent *event);
	void calculate();
	virtual bool screenX11Event(XEvent *event);
private:
	class Screen;
	friend class Screen;
	struct Data {
		PlayEngine *engine;
		QWidget *video;
		QWidget *screen;
		QRect osdRect;
	};
	Data *d;
};

}

#endif
