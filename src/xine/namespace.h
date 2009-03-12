#ifndef BACKEND_H
#define BACKEND_H

#include <QtGui/QColor>
#include <QtGui/QFont>

namespace Backend {

enum State {Playing, Stopped, Paused};

class OsdStyle {
public:
	enum Scale {FitToWidth, FitToHeight, FitToDiagonal};
	OsdStyle(): borderColor(Qt::black), textColor(Qt::white) {
		alignment = Qt::AlignCenter;
		bottom = top = left = right = 0.0;
		borderWidth = 0.05;
		textSize = 0.05;
		scale = FitToDiagonal;
	}
	QFont font;
	QColor borderColor, textColor;
	double borderWidth, textSize;
	Qt::Alignment alignment;
	Scale scale;
private:
	double bottom, top, left, right;
};

}

#endif
