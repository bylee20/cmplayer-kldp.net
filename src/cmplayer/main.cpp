#include "application.hpp"
#include <QtCore/QDebug>
#include <QtCore/QMetaType>
#include <QtCore/QDateTime>

int main(int argc, char **argv) {
	Application app(argc, argv);
	const int ret = app.exec();
	return ret;
}
