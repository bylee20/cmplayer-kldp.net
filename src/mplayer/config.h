#ifndef MPLAYER_CONFIG_H
#define MPLAYER_CONFIG_H

#include <core/config.h>
#include <QtCore/QObject>

namespace MPlayer {

class Config : public Core::Config {
public:
	Config();
	virtual const Map &item() const;
	virtual void setData(const QString &name, const QVariant &data) const;
	virtual Obj *obj() const {return &d.obj;}
	QString executable() const;
	QStringList option() const;
private:
	QString id() const;
	struct Data {
		Data();
		Map map;
		Obj obj;
	};
	static Data d;
};

}

#endif

// #ifndef BACKEND_MPLAYER_CONFIG_H
// #define BACKEND_MPLAYER_CONFIG_H
// 
// #if 0
// 
// #include <QtCore/QString>
// // #include <core/config.h>
// // #include <core/configwidget.h>
// 
// class QLineEdit;				class QPushButton;
// 
// namespace Core {
// 
// namespace MPlayer {
// 
// class Config;
// 
// class ConfigWidget : public Core::ConfigWidget {
// 	Q_OBJECT
// private slots:
// 	void findMPlayer();
// 	void openMPlayer();
// private:
// 	friend class Core::MPlayer::Config;
// 	ConfigWidget(QWidget *parent);
// 	QLineEdit *mplayer, *options;
// 	QPushButton *find, *open;
// };
// 
// class Config : public Core::Config {
// public:
// 	Config() {if (!d.loaded) {load(); d.loaded = true;}}
// 	QWidget *widget(QWidget *parent);
// 	QList<Core::WizardPage*> wizard(QWidget *parent);
// 	void update(Core::WizardPage *wizard);
// 	void update(QWidget *widget);
// 	void save();
// 	void load();
// 	const QString &videoDriver() const {return d.vo;}
// 	const QString &audioDriver() const {return d.ao;}
// 	double volumeAmplifiaction() const {return d.volAmp;}
// 	const QStringList &options() const {return d.options;}
// 	const QString &mplayer() const {return d.mplayer;}
// private:
// 	struct Data {
// 		Data();
// 		double volAmp;
// 		QString vo, ao, mplayer;
// 		const QString filePath;
// 		QStringList options;
// 		bool loaded;
// 	};
// 	static Data d;
// };
// 
// }
// 
// }
// 
// #endif
// 
// #endif
