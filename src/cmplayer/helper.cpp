#include "helper.h"
#include <core/info.h>
#include <QtCore/QDir>
#include <QtCore/QCoreApplication>
#include <QtCore/QString>

struct Helper::Data {
	Data(Helper *parent) : p(parent) {
		const QString pp = Core::Info::privatePath();
		configFile = pp + "/config.ini";
		recentFile = pp + "/recent.ini";
		stateFile = pp + "/state.ini";
	}
	Helper *p;
	QString configFile;
	QString recentFile;
	QString stateFile;
};

Helper::Helper(): d(new Data(this)) {}

Helper::~Helper() {
	delete d;
}

const QString &Helper::configFile() {
	return get()->d->configFile;
}

const QString &Helper::recentFile() {
	return get()->d->recentFile;
}

const QString &Helper::stateFile() {
	return get()->d->stateFile;
}

QString Helper::mediaExtensionFilter() {
	static const QString filter = QCoreApplication::translate("Helper", "Video Files") +' '
			+ Core::Info::videoExtension().toFilter() + ";;"
			+ QCoreApplication::translate("Helper", "Audio Files") + ' '
			+ Core::Info::audioExtension().toFilter() + ";;"
			+ QCoreApplication::translate("Helper", "All Files") + ' ' + "(*.*)";
	return filter;
}

// void Helper::drawBackground(QPainter *painter, QWidget *widget, int title) {
// 	painter->save();
// 	painter->fillRect(widget->rect(), Qt::black);
// 	painter->setPen(qRgb(0x88, 0x88, 0x88));
// 	painter->drawRect(1, 1, width() - 3, height() - 3);
// 	const QRectF box = boxRect();
// 	const double  title = d->titleBar->height();
// 	const double frameBound = d->frame->geometry().bottom();
// 	QLinearGradient grad(box.topLeft(), box.bottomLeft());
// 	grad.setColorAt(0, Qt::black);
// 	grad.setColorAt(frameBound/box.height(), qRgb(100, 100, 100));
// 	grad.setColorAt(1, qRgb(200, 200, 200));
// 	painter->fillRect(box, grad);
// 	grad = QLinearGradient(0, -1*title, 0, title);
// 	grad.setColorAt(0, Qt::transparent);
// 	grad.setColorAt(1, Qt::white);
// 	QBrush light = QBrush(grad);
// 	QPainterPath path;
// 	path.moveTo(box.topLeft());
// 	path.lineTo(box.topLeft() + QPointF(0, 0.9*title));
// 	path.cubicTo(box.topLeft() + QPointF(box.width()*0.25, title)
// 			, box.topLeft() + QPointF(box.width()*0.25, title)
// 			, box.topLeft() + QPointF(box.width()*0.5, title));
// 	path.cubicTo(box.topLeft() + QPointF(box.width()*0.75, title)
// 			, box.topLeft() + QPointF(box.width()*0.75, title)
// 			, box.topLeft() + QPointF(box.width(), 0.9*title));
// 	path.lineTo(box.topLeft() + QPointF(box.width(), 0));
// 	path.closeSubpath();
// 	painter->setRenderHint(QPainter::Antialiasing);
// 	painter->setPen(Qt::NoPen);
// 	painter->setOpacity(0.5);
// 	painter->setBrush(light);
// 	painter->drawPath(path);
// 	painter->restore();
// }
