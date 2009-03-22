#include "mainwindow.h"
#include "appconnection.h"
#include "application.h"
#include <QtCore/QDir>
#include <QtCore/QDebug>

int main (int argc, char **argv) {
	Application app(argc, argv);
// 	app.setQuitOnLastWindowClosed(false);
// 	AppConnection connection;
// 	connection.createConnection();
	return app.exec();
}

