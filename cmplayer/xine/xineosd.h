#ifndef XINE_XINEOSD_H
#define XINE_XINEOSD_H

#include <backend/osdstyle.h>
#include <QObject>

namespace Backend {

namespace Xine {

class VideoOutput;

class XineOsd : public QObject {
	Q_OBJECT
public:
	enum MarginPos {MLeft = 0, MRight = 1, MTop = 2, MBottom = 3};
	XineOsd(VideoOutput *video);
	~XineOsd();
	void setStyle(const OsdStyle &style);
	void drawText(const QString &text);
	bool isValid() const;
	void setVisible(bool visible);
	bool isVisible() const {return m_visible;}
	void setAlignment(Qt::Alignment align);
	void update();
	void repaint();
	void setMargin(MarginPos pos, double value);
	double margin(MarginPos pos) const {return m_margins[pos];}
public slots:
	void show() {setVisible(true);}
	void hide() {setVisible(false);}
private slots:
	void slotStreamOpenChanged(bool open);
	void updateRect();
private:
	void drawImage(const QImage &image);
	void updateFontSize();
	QPoint getPos(const QSize &size) const;
	class Data;
	Data *d;
	double m_margins[4];
	bool m_visible;
	OsdStyle m_style;
	Qt::Alignment m_align;
};

}

}

#endif

