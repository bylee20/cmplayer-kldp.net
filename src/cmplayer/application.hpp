#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "qtsingleapplication/qtsingleapplication.h"

class QUrl;		class Mrl;
class MainWindow;	class QMenuBar;

class Application : public QtSolution::QtSingleApplication {
	Q_OBJECT
public:
	Application(int &argc, char **argv);
	~Application();
	QString defaultStyleName();
	void setStyle(const QString &name);
	static Mrl getMrlFromCommandLine();
	MainWindow *mainWindow() const;
	void setAlwaysOnTop(QWidget *widget, bool onTop);
#ifdef Q_WS_MAC
	QMenuBar *globalMenuBar() const;
#endif
private slots:
	void initialize();
	void open(const QString &url);
	void parseMessage(const QString &message);
private:
	void customEvent(QEvent *event);
	struct Data;
	Data *d;
};

static inline Application *app() {return static_cast<Application*>(qApp);}

#endif // APPLICATION_HPP
