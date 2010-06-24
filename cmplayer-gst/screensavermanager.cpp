#include "screensavermanager.hpp"
#include <QtGui/QX11Info>
#include <X11/Xlib.h>

ScreensaverManager::ScreensaverManager() {
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(reset()));
	m_timer.setInterval(10000);
}

void ScreensaverManager::setDisabled(bool disabled) {
	if (disabled)
		get().m_timer.start();
	else
		get().m_timer.stop();
}

void ScreensaverManager::reset() {
	XResetScreenSaver(QX11Info::display());
}

ScreensaverManager &ScreensaverManager::get() {
	static ScreensaverManager self;
	return self;
}
