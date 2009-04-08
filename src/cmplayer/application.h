#ifndef APPLICATION_H
#define APPLICATION_H

#include <QtGui/QApplication>

class QUrl;

class Application : public QApplication {
	Q_OBJECT
public:
	Application(int &argc, char **argv);
	QString defaultStyleName();
	void setStyle(const QString &name);
	~Application();
	static QUrl getUrlFromCommandLine();
private slots:
	void initialize();
	void open(const QString &url);
	void raise();
private:
	struct Data;
	Data *d;
};

Application *app();

#endif
