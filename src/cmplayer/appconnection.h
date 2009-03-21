#ifndef APPCONNECTION_H
#define APPCONNECTION_H

#include <QtCore/QObject>
#include <QtDBus/QDBusAbstractAdaptor>
#include <QtDBus/QDBusAbstractInterface>

class AppConnection : public QObject {
	Q_OBJECT
public:
	AppConnection();
	~AppConnection();
	void createConnection();
	bool isUnique() const;
signals:
	void connected();
	void alreadyCreated();
	void gotNewSource(const QString &url);
	void rasingRequested();
private slots:
	void slotConnected();
	void slotCreated();
	void createMainWindow();
	void slotGotNewSource(const QString &url);
	void slotRasingRequestetd();
private:
	class Iface;
	class Adaptor;
	struct Data;
	Data *d;
};

class AppConnection::Adaptor : public QDBusAbstractAdaptor {
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "net.xylosper.CMPlayer.AppConnection")
	Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"net.xylosper.CMPlayer.AppConnection\" >\n"
"    <signal name=\"connected\" />\n"
"    <signal name=\"alreadyCreated\" />\n"
"    <signal name=\"gotNewSource\" >\n"
"      <arg direction=\"out\" type=\"s\" name=\"url\" />\n"
"    </signal>\n"
"    <signal name=\"rasingRequested\" />\n"
"  </interface>\n"
        "")
public:
	Adaptor(AppConnection *parent);
public slots:
signals:
	void connected();
	void alreadyCreated();
	void gotNewSource(const QString &url);
	void rasingRequested();
private:
	
};

class AppConnection::Iface : public QDBusAbstractInterface {
	Q_OBJECT
public:
	static const char *name() {return "net.xylosper.CMPlayer.AppConnection";}
	Iface(AppConnection *parent);
public slots:
signals:
	void connected();
	void alreadyCreated();
	void gotNewSource(const QString &url);
	void rasingRequested();
private:
	
};

#endif
