#include "logoview.hpp"
#include <QtGui/QPainter>
#include "textosdrenderer.hpp"
#include "timelineosdrenderer.hpp"
#include <QtCore/QTimer>

//struct LogoViewIface::Data {

//};

//LogoViewIface::LogoViewIface()
//: d(new Data) {

//}

//LogoViewIface::~LogoViewIface() {
//	delete d;
//}


struct LogoView::Data {
	QPixmap logo;
	QBrush bgBrush, lightBrush;
	QPainterPath lightPath;
	TextOsdRenderer *message;
	TimeLineOsdRenderer *timeLine;
//	QTimer updater;
};

LogoView::LogoView(QWidget *parent)
: QWidget(parent), d(new Data) {
	d->logo.load(":/img/cmplayer-logo-center.png");
	QLinearGradient grad(0.5, 1.0, 0.75, 0.13);
	grad.setColorAt(0.0, qRgb(51, 131, 230));
	grad.setColorAt(1.0, qRgb(110, 202, 247));
	d->bgBrush = QBrush(grad);

	grad = QLinearGradient(0.3, 0.1, 0.5, 0.9);
	grad.setColorAt(0.0, Qt::white);
	grad.setColorAt(1.0, Qt::transparent);
	d->lightBrush = QBrush(grad);

	const double oh = 0.6;
	d->lightPath.moveTo(0.0, 0.0);
	d->lightPath.lineTo(0.0, oh*0.8);
	d->lightPath.cubicTo(0.1, oh*0.9, 0.3, oh, 0.4, oh);
	d->lightPath.cubicTo(0.6, oh, 0.8, oh*0.9, 1.0, oh*0.6);
	d->lightPath.lineTo(1.0, 0.0);
	d->lightPath.closeSubpath();

	setMinimumSize(200, 150);

	d->message = new TextOsdRenderer(Qt::AlignTop | Qt::AlignLeft);
	d->timeLine = new TimeLineOsdRenderer();

//	connect(&d->updater, SIGNAL(timeout()), this, SLOT(update()));
	connect(d->message, SIGNAL(needToRerender()), this, SLOT(update()));
	connect(d->timeLine, SIGNAL(needToRerender()), this, SLOT(update()));
//	d->updater.setSingleShot(true);
}

LogoView::~LogoView() {
	delete d->message;
	delete d->timeLine;
	delete d;
}

void LogoView::paintEvent(QPaintEvent */*event*/) {
	QPainter painter(this);
	const double w = width();
	const double h = height();

	painter.save();
	painter.setRenderHint(QPainter::Antialiasing);
	painter.scale(w, h);
	painter.setPen(Qt::NoPen);
	painter.fillRect(rect(), d->bgBrush);
	painter.scale(1.0/w, 1.0/h);

	const int len = qMin(qRound(qMin(w, h)*0.5), d->logo.width());
	QRect rect;
	rect.setX((w-len)*0.5 + 0.5);
	rect.setY((h-len)*0.5 + 0.5);
	rect.setWidth(len + 0.5);
	rect.setHeight(len + 0.5);
	if (len != d->logo.width())
		painter.drawPixmap(rect, d->logo.scaled(len, len
				, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	else
		painter.drawPixmap(rect, d->logo);
	painter.scale(w, h);
	painter.setOpacity(0.2);
	painter.setBrush(d->lightBrush);
	painter.drawPath(d->lightPath);
	painter.restore();

	painter.save();
	painter.translate(d->message->pos());
	d->message->render(&painter);
	painter.restore();
	painter.save();
	painter.translate(d->timeLine->pos());
	d->timeLine->render(&painter);
	painter.restore();
}

void LogoView::resizeEvent(QResizeEvent *event) {
	d->message->setArea(rect(), 1, 1);
	d->timeLine->setArea(rect(), 1, 1);
	QWidget::resizeEvent(event);
}

void LogoView::showText(const RichString &text, int last) {
	d->message->showText(text, last);
//	update();
}

void LogoView::showTimeLine(int pos, int duration, int last) {
	d->timeLine->show(pos, duration, last);
//	update();
}
