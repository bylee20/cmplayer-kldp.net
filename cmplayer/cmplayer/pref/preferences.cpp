#include "preferences.h"
#include "subtitle.h"
#include "general.h"
#include "interface.h"
#include "../helper.h"
#include <QSettings>

namespace Pref {

Preferences *get() {
	return Preferences::get();
}

Preferences::Preferences()
: m_general(new General()), m_subtitle(new Subtitle()), m_interface(new Interface()) {
	load();
}

void Preferences::setGeneral(const General &general) {
	*m_general = general;
}

void Preferences::setSubtitle(const Subtitle &subtitle) {
	*m_subtitle = subtitle;
}

void Preferences::setInterface(const Interface &interface) {
	*m_interface = interface;
}

void Preferences::load() {
	QSettings set(Helper::configFile(), QSettings::IniFormat);
	int ver = set.value("Version", 0).toInt();
	if (ver != version)
		set.clear();
	m_general->load(&set);
	m_subtitle->load(&set);
	m_interface->load(&set);
}

void Preferences::save() const {
	QSettings set(Helper::configFile(), QSettings::IniFormat);
	set.setValue("Version", version);
	m_general->save(&set);
	m_subtitle->save(&set);
	m_interface->save(&set);
}

}
