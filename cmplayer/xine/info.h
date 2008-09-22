#ifndef BACKEND_XINEINFO_H
#define BACKEND_XINEINFO_H

#include <backend/info.h>

namespace Backend {

namespace Xine {

class Info : public Backend::Info {
public:
	Info() {}
	~Info() {}
	virtual bool supportsVolumeAmplification() const {return true;}
	virtual double maximumVolumeAmplification() const {return 2.0;}
	virtual bool supportsExpansion() const {return true;}
	virtual bool supportsSeekingByBytes() const {return true;}
	virtual bool supportsSeekingByDrag() const {return true;}
	virtual bool supportsDvdMenu() const {return true;}
	virtual bool supportsColorSubtitle() const {return true;}
	const QString &compileVersion() const {return d.cv;}
	const QString &runtimeVersion() const {return d.rv;}
	const QStringList &videoDrivers() const {
		if (d.vos.isEmpty())
			return const_cast<Info*>(this)->getVideoDrivers();
		return d.vos;
	}
	const QString &name() const {return d.name;}
	const QStringList &audioDrivers() const {
		if (d.aos.isEmpty())
			return const_cast<Info*>(this)->getAudioDrivers();
		return d.aos;
	}
	const Extensions &videoExtensions() const {return d.ves;}
	const Extensions &audioExtensions() const {return d.aes;}
	const Extensions &subtitleExtensions() const {return d.ses;}
private:
	const QStringList &getVideoDrivers();
	const QStringList &getAudioDrivers();
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
