#ifndef XINE_XINEOSD_H
#define XINE_XINEOSD_H

#include <QFont>
#include <QColor>
#include <QObject>

namespace Xine {

class VideoOutput;

class XineOsd : public QObject {
	Q_OBJECT
public:
	class Clut;
	struct Style {
		enum Scale {FitToHeight, FitToWidth, FitToDiagonal};
		Style(): font(), borderColor(Qt::black), textColor(Qt::white)
		, size(0.05), scale(FitToDiagonal), highQuality(false) {}
		bool operator != (const Style &rhs) const {
			return (font != rhs.font) || (textColor != rhs.textColor)
					|| (borderColor != rhs.borderColor) || (size != rhs.size)
					|| (scale != rhs.scale) || (highQuality != rhs.highQuality);
		}
		bool operator == (const Style &rhs) const {return !((*this) != rhs);}
		QFont font; QColor borderColor, textColor; double size; Scale scale; bool highQuality;
	};
	enum MarginPos {MLeft = 0, MRight = 1, MTop = 2, MBottom = 3};
	XineOsd(VideoOutput *video);
	~XineOsd();
	void setStyle(const Style &style);
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
	void drawImage(const QPixmap &pixmap);
	void updateFontSize();
	QPoint getPos(const QSize &size) const;
	class Data;
	Data *d;
	double m_margins[4];
	bool m_visible;
	Style m_style;
	Qt::Alignment m_align;
};

}

#endif

