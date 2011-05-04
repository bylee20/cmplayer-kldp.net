#include "application.hpp"
#include "recentinfo.hpp"
#include "videorenderer.hpp"
#include "audiocontroller.hpp"
#include "playengine.hpp"
#include "menu.hpp"
#include <QtGui/QMessageBox>
#include <unistd.h>
#include <QtGui/QFileOpenEvent>
#include "events.hpp"
#include "translator.hpp"
#include "mainwindow.hpp"
#include "pref.hpp"
#include "mrl.hpp"
#include <QtGui/QStyleFactory>
#include <QtGui/QStyle>
#include <QtCore/QFileInfo>
#include <QtCore/QTimer>
#include <QtCore/QDebug>
#include <QtCore/QProcess>
#include <QtCore/QUrl>
#include <QtGui/QMenuBar>
#include <QtOpenGL/QGLFormat>
#include "appstate.hpp"
#include "libvlc.hpp"

#if defined(Q_WS_MAC)
#include "application_mac.hpp"
#elif defined(Q_WS_X11)
#include "application_x11.hpp"
#endif

struct Application::Data {
	QString defStyle;
	QMenuBar *mb;
	QUrl url;
	QProcess *cpu;
	MainWindow *main;
#if defined(Q_WS_MAC)
	ApplicationMac helper;
#elif defined(Q_WS_X11)
	ApplicationX11 helper;
#endif
};

void Application::messageHandler(QtMsgType type, const char *msg) {
	FILE *output = stdout;
	switch (type) {
	case QtDebugMsg:
		fprintf(output, "%s\n", msg);
		break;
	case QtWarningMsg:
		fprintf(output, "Warning: %s\n", msg);
		break;
	case QtCriticalMsg:
		fprintf(output, "Critical: %s\n", msg);
		break;
	case QtFatalMsg:
		fprintf(output, "Fatal: %s\n", msg);
		abort();
	default:
		return;
	}
	fflush(output);
}


Application::Application(int &argc, char **argv)
: QtSingleApplication("net.xylosper.CMPlayer", argc, argv), d(new Data) {
	setOrganizationName("xylosper");
	setOrganizationDomain("xylosper.net");
	setApplicationName("CMPlayer");

	Pref::init();
	setWindowIcon(defaultIcon());

	d->defStyle = style()->objectName();
	setStyle(Pref::get().window_style);
	d->main = 0;
	d->mb = 0;
#ifdef Q_WS_MAC
	d->mb = new QMenuBar;
#endif
	setQuitOnLastWindowClosed(false);
//	qInstallMsgHandler(messageHandler);
	QTimer::singleShot(1, this, SLOT(initialize()));
	d->cpu = new QProcess(this);
	connect(d->cpu, SIGNAL(readyReadStandardOutput()), this, SLOT(readProcInfo()));
}

void Application::initStaticObjects() {
	RecentInfo::init();
	PlayEngine::init();
	VideoRenderer::init();
	AudioController::init();
	LibVLC::init();
	AppState::init();
	Menu::init();
	MainWindow::init();
	d->main = &MainWindow::get();
}

Application::~Application() {
	MainWindow::fin();
	if (d->cpu->state() != QProcess::NotRunning)
	    d->cpu->kill();
	delete d->mb;
	d->main = 0;
	delete d;

	Menu::fin();
	AppState::fin();
	PlayEngine::fin();
	VideoRenderer::fin();
	AudioController::fin();
	LibVLC::fin();
	RecentInfo::fin();

	Pref::fin();
}

QIcon Application::defaultIcon() {
	static QIcon icon;
	static bool init = false;
	if (!init) {
		icon.addFile(":/img/cmplayer16.png", QSize(16, 16));
		icon.addFile(":/img/cmplayer22.png", QSize(22, 22));
		icon.addFile(":/img/cmplayer24.png", QSize(24, 24));
		icon.addFile(":/img/cmplayer32.png", QSize(32, 32));
		icon.addFile(":/img/cmplayer48.png", QSize(48, 48));
		icon.addFile(":/img/cmplayer64.png", QSize(64, 64));
		icon.addFile(":/img/cmplayer128.png", QSize(128, 128));
		icon.addFile(":/img/cmplayer256.png", QSize(256, 256));
		icon.addFile(":/img/cmplayer512.png", QSize(512, 512));
		init = true;
	}
	return icon;
}

void Application::setAlwaysOnTop(QWidget *widget, bool onTop) {
	d->helper.setAlwaysOnTop(widget->effectiveWinId(), onTop);
}

QString Application::test() {
#ifdef Q_OS_MAC
	return d->helper.test();
#else
	return QString();
#endif
}

void Application::getProcInfo() {
	d->cpu->start("ps", QStringList() << "-p" << QString::number(getpid()) << "-o" << "pcpu,rss,pmem", QProcess::ReadOnly);
}

void Application::readProcInfo() {
	const QByteArray output = d->cpu->readAllStandardOutput();
	const int br = qMax(output.indexOf('\n'), output.indexOf('\r'));
	QList<QByteArray> cols;
	cols.append(QByteArray());
	for (int i=br; i<output.size(); ++i) {
		if (isspace(output[i])) {
			if (!cols.last().isEmpty())
				cols.append(QByteArray());
		} else
			cols.last() += output[i];
	}
	if (cols.size() >= 3)
		emit gotProcInfo(cols[0].toDouble(), cols[1].toInt(), cols[2].toDouble());
}

void Application::setScreensaverDisabled(bool disabled) {
	d->helper.setScreensaverDisabled(disabled);
}

bool Application::event(QEvent *event) {
	switch ((int)event->type()) {
	case QEvent::FileOpen: {
		d->url = static_cast<QFileOpenEvent*>(event)->url().toString();
		if (d->main) {
			d->main->openMrl(d->url.toString());
			d->url.clear();
		}
		event->accept();
		return true;
	} case Event::Reopen:
		d->main->show();
		event->accept();
		return true;
	default:
		return QApplication::event(event);
	}
}

QStringList Application::devices() const {
	return d->helper.devices();
}

void Application::initialize() {
	if (!QGLFormat::hasOpenGL()) {
		QMessageBox::critical(0, "CMPlayer"
			, tr("CMPlayer needs OpenGL to render video. Your system has no OpenGL support. Exit CMPlayer."));
		quit();
		return;
	}
	Mrl mrl = getMrlFromCommandLine();
	if (mrl.isEmpty() && !d->url.isEmpty()) {
		mrl = d->url.toString();
		d->url.clear();
	}
	if (Pref::get().single_app && sendMessage("wakeUp")) {
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
		initStaticObjects();
		d->main->show();
		if (!mrl.isEmpty())
			d->main->openMrl(mrl);
		setActivationWindow(d->main, false);
		CONNECT(this, messageReceived(QString), this, parseMessage(QString));
	}
}

#ifdef Q_WS_MAC
QMenuBar *Application::globalMenuBar() const {
	return d->mb;
}
#endif

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
