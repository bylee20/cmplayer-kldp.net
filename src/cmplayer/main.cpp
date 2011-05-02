#include "application.hpp"
#include <QtCore/QDebug>

int main(int argc, char **argv) {
	Application app(argc, argv);
	const int ret = app.exec();
	return ret;
}
