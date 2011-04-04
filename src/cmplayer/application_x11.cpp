#include "application_x11.hpp"

#ifdef Q_WS_X11

#include <QtCore/QTimer>
#include <QtGui/QX11Info>
#include <X11/Xlib.h>

struct ApplicationX11::Data {
	QTimer *ss_timer;
};

ApplicationX11::ApplicationX11(QObject *parent)
: QObject(parent), d(new Data) {
	d->ss_timer = new QTimer;
	d->ss_timer->setInterval(10000);
	connect(d->ss_timer, SIGNAL(timeout()), this, SLOT(ss_reset()));
}

ApplicationX11::~ApplicationX11() {
	delete d->ss_timer;
	delete d;
}

void ApplicationX11::setScreensaverDisabled(bool disabled) {
	if (disabled)
		d->ss_timer->start();
	else
		d->ss_timer->stop();
}

void ApplicationX11::ss_reset() {
	XResetScreenSaver(QX11Info::display());
}

void ApplicationX11::setAlwaysOnTop(WId wid, bool onTop) {
}

QStringList ApplicationX11::devices() const {
	return QStringList();
}

#endif
