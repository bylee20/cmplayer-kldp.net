#include "bushelper.hpp"
#include "playengine.hpp"
#include <QtCore/QDebug>
BusHelper::BusHelper(GstBus *bus, PlayEngine *engine)
: QThread(engine) {
	m_bus = bus;
	m_quit = false;
	m_engine = engine;
}

void BusHelper::run() {
	while (!m_quit) {
		if (m_bus) {
			GstMessage *msg = 0;
			if (!m_quit && (msg = gst_bus_pop(m_bus))) {
				QMetaObject::invokeMethod(m_engine
					, "handleGstMessage"
					, Qt::QueuedConnection
					, Q_ARG(void*, (void*)msg)
				);
			}
		}
		if (m_quit)
			break;
		msleep(2);
	}
}

void BusHelper::stop() {
	m_quit = true;
	if (!wait(30))
		terminate();
}
