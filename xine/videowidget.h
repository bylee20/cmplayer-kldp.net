#ifndef XINEVIDEOWIDGET_H_
#define XINEVIDEOWIDGET_H_

#define USE_XCB 1

#include <QWidget>
#include <xine.h>

#if (USE_XCB)
#include <xcb/xcb.h>
#endif


namespace Xine {

class VideoOutput;

class VideoWidget : public QWidget {
	Q_OBJECT
public:
	VideoWidget(VideoOutput *video, QWidget *parent);
	~VideoWidget();
	virtual QSize sizeHint() const;
	void init();
	QRect videoRect() const;
protected:
#if (USE_XCB)
	void paintEvent(QPaintEvent *event);
#else
	bool x11Event(XEvent *event);
#endif
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
private:
	friend class VideoOutput;
	static void cbDestSize(void*, int, int, double,	int*, int*, double*);
	static void cbFrameOutput(void*, int, int, double, int*, int*, int*, int*, double*, int*, int*);
	VideoOutput *m_video;
#if (USE_XCB)
	xcb_visual_t m_visual;
	xcb_connection_t *m_connection;
#else
	x11_visual_t m_visual;
	Display *m_connection;
#endif
};

}

#endif
