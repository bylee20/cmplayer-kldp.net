#include "sliders.h"
#include "videoplayer.h"
#include <QtGui/QApplication>
#include <QtGui/QMouseEvent>
#include <QtGui/QStyle>
#include <QtCore/QDebug>

JumpSlider::JumpSlider(QWidget *parent)
: QSlider(parent) {
	setOrientation(Qt::Horizontal);
	setFocusPolicy(Qt::NoFocus);
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

SeekSlider::SeekSlider(VideoPlayer *player, QWidget *parent)
: JumpSlider(parent), player(player), tick(false) {
	player = player;
	setRange(0, player->duration());
	setValue(player->currentTime());
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(seek(int)));
	connect(player, SIGNAL(tick(int)), this, SLOT(slotTick(int)));
	connect(player, SIGNAL(seekableChanged(bool)), this, SLOT(setEnabled(bool)));
	connect(player, SIGNAL(durationChanged(int)), this, SLOT(setDuration(int)));
	setEnabled(player->isSeekable());
}

void SeekSlider::seek(int time) {
	if (!tick && player)
		player->seek(time, false, false);
}

VolumeSlider::VolumeSlider(VideoPlayer *player, QWidget *parent)
: JumpSlider(parent) {
	setMaximumWidth(70);
	setRange(0, 100);
	setValue(player->volume());
	connect(this, SIGNAL(valueChanged(int)), player, SLOT(setVolume(int)));
	connect(player, SIGNAL(volumeChanged(int)), this, SLOT(setValue(int)));
}


