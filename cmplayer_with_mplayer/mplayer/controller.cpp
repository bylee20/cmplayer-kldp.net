#include "controller.h"

namespace MPlayer {

Controller::Controller(QObject *parent)
: QObject(parent) {
}

Controller::~Controller() {
}

void link(Controller *con1, Controller *con2) {
	con1->link(con2);
	con2->link(con1);
}

}
