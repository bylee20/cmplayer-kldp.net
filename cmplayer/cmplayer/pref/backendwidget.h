#ifndef PREFBACKENDWIDGET_H
#define PREFBACKENDWIDGET_H

#include <QtGui/QWidget>

class QSettings;

namespace Backend {class BackendIface;}

namespace Pref {

class BackendPref;

class BackendWidget : public QWidget {
	Q_OBJECT
public:
	BackendWidget(const BackendPref &backend, QWidget *parent = 0);
	~BackendWidget();
	const BackendPref &backend() const;
	void save();
private:
	struct Data;
	Data *d;
};

}

#endif
