#ifndef BACKEND_MPLAYER_CONFIG_H
#define BACKEND_MPLAYER_CONFIG_H

#include <QString>
#include <backend/config.h>
#include <backend/configwidget.h>

class QLineEdit;				class QPushButton;

namespace Backend {

namespace MPlayer {

class Config;

class ConfigWidget : public Backend::ConfigWidget {
	Q_OBJECT
private slots:
	void findMPlayer();
	void openMPlayer();
private:
	friend class Backend::MPlayer::Config;
	ConfigWidget(QWidget *parent);
	QLineEdit *mplayer, *options;
	QPushButton *find, *open;
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
	const QStringList &options() const {return d.options;}
	const QString &mplayer() const {return d.mplayer;}
private:
	struct Data {
		Data();
		double volAmp;
		QString vo, ao, mplayer;
		const QString filePath;
		QStringList options;
		bool loaded;
	};
	static Data d;
};

}

}

#endif
