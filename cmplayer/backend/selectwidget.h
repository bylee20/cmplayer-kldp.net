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
	const BackendList &backends() const;
	BackendObject *backend() const;
	void setBackend(BackendObject *obj);
signals:
	void selected(Backend::BackendObject *obj);
	void activated(Backend::BackendObject *obj);
private slots:
	void slotSelected();
	void slotActivated(QTreeWidgetItem *item);
private:
	struct Data;
	Data *d;
};

}

#endif
