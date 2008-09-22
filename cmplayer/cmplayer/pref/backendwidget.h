#ifndef PREFBACKENDWIDGET_H
#define PREFBACKENDWIDGET_H

#include <QWidget>

class QSettings;

namespace Backend {class FactoryIface;}

namespace Pref {

	class BackendPref;

class BackendWidget : public QWidget {
	Q_OBJECT
public:
	BackendWidget(QWidget *parent = 0);
	~BackendWidget();
	const BackendPref &backend() const;
	void save();
private:
	void addBackend(const QString &fileName);
	struct Data;
	Data *d;
};

}

#endif
