#include "designedwidgetiface.h"
#include "squeezedlabel.h"
#include "button.h"
#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPainter>
#include <QtCore/QDebug>

struct DesignedWidgetIface::TitleBar::Data {
	Button *max, *close;
	SqueezedLabel *title;
	QLabel *icon;
	QWidget *widget;
	QSpacerItem *spacer;
	QHBoxLayout *hbox;
	int button;
};

DesignedWidgetIface::TitleBar::TitleBar(QWidget *parent)
: QWidget(parent), d(new Data) {
	setStyleSheet("QLabel {color:white; font-size:12px;}");
	d->widget = 0;
	d->icon = new QLabel(this);
	d->icon->hide();
	d->title = new SqueezedLabel(this);
	d->title->setAlignment(Qt::AlignCenter);
	d->max = new Button(QIcon(":/img/window-duplicate-gray.png"), this);
	d->max->setIconSize(10);
	d->close = new Button(QIcon(":/img/edit-delete-gray.png"), this);
	d->close->setIconSize(10);
	d->spacer = new QSpacerItem(0, 0);
	setSpacerSize(0);
	
	d->hbox = new QHBoxLayout(this);
	d->hbox->setSpacing(0);
	d->hbox->addWidget(d->icon);
	d->hbox->addItem(d->spacer);
	d->hbox->addWidget(d->title);
	d->hbox->addWidget(d->max);
	d->hbox->addWidget(d->close);
	d->hbox->setContentsMargins(0, 0, 0, 2);
	setFixedHeight(sizeHint().height());
	setSpacerSize(height()*2);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	d->button = 2;
}

DesignedWidgetIface::TitleBar::~TitleBar() {
	delete d;
}

void DesignedWidgetIface::TitleBar::setSpacerSize(int size) {
	d->spacer->changeSize(size, 0, QSizePolicy::Minimum, QSizePolicy::Fixed);
}

void DesignedWidgetIface::TitleBar::toggleMaximized() {
	if (d->widget) {
		if (d->widget->isMaximized())
			d->widget->showNormal();
		else
			d->widget->showMaximized();
	}
}

Button *DesignedWidgetIface::TitleBar::addButton(const QIcon &icon
		, QObject *object, const char *slot) {
	Button *button = new Button(icon, this);
	button->setIconSize(10);
	d->hbox->insertWidget(3, button);
	QObject::connect(button, SIGNAL(clicked()), object, slot);
	return button;
}

void DesignedWidgetIface::TitleBar::connect(QWidget *widget) {
	if ((d->widget = widget)) {
		QObject::connect(d->max, SIGNAL(clicked()), this, SLOT(toggleMaximized()));
		QObject::connect(d->close, SIGNAL(clicked()), d->widget, SLOT(close()));
	}
}
void DesignedWidgetIface::TitleBar::setTitle(const QString &title) {
	d->title->setText(title);
}
QString DesignedWidgetIface::TitleBar::title() const {
	return d->title->text();
}
void DesignedWidgetIface::TitleBar::paintEvent(QPaintEvent */*event*/) {
	QPainter painter(this);
	painter.fillRect(rect(), Qt::black);
	painter.scale(width(), height());
	QLinearGradient grad(0, -1, 0, 1);
	grad.setColorAt(0, Qt::transparent);
	grad.setColorAt(1, Qt::white);
	QBrush light = QBrush(grad);
	QPainterPath path;
	path.moveTo(0, 0);
	path.lineTo(0, 0.8);
	path.cubicTo(0.25, 0.9, 0.25, 0.9, 0.5, 0.9);
	path.cubicTo(0.75, 0.9, 0.75, 0.9, 1, 0.8);
	path.lineTo(1, 0);
	path.closeSubpath();
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setOpacity(0.5);
	painter.setBrush(light);
	painter.drawPath(path);
}

DesignedWidgetIface::DesignedWidgetIface() {
	m_titleBar = new TitleBar(0);
}

DesignedWidgetIface::~DesignedWidgetIface() {
}

DesignedWidgetIface::TitleBar *DesignedWidgetIface::titleBar() {
	return m_titleBar;
}

void DesignedWidgetIface::TitleBar::setIcon(const QIcon &icon) {
	d->icon->setFixedSize(height(), height());
	d->icon->setPixmap(icon.pixmap(d->icon->size()));
	setSpacerSize(height());
	d->icon->show();
}

void DesignedWidgetIface::drawBackground(QPainter *painter, QWidget *widget) {
	painter->fillRect(widget->rect(), Qt::black);
	painter->setPen(qRgb(0x88, 0x88, 0x88));
	painter->drawRect(1, 1, widget->width() - 3, widget->height() - 3);
}


