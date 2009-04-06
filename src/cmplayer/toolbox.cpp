#include "toolbox.h"
#include "playlistwidget.h"
#include "dragcharm.h"
#include "helper.h"
#include <core/info.h>
#include <QtGui/QFrame>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPainter>
#include <QtCore/QDebug>
#include <QtGui/QPushButton>
#include <QtGui/QCloseEvent>


class ToolBox::Frame : public QFrame {
public:
	Frame(QWidget *parent): QFrame(parent) {
		w = new Widget(this);
		QVBoxLayout *vbox = new QVBoxLayout(this);
		vbox->setContentsMargins(0, 0, 0, 0);
		vbox->addWidget(w);
		
		vbox = new QVBoxLayout(w);
		vbox->setContentsMargins(5, 10, 5, 10);
		vbox->addWidget(tab = new QTabWidget(w));
		
		w->setObjectName("toolBoxBg");
		setObjectName("toolBoxFrame");
		setStyleSheet("\
			QFrame#toolBoxFrame {\
				margin: 0px; padding: 0px;\
				border: 0px solid #aaa; border-radius: 5px;\
			}\
		");
	}/*
	void setWidget(QWidget *widget) {
		
	}*/
	QTabWidget *tab;
private:
	class Widget : public QWidget {
	public:
		Widget(QWidget *parent): QWidget(parent) {
			bg = palette().color(QPalette::Background);
			setAutoFillBackground(true);
		}
	private:
		void paintEvent(QPaintEvent */*event*/) {
			QPainter painter(this);
			const double top = 1.0/height();
			const double bottom = 1.0 - 4.0/height(); 
			QLinearGradient grad(0, 0, 0, height());
			grad.setColorAt(0, qRgb(0x11, 0x11, 0x11));
			grad.setColorAt(top, bg);
			grad.setColorAt(bottom, bg);
			grad.setColorAt(1.0, qRgb(0x11, 0x11, 0x11));
			painter.fillRect(rect(), grad);
		}
		QColor bg;
	};
	Widget *w;
};

struct ToolBox::Data {
	PlaylistWidget *playlist;
	Frame *frame;
	DragCharm dragCharm;
};

ToolBox::ToolBox(PlaylistModel *model, QWidget *parent)
: QDialog(parent, Qt::FramelessWindowHint), d(new Data) {
	setFocusPolicy(Qt::NoFocus);
	d->frame = new Frame(this);
	d->playlist = new PlaylistWidget(model, d->frame);
// 	d->frame->setWidget(d->playlist);
	d->frame->tab->addTab(d->playlist, tr("Playlist"));
	setWindowTitle("TOOL BOX");
	titleBar()->setTitle("TOOL BOX");
	titleBar()->connect(this);
// 	titleBar()->addButton(QIcon(":/img/view-split-left-right.png"), this, SIGNAL(snapRequested()));
	
	QVBoxLayout *vbox = new QVBoxLayout(this);
	vbox->addWidget(titleBar());
	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addWidget(d->frame);
	vbox->addLayout(hbox);
	vbox->addWidget(d->playlist->buttons());
	vbox->setContentsMargins(3, 3, 3, 3);
	vbox->setSpacing(0);
	hbox->setContentsMargins(1, 0, 1, 1);

	d->dragCharm.activate(this);
	d->dragCharm.setBorder(7);
}


ToolBox::~ToolBox() {
	delete d;
}

void ToolBox::closeEvent(QCloseEvent *event) {
	event->ignore();
	emit hidingRequested();
}

void ToolBox::paintEvent(QPaintEvent */*event*/) {
	QPainter painter(this);
	drawBackground(&painter, this);
	const QRectF box = boxRect();
	const double frameBound = d->frame->geometry().bottom();
	QLinearGradient grad(box.topLeft(), box.bottomLeft());
	grad.setColorAt(0, Qt::black);
	grad.setColorAt(frameBound/box.height(), qRgb(100, 100, 100));
	grad.setColorAt(1, qRgb(200, 200, 200));
	painter.fillRect(box, grad);
}

void ToolBox::resizeEvent(QResizeEvent *event) {
	QDialog::resizeEvent(event);
	d->dragCharm.setRect(boxRect());
}