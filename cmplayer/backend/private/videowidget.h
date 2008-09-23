#ifndef BACKEND_VIDEOWIDGET_H
#define BACKEND_VIDEOWIDGET_H

#include <QtGui/QWidget>

namespace Backend {

class VideoOutput;

class VideoWidget : public QWidget {
	Q_OBJECT
public:
	VideoWidget(VideoOutput *video);
	virtual QSize sizeHint() const;
protected:
	void resizeEvent(QResizeEvent *event);
private:
	friend class VideoOutput;
	VideoOutput *m_video;
};

}

#endif
