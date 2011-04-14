#include "application.hpp"
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

#if defined(Q_WS_MAC)
#include "application_mac.hpp"
#elif defined(Q_WS_X11)
#include "application_x11.hpp"
#endif

struct Application::Data {
	MainWindow *main;
	QString defStyle;
	QMenuBar *mb;
	QUrl url;
	QProcess *cpu;
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
	setWindowIcon(defaultIcon());

	d->defStyle = style()->objectName();
	setStyle(Pref::get().window_style);
	d->main = 0;
	d->mb = 0;
#ifdef Q_WS_MAC
	d->mb = new QMenuBar;
#endif
	setQuitOnLastWindowClosed(false);
	qInstallMsgHandler(messageHandler);
	QTimer::singleShot(1, this, SLOT(initialize()));
	d->cpu = new QProcess(this);
	connect(d->cpu, SIGNAL(readyReadStandardOutput()), this, SLOT(readProcInfo()));
}

Application::~Application() {
	if (d->cpu->state() != QProcess::NotRunning)
	    d->cpu->kill();
	delete d->mb;
	delete d->main;
	qInstallMsgHandler(0);
	delete d;
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
	d->cpu->start("ps", QStringList() << "-v" << QString::number(getpid()), QProcess::ReadOnly);
}

static QByteArray parseProcInfo(const QByteArray &output, const int br, const char *column) {
	const int pc = output.indexOf(column);
	int idx = br + pc + qstrlen(column);
	if (idx < 0 || idx >= output.size())
		return QByteArray();
	QByteArray temp;
	temp.reserve(6);
	while (idx >= 0) {
		const char c = output[idx];
		if (isspace(c))
			break;
		temp += c;
		--idx;
	}
	QByteArray value;
	value.reserve(temp.size()+1);
	for (int i = temp.size()-1; i >= 0; --i)
		value += temp[i];
	return value;
}


void Application::readProcInfo() {
	const QByteArray output = d->cpu->readAllStandardOutput();
	const int br = qMax(output.indexOf('\n'), output.indexOf('\r'));
	const QByteArray cpu = parseProcInfo(output, br, "%CPU");
	const QByteArray rss = parseProcInfo(output, br, "RSS");
	const QByteArray mem = parseProcInfo(output, br, "%MEM");
	emit gotProcInfo(cpu.toDouble(), rss.toInt(), mem.toDouble());
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

MainWindow *Application::mainWindow() const {
	return d->main;
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
		d->main = new MainWindow;
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
