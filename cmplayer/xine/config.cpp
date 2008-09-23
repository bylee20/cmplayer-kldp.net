#include "config.h"
#include "info.h"
#include <QtCore/QSettings>

namespace Backend {

namespace Xine {

Config::Data Config::d;

ConfigWidget::ConfigWidget(QWidget *parent)
: Backend::ConfigWidget(Info(), parent) {}

QWidget *Config::widget(QWidget *parent) {
	ConfigWidget *w = new ConfigWidget(parent);
	w->setVolumeAmplification(d.volAmp);
	w->setVideoDriver(d.vo);
	w->setAudioDriver(d.ao);
	return w;
}

void Config::update(QWidget *widget) {
	ConfigWidget *w = qobject_cast<ConfigWidget*>(widget);
	if (!w)
		return;
	d.volAmp = w->volumeAmplification();
	d.vo = w->videoDriver();
	d.ao = w->audioDriver();
}

void Config::save() {
	QSettings set(Info().privatePath() + "/config_xine.ini", QSettings::IniFormat);
	set.beginGroup("Audio");
	set.setValue("AudioDriver", d.ao);
	set.setValue("VolumeAmplification", d.volAmp);
	set.endGroup();
	set.beginGroup("Video");
	set.setValue("VideoDriver", d.vo);
	set.endGroup();
}

void Config::load() {
	QSettings set(Info().privatePath() + "/config_xine.ini", QSettings::IniFormat);
	set.beginGroup("Audio");
	d.ao = set.value("AudioDriver", "auto").toString();
	d.volAmp = set.value("VolumeAmplification", 1.0).toDouble();
	set.endGroup();
	set.beginGroup("Video");
	d.vo = set.value("VideoDriver", "auto").toString();
	set.endGroup();
}

}

}
