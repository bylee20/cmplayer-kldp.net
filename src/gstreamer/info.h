#ifndef GST_INFO_H
#define GST_INFO_H

#include <core/info.h>

namespace Gst {

class Info : public Core::Info {
public:
	virtual QStringList videoRenderers() const {return d.videoRenderers;}
	virtual QStringList audioRenderers() const {return d.audioRenderers;}
	virtual QString name() const {return "GStreamer";}
private:
	struct Data {
		Data();
		QStringList videoRenderers;
		QStringList audioRenderers;
	};
	static Data d;
};

}

#endif
