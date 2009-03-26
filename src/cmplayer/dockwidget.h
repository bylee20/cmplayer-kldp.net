#ifndef LISTDOCK_H
#define LISTDOCK_H

#include <QtGui/QDockWidget>
#include <QtGui/QDialog>

class QModelIndex;				class PlaylistModel;
class QComboBox;

class ShutdownDialog : public QDialog {
	Q_OBJECT
public:
	ShutdownDialog(QWidget *parent);
	void accept();
	QStringList command() const;
private:
	QComboBox *combo;
};


class DockWidget : public QDockWidget {
	Q_OBJECT
public:
	DockWidget(PlaylistModel *model, QWidget *parent = 0);
	~DockWidget();
	void setWidth(int width);
public slots:
	void checkRoot(bool checked);
	void setAutoShutdown(bool shut);
signals:
	void hidingRequested();
private slots:
	void recoverWidthRange();
	void add();
	void erase();
	void up();
	void down();
	void checkShutdown();
	void open();
	void save();
	void slotDblClicked(const QModelIndex &index);
	void adjustCellSize();
private:
	void showEvent(QShowEvent *event);
	void hideEvent(QHideEvent *event);
	void closeEvent(QCloseEvent *event);
	struct Data;
	friend struct Data;
	Data *d;
};

#endif
