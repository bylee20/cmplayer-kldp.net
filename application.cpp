#include "application.hpp"
#include "mainwindow.hpp"
//#include "pref.h"
#include "translator.hpp"
#include "mrl.hpp"
#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtCore/QFileInfo>
#include <QtCore/QDebug>
#include <QtGui/QStyleFactory>
#include <QtGui/QStyle>

struct Application::Data {
	MainWindow *main;
	QString defStyle;
};

Application::Application(int &argc, char **argv)
: QtSingleApplication("net.xylosper.CMPlayer", argc, argv), d(new Data) {
	d->defStyle = style()->objectName();
//	Translator::load(Pref::get().locale);
//	setStyle(Pref::get().windowStyle);
	d->main = 0;
	setQuitOnLastWindowClosed(true);
	QTimer::singleShot(1, this, SLOT(initialize()));
}

Application::~Application() {
	delete d->main;
	delete d;
}

void Application::initialize() {
//	const Mrl mrl = MainWindow::getMrlFromCommandLine();
//	if (Pref::get().singleApplication && sendMessage("wakeUp")) {
//		if (!mrl.isEmpty())
//			sendMessage("mrl " + mrl.toString());
//		quit();
//	} else {
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
//		if (mrl.isEmpty())
			d->main = new MainWindow;
//		else
//			d->main = new MainWindow(mrl);
		d->main->show();
		setActivationWindow(d->main, false);
		connect(this, SIGNAL(messageReceived(const QString&))
				, this, SLOT(parseMessage(const QString&)));
//	}
}

QUrl Application::getUrlFromCommandLine() {
	const QStringList args = arguments();
	if (args.size() > 1) {
		QUrl mrl(args.last());
		if (mrl.scheme().isEmpty())
			mrl = QUrl::fromLocalFile(QFileInfo(args.last()).absoluteFilePath());
		return mrl;
	} else
		return QUrl();
}

void Application::open(const QString &mrl) {
//	if (!mrl.isEmpty() && d->main)
//		d->main->openMrl(mrl);
}

void Application::raise() {
//	if (d->main && !d->main->hasFocus() && !d->main->isOnTop()) {
//		d->main->hide();
//		d->main->show();
//	}
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
