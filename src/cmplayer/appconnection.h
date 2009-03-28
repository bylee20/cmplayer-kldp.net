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
	bool isUnique() const;
	void requestToRaise();
	void requestToOpen(const QString &url);
signals:
	void raiseRequested();
	void openRequested(const QString &url);
signals: // private:
	void connect();
	void alreadyExists();
	void open(const QString &url);
	void raise();
private slots:
	void slotConnect();
	void slotExists();
	void slotOpen(const QString &url);
	void slotRaise();
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
"    <signal name=\"connect\" />\n"
"    <signal name=\"alreadyExists\" />\n"
"    <signal name=\"open\" >\n"
"      <arg direction=\"out\" type=\"s\" name=\"url\" />\n"
"    </signal>\n"
"    <signal name=\"raise\" />\n"
"  </interface>\n"
        "")
public:
	Adaptor(AppConnection *parent);
public slots:
signals:
	void connect();
	void alreadyExists();
	void open(const QString &url);
	void raise();
private:
	
};

class AppConnection::Iface : public QDBusAbstractInterface {
	Q_OBJECT
public:
	static const char *name() {return "net.xylosper.CMPlayer.AppConnection";}
	static QString uniquePath() {return QString("/xylosper/CMPlayer");}
	Iface(AppConnection *parent);
public slots:
signals:
	void connect();
	void alreadyExists();
	void open(const QString &url);
	void raise();
private:
	
};

#endif
