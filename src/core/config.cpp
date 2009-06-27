#include "config.h"
#include "info.h"
#include <QtCore/QSettings>

namespace Core {

void Config::apply() const {
	obj()->update();
	save();
}

void Config::load() const {
	QSettings set(Info::privatePath() + "/backend.ini", QSettings::IniFormat);
	set.beginGroup(id());
	const Map map = item();
	Map::const_iterator it = map.begin();
	for (; it != map.end(); ++it)
		setData(it.key(), set.value(it.key(), it.value().data()));
	set.endGroup();
}

void Config::save() const {
	QSettings set(Info::privatePath() + "/backend.ini", QSettings::IniFormat);
	set.beginGroup(id());
	const Map map = item();
	Map::const_iterator it = map.begin();
	for (; it != map.end(); ++it)
		set.setValue(it.key(), it.value().data());
	set.endGroup();
}

}
