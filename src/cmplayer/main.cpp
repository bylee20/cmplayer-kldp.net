#include "mainwindow.h"
#include "appconnection.h"
#include <QtGui/QApplication>
#include <QtCore/QDir>
#include <QtCore/QDebug>

int main (int argc, char **argv) {
	QApplication app(argc, argv);
	app.setQuitOnLastWindowClosed(false);
	AppConnection connection;
	connection.createConnection();
	return app.exec();
}

