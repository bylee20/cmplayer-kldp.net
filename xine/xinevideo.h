#ifndef XINEWIDGET_H
#define XINEWIDGET_H

#include <QWidget>
#include <xine.h>
#include <xcb/xcb.h>

class QTimer;

namespace Xine {

class XineEngine;

class XineVideo : public QWidget {
	Q_OBJECT
public:
	XineVideo(XineEngine *engine, QWidget *parent);
	~XineVideo();
	//bool isValid() const {return m_valid;}
	bool open(const QString &driver);
	void close();
	bool isOpen() const {return m_open;}
	xine_video_port_t *&port() {return m_port;}
	const QString &driver() const {return m_driver;}
protected:
	virtual void paintEvent(QPaintEvent *event);
private:
	static void cbDestSize(void *user_data, int video_width, int video_height, double video_pixel_aspect,
			int *dest_width, int *dest_height, double *dest_pixel_aspect);
	static void cbFrameOutput(void *user_data, int video_width, int video_height,
			double video_pixel_aspect, int *dest_x, int *dest_y,
			int *dest_width, int *dest_height,
			double *dest_pixel_aspect, int *win_x, int *win_y);
	XineEngine *m_engine;
	xine_t *m_xine;
	xine_video_port_t *m_port;
	xcb_visual_t m_visual;
	xcb_connection_t *m_connection;
	bool m_open;
	QString m_driver;
};

}

#endif

