#ifndef MPLAYER_INFO_H
#define MPLAYER_INFO_H

#include <core/info.h>

namespace MPlayer {

class Info : public Core::Info {
public:
	virtual const QStringList &videoRenderer() const {
		if (d.vo.isEmpty()) getInfo(); return d.vo;}
	virtual const QStringList &audioRenderer() const {
		if (d.ao.isEmpty()) getInfo(); return d.ao;}
	virtual const QStringList &videoFilters() const {
		if (d.vf.isEmpty()) getInfo(); return d.vf;}
	virtual const QStringList &audioFilter() const {
		if (d.af.isEmpty()) getInfo(); return d.af;}
	virtual QString name() const {return QString("MPlayer");}
	virtual double maximumAmplifyingRate() const {return 10.0;}
private:
	void getInfo() const;
	struct Data {
		Data();
		QStringList vo, ao, vf, af;
	};
	static Data d;
};

}

#endif
