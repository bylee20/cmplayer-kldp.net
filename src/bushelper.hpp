#ifndef BUSHELPER_HPP
#define BUSHELPER_HPP

#include <QtCore/QThread>
#include <gst/gst.h>

class PlayEngine;

class BusHelper : public QThread {
	Q_OBJECT
public:
	BusHelper(GstBus *bus, PlayEngine *engine);
	void stop();
private:
	void run();
	GstBus *m_bus;
	bool m_quit;
	PlayEngine *m_engine;
};

#endif // BUSHELPER_HPP
