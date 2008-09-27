#include "playmenubar.h"
#include "ui_playmenubar.h"
#include <backend/utility.h>
#include <backend/playengine.h>
#include <backend/seekslider.h>
#include <backend/volumeslider.h>
#include <QtGui/QToolButton>

struct PlayMenuBar::Data {
	Data(PlayMenuBar *parent) {
		ui.setupUi(parent);
		seeker = new Backend::SeekSlider(parent);
		amp = new Backend::VolumeSlider(parent);
	}
	Ui::Ui_PlayMenuBar ui;
	Backend::PlayEngine *engine;
	Backend::SeekSlider *seeker;
	Backend::VolumeSlider *amp;
};

PlayMenuBar::PlayMenuBar(const QList<QAction*> &acts, QWidget *parent)
: QWidget(parent), d(new Data(this)) {
	QHBoxLayout *hbox = new QHBoxLayout(d->ui.tools_widget);
	hbox->setMargin(0);
	hbox->setSpacing(0);
	bool seeker = true;
	for (int i=0; i<acts.size(); ++i) {
		if (acts[i]) {
			QToolButton *button = new QToolButton(this);
			button->setAutoRaise(true);
			button->setFocusPolicy(Qt::NoFocus);
			button->setDefaultAction(acts[i]);
			hbox->addWidget(button);
		} else {
			if (seeker)
				hbox->addWidget(d->seeker);
			else
				hbox->addWidget(d->amp);
			seeker = false;
		}
	}
	adjustSize();
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

PlayMenuBar::~PlayMenuBar() {
	delete d;
}

void PlayMenuBar::setPlayText(const QString &text) {
	d->ui.play_info_label->setText(text);
}

void PlayMenuBar::setTotalTime(int ms) {
	d->ui.total_time_label->setText(Backend::Utility::msecsToString(ms));
}

void PlayMenuBar::setCurrentTime(int ms) {
	static int before = -1;
	if (ms != before) {
		d->ui.current_time_label->setText(Backend::Utility::msecsToString(ms));
		before = ms;
	}
}

void PlayMenuBar::setPlayEngine(Backend::PlayEngine *engine) {
	if (d->engine == engine)
		return;
	d->engine = engine;
	connect(d->engine, SIGNAL(totalTimeChanged(int)), this, SLOT(setTotalTime(int)));
	connect(d->engine, SIGNAL(tick(int)), this, SLOT(setCurrentTime(int)));
	
	d->seeker->setPlayEngine(d->engine);
	d->amp->setAudioOutput(d->engine->audio());
}

