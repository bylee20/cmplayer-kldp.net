#include "appconnection.h"
#include "mainwindow.h"
#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include <QtGui/QApplication>
#include <QtCore/QUrl>

struct AppConnection::Data {
	Adaptor *adaptor;
	Iface *iface;
	bool connected;
	bool unique;
	QTimer timer;
	MainWindow *main;
};

AppConnection::AppConnection()
: d(new Data) {
	d->main = 0;
	d->timer.setSingleShot(true);
	d->unique = true;
	d->connected = false;
	d->adaptor = new Adaptor(this);
	QDBusConnection::sessionBus().registerObject("/", this);
	d->iface = new Iface(this);
	
	connect(d->iface, SIGNAL(alreadyCreated()), this, SLOT(slotCreated()));
	connect(d->iface, SIGNAL(gotNewSource(QString))
			, this, SLOT(slotGotNewSource(QString)));
	connect(d->iface, SIGNAL(rasingRequested()), this, SLOT(slotRasingRequestetd()));
	connect(&d->timer, SIGNAL(timeout()), this, SLOT(createMainWindow()));
	
	emit connected();
	d->timer.start(100);
}

AppConnection::~AppConnection() {
	delete d->main;
	delete d;
}

void AppConnection::createMainWindow() {
	if (isUnique()) {
		d->main = new MainWindow;
		d->main->show();
	}
}

void AppConnection::slotConnected() {
	d->connected = true;
	emit alreadyCreated();
}

void AppConnection::slotCreated() {
	if (!d->connected) {
		d->unique = false;
		d->timer.stop();
		const QUrl url = MainWindow::getUrlFromCommandLine();
		if (!url.isEmpty())
			emit gotNewSource(url.toString());
		emit rasingRequested();
		qApp->quit();
	}
}

bool AppConnection::isUnique() const {
	return d->unique;
}

void AppConnection::slotGotNewSource(const QString &url) {
	if (isUnique())
		d->main->open(QUrl(url));
}

void AppConnection::slotRasingRequestetd() {
	if (isUnique() && !d->main->hasFocus() && !d->main->isOnTop()) {
		d->main->hide();
		d->main->show();
	}
}

void AppConnection::createConnection() {
	connect(d->iface, SIGNAL(connected()), this, SLOT(slotConnected()));
}

AppConnection::Adaptor::Adaptor(AppConnection *parent)
: QDBusAbstractAdaptor(parent) {
	setAutoRelaySignals(true);
}

AppConnection::Iface::Iface(AppConnection *parent)
: QDBusAbstractInterface(QString(), QString(), name()
		, QDBusConnection::sessionBus(), parent) {
}
