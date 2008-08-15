#include "utility.h"
#include <QApplication>
#include <QDesktopWidget>

namespace Xine {

double Utility::desktopRatio() {
	static const double ratio = double(desktopSize().width())/double(desktopSize().height());
	return ratio;
}

const QSize &Utility::desktopSize() {
	static const QSize size = QApplication::desktop()->size();
	return size;
}

}
