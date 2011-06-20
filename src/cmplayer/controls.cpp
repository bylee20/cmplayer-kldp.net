#include "audiocontroller.hpp"
#include "playengine.hpp"
#include "controls.hpp"
#include "libvlc.hpp"
#include <QtGui/QApplication>
#include <QtGui/QMouseEvent>
#include <QtGui/QAction>
#include <QtGui/QStyle>
#include <QtCore/QRegExp>
#include <QtCore/QDebug>

struct Button::Data {
	QAction *action;
};

Button::Button(QWidget *parent)
: QToolButton(parent), d(new Data) {
	init();
}

Button::Button(const QIcon &icon, QWidget *parent)
: QToolButton(parent), d(new Data) {
	init();
	setIcon(icon);
}

Button::Button(const QString &text, QWidget *parent)
: QToolButton(parent), d(new Data) {
	init();
	setText(text);
}

Button::~Button() {
	delete d;
}

void Button::init() {
	d->action = 0;
	setObjectName("flat");
	setFocusPolicy(Qt::NoFocus);
	setAutoRaise(true);
}

void Button::setIconSize(int extent) {
	QToolButton::setIconSize(QSize(extent, extent));
}

void Button::setAction(QAction *action, bool update) {
	if (d->action == action)
		return;
	if (!(d->action = action))
		return;
	setCheckable(action->isCheckable());
	if (isCheckable()) {
		connect(this, SIGNAL(toggled(bool)), this, SLOT(toggleAction(bool)));
		connect(action, SIGNAL(toggled(bool)), this, SLOT(setChecked(bool)));
	} else
		connect(this, SIGNAL(clicked()), action, SLOT(trigger()));
	if (update) {
		if (action->icon().isNull())
			setText(action->text());
		else {
			setIcon(action->icon());
			setToolTip(action->text());
		}
	}
}

void Button::setBlock(bool block) {
	setObjectName(block ? "block" : "flat");
}

void Button::toggleAction(bool checked) {
	if (d->action && d->action->isChecked() != checked)
		d->action->trigger();
}



JumpSlider::JumpSlider(QWidget *parent)
: QSlider(Qt::Horizontal, parent) {
	setFocusPolicy(Qt::NoFocus);
	setSingleStep(1);
	setPageStep(1);
}

void JumpSlider::mousePressEvent(QMouseEvent *event) {
	const qint64 range = maximum() - minimum();
	const qint64 width = this->width();
	const qint64 newVal = static_cast<qint64>(event->x()) * range / width;
	const qint64 metric = qApp->style()->pixelMetric(QStyle::PM_SliderLength);
	const qint64 sub = (metric * range)/width;
	if (qAbs(newVal - value()) > sub)
		setValue(newVal);
	else
		QSlider::mousePressEvent(event);
}

SeekSlider::SeekSlider(QWidget *parent)
: JumpSlider(parent), engine(LibVLC::engine()), tick(false) {
	setRange(0, engine->duration());
	setValue(engine->position());
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(seek(int)));
	connect(engine, SIGNAL(tick(int)), this, SLOT(slotTick(int)));
	connect(engine, SIGNAL(seekableChanged(bool)), this, SLOT(setEnabled(bool)));
	connect(engine, SIGNAL(durationChanged(int)), this, SLOT(setDuration(int)));
	setEnabled(engine->isSeekable());
}

void SeekSlider::seek(int time) {
	if (!tick && engine)
		engine->seek(time);
}

void SeekSlider::setDuration(int duration) {
	tick = true;
	setRange(0, duration);
	tick = false;
}

void SeekSlider::slotTick(int time) {
	tick = true;
	setValue(time);
	tick = false;
}

VolumeSlider::VolumeSlider(QWidget *parent)
: JumpSlider(parent) {
	setMaximumWidth(70);
	setRange(0, 100);
	setValue(LibVLC::audio()->volume());
	connect(this, SIGNAL(valueChanged(int)), LibVLC::audio(), SLOT(setVolume(int)));
	connect(LibVLC::audio(), SIGNAL(volumeChanged(int)), this, SLOT(setValue(int)));
}


EncodingComboBox::EncodingComboBox(QWidget *parent)
: QComboBox(parent) {
	enc = QStringList() << "UTF-8" << "Unicode"
			<< tr("Western European Languages") + " (ISO-8859-1)"
			<< tr("Western European Languages With Euro") + " (ISO-8859-15)"
			<< tr("Slavic/Central European Languages") + " (ISO-8859-2)"
			<< tr("Slavic/Central European Windows") + " (CP1250)"
			<< tr("Esperanto, Galician, Maltese, Turkish") + " (ISO-8859-3)"
			<< tr("Old Baltic Charset") + " (ISO-8859-4)"
			<< tr("Cyrillic") + " (ISO-8859-5)"
			<< tr("Cyrillic Windows") + " (CP1251)"
			<< tr("Arabic") + " (ISO-8859-6)"
			<< tr("Modern Greek") + " (ISO-8859-7)"
			<< tr("Turkish") + " (ISO-8859-9)"
			<< tr("Baltic") + " (ISO-8859-13)"
			<< tr("Celtic") + " (ISO-8859-14)"
			<< tr("Hebrew Charset") + " (ISO-8859-8)"
			<< tr("Russian") + " (KOI8-R)"
			<< tr("Ukrainian, Belarusian") + " (KOI8-U/RU)"
			<< tr("Simplified Chinese Charset") + " (CP936)"
			<< tr("Traditional Chinese Charset") + " (BIG5)"
			<< tr("Japanese Charset") + " (SHIFT-JIS)"
			<< tr("Korean Charset") + " (CP949)"
			<< tr("Thai Charset") + " (CP874)";
	addItems(enc);
	setEditable(true);
}

QString EncodingComboBox::encoding() const {
	QString enc = currentText().trimmed();
	QRegExp rxEnc(".* \\((.*)\\)");
	return (rxEnc.indexIn(enc) == -1) ? enc : rxEnc.cap(1);
}

void EncodingComboBox::setEncoding(const QString &encoding) {
	static const QRegExp rxEncoding(".* \\(" + encoding.toUpper() + "\\)");
	const int idx = enc.indexOf(rxEncoding);
	if (idx != -1)
		setCurrentIndex(idx);
	else
		setEditText(encoding);
}
