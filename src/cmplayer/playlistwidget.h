#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QDialog>

class QModelIndex;			class PlaylistModel;
class QComboBox;				class QMenu;

class PlaylistWidget : public QWidget {
	Q_OBJECT
public:
	PlaylistWidget(PlaylistModel *model, QWidget *parent = 0);
	~PlaylistWidget();
	void retranslateUi();
public slots:
	void checkRoot(bool checked);
	void setAutoShutdown(bool shut);
	void adjustCellSize();
private slots:
	void add();
	void erase();
	void up();
	void down();
	void checkShutdown();
	void open();
	void save();
	void slotDblClicked(const QModelIndex &index);
	void showContextMenu(const QPoint &pos);
private:
	void move(bool up);
	class ShutdownDialog;
	struct Data;
	Data *d;
};

class PlaylistWidget::ShutdownDialog : public QDialog {
	Q_OBJECT
public:
	ShutdownDialog(QWidget *parent);
	void accept();
	QStringList command() const;
private:
	QComboBox *combo;
};

#endif
