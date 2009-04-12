#ifndef GST_BUSTHREAD_H
#define GST_BUSTHREAD_H

#include <QtCore/QThread>
#include <gst/gst.h>

namespace Gst {

class PlayEngine;
	
class BusThread : public QThread {
public:
	BusThread(PlayEngine *engine);
	~BusThread();
	void setPlayBin(GstElement *pipeline);
	void stop();
private:
	void run();
	PlayEngine *engine;
	GstElement *pipeline;
	GstBus *bus;
};
}

#endif
