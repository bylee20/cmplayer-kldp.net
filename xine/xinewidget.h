#ifndef XINEWIDGET_H
#define XINEWIDGET_H

#include <QWidget>
#include <xine.h>
#include <xcb/xcb.h>

class QTimer;

namespace Xine {

class XineEngine;

class XineWidget : public QWidget {
	Q_OBJECT
public:
	XineWidget(XineEngine *engine, const QString &videoDriver, QWidget *parent = 0);
	~XineWidget();
	bool isValid() const {return m_valid;}
	xine_video_port_t *videoPort() {return m_videoPort;}
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
	xine_video_port_t *m_videoPort;
	xcb_visual_t m_visual;
	xcb_connection_t *m_connection;
	bool m_valid;
};

}

#endif

