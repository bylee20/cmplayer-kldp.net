#include "application.hpp"
#include "translator.hpp"
#include "mainwindow.hpp"
#include "libvlc.hpp"
#include "pref.hpp"
#include "mrl.hpp"
#include <QtGui/QStyleFactory>
#include <QtGui/QStyle>
#include <QtCore/QFileInfo>
#include <QtCore/QTimer>
#include <QtCore/QDebug>
#include <QtCore/QUrl>

struct Application::Data {
	MainWindow *main;
	QString defStyle;
};

Application::Application(int &argc, char **argv)
: QtSingleApplication("net.xylosper.CMPlayer", argc, argv), d(new Data) {
	setOrganizationName("xylosper");
	setOrganizationDomain("xylosper.net");
	setApplicationName("CMPlayer");
//	QSettings::setDefaultFormat(QSettings::IniFormat);

	LibVLC::init();
	d->defStyle = style()->objectName();
//	Translator::load(Pref::get().locale);
	setStyle(Pref::get().windowStyle);
	d->main = 0;
	setQuitOnLastWindowClosed(false);
	QTimer::singleShot(1, this, SLOT(initialize()));
}

Application::~Application() {
	delete d->main;
	delete d;
	LibVLC::release();
}

void Application::initialize() {
	const Mrl mrl = getMrlFromCommandLine();
	if (Pref::get().singleApplication && sendMessage("wakeUp")) {
		if (!mrl.isEmpty())
			sendMessage("mrl " + mrl.toString());
		quit();
	} else {
		setStyleSheet("\
			Button {\
				margin:0px; padding: 2px;\
			}\
			Button#flat {\
				border: none; border-radius: 3px;\
			}\
			Button#block {\
				border: 1px solid #999; border-radius: 0px; padding: 1px;\
				background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #fff, stop:1 #ccc);\
			}\
				Button#flat:hover, Button#flat:checked, Button#block:hover {\
				border: 1px solid #6ad; padding: 1px;\
			}\
			Button#flat:pressed, Button#block:pressed {\
				border: 2px solid #6ad; padding: 0px;\
			}\
			Button#block:checked, Button#block:pressed {\
				background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #777, stop:1 #bbb);\
			}\
			JumpSlider::groove:horizontal {\
				border: 1px solid #6ad; height: 3px; margin: 0px 0px; padding: 0px;\
				background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #fff, stop:1 #ccc);\
			}\
			JumpSlider::handle:horizontal {\
				background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #aaa, stop:1 #999);\
				border: 1px solid #5c5c5c; border-radius: 2px;\
				width: 5px; margin: -2px 0px; padding: 1px;\
			}\
			JumpSlider::handle:horizontal:hover {\
				background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #fff, stop:1 #ccc);\
				border: 1px solid #6ad; padding: 1px;\
			}\
			JumpSlider::handle:horizontal:pressed {\
				background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #fff, stop:1 #ccc);\
				border: 2px solid #6ad; padding: 0px;\
			}\
			JumpSlider::add-page:horizontal {\
				border: 1px solid #999; height: 3px; margin: 0px 0px; padding: 0px;\
				background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #333, stop:1 #bbb);\
			}"
		);
		d->main = new MainWindow;
		d->main->show();
		if (!mrl.isEmpty())
			d->main->openMrl(mrl);
		setActivationWindow(d->main, false);
		connect(this, SIGNAL(messageReceived(const QString&))
				, this, SLOT(parseMessage(const QString&)));
	}
}

Mrl Application::getMrlFromCommandLine() {
	const QStringList args = arguments();
	return args.size() > 1 ? Mrl(args.last()) : Mrl();
}

void Application::open(const QString &mrl) {
	if (!mrl.isEmpty() && d->main)
		d->main->openMrl(mrl);
}

QString Application::defaultStyleName() {
	return d->defStyle;
}

void Application::setStyle(const QString &name) {
	const QString key = name.isEmpty() ? d->defStyle : name;
	if (style()->objectName() != key)
		QApplication::setStyle(QStyleFactory::create(key));
}

void Application::parseMessage(const QString &message) {
	if (message == "wakeUp") {
		activateWindow();
	} else if (message.left(3) == "mrl") {
		open(message.right(message.size()-4));
	}
}
