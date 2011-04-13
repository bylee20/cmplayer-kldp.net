#ifndef PREF_WIDGET_HPP
#define PREF_WIDGET_HPP

#include <QtGui/QWidget>
#include "pref.hpp"

class QTreeWidgetItem;		class QComboBox;


class Pref::Widget : public QWidget {
	Q_OBJECT
public:
	Widget(QWidget *parent = 0);
	~Widget();
	int pageCount() const;
	int currentPage() const;
	QString pageName(int idx) const;
	QIcon pageIcon(int idx) const;
	QSize sizeHint() const;
signals:

public slots:
	void apply();
	void setCurrentPage(int idx) const;
private slots:
	void getShortcut(int id);
	void slotCurrentItemChanged(QTreeWidgetItem *current);
	void slotMediaItemClicked(QTreeWidgetItem *item, int column);
	void checkSubtitleSelect(int index);
	void setSubFont();
	void setSubColor();
private:
	void setColorLabel(QLabel *label, const QColor &color);
	static QString toString(const QLocale &locale);
	static QVariant currentComboData(QComboBox *combo);
	static void setComboIndex(QComboBox *combo, const QVariant &value);
	struct Data;
	class MenuTreeItem;
	class MediaTreeItem;
	class MouseGroup;
	struct Data;
	Data *d;
};

#endif // PREF_WIDGET_HPP
