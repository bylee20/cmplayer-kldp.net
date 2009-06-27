#ifndef PLAYENGINECONFIGWIDGET_H
#define PLAYENGINECONFIGWIDGET_H

#include <QtGui/QWidget>

class PlayEngineConfigWidget : public QWidget {
	Q_OBJECT
public:
	PlayEngineConfigWidget(QWidget *parent = 0);
	~PlayEngineConfigWidget();
	void apply();
private:
	class Tab;
	struct Data;
	Data *d;
};

#endif
