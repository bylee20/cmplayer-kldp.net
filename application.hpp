#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "qtsingleapplication/qtsingleapplication.h"

class QUrl;		class Mrl;

class Application : public QtSingleApplication {
	Q_OBJECT
public:
	Application(int &argc, char **argv);
	QString defaultStyleName();
	void setStyle(const QString &name);
	~Application();
	static Mrl getMrlFromCommandLine();
private slots:
	void initialize();
	void open(const QString &url);
	void parseMessage(const QString &message);
private:
	struct Data;
	Data *d;
};

inline Application *app() {return static_cast<Application*>(qApp);}

#endif // APPLICATION_HPP
