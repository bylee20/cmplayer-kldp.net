#ifndef CORE_OSDSTYLE_H
#define CORE_OSDSTYLE_H

#include <QtGui/QColor>
#include <QtGui/QFont>

class QSettings;

namespace Core {

class OsdStyle {
public:
	enum Scale {FitToWidth, FitToHeight, FitToDiagonal};
	OsdStyle(): bgColor(Qt::black), fgColor(Qt::white) {
		alignment = Qt::AlignCenter;
		borderWidth = 0.05;
		textSize = 0.05;
		scale = FitToDiagonal;
	}
	void save(QSettings *set, const QString &group) const;
	void load(QSettings *set, const QString &group);
	QFont font;
	QColor bgColor, fgColor;
	double borderWidth, textSize;
	Qt::Alignment alignment;
	Scale scale;
};

}

#endif
