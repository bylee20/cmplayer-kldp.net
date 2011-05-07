#include "application.hpp"
#include <QtCore/QDebug>
#include "pref.hpp"
#include "skinhelper.hpp"

int main(int argc, char **argv) {
	qmlRegisterType<SkinHelper>("CMPlayerSkin", 0, 1, "Skin");

	Application app(argc, argv);
	Pref pref;
	Pref::obj = &pref;
	const int ret = app.exec();
	return ret;
}
