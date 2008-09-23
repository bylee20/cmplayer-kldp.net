#include "preferences.h"
#include "subtitle.h"
#include "general.h"
#include "interface.h"
#include "backend.h"
#include "../helper.h"
#include <QtCore/QSettings>

namespace Pref {

struct Preferences::Data {
	General general;
	Subtitle subtitle;
	Interface interface;
	BackendPref backend;
};

Preferences *get() {
	return Preferences::get();
}

Preferences::Preferences()
: d(new Data), m_gen(d->general), m_sub(d->subtitle)
, m_iface(d->interface), m_back(d->backend) {
	load();
}

Preferences::~Preferences() {
	delete d;
}

void Preferences::setGeneral(const General &general) {
	d->general = general;
}

void Preferences::setSubtitle(const Subtitle &subtitle) {
	d->subtitle = subtitle;
}

void Preferences::setInterface(const Interface &interface) {
	d->interface = interface;
}

void Preferences::setBackend(const BackendPref &backend) {
	d->backend = backend;
}

void Preferences::load() {
	QSettings set(Helper::configFile(), QSettings::IniFormat);
	int ver = set.value("Version", 0).toInt();
	if (ver != version)
		set.clear();
	d->general.load(&set);
	d->subtitle.load(&set);
	d->interface.load(&set);
	d->backend.load(&set);
}

void Preferences::save() const {
	QSettings set(Helper::configFile(), QSettings::IniFormat);
	set.setValue("Version", version);
	d->general.save(&set);
	d->subtitle.save(&set);
	d->interface.save(&set);
	d->backend.save(&set);
}

}
