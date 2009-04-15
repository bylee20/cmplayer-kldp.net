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
	virtual QString compileVersion() const;
	virtual QString runtimeVersion() const;
	const QString &executable() const {if (!d.got) getInfo(); return d.exe;}
	const QStringList &option() const {if (!d.got) getInfo(); return d.option;}
private:
	void getInfo() const;
	struct Data {
		Data();
		bool got;
		QStringList vo, ao, vf, af, option;
		QString ver, exe;
	};
	static Data d;
};

}

#endif
