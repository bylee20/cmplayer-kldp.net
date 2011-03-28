#include "appstate.hpp"
#include <QtCore/QSize>
#include <QtCore/QRect>
#include <QtCore/QStringList>
#include <QtCore/QSettings>

AppState::Data::Data()
: keys(TypeMax), values(TypeMax) {
#define INIT(type, init) {keys[type] = #type;values[type] = QVariant(init);}
	INIT(AspectRatio, -1.0);
	INIT(Crop, -1.0);
	INIT(PlaySpeed, 1.0);
	INIT(Volume, 100);
	INIT(Muted, false);
	INIT(Amp, 1.0);
	INIT(SubPos, 1.0);
	INIT(SubSync, 0);
	INIT(LastOpenFile, QString());
	INIT(TrayFirst, true);
	INIT(VolNorm, true);
	INIT(OpenUrlList, QStringList());
	INIT(UrlEncoding, QString());
#undef INIT
}

AppState::Data AppState::d;

void AppState::save() const {
	QSettings set;
	set.beginGroup("app-state");
	for (int i=0; i<d.values.size(); ++i)
		set.setValue(d.keys[i], d.values[i]);
	set.endGroup();
}

void AppState::load() {
	QSettings set;
	set.beginGroup("app-state");
	for (int i=0; i<d.values.size(); ++i)
		d.values[i] = set.value(d.keys[i], d.values[i]);
	set.endGroup();
}

