#include "appstate.hpp"
#include "enums.hpp"
#include "overlay.hpp"
#include "global.hpp"
#include <QtCore/QSize>
#include <QtCore/QRect>
#include <QtCore/QStringList>
#include <QtCore/QSettings>

AppState::Data *AppState::d = 0;

void AppState::init() {
	d = new Data;
	d->keys.resize(TypeMax);
	d->values.resize(TypeMax);
#define INIT(type, init) {d->keys[type] = #type; d->values[type] = QVariant(init);}
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
	INIT(StaysOnTop, Enum::StaysOnTop::Playing.name());
	INIT(SubLetterbox, true);
	INIT(SubAlignTop, false);
	INIT(OverlayType, (int)Overlay::Auto);
#undef INIT
}

void AppState::save() const {
	QSettings set;
	set.beginGroup("app-state");
	for (int i=0; i<d->values.size(); ++i)
		set.setValue(d->keys[i], d->values[i]);
	set.endGroup();
}

void AppState::load() {
	QSettings set;
	set.beginGroup("app-state");
	for (int i=0; i<d->values.size(); ++i)
		d->values[i] = set.value(d->keys[i], d->values[i]);
	set.endGroup();
}
