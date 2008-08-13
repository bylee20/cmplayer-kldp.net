#ifndef MPLAYER_UTILITY_H
#define MPLAYER_UTILITY_H

#include <QTime>

class QSize;

namespace Xine {

class Utility {
public:
	static QTime msecsToTime(qint64 msecs) {
		return get()->zero.addMSecs(msecs);
	}
	static QString msecsToString(qint64 msecs, const QString &format = QString("hh:mm:ss")) {
		return msecsToTime(msecs).toString(format);
	}
	static qint64 timeToMSecs(const QTime &time) {
		return get()->zero.msecsTo(time);
	}
	static qint64 stringToMSecs(const QString &str, const QString &format = QString("hh:mm:ss")) {
		return timeToMSecs(QTime::fromString(str, format));
	}
	static const QTime &nullTime() {return get()->zero;}
	static double desktopRatio();
	static const QSize &desktopSize();
private:
	static Utility *get() {static Utility obj; return &obj;}
	Utility(): zero() {}
	const QTime zero;
};

}

#endif
