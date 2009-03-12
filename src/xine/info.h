#ifndef XINE_INFO_H
#define XINE_INFO_H

#include <core/info.h>

namespace Xine {

class Info : public Core::Info {
public:
	virtual const QStringList &videoRenderer() const;
	virtual const QStringList &audioRenderer() const;
	virtual QString name() const {return QString("xine");}
	virtual double maximumAmplifyingRate() const {return 10.0;}
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
