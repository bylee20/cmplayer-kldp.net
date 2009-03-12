#ifndef PREFDIALOG_H
#define PREFDIALOG_H

#include <QtGui/QDialog>

class QComboBox;			class QTreeWidgetItem;

class PrefDialog : public QDialog {
	Q_OBJECT
public:
	PrefDialog(QWidget *parent = 0);
	~PrefDialog();
private slots:
	void apply();
	void getShortcut(int id);
	void slotCurrentItemChanged(QTreeWidgetItem *current);
	void slotMediaItemClicked(QTreeWidgetItem *item, int column);
	void getBackendPath();
private:
	static int currentComboData(QComboBox *combo);
	static void setComboIndex(QComboBox *combo, int value);
	virtual void accept();
	struct Data;
	class MenuTreeItem;
	class MediaTreeItem;
	class MouseGroup;
	Data *d;
};

#endif
