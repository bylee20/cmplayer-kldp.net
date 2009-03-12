#include "playinfowidget.h"
#include "squeezedlabel.h"
#include <QHBoxLayout>
#include <core/utility.h>
#include <core/mediasource.h>

struct PlayInfoWidget::Data {
	SqueezedLabel *source;
	QLabel *state, *track, *time, *duration;
	int prevSecs;
};

PlayInfoWidget::PlayInfoWidget(QWidget *parent)
: QWidget(parent), d(new Data) {
	d->prevSecs = 0;
	QHBoxLayout *hbox = new QHBoxLayout(this);
	d->source = new SqueezedLabel(this);
	d->time = new QLabel("00:00:00", this);
	QLabel *slash = new QLabel("/", this);
	d->duration = new QLabel("00:00:00", this);
	d->state = new QLabel(this);
	d->track = new QLabel(this);
	
	slash->adjustSize();
	const int height = slash->height();
	setMaximumHeight(height);
	setMinimumHeight(height);

	hbox->addWidget(d->track);
	hbox->addWidget(d->state);
	hbox->addWidget(d->source);
	hbox->addWidget(d->time);
	hbox->addWidget(slash);
	hbox->addWidget(d->duration);
	setAutoFillBackground(true);
	QPalette p = palette();
	p.setColor(QPalette::ButtonText, Qt::white);
	p.setColor(QPalette::WindowText, Qt::white);
	p.setColor(QPalette::Button, Qt::black);
	p.setColor(QPalette::Window, Qt::black);
	setPalette(p);
	
	const QSizePolicy sp(QSizePolicy::Minimum, QSizePolicy::Minimum);
	d->time->setSizePolicy(sp);
	d->state->setSizePolicy(sp);
	d->track->setSizePolicy(sp);
	d->duration->setSizePolicy(sp);
	slash->setSizePolicy(sp);
	setSizePolicy(sp);
	
	hbox->setMargin(0);
	hbox->setSpacing(0);

	setState(Core::Stopped);
}

PlayInfoWidget::~PlayInfoWidget() {
	delete d;
}

void PlayInfoWidget::setCurrentSource(const Core::MediaSource &source) {
	d->source->setText(source.displayName());
}

void PlayInfoWidget::setState(Core::State state) {
	QString text;
	if (state == Core::Stopped)
		text += tr("Stopped");
	else if (state == Core::Playing)
		text += tr("Playing");
	else
		text += tr("Paused");
	d->state->setText("(" + text + ") ");
}

void PlayInfoWidget::setDuration(int duration) {
	d->duration->setText(Core::Utility::msecsToString(duration));
}

void PlayInfoWidget::setPlayTime(int time) {
	const int secs = time/1000;
	if (d->prevSecs != secs) {
		d->time->setText(Core::Utility::secsToString(secs));
		d->prevSecs = secs;
	}
}

void PlayInfoWidget::setTrackNumber(int nth, int total) {
	if (nth < 1 || total < 2) 
		d->track->clear();
	else
		d->track->setText(QString("%1/%2 ").arg(nth).arg(total));
}

