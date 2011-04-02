#ifndef ABOUTDIALOG_HPP
#define ABOUTDIALOG_HPP

#include <QtGui/QDialog>

class AboutDialog : public QDialog {
	Q_OBJECT
public:
	AboutDialog(QWidget *parent = 0);
	~AboutDialog();
private slots:
	void showFullLicense();
private:
	struct Data;
	Data *d;
};

class OpenDVDDialog : public QDialog {
	Q_OBJECT
public:
	OpenDVDDialog(QWidget *parent = 0);
	~OpenDVDDialog();
	void setDevices(const QStringList &devices);
	QString device() const;
public slots:
	void checkDevice(const QString &device);
private:
	struct Data;
	Data *d;
};

#endif // ABOUTDIALOG_HPP
