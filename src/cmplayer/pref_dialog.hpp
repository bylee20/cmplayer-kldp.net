#ifndef PREF_DIALOG_HPP
#define PREF_DIALOG_HPP

#include "pref.hpp"
#include <QtGui/QDialog>

class QComboBox;		class QTreeWidgetItem;
class QLocale;

class Pref::Dialog : public QDialog {
	Q_OBJECT
public:
	Dialog(QWidget *parent = 0);
	~Dialog();
private slots:
	void apply();
	void getShortcut(int id);
	void slotCurrentItemChanged(QTreeWidgetItem *current);
	void slotMediaItemClicked(QTreeWidgetItem *item, int column);
	void checkSubtitleSelect(int index);
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

#endif // PREF_DIALOG_HPP
