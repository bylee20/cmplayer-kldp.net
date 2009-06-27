#include "config.h"
#include "info.h"

namespace Xine {

Config::Data Config::d;

Config::Data::Data() {
	QString name, desc;
	QVariant data;

	name = "MPlayer Path";
	desc = "Specify the path of mplayer executable file.";
	data = "mplayer";
	d.map[name] = Item(name, desc, data);
}

const Config::Map &Config::item() const {
	return d.map;
}

void Config::setData(const QString &name, const QVariant &data) const {
	Map::iterator it = d.map.find(name);
	if (it != d.map.end() && data.canConvert(it.value().data().type()))
		it.value().setData(data);
}

QString Config::id() const {
	return Info().name();
}

}
