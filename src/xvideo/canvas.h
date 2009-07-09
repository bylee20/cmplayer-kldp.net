#ifndef XVIDEO_CANVAS_H
#define XVIDEO_CANVAS_H

#include <QtGui/QWidget>

namespace XVideo {

class VideoRenderer;

class Canvas : public QWidget {
public:
	Canvas(VideoRenderer *parent);
	~Canvas();
	bool open();
	void close();
	bool checkXv();
private:
	void customEvent(QEvent *event);
	void paintEvent(QPaintEvent */*event*/);
	struct Data;
	inline Data &d() {return *m_d;}
	inline const Data &d() const {return *m_d;}
	Data *m_d;
};

}

#endif // CANVAS_H
