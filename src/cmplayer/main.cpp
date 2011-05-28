#include "application.hpp"
#include <QtCore/QDebug>
#include "pref.hpp"
#include "skinhelper.hpp"
#include "skinmisc.hpp"

template <typename T>
static inline void qml(const char *name) {qmlRegisterType<T>("CMPlayerSkin", 0, 1, name);}

int main(int argc, char **argv) {
	qml<Skin::Helper>("Skin");
	qml<Skin::Spacer>("Spacer");
	qml<Skin::Storage>("Storage");
	qml<Skin::ValueHint>("ValueHint");
	qml<Skin::UrlHint>("UrlHint");
	qml<Skin::BackgroundHint>("BackgroundHint");
	qml<Skin::BorderHint>("BorderHint");
	qml<Skin::Media>("Media");
	qml<Skin::Frame>("Frame");
	qml<Skin::Gradient>("CGradient");
	qml<Skin::GradientStop>("CGradientStop");
	qml<Skin::RectItem>("CRectangle");
	qml<Skin::SliderItem>("CSlider");
	qml<Skin::ButtonItem>("CButton");
	Application app(argc, argv);
	Pref pref;
	Pref::obj = &pref;
	const int ret = app.exec();
	return ret;
}
