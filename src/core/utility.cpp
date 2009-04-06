#include "utility.h"
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QX11Info>
#include <QtCore/QRegExp>
#include <QtCore/QDebug>
#include <QtCore/QTime>
#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <X11/Xlib.h>

namespace Core {

class Utility::Thread : public QThread {
public:
	void msleep(int msec) {QThread::msleep(msec);}
private:
};
	
struct Utility::Data {
	Data(): zero() {}
	const QTime zero;
	Thread *thread;
	QTimer ssTimer;
};

Utility::Utility()
: d(new Data) {
	d->thread = new Thread;
	connect(&d->ssTimer, SIGNAL(timeout()), this, SLOT(resetScreensaver()));
	d->ssTimer.setInterval(10000);
}

Utility::~Utility() {
	d->ssTimer.stop();
	delete d->thread;
	delete d;
}

Utility *Utility::self() {
	static Utility self;
	return &self;
}

double Utility::desktopRatio() {
	static const double ratio = double(desktopSize().width())/double(desktopSize().height());
	return ratio;
}

const QSize &Utility::desktopSize() {
	static const QSize size = QApplication::desktop()->size();
	return size;
}

bool Utility::isEmpty(const QString &str) {
	static const QRegExp rx("(</?\\s*[a-z]+[^>]*>|&nbsp;|[\\s\\n\\r]+)");
	return  str.toLower().remove(rx).isEmpty();
}

QTime Utility::secsToTime(int secs) {
	return data()->zero.addSecs(secs);
}

QTime Utility::msecsToTime(qint64 msecs) {
	return data()->zero.addMSecs(msecs);
}

QString Utility::msecsToString(qint64 msecs, const QString &format) {
	return msecsToTime(msecs).toString(format);
}

QString Utility::secsToString(int secs, const QString &format) {
	return secsToTime(secs).toString(format);
}

qint64 Utility::timeToMSecs(const QTime &time) {
	return data()->zero.msecsTo(time);
}

qint64 Utility::stringToMSecs(const QString &str, const QString &format) {
	return timeToMSecs(QTime::fromString(str, format));
}

const QTime &Utility::nullTime() {
	return data()->zero;
}

void Utility::msleep(int msec) {
	data()->thread->msleep(msec);
}

void Utility::setScreensaverDisabled(bool disabled) {
	if (disabled)
		data()->ssTimer.start();
	else
		data()->ssTimer.stop();
}

void Utility::resetScreensaver() {
	XResetScreenSaver(QX11Info::display());
}

}
