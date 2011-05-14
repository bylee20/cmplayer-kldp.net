#include "application.hpp"
#include <QtCore/QDebug>
#include "pref.hpp"
#include "skinhelper.hpp"

template <typename T>
static inline void qml(const char *name) {qmlRegisterType<T>("CMPlayerSkin", 0, 1, name);}

int main(int argc, char **argv) {
	qml<Skin::Helper>("Skin");
	qml<Skin::Screen>("Screen");
	qml<Skin::Storage>("Storage");
	qml<Skin::ValueHint>("ValueHint");
	qml<Skin::UrlHint>("UrlHint");
	qml<Skin::IconHint>("IconHint");
	qml<Skin::BackgroundHint>("BackgroundHint");
	qml<Skin::BorderHint>("BorderHint");
	qml<Skin::Button>("StyleButton");
	qml<Skin::Media>("Media");
	qml<Skin::Slider>("StyleSlider");
	qml<Skin::RectHint>("RectHint");
	qml<Skin::GrooveHint>("GrooveHint");
	qml<Skin::Rect>("Rect");
	qml<Skin::HandleHint>("HandleHint");
	Application app(argc, argv);
	Pref pref;
	Pref::obj = &pref;
	const int ret = app.exec();
	return ret;
}
