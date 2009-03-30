#ifndef PREFDIALOG_H
#define PREFDIALOG_H

#include <QtGui/QDialog>

class QComboBox;			class QTreeWidgetItem;
class QLocale;

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
private:
	static QString toString(const QLocale &locale);
	static QVariant currentComboData(QComboBox *combo);
	static void setComboIndex(QComboBox *combo, const QVariant &value);
	virtual void accept();
	struct Data;
	class MenuTreeItem;
	class MediaTreeItem;
	class MouseGroup;
	Data *d;
};

#endif
