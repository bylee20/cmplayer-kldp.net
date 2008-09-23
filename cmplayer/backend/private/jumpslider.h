#ifndef BACKEND_JUMPSLIDER_H
#define BACKEND_JUMPSLIDER_H

#include <QtGui/QSlider>

namespace Backend {

class JumpSlider : public QSlider {
	Q_OBJECT
public:
	JumpSlider(QWidget *parent = 0);
protected:
	virtual void mousePressEvent(QMouseEvent *event);
};

}

#endif
