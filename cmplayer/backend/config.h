#ifndef BACKEND_CONFIG_H
#define BACKEND_CONFIG_H

class QWidget;

namespace Backend {

class Config {
public:
	virtual ~Config() {}
	virtual QWidget *widget(QWidget *parent) = 0;
	virtual void update(QWidget *widget) = 0;
	virtual void save() = 0;
	virtual void load() = 0;
protected:
	Config() {}
};

}

#endif
