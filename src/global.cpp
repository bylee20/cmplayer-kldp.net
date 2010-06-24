#include "global.hpp"
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QX11Info>
#include <QtCore/QRegExp>
#include <QtCore/QDebug>
#include <QtCore/QTime>
#include <QtCore/QThread>
#include <QtCore/QTimer>

namespace Global {

struct Data {
	Data(): zero() {

	}
	const QTime zero;
	QTimer ssTimer;
};

static Data *data() {
	static Data d;
	return &d;
}



double desktopRatio() {
	const QSize size = desktopSize();
	return double(size.width())/double(size.height());
}

QSize desktopSize() {
	return QApplication::desktop()->size();
}

//QString toPlainText(const QString &str) {
//	static const QRegExp rx("(</?\\s*[a-zA-Z]+[^>]*>|&[nN][bB][sS][pP];|[\\s\\n\\r]+)");
//	return  QString(str).remove(rx);
//}

QTime secsToTime(int secs) {
	return data()->zero.addSecs(secs);
}

QTime msecsToTime(qint64 msecs) {
	return data()->zero.addMSecs(msecs);
}

QString msecsToString(qint64 msecs, const QString &format) {
	return msecsToTime(msecs).toString(format);
}

QString secsToString(int secs, const QString &format) {
	return secsToTime(secs).toString(format);
}

qint64 timeToMSecs(const QTime &time) {
	return data()->zero.msecsTo(time);
}

qint64 stringToMSecs(const QString &str, const QString &format) {
	return timeToMSecs(QTime::fromString(str, format));
}

const QTime &nullTime() {
	return data()->zero;
}

}
