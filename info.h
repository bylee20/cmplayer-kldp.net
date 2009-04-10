#ifndef GST_INFO_H
#define GST_INFO_H

#include <core/info.h>

namespace Gst {

class Info : public Core::Info {
public:
	virtual const QStringList &videoRenderer() const {return d.videoRenderers;}
	virtual const QStringList &audioRenderer() const {return d.audioRenderers;}
	virtual QString name() const {return "GStreamer";}
	virtual double maximumAmplifyingRate() const {return 10.0;}
	virtual QString compileVersion() const {return QString();}
	virtual QString runtimeVersion() const {return QString();}
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
