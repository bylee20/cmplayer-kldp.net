#ifndef CORE_UTILITY_H
#define CORE_UTILITY_H

#include <QtCore/QString>
#include <QtCore/QObject>

class QSize;				class QTime;

namespace Core {

class Utility : public QObject {
	Q_OBJECT
public:
	static QTime secsToTime(int secs);
	static QTime msecsToTime(qint64 msecs);
	static QString msecsToString(qint64 msecs, const QString &format = QString("hh:mm:ss"));
	static QString secsToString(int secs, const QString &format = QString("hh:mm:ss"));
	static qint64 timeToMSecs(const QTime &time);
	static qint64 stringToMSecs(const QString &str, const QString &format = QString("hh:mm:ss"));
	static const QTime &nullTime();
	static double desktopRatio();
	static const QSize &desktopSize();
//	static QString toPlainText(const QString &str);
	static void msleep(int msec);
	static void setScreensaverDisabled(bool disabled);
private slots:
	void resetScreensaver();
private:
	class Thread;
	class Data;
	static Utility *self();
	static Data *data() {return self()->d;}
	Utility();
	~Utility();
	Data *d;
};

}

#endif
