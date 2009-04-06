/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the Graphics Dojo project on Qt Labs.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 or 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "dragcharm.h"
#include <QtCore/QEvent>
#include <QtCore/QHash>
#include <QtGui/QMouseEvent>
#include <QtGui/QWidget>
#include <QtCore/QDebug>

struct DragCharm::Data {
	QWidget *widget;
	bool moving, resizing;
	QPoint startDrag;
	QRect rect;
	int border, bound;
};

DragCharm::DragCharm(QObject *parent)
: QObject(parent), d(new Data) {
	d->widget = 0;
	d->bound = -1;
}

DragCharm::~DragCharm() {
	delete d;
}

void DragCharm::setTracking(QWidget *widget) {
	if (widget) {
		QList<QWidget*> children = widget->findChildren<QWidget*>();
		for (int i=0; i<children.size(); ++i) {
			setTracking(children[i]);
		}
		widget->setMouseTracking(true);
	}
}

void DragCharm::setBorder(int width) {
	d->border = width;
	setTracking(d->widget);
}

void DragCharm::setRect(const QRect &rect) {
	d->rect = rect;
}

void DragCharm::activate(QWidget *widget) {
	deactivate();
	if ((d->widget = widget)) {
		d->rect = QRect();
		d->border = -1;
		d->startDrag = QPoint(0, 0);
		d->resizing = d->moving = false;
		d->widget->installEventFilter(this);
	}
}

void DragCharm::deactivate() {
	if (d->widget)
		d->widget->removeEventFilter(this);
}

bool DragCharm::eventFilter(QObject *object, QEvent *event) {
	if (!d->widget || d->widget != object || d->widget->isFullScreen())
		return false;
	QEvent::Type type = event->type();
	if (type != QEvent::MouseButtonPress &&
			type != QEvent::MouseButtonRelease &&
			type != QEvent::MouseMove)
		return false;

	QMouseEvent *me = static_cast<QMouseEvent*>(event);
	if (me->modifiers() != Qt::NoModifier)
		return false;
	Qt::MouseButton button = me->button();

	bool consumed = false;

	const QPoint pos = me->pos();
	const QPoint gpos = me->globalPos();
	
	int bound = 0;
	if (d->border > 0) {
		if (pos.x() <= d->border)
			bound |= Left;
		else if (pos.x() > d->widget->width() - d->border)
			bound |= Right;
		if (pos.y() <= d->border)
			bound |= Top;
		else if (pos.y() > d->widget->height() - d->border)
			bound |= Bottom;
	}
	
	if (type == QEvent::MouseButtonPress && button == Qt::LeftButton) {
		d->resizing = bound != 0;
		if (!d->resizing)
			d->moving = d->rect.isNull() || d->rect.contains(pos);
		if (d->resizing || d->moving) {
			d->bound = bound;
			d->startDrag = gpos;
			event->accept();
			consumed = true;
		}
	} else if (type == QEvent::MouseButtonRelease) {
		d->startDrag = QPoint(0, 0);
		d->bound = 0;
		d->moving = false;
		d->resizing = false;
	} else if (type == QEvent::MouseMove) {
		const QPoint diff = gpos - d->startDrag;
		if (d->moving) {
			d->widget->move(d->widget->pos() + diff);
			d->startDrag = gpos;
			consumed = true;
		} else if (d->resizing) {
			QSize size = d->widget->size();
			QPoint pos = d->widget->pos();
			bool resize = false;
			if (d->bound & Top || d->bound & Bottom) {
				if (d->bound & Top) {
					size.rheight() -= diff.y();
					pos.ry() += diff.y();
				} else
					size.rheight() += diff.y();
				if (size.height() > d->widget->minimumHeight()) {
					resize = true;
					d->startDrag.ry() = gpos.y();
				}
			}
			if (d->bound & Left || d->bound & Right) {
				if (d->bound & Left) {
					size.rwidth() -= diff.x();
					pos.rx() += diff.x();
				} else
					size.rwidth() += diff.x();
				if (size.width() > d->widget->minimumWidth()) {
					resize = true;
					d->startDrag.rx() = gpos.x();
				}
			}
			if (resize) {
				d->widget->resize(size);
				d->widget->move(pos);
				consumed = true;
			}
		}
		if (d->border > 0 && d->bound<= 0) {
			switch(bound) {
			case Top:
			case Bottom:
				d->widget->setCursor(Qt::SizeVerCursor);
				break;
			case Left:
			case Right:
				d->widget->setCursor(Qt::SizeHorCursor);
				break;
			case TopLeft:
			case BottomRight:
				d->widget->setCursor(Qt::SizeFDiagCursor);
				break;
			case TopRight:
			case BottomLeft:
				d->widget->setCursor(Qt::SizeBDiagCursor);
				break;
			default:
				d->widget->unsetCursor();
				break;
			};
		}
	}

	return consumed;
}
