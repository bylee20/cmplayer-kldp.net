#include "application.hpp"
#include <QtCore/QDebug>
#include "richtext.hpp"

int main(int argc, char **argv) {
	Application app(argc, argv);
//	RichText text;
//	text.setText("<p style=\"0px;\"><span style=\" color:#ffffff;\">400x300</span>");
	const int ret = app.exec();
	return ret;
}
