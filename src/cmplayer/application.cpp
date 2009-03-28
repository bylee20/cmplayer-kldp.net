#include "application.h"
#include "appconnection.h"
#include "mainwindow.h"
#include "pref.h"
#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtCore/QFileInfo>
#include <QtCore/QDebug>

struct Application::Data {
	AppConnection connection;
	MainWindow *main;
};

Application::Application(int &argc, char **argv)
: QApplication(argc, argv), d(new Data) {
	d->main = 0;
	setQuitOnLastWindowClosed(false);
	connect(&d->connection, SIGNAL(openRequested(const QString&))
			, this, SLOT(open(const QString &)));
	connect(&d->connection, SIGNAL(raiseRequested()), this, SLOT(raise()));
	QTimer::singleShot(10, this, SLOT(initialize()));
}

Application::~Application() {
	delete d->main;
	delete d;
}

void Application::initialize() {
	const QUrl url = MainWindow::getUrlFromCommandLine();
	if (!Pref::get()->singleApplication() || d->connection.isUnique()) {
		if (url.isEmpty())
			d->main = new MainWindow;
		else
			d->main = new MainWindow(url);
		d->main->show();
	} else {
		if (!url.isEmpty())
			d->connection.requestToOpen(url.toString());
		d->connection.requestToRaise();
		quit();
	}
}

QUrl Application::getUrlFromCommandLine() {
	const QStringList args = arguments();
	if (args.size() > 1) {
		QUrl url(args.last());
		if (url.scheme().isEmpty())
			url = QUrl::fromLocalFile(QFileInfo(args.last()).absoluteFilePath());
		return url;
	} else
		return QUrl();
}

void Application::open(const QString &url) {
	if (!url.isEmpty() && d->main)
		d->main->open(QUrl(url));
}

void Application::raise() {
	if (d->main && !d->main->hasFocus() && !d->main->isOnTop()) {
		d->main->hide();
		d->main->show();
	}
}
