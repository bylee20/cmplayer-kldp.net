#ifndef BACKEND_SELECTWIDGET_H
#define BACKEND_SELECTWIDGET_H

#include <QtGui/QWidget>
#include <backend/manager.h>

class QTreeWidgetItem;

namespace Backend {

class SelectWidget : public QWidget {
public:
	Q_OBJECT
public:
	SelectWidget(QWidget *parent = 0);
	~SelectWidget();
	const Backend::FactoryMap &factories() const;
	QString selectedFileName() const;
	Backend::FactoryIface *selectedObject() const;
signals:
	void selected(const QString &fileName);
	void selected(Backend::FactoryIface *factory);
	void activated(const QString &fileName);
	void activated(Backend::FactoryIface *factory);
private slots:
	void slotSelected();
	void slotActivated(QTreeWidgetItem *item);
private:
	struct Data;
	Data *d;
};

}

#endif
