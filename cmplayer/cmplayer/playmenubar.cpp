#include "playmenubar.h"
#include "ui_playmenubar.h"
#include <backend/utility.h>

struct PlayMenuBar::Data {
	Data(PlayMenuBar *parent) : p(parent) {ui.setupUi(p);}
	PlayMenuBar *p;
	Ui::Ui_PlayMenuBar ui;
};

PlayMenuBar::PlayMenuBar(QWidget *parent)
: QWidget(parent) {
	d = new Data(this);
}

PlayMenuBar::~PlayMenuBar() {
	delete d;
}

void PlayMenuBar::init(const QList<QWidget *> &tools) {
	QHBoxLayout *layout = new QHBoxLayout;
	layout->setMargin(0);
	layout->setSpacing(0);
	for (int i=0; i<tools.size(); ++i)
		layout->addWidget(tools[i]);
	d->ui.tools_widget->setLayout(layout);
	adjustSize();
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
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
