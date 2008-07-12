#ifndef MPLAYERCONTROLLER_H
#define MPLAYERCONTROLLER_H

#include <QObject>

namespace MPlayer {

class Controller : public QObject {
public:
	Controller(QObject *parent = 0);
	virtual ~Controller();
protected:
	virtual void link(Controller *controller) = 0;
	virtual void unlink(Controller *controller) = 0;
private:
	friend void link(Controller *controller1, Controller *controller2);
};

void link(Controller *con1, Controller *con2);

}

#endif
