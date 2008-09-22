#ifndef BACKEND_XINE_CONFIG_H
#define BACKEND_XINE_CONFIG_H

#include <QString>
#include <backend/config.h>
#include <backend/configwidget.h>

namespace Backend {

namespace Xine {

class ConfigWidget : public Backend::ConfigWidget {
	Q_OBJECT
public:
	ConfigWidget(QWidget *parent);
};

class Config : public Backend::Config {
public:
	Config() {if (!d.loaded) {load(); d.loaded = true;}}
	QWidget *widget(QWidget *parent);
	void update(QWidget *widget);
	void save();
	void load();
	const QString &videoDriver() const {return d.vo;}
	const QString &audioDriver() const {return d.ao;}
	double volumeAmplifiaction() const {return d.volAmp;}
private:
	struct Data {
		Data(): volAmp(1.0), vo("auto"), ao("auto"), loaded(false) {}
		double volAmp;
		QString vo, ao;
		bool loaded;
	};
	static Data d;
};

}

}

#endif
