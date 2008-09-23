#ifndef BACKEND_CONFIGWIDGET_H
#define BACKEND_CONFIGWIDGET_H

#include <QtGui/QWidget>

namespace Backend {

class Info;

class ConfigWidget : public QWidget {
	Q_OBJECT
public:
	~ConfigWidget();
	double volumeAmplification() const;
	QString videoDriver() const;
	QString audioDriver() const;
	void setVolumeAmplification(double volAmp);
	void setVideoDriver(const QString &driver);
	void setAudioDriver(const QString &driver);
protected:
	ConfigWidget(const Info &info, QWidget *parent);
private:
	struct Data;
	Data *d;
};

}

#endif
