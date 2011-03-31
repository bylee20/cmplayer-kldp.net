#ifndef SCREENSAVERMANAGER_HPP
#define SCREENSAVERMANAGER_HPP

#ifdef Q_WS_X11

#include <QtCore/QObject>
#include <QtCore/QTimer>

class ScreensaverManager : public QObject {
	Q_OBJECT
public:
	static void setDisabled(bool disabled);
private slots:
	void reset();
private:
	static ScreensaverManager &get();
	ScreensaverManager();
	QTimer m_timer;
};

#endif

#endif // SCREENSAVERMANAGER_HPP
