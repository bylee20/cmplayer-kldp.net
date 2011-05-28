#include "skinmisc.hpp"
#include <QtGui/QPixmapCache>
#include <QtGui/QPainter>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QGraphicsSceneHoverEvent>
#include <QtGui/QMenu>
#include <QtCore/QMap>
#include <QtGui/QGraphicsAnchorLayout>
#include "record.hpp"

namespace Skin {

void GradientStop::update() {
	Gradient *grad = qobject_cast<Gradient*>(parent());
	if (grad)
		grad->reset();
}

void Gradient::reset() {
	delete m_grad;
	m_grad = 0;
}

const QLinearGradient *Gradient::gradient() const {
	if (m_grad)
		return m_grad;
	m_grad = new QLinearGradient;
	for (int i=0; i<m_stops.size(); ++i) {
		m_grad->setColorAt(m_stops[i]->position(), m_stops[i]->color());
	}
	return m_grad;
}

struct RectItem::Data {
	QRectF geometry;
	BackgroundHint *bg;
	BorderHint *border;
	ValueHint *radius;
	bool hovered;
	bool pressed;
	QRectF dragArea;
	QPointF dragPos;
	bool dragging;
	QPen pen;
	QBrush brush;
	QRectF rect;
	QSizeF size;
	QPainterPath path;
	QPixmap pixmap;
	QMap<QUrl, QPixmapCache::Key> cache;
	QPointF mPos;

	bool draggable() const {
		if (dragArea.top() > dragArea.bottom() || dragArea.left() > dragArea.bottom())
			return false;
		if (dragArea.top() == dragArea.bottom() && dragArea.left() == dragArea.right())
			return false;
		return true;
	}
};

RectItem::RectItem(QDeclarativeItem *parent): QDeclarativeItem(parent), d(new Data) {
	d->geometry = boundingRect();
	d->dragging = false;
	d->dragArea.setCoords(0, 0, -1, -1);
	d->hovered = d->pressed = false;
	d->bg = 0;
	d->border = 0;
	d->radius = 0;
	setBackground(new BackgroundHint(this));
	setBorder(new BorderHint(this));
	setRadius(new ValueHint(this));

	setFlag(QGraphicsItem::ItemHasNoContents, false);
	setAcceptHoverEvents(true);
	setAcceptedMouseButtons(Qt::LeftButton);

	updateBackground();
}

RectItem::~RectItem() {
	delete d;
}

void RectItem::setDragTopLeft(const QPointF &pos) {
	d->dragArea.setTopLeft(pos);
}

void RectItem::setDragBottomRight(const QPointF &pos) {
	d->dragArea.setBottomRight(pos);
}

QPointF RectItem::dragTopLeft() const {
	return d->dragArea.topLeft();
}

QPointF RectItem::dragBottomRight() const {
	return d->dragArea.bottomRight();
}

QRectF RectItem::geometry() const {
	return d->geometry;
}

void RectItem::geometryChanged(const QRectF &geometry, const QRectF &old) {
	QDeclarativeItem::geometryChanged(geometry, old);
	if (d->geometry != geometry) {
		if (d->geometry.size() != geometry.size()) {
			d->geometry = geometry;
			updateSize();
		} else
			d->geometry = geometry;
		emit geometryChanged();
	}
}

void RectItem::updateSize() {
	d->rect = boundingRect();
	const qreal border = d->pen.widthF();
	if (d->pen.color().alpha() > 0 && border > 0.0) {
		d->rect.translate(border*0.5, border*0.5);
		d->rect.setWidth(d->rect.width() - border);
		d->rect.setHeight(d->rect.height() - border);
	}

	if (d->bg) {
		const Gradient *grad = qobject_cast<Gradient*>(d->bg->gradient()->hint(d->pressed, d->hovered).value<QObject*>());
		if (grad) {
			QLinearGradient qgrad = *grad->gradient();
			qgrad.setStart(d->rect.topLeft());
			qgrad.setFinalStop(d->rect.bottomLeft());
			d->brush = QBrush(qgrad);
		}
	}

	if (d->radius) {
		const qreal radius = d->radius->hint(d->pressed, d->hovered).toDouble();
		d->path = QPainterPath();
		if (radius > 0.0)
			d->path.addRoundedRect(d->rect, radius, radius);
		else
			d->path.addRect(d->rect);
	}
	update();
}

void RectItem::updateBackground() {
	if (d->border) {
		const qreal border = d->border->width()->hint(d->pressed, d->hovered).toDouble();
		const QColor b_color = d->border->color()->hint(d->pressed, d->hovered).value<QColor>();
		if (border > 0.0 && b_color.alpha() > 0) {
			d->pen.setColor(b_color);
			d->pen.setWidthF(border);
		} else {
			d->pen.setColor(Qt::transparent);
			d->pen.setWidthF(0.0);
		}
	}

	if (d->bg) {
		d->brush = QBrush(d->bg->color()->hint(d->pressed, d->hovered).value<QColor>());
		const QUrl url = d->bg->image()->hint(d->pressed, d->hovered).toUrl();
		if (!url.isEmpty()) {
			d->pixmap = QPixmap();
			if (!QPixmapCache::find(d->cache[url], &d->pixmap)) {
				if (d->pixmap.load(url.toLocalFile()))
					d->cache[url] = QPixmapCache::insert(d->pixmap);
			}
		}
	}
	updateSize();
}

BackgroundHint *RectItem::background() const {
	return d->bg;
}

BorderHint *RectItem::border() const {
	return d->border;
}

ValueHint *RectItem::radius() const {
	return d->radius;
}

bool RectItem::isHovered() const {
	return d->hovered;
}

bool RectItem::isPressed() const {
	return d->pressed;
}

void RectItem::setBackground(BackgroundHint *bg) {
	if (bg && d->bg != bg) {
		if (d->bg)
			disconnect(d->bg, 0, this, 0);
		d->bg = bg;
		emit backgroundChanged();
		connect(d->bg, SIGNAL(changed()), this, SLOT(updateBackground()));
		updateBackground();
	}
}

void RectItem::setBorder(BorderHint *border) {
	if (border && d->border != border) {
		if (d->border)
			disconnect(d->border, 0, this, 0);
		d->border = border;
		emit borderChanged();
		connect(d->border, SIGNAL(changed()), this, SLOT(updateBackground()));
		updateBackground();
	}
}

void RectItem::setRadius(ValueHint *radius) {
	if (radius && d->radius != radius) {
		if (d->radius)
			disconnect(d->radius, 0, this, 0);
		d->radius = radius;
		connect(d->radius, SIGNAL(changed()), this, SLOT(updateSize()));
		updateSize();
	}
}

void RectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem */*opt*/, QWidget */*w*/) {
	painter->save();
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setRenderHint(QPainter::HighQualityAntialiasing, true);

	painter->setPen(d->pen);
	painter->setBrush(d->brush);
	painter->drawPath(d->path);

	if (!d->pixmap.isNull()) {
		QSizeF size = d->pixmap.size();
		size.scale(d->rect.size(), Qt::KeepAspectRatio);
		QRectF rect(d->rect.topLeft(), size);
		rect.moveTop((d->rect.height()-size.height())*0.5);
		rect.moveLeft((d->rect.width()-size.width())*0.5);
		painter->drawPixmap(rect, d->pixmap, d->pixmap.rect());
	}

	painter->restore();
}

QPointF RectItem::mousePos() const {
	return d->mPos;
}

void RectItem::fakeUpdate(bool pressed, bool hovered) {
	const bool oldPressed = d->pressed;
	const bool oldHovered = d->hovered;
	d->pressed = pressed;
	d->hovered = hovered;
	updateBackground();
	d->pressed = oldPressed;
	d->hovered = oldHovered;
}

void RectItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
	d->hovered = true;
	updateBackground();
	emit hoveredChanged();
	d->mPos = event->pos();
	emit mousePosChanged();
}

void RectItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
	d->hovered = false;
	updateBackground();
	emit hoveredChanged();
	d->mPos = event->pos();
	emit mousePosChanged();
}

void RectItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
//	QDeclarativeItem::hoverMoveEvent(event);
	d->mPos = event->pos();
	emit mousePosChanged();
//	qDebug() << event->pos();
}

void RectItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	d->pressed = true;
	updateBackground();
	emit pressedChanged();
	emit pressed();
	d->mPos = event->pos();
	emit mousePosChanged();

	if (d->draggable()) {
		d->dragging = true;
		d->dragPos = event->scenePos();
	}
}

void RectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
	d->mPos = event->pos();
	emit mousePosChanged();

	if (d->dragging && d->draggable()) {
		const QPointF p = event->scenePos() - d->dragPos + pos();
		const qreal x = qBound(d->dragArea.left(), p.x(), d->dragArea.right());
		const qreal y = qBound(d->dragArea.top(), p.y(), d->dragArea.bottom());
		setPos(x, y);
		d->dragPos = event->scenePos();
		emit dragged();
	} else
		d->dragging = false;
}

void RectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	d->pressed = false;
	updateBackground();
	emit pressedChanged();
	emit released();
	d->mPos = event->pos();
	emit mousePosChanged();

	d->dragging = false;
	d->dragPos = QPointF();
}

/*****************************************************************************/

SliderItem::SliderItem() {
	m_filled = new RectItem(this);
	m_unfilled = new RectItem(this);
	m_handle = new RectItem(this);
	m_min = m_max = m_value = 0;

	connect(m_filled, SIGNAL(heightChanged()), this, SLOT(arrange()));
	connect(m_unfilled, SIGNAL(heightChanged()), this, SLOT(arrange()));
	connect(this, SIGNAL(heightChanged()), this, SLOT(arrange()));
	connect(this, SIGNAL(widthChanged()), this, SLOT(refill()));
	connect(this, SIGNAL(valueChanged()), this, SLOT(refill()));
	connect(m_filled, SIGNAL(mousePosChanged()), this, SLOT(onMousePosChanged()));
	connect(m_unfilled, SIGNAL(mousePosChanged()), this, SLOT(onMousePosChanged()));
	connect(m_handle, SIGNAL(mousePosChanged()), this, SLOT(onMousePosChanged()));

	setAcceptedMouseButtons(Qt::LeftButton);

	arrange();
}

SliderItem::~SliderItem() {
}

void SliderItem::refill() {
	const qreal middle = rate()*width();
	m_handle->setX(qBound(0.0, middle - m_handle->width()*0.5, width() - m_handle->width()));

	m_filled->setX(boundingRect().left());
	m_filled->setWidth(middle);

	m_unfilled->setX(middle);
	m_unfilled->setWidth(width() - middle);
}

void SliderItem::onMousePosChanged() {
	RectItem *item = qobject_cast<RectItem*>(sender());
	if (!item)
		return;
	if (item != m_handle)
		m_handle->fakeUpdate(item->isPressed(), false);
	if (!item->isPressed())
		return;
	const QPointF pos = item->QGraphicsItem::mapToItem(this, item->mousePos());
	const int value = qBound(m_min, qRound(m_min + pos.x()/width()*(m_max - m_min)), m_max);
	if (value != m_value) {
		setValue(value);
		emit valueChangedByUser();
	}
}

void SliderItem::arrange() {
	m_handle->setY((height() - m_handle->height())*0.5);
	m_filled->setY((height() - m_filled->height())*0.5);
	m_unfilled->setY((height() - m_unfilled->height())*0.5);
	refill();
}

/************************************************************************/

ButtonItem::ButtonItem(QDeclarativeItem *parent): RectItem(parent) {
	m_dragged = m_checked = false;
	m_action = 0;
	connect(this, SIGNAL(pressed()), this, SLOT(onPressed()));
	connect(this, SIGNAL(released()), this, SLOT(onReleased()));
	connect(this, SIGNAL(dragged()), this, SLOT(onDragged()));
}

void ButtonItem::exec() {
	if (m_action) {
		if (m_action->menu())
			m_action->menu()->exec(QCursor::pos());
		else
			m_action->trigger();
	}
}

void ButtonItem::onPressed() {
	m_dragged = false;
}

void ButtonItem::onDragged() {
	m_dragged = true;
}

void ButtonItem::onReleased() {
	if (!m_dragged && containsMouse()) {
		if (m_action)
			exec();
		emit clicked();
	}
	m_dragged = false;
}


Media::~Media() {
//	qDebug() << "delete media:" << m_info;
}

void Storage::save() {
	if (m_name.isEmpty()) {
		qWarning("Skin name is empty. Cannot save storage.");
		return;
	}
	const QStringList keys = this->keys();
	Record r("skin");
	r.beginGroup(m_name);
	for (int i=0; i<keys.size(); ++i)
		r.setValue(keys[i], this->value(keys[i]));
	r.endGroup();
}

void Storage::load() {
	if (m_name.isEmpty()) {
		qWarning("Skin name is empty. Cannot load storage.");
		return;
	}
	Record r("skin");
	r.beginGroup(m_name);
	const QStringList keys = this->keys();
	for (int i=0; i<keys.size(); ++i) {
		QVariant &data = (*this)[keys[i]];
		data = r.value(keys[i], data);
	}
	r.endGroup();
}

Spacer::Spacer(QDeclarativeItem *parent)
: QDeclarativeItem(parent) {}

void Spacer::geometryChanged(const QRectF &newOne, const QRectF &old) {
	QDeclarativeItem::geometryChanged(newOne, old);
	if (newOne != m_rect) {
		m_rect = newOne;
		emit geometryChanged();
	}
}

void Spacer::setGeometry(const QRectF &rect) {
	if (m_rect != rect) {
		setPos(rect.topLeft());
		setSize(rect.size());
	}
}


}

