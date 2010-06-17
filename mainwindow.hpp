#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QtGui/QMainWindow>

class Mrl;

class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	MainWindow();
	~MainWindow();
private slots:
	void openFile();
	void togglePlayPause();
	void showContextMenu(const QPoint &pos);
	void updateMrl(const Mrl &mrl);
	void seek(int diff);
	void setVolume(int volume);
	void setMuted(bool muted);
private:
	void showMessage(const QString &message, int last = 5000);
	void showMessage(const QString &cmd, int value
		, const QString &unit, bool sign = false, int last = 5000);
	void showMessage(const QString &cmd, const QString &description, int last = 5000);
	void setupUi();
	struct Data;
	Data *d;
};

#endif // MAINWINDOW_HPP
