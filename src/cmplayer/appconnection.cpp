#include "appconnection.h"
#include "mainwindow.h"
#include "application.h"
#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include <QtCore/QUrl>

struct AppConnection::Data {
	Adaptor *adaptor;
	Iface *iface;
	bool connected;
	bool unique;
};

AppConnection::AppConnection()
: d(new Data) {
	d->unique = true;
	d->connected = false;
	d->adaptor = new Adaptor(this);
	QDBusConnection::sessionBus().registerObject("/", this);
	d->iface = new Iface(this);
	
	QObject::connect(d->iface, SIGNAL(alreadyExists()), this, SLOT(slotExists()));
	QObject::connect(d->iface, SIGNAL(open(QString)), this, SLOT(slotOpen(QString)));
	QObject::connect(d->iface, SIGNAL(raise()), this, SLOT(slotRaise()));
	
	emit connect();
	
	QObject::connect(d->iface, SIGNAL(connect()), this, SLOT(slotConnect()));
}

AppConnection::~AppConnection() {
	delete d;
}

void AppConnection::slotConnect() {
	d->connected = true;
	emit alreadyExists();
}

void AppConnection::slotExists() {
	if (!d->connected)
		d->unique = false;
}

void AppConnection::requestToRaise() {
	emit raise();
}

void AppConnection::requestToOpen(const QString &url) {
	emit open(url);
}

bool AppConnection::isUnique() const {
	return d->unique;
}

void AppConnection::slotOpen(const QString &url) {
	if (isUnique())
		emit openRequested(url);
}

void AppConnection::slotRaise() {
	if (isUnique())
		emit raiseRequested();
}

AppConnection::Adaptor::Adaptor(AppConnection *parent)
: QDBusAbstractAdaptor(parent) {
	setAutoRelaySignals(true);
}

AppConnection::Iface::Iface(AppConnection *parent)
: QDBusAbstractInterface(QString(), QString(), name()
		, QDBusConnection::sessionBus(), parent) {
}
