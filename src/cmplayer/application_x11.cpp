#include "application_x11.hpp"

#ifdef Q_WS_X11

#include <QtGui/QApplication>
#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include <QtGui/QX11Info>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

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
	qDebug() << onTop;
	XEvent e;

	memset(&e, 0, sizeof(e));
	e.xclient.type = ClientMessage;
	e.xclient.message_type = XInternAtom(QX11Info::display(), "_NET_WM_STATE", False);
	e.xclient.display = QX11Info::display();
	e.xclient.window = wid;
	e.xclient.format = 32;
	e.xclient.data.l[0] = onTop ? 1 : 0;
	e.xclient.data.l[1] = XInternAtom(QX11Info::display(), "_NET_WM_STATE_ABOVE", False);
	e.xclient.data.l[2] = XInternAtom(QX11Info::display(), "_NET_WM_STATE_STAYS_ON_TOP", False);
	qDebug() << XSendEvent(QX11Info::display(), QX11Info::appRootWindow(), False, SubstructureRedirectMask, &e);

//	wid = QX11Info::appRootWindow();
//	Display *display = QX11Info::display();
//	Atom actual_type;
//	int actual_format;
//	unsigned long nitems;
//	unsigned long bytes;
//	Atom *data;
//	const int status = XGetWindowProperty(
//		display, wid, XInternAtom(display, "_NET_WM_STATE", True), 0, (~0L), False
//		, XA_ATOM, &actual_type, &actual_format, &nitems, &bytes, (unsigned char**)&data);
//	if (status != Success) {
//		qDebug() << "cannot get window property!";
//		return;
//	}
//	Q_ASSERT((nitems == 0) || (actual_format == 32));
//	Atom *atoms = new Atom[nitems + 2];
//	uint count = 0;
//	if (onTop) {
//		bool hasOnTop = false, hasAbove = false;
//		for (; count<nitems; ++count) {
//			atoms[count] = data[nitems];
//			const char *name = XGetAtomName(display, data[count]);
//			if (!hasOnTop)
//				hasOnTop = strcmp("_NET_WM_STATE_STAYS_ON_TOP", name) == 0;
//			if (!hasAbove)
//				hasAbove = strcmp("_NET_WM_STATE_ABOVE", name) == 0;

//		}
//		if (!hasAbove) {
//			atoms[count] = XInternAtom(QX11Info::display(), "_NET_WM_STATE_ABOVE", False);
//			++count;
//		}
//		if (!hasOnTop) {
//			atoms[count] = XInternAtom(QX11Info::display(), "_NET_WM_STATE_STAYS_ON_TOP", False);
//			++count;
//		}
//	} else {
//		for (uint i=0; i<nitems; ++i) {
//			const char *name = XGetAtomName(display, data[i]);
//			qDebug() << name;
//			if (strcmp("_NET_WM_STATE_STAYS_ON_TOP", name) == 0)
//				continue;
//			if (strcmp("_NET_WM_STATE_ABOVE", name) == 0)
//				continue;
//			atoms[count] = data[i];
//			++count;
//		}
//	}
//	qDebug() << count << nitems;
//	if (count == nitems)
//		return;
//	if (count == 0) {
//		XDeleteProperty(display, wid, XInternAtom(display, "_NET_WM_STATE", False));
//	} else {
//		XChangeProperty(display, wid, XInternAtom(display, "_NET_WM_STATE", False), XA_ATOM, 32, PropModeReplace
//			, (const unsigned char*)atoms, count);
//	}
//	QApplication::syncX();
}

QStringList ApplicationX11::devices() const {
	return QStringList();
}

#endif
