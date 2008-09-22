#ifndef BACKEND_MPLAYER_INFO_H
#define BACKEND_MPLAYER_INFO_H

#include <backend/info.h>

namespace Backend {

namespace MPlayer {

class Info : public Backend::Info {
public:
	Info() {}
	~Info() {}
	virtual bool supportsVolumeAmplification() const {return true;}
	virtual double maximumVolumeAmplification() const {return 5.0;}
	virtual bool supportsExpansion() const {return true;}
	virtual bool supportsSeekingByBytes() const {return false;}
	virtual bool supportsSeekingByDrag() const {return false;}
	virtual bool supportsDvdMenu() const {return false;}
	const QString &name() const {return d.name;}
	const QString &compileVersion() const {return d.cv;}
	const QString &runtimeVersion() const {
		if (d.rv.isEmpty()) const_cast<Info*>(this)->getInfo(); return d.rv;}
	const QStringList &videoDrivers() const {
		if (d.vos.isEmpty()) const_cast<Info*>(this)->getInfo(); return d.vos;}
	const QStringList &audioDrivers() const {
		if (d.aos.isEmpty()) const_cast<Info*>(this)->getInfo(); return d.aos;}
	const Extensions &videoExtensions() const {return d.ves;}
	const Extensions &audioExtensions() const {return d.aes;}
	const Extensions &subtitleExtensions() const {return d.ses;}
private:
	void getInfo();
	struct Data {
		Data();
		QStringList vos, aos;
		QString cv, rv, name;
		Extensions ves, aes, ses;
	};
	static Data d;
};

}

}

#endif
