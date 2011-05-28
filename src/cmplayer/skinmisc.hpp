#ifndef SKINMISC_HPP
#define SKINMISC_HPP

#include <QtDeclarative/QDeclarativeItem>
#include <QtDeclarative/QDeclarativePropertyMap>

class QGraphicsAnchorLayout;

namespace Skin {

class GradientStop : public QObject {
	Q_OBJECT
	Q_PROPERTY(qreal position READ position WRITE setPosition)
	Q_PROPERTY(QColor color READ color WRITE setColor)
public:
	GradientStop(QObject *parent=0) : QObject(parent) {}
	qreal position() const { return m_position; }
	void setPosition(qreal position) { m_position = position; update(); }
	QColor color() const { return m_color; }
	void setColor(const QColor &color) { m_color = color; update(); }
private:
	void update();
	qreal m_position;
	QColor m_color;
};

class Gradient : public QObject {
	Q_OBJECT
	Q_PROPERTY(QDeclarativeListProperty<Skin::GradientStop> stops READ stops)
	Q_CLASSINFO("DefaultProperty", "stops")
public:
	Gradient(QObject *parent = 0): QObject(parent), m_grad(0) {}
	~Gradient() { delete m_grad; }
	QDeclarativeListProperty<GradientStop> stops() {return QDeclarativeListProperty<GradientStop>(this, m_stops);}
	const QLinearGradient *gradient() const;
signals:
	void updated();
private:
	void reset();
private:
	QList<GradientStop*> m_stops;
	mutable QLinearGradient *m_grad;
	friend class GradientStop;
};


class ValueHint : public QObject {
	Q_OBJECT
	Q_PROPERTY(QVariant plain READ plain WRITE setPlain NOTIFY plainChanged DESIGNABLE false)
	Q_PROPERTY(QVariant disabled READ disabled WRITE setDisabled NOTIFY disabledChanged DESIGNABLE false)
	Q_PROPERTY(QVariant hovered READ hovered WRITE setHovered NOTIFY hoveredChanged DESIGNABLE false)
	Q_PROPERTY(QVariant pressed READ pressed WRITE setPressed NOTIFY pressedChanged DESIGNABLE false)
public:
	ValueHint(QObject *parent = 0): QObject(parent) {}
	QVariant plain() const {return m_plain;}
	QVariant disabled() const {return m_disabled.isValid() ? m_disabled : m_plain;}
	QVariant hovered() const {return m_hovered.isValid() ? m_hovered : m_plain;}
	QVariant pressed() const {return m_pressed.isValid() ? m_pressed : (m_hovered.isValid() ? m_hovered : m_plain);}

	QVariant hint(bool pressed, bool hovered) const {
		if (pressed)
			return this->pressed();
		if (hovered)
			return this->hovered();
		return plain();
	}

	QVariant getPlain() const {return m_plain;}
	QVariant getDisabled() const {return m_disabled;}
	QVariant getHovered() const {return m_hovered;}
	QVariant getPressed() const {return m_pressed;}

	void setPlain(const QVariant &var) {if (m_plain != var) {m_plain = var; emit plainChanged(); emit changed();}}
	void setHovered(const QVariant &var) {if (m_hovered != var) {m_hovered = var; emit hoveredChanged(); emit changed();}}
	void setDisabled(const QVariant &var) {if (m_disabled != var) {m_disabled = var; emit disabledChanged(); emit changed();}}
	void setPressed(const QVariant &var) {if (m_pressed != var) {m_pressed = var; emit pressedChanged(); emit changed();}}
signals:
	void changed();
	void plainChanged();
	void disabledChanged();
	void hoveredChanged();
	void pressedChanged();
private:
	QVariant m_plain, m_hovered, m_pressed, m_disabled;
};

class UrlHint : public QObject {
	Q_OBJECT
	Q_PROPERTY(QUrl plain READ plain WRITE setPlain NOTIFY plainChanged DESIGNABLE false)
	Q_PROPERTY(QUrl disabled READ disabled WRITE setDisabled NOTIFY disabledChanged DESIGNABLE false)
	Q_PROPERTY(QUrl hovered READ hovered WRITE setHovered NOTIFY hoveredChanged DESIGNABLE false)
	Q_PROPERTY(QUrl pressed READ pressed WRITE setPressed NOTIFY pressedChanged DESIGNABLE false)
public:
	UrlHint(QObject *parent = 0): QObject(parent) {}
	QUrl plain() const {return m_plain;}
	QUrl disabled() const {return !m_disabled.isEmpty() ? m_disabled : m_plain;}
	QUrl hovered() const {return !m_hovered.isEmpty() ? m_hovered : m_plain;}
	QUrl pressed() const {return !m_pressed.isEmpty() ? m_pressed : (!m_hovered.isEmpty() ? m_hovered : m_plain);}

	QVariant hint(bool pressed, bool hovered) const {
		if (pressed)
			return this->pressed();
		if (hovered)
			return this->hovered();
		return plain();
	}

	QUrl getPlain() const {return m_plain;}
	QUrl getDisabled() const {return m_disabled;}
	QUrl getHovered() const {return m_hovered;}
	QUrl getPressed() const {return m_pressed;}

	void setPlain(const QUrl &var) {if (m_plain != var) {m_plain = var; emit plainChanged(); emit changed();}}
	void setHovered(const QUrl &var) {if (m_hovered != var) {m_hovered = var; emit hoveredChanged(); emit changed();}}
	void setDisabled(const QUrl &var) {if (m_disabled != var) {m_disabled = var; emit disabledChanged(); emit changed();}}
	void setPressed(const QUrl &var) {if (m_pressed != var) {m_pressed = var; emit pressedChanged(); emit changed();}}

signals:
	void changed();
	void plainChanged();
	void disabledChanged();
	void hoveredChanged();
	void pressedChanged();
private:
	QUrl m_plain, m_hovered, m_pressed, m_disabled;
};

class BackgroundHint : public QObject {
	Q_OBJECT
	Q_PROPERTY(Skin::ValueHint *color READ color WRITE setColor NOTIFY colorChanged DESIGNABLE false)
	Q_PROPERTY(Skin::ValueHint *gradient READ gradient WRITE setGradient NOTIFY gradientChanged DESIGNABLE false)
	Q_PROPERTY(Skin::UrlHint *image READ image WRITE setImage NOTIFY imageChanged DESIGNABLE false)
	Q_PROPERTY(int fillMode READ fillMode WRITE setFillMode NOTIFY fillModeChanged DESIGNABLE false)
public:
	BackgroundHint(QObject *parent = 0): QObject(parent) {
		m_color = 0;
		m_image = 0;
		m_gradient = 0;
		setColor(new ValueHint(this));
		setImage(new UrlHint(this));
		setGradient(new ValueHint(this));
		m_color->setPlain(Qt::transparent);
		m_fillMode = 0;
		m_gradient->setPlain(QVariant::fromValue<QObject*>(0));
	}
	ValueHint *color() const {return m_color;}
	UrlHint *image() const {return m_image;}
	ValueHint *gradient() const {return m_gradient;}
	void setColor(ValueHint *color) {
		if (color && m_color != color) {
			if (m_color)
				disconnect(m_color, 0, this, 0);
			m_color = color;
			emit colorChanged();
			emit changed();
			connect(m_color, SIGNAL(changed()), this, SIGNAL(changed()));
		}
	}
	void setGradient(ValueHint *grad) {
		if (grad && m_gradient != grad) {
			if (m_gradient)
				disconnect(m_gradient, 0, this, 0);
			m_gradient = grad;
			emit gradientChanged();
			emit changed();
			connect(m_gradient, SIGNAL(changed()), this, SIGNAL(changed()));
		}
	}
	void setImage(UrlHint *image) {
		if (image && m_image != image) {
			if (m_image)
				disconnect(m_image, 0, this, 0);
			m_image = image;
			emit imageChanged();
			emit changed();
			connect(m_image, SIGNAL(changed()), this, SIGNAL(changed()));
		}
	}
	int fillMode() const {return m_fillMode;}
	void setFillMode(const int &var) {
		if (m_fillMode != var) {
			m_fillMode = var;
			emit fillModeChanged();
			emit changed();
		}
	}
signals:
	void fillModeChanged();
	void colorChanged();
	void imageChanged();
	void gradientChanged();
	void changed();
private:
	ValueHint *m_color, *m_gradient;
	UrlHint *m_image;
	int m_fillMode;
};

class BorderHint : public QObject {
	Q_OBJECT
	Q_PROPERTY(Skin::ValueHint *color READ color WRITE setColor NOTIFY colorChanged DESIGNABLE false)
	Q_PROPERTY(Skin::ValueHint *width READ width WRITE setWidth NOTIFY widthChanged DESIGNABLE false)
public:
	BorderHint(QObject *parent = 0): QObject(parent) {
		m_color = 0;
		m_width = 0;
		setColor(new ValueHint(this));
		setWidth(new ValueHint(this));
		m_color->setPlain(Qt::transparent);
		m_width->setPlain(0);
	}
	ValueHint *color() const {return m_color;}
	ValueHint *width() const {return m_width;}
	void setColor(ValueHint *color) {
		if (color && m_color != color) {
			if (m_color)
				disconnect(m_color, 0, this, 0);
			m_color = color;
			emit colorChanged();
			emit changed();
			connect(m_color, SIGNAL(changed()), this, SIGNAL(changed()));
		}
	}
	void setWidth(ValueHint *w) {
		if (w && m_width != w) {
			if (m_width)
				disconnect(m_width, 0, this, 0);
			m_width = w;
			emit widthChanged();
			emit changed();
			connect(m_width, SIGNAL(changed()), this, SIGNAL(changed()));
		}
	}
signals:
	void colorChanged();
	void widthChanged();
	void changed();
private:
	ValueHint *m_color, *m_width;
};

//class IconHint : public QObject {
//	Q_OBJECT
//	Q_PROPERTY(QUrl normal READ normal WRITE setNormal)
//	Q_PROPERTY(QUrl checked READ checked WRITE setChecked)
//public:
//	IconHint(QObject *parent = 0): QObject(parent) {}
//	QUrl normal() const {return m_normal;}
//	QUrl checked() const {return m_checked;}
//	void setNormal(const QUrl &url) {m_normal = url;}
//	void setChecked(const QUrl &url) {m_checked = url;}
//private:
//	QUrl m_normal, m_checked;
//};

//class Button : public QDeclarativeItem {
//	Q_OBJECT
//	Q_PROPERTY(Skin::IconHint *icon READ icon DESIGNABLE false CONSTANT FINAL)
//	Q_PROPERTY(QAction *action READ action WRITE setAction NOTIFY actionChanged DESIGNABLE false)
//public:
//	Button(QDeclarativeItem *parent = 0): QDeclarativeItem(parent) {
//		m_icon = new IconHint(this);
//		m_action = 0;
//	}
//	IconHint *icon() const {return m_icon;}
//	QAction *action() const {return m_action;}
//	void setAction(QAction *act) {if (m_action != act) {m_action = act; emit actionChanged();}}
//	Q_INVOKABLE void exec();
//signals:
//	void actionChanged();
//private:
//	IconHint *m_icon;
//	QAction *m_action;
//};



//class RectHint: public QObject {
//	Q_OBJECT
//	Q_PROPERTY(Skin::BackgroundHint *background READ background WRITE setBackground NOTIFY backgroundChanged DESIGNABLE false)
//	Q_PROPERTY(Skin::BorderHint *border READ border WRITE setBorder NOTIFY borderChanged DESIGNABLE false)
//	Q_PROPERTY(Skin::ValueHint *radius READ radius WRITE setRadius NOTIFY radiusChanged)
//public:
//	RectHint(QObject *parent = 0): QObject(parent) {
//		m_bg = new BackgroundHint(this);
//		m_border = new BorderHint(this);
//		m_radius = new ValueHint(this);
//		m_radius->setPlain(0.0);
//	}
//	BackgroundHint *background() const {return m_bg;}
//	BorderHint *border() const {return m_border;}
//	ValueHint *radius() const {return m_radius;}
//	void setRadius(ValueHint *radius) {if (m_radius != radius) {m_radius = radius; emit radiusChanged();}}
//	void setBackground(BackgroundHint *bg) {if (m_bg != bg) {m_bg = bg; emit backgroundChanged();}}
//	void setBorder(BorderHint *border) {if (m_border != border) {m_border = border; emit borderChanged();}}
//signals:
//	void borderChanged();
//	void backgroundChanged();
//	void radiusChanged();
//private:
//	BackgroundHint *m_bg;
//	BorderHint *m_border;
//	ValueHint *m_radius;
//};

//class HandleHint: public RectHint {
//	Q_OBJECT
//	Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
//	Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
//public:
//	HandleHint(QObject *parent = 0): RectHint(parent) {m_width = m_height = 1;}
//	void setWidth(int w) {if (m_width != w) {m_width = w; emit widthChanged();}}
//	void setHeight(int h) {if (m_height != h) {m_height = h; emit heightChanged();}}
//	int width() const {return m_width;}
//	int height() const {return m_height;}
//signals:
//	void widthChanged();
//	void heightChanged();
//private:
//	int m_width, m_height;
//};

//class Rect : public QDeclarativeItem {
//	Q_OBJECT
//	Q_PROPERTY(Skin::BackgroundHint *background READ background WRITE setBackground NOTIFY backgroundChanged DESIGNABLE false)
//	Q_PROPERTY(Skin::BorderHint *border READ border WRITE setBorder NOTIFY borderChanged DESIGNABLE false)
//	Q_PROPERTY(Skin::ValueHint *radius READ radius WRITE setRadius NOTIFY radiusChanged)
//public:
//	Rect(QDeclarativeItem *parent = 0): QDeclarativeItem(parent) {
//		m_hint = new RectHint(this);
//		connect(m_hint, SIGNAL(backgroundChanged()), this, SIGNAL(backgroundChanged()));
//		connect(m_hint, SIGNAL(borderChanged()), this, SIGNAL(borderChanged()));
//		connect(m_hint, SIGNAL(radiusChanged()), this, SIGNAL(radiusChanged()));
//	}
//	BackgroundHint *background() const {return m_hint->background();}
//	BorderHint *border() const {return m_hint->border();}
//	ValueHint *radius() const {return m_hint->radius();}
//	void setBackground(BackgroundHint *bg) {m_hint->setBackground(bg);}
//	void setBorder(BorderHint *border) {m_hint->setBorder(border);}
//	void setRadius(ValueHint *radius) {m_hint->setRadius(radius);}
//signals:
//	void borderChanged();
//	void backgroundChanged();
//	void radiusChanged();
//private:
//	RectHint *m_hint;
//};

class RectItem : public QDeclarativeItem {
	Q_OBJECT
	Q_PROPERTY(Skin::BackgroundHint *background READ background WRITE setBackground NOTIFY backgroundChanged DESIGNABLE false)
	Q_PROPERTY(Skin::BorderHint *border READ border WRITE setBorder NOTIFY borderChanged DESIGNABLE false)
	Q_PROPERTY(Skin::ValueHint *radius READ radius WRITE setRadius NOTIFY radiusChanged DESIGNABLE false)
//	Q_PROPERTY(bool mouseTracking READ mouseTracking WRITE mouseTracking NOTIFY mouseTrackingChanged)
	Q_PROPERTY(bool hovered READ isHovered NOTIFY hoveredChanged)
	Q_PROPERTY(bool pressed READ isPressed NOTIFY pressedChanged)
	Q_PROPERTY(bool containsMouse READ containsMouse)
	Q_PROPERTY(QPointF mousePos READ mousePos NOTIFY mousePosChanged)
	Q_PROPERTY(QPointF dragTopLeft READ dragTopLeft WRITE setDragTopLeft)
	Q_PROPERTY(QPointF dragBottomRight READ dragBottomRight WRITE setDragBottomRight)
public:
	RectItem(QDeclarativeItem *parent = 0);
	~RectItem();
	BackgroundHint *background() const;
	BorderHint *border() const;
	ValueHint *radius() const;
	void setBackground(BackgroundHint *bg);
	void setBorder(BorderHint *border);
	void setRadius(ValueHint *radius);
	bool isHovered() const;
	bool isPressed() const;
	Q_INVOKABLE void fakeUpdate(bool pressed, bool isHovered);
	QRectF geometry() const;
	QPointF mousePos() const;
	QPointF dragTopLeft() const;
	QPointF dragBottomRight() const;
	void setDragTopLeft(const QPointF &pos);
	void setDragBottomRight(const QPointF &pos);
	bool containsMouse() const {return boundingRect().contains(mousePos());}
signals:
	void borderChanged();
	void backgroundChanged();
	void radiusChanged();
	void hoveredChanged();
	void pressedChanged();
	void geometryChanged();
	void mousePosChanged();
	void pressed();
	void released();
	void dragged();
private slots:
	void updateBackground();
	void updateSize();
private:
	void geometryChanged(const QRectF &geometry, const QRectF &old);
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);
	struct Data;
	Data *d;
};

class SliderItem : public QDeclarativeItem {
	Q_OBJECT
	Q_PROPERTY(Skin::RectItem *handle READ handle DESIGNABLE false CONSTANT FINAL)
	Q_PROPERTY(Skin::RectItem *filled READ filled DESIGNABLE false CONSTANT FINAL)
	Q_PROPERTY(Skin::RectItem *unfilled READ unfilled DESIGNABLE false CONSTANT FINAL)
	Q_PROPERTY(int minimum READ minimum WRITE setMinimum NOTIFY minimumChanged DESIGNABLE false)
	Q_PROPERTY(int maximum READ maximum WRITE setMaximum NOTIFY maximumChanged DESIGNABLE false)
	Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged DESIGNABLE false)
	Q_PROPERTY(qreal rate READ rate NOTIFY rateChanged)
public:
	SliderItem();
	~SliderItem();
	RectItem *handle() const {return m_handle;}
	RectItem *filled() const {return m_filled;}
	RectItem *unfilled() const {return m_unfilled;}
	int minimum() const {return m_min;}
	int maximum() const {return m_max;}
	int value() const {return m_value;}
	qreal rate() const {return m_max <= m_min ? 0.0 : qBound(0.0, (qreal)(m_value - m_min)/(qreal)(m_max - m_min), 1.0);}
	void setMinimum(int min) {if (m_min != min) {m_min = min; emit minimumChanged(); emit rateChanged();}}
	void setMaximum(int max) {if (m_max != max) {m_max = max; emit maximumChanged(); emit rateChanged();}}
	void setValue(int value) {
		if (value != m_value && m_min <= value && value <= m_max) {
			m_value = value; emit valueChanged(); emit rateChanged();
		}
	}
signals:
	void rateChanged();
	void minimumChanged();
	void maximumChanged();
	void valueChanged();
	void valueChangedByUser();
private slots:
	void arrange();
	void refill();
	void onMousePosChanged();
private:
	QGraphicsAnchorLayout *m_layout;
	RectItem *m_handle, *m_filled, *m_unfilled;
	int m_min, m_max, m_value;
};

class ButtonItem : public RectItem {
	Q_OBJECT
	Q_PROPERTY(bool checked READ isChecked WRITE setChecked NOTIFY checkedChanged)
	Q_PROPERTY(QAction *action READ action WRITE setAction NOTIFY actionChanged DESIGNABLE false)
public:
	ButtonItem(QDeclarativeItem *parent = 0);
	bool isChecked() const {return m_checked;}
	void setChecked(bool checked) {if (m_checked != checked) {emit toggled(m_checked = checked); emit checkedChanged();}}
	QAction *action() const {return m_action;}
	void setAction(QAction *act) {if (m_action != act) {m_action = act; emit actionChanged();}}
	Q_INVOKABLE void exec();
public slots:
	void toggle() {setChecked(!m_checked);}
signals:
	void checkedChanged();
	void actionChanged();
	void clicked();
	void toggled(bool checked);
private slots:
	void onPressed();
	void onReleased();
	void onDragged();
private:
	bool m_checked;
	bool m_dragged;
	QAction *m_action;
};




class Media : public QObject {
	Q_OBJECT
	Q_PROPERTY(QString info READ info DESIGNABLE false)
	Q_PROPERTY(int index READ index DESIGNABLE false)
	Q_PROPERTY(int nth READ nth DESIGNABLE false)
public:
	Media(QObject *parent = 0): QObject(parent) {m_index = 0;}
	~Media();
	QString info() const {return m_info;}
	int index() const {return m_index;}
	int nth() const {return m_index + 1;}
	void setIndex(int index) {m_index = index;}
	void setInfo(const QString &info) {m_info = info;}
private:
	int m_index;
	QString m_info;
};

class Storage : public QDeclarativePropertyMap {
	Q_OBJECT
public:
	void setName(const QString &name) {m_name = name;}
	Q_INVOKABLE void save();
	Q_INVOKABLE void load();
private:
	QString m_name;
};

class Spacer : public QDeclarativeItem {
	Q_OBJECT
	Q_PROPERTY(QRectF geometry READ geometry WRITE setGeometry NOTIFY geometryChanged)
public:
	static const int Type = UserType+1;
	Spacer(QDeclarativeItem *parent = 0);
	QRectF geometry() const {return m_rect;}
	int type() const {return Type;}
	void setGeometry(const QRectF &rect);
signals:
	void geometryChanged();
private:
	void geometryChanged(const QRectF &newOne, const QRectF &old);
	QRectF m_rect;
};

class Frame : public Spacer {
	Q_OBJECT
//	Q_PROPERTY(QSize zeroSize READ zeroSize WRITE setZeroSize NOTIFY zeroSizeChanged)
	Q_PROPERTY(Skin::Spacer *view READ view CONSTANT FINAL)
public:
	Frame(): m_zero(0, 0), m_view(new Spacer(this)) {}
//	QSize zeroSize() const {return m_zero;}
	QSize zero() const {return (geometry().size() - m_view->geometry().size()).toSize();}
//	void setZeroSize(const QSize &size) {if (m_zero != size) {m_zero = size; emit zeroSizeChanged();}}
	Spacer *view() const {return m_view;}
signals:
	void zeroSizeChanged();
private:
	QSize m_zero;
	Spacer *m_view;
};



//class Hider : public Rect {
//	Q_OBJECT
//public:
//	Hider(QObject *parent);
//	~Hider();
//private:

//	Rectangle {
//		id: hider

//		property alias checked: button.checked

//		anchors.leftMargin: 3
//		anchors.left:parent.left
//		width: 8
//		height: width
//		radius: width*0.5
//		color: "#333"
//		border.width: 2
//		border.color: "#555"
//		z: 1e10

//		onCheckedChanged: {
//			if (autoHide)
//				autoHide.visible = !checked
//		}

//		function reanchor() {
//			if (y + height*0.5 < skin.height*0.5) {
//				anchors.topMargin = y
//				anchors.bottomMargin = -1
//				anchors.top = skin.top
//			} else {
//				anchors.bottomMargin = skin.height - (y + height)
//				anchors.bottom = skin.bottom
//				anchors.topMargin = -1
//			}
//		}

//		BaseButton {
//			id: button
//			anchors.fill: parent
//			radius.plain: parent.radius
//			border.color.plain: "transparent"
//			border.color.hovered: "#6ad"
//			border.color.pressed: "#6ad"
//			border.width.plain: 0
//			border.width.hovered: 1
//			border.width.pressed: 2
//			MouseArea {
//				property real prevY: -1

//				anchors.fill: parent
//				drag.axis: Drag.YAxis
//				drag.target: hider
//				drag.minimumY: skin.y + 3.0
//				drag.maximumY: skin.y + skin.height - 3.0 - parent.height
//				onPressed: {
//					hider.anchors.top = hider.anchors.bottom = undefined
//					prevY = hider.y
//					button.pressed = true
//				}
//				onReleased: {
//					button.pressed = false
//					if (prevY == hider.y)
//						button.checked = !button.checked
//					hider.reanchor()
//				}
//			}
//		}
//	}
//};

}

#endif // SKINMISC_HPP
