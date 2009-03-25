#include "utility.h"
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtCore/QRegExp>
#include <QtCore/QDebug>

namespace Core {

double Utility::desktopRatio() {
	static const double ratio = double(desktopSize().width())/double(desktopSize().height());
	return ratio;
}

const QSize &Utility::desktopSize() {
	static const QSize size = QApplication::desktop()->size();
	return size;
}

bool Utility::isEmpty(const QString &str) {
	static const QRegExp rx("(</?\\s*[a-z]+[^>]*>|&nbsp;|\\s)");
	return  str.toLower().remove(rx).isEmpty();
}

}
