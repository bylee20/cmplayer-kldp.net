// #include "playinfowidget.h"
// #include "squeezedlabel.h"
// #include <core/utility.h>
// #include <core/mediasource.h>
// #include <QtCore/QTimer>
// #include <QtGui/QHBoxLayout>
// #include <QtGui/QStackedWidget>
// 
// struct PlayInfoWidget::Data {
// 	SqueezedLabel *source;
// 	QStackedWidget *stack;
// 	QLabel *state, *track, *time, *duration, *msg;
// 	int prevSecs;
// 	QTimer hider;
// };
// 
// PlayInfoWidget::PlayInfoWidget(QWidget *parent)
// : QWidget(parent), d(new Data) {
// 	d->prevSecs = 0;
// 	
// 	QHBoxLayout *hbox = new QHBoxLayout(this);
// 	d->stack = new QStackedWidget(this);
// 	d->time = new QLabel("00:00:00", this);
// 	QLabel *slash = new QLabel("/", this);
// 	slash->adjustSize();
// 	const int height = slash->height();
// 	d->duration = new QLabel("00:00:00", this);
// 	hbox->addWidget(d->stack);
// 	hbox->addWidget(d->time);
// 	hbox->addWidget(slash);
// 	hbox->addWidget(d->duration);
// 
// 	hbox->setContentsMargins(0, 0, 0, 0);
// 	hbox->setSpacing(0);
// 
// 	QWidget *w = new QWidget(d->stack);
// 	hbox = new QHBoxLayout(w);
// 	d->stack->addWidget(w);
// 	d->track = new QLabel(w);
// 	d->state = new QLabel(w);
// 	d->source = new SqueezedLabel(w);
// 	hbox->addWidget(d->track);
// 	hbox->addWidget(d->state);
// 	hbox->addWidget(d->source);
// 	d->stack->addWidget(w);
// 	
// 	hbox->setContentsMargins(0, 0, 0, 0);
// 	hbox->setSpacing(0);
// 
// 	d->msg = new QLabel(this);
// 	d->stack->addWidget(d->msg);
// 	
// 	d->stack->setCurrentIndex(Info);
// 	setFixedHeight(height);
// 
// 	setStyleSheet("background-color: black; color: white;");
// 
// 	setState(Core::Stopped);
// 	d->hider.setSingleShot(true);
// 	connect(&d->hider, SIGNAL(timeout()), this, SLOT(hideMessage()));
// }
// 
// PlayInfoWidget::~PlayInfoWidget() {
// 	delete d;
// }
// 
// void PlayInfoWidget::setCurrentSource(const Core::MediaSource &source) {
// 	d->source->setText(source.displayName());
// 	hideMessage();
// }
// 
// void PlayInfoWidget::setState(Core::State state) {
// 	QString text;
// 	if (state == Core::Stopped)
// 		text += tr("Stopped");
// 	else if (state == Core::Finished)
// 		text += tr("Finished");
// 	else if (state == Core::Playing)
// 		text += tr("Playing");
// 	else
// 		text += tr("Paused");
// 	d->state->setText("(" + text + ") ");
// 	hideMessage();
// }
// 
// void PlayInfoWidget::setDuration(int duration) {
// 	d->duration->setText(Core::Utility::msecsToString(duration));
// }
// 
// void PlayInfoWidget::setPlayTime(int time) {
// 	const int secs = time/1000;
// 	if (d->prevSecs != secs) {
// 		d->time->setText(Core::Utility::secsToString(secs));
// 		d->prevSecs = secs;
// 	}
// }
// 
// void PlayInfoWidget::setTrackNumber(int nth, int total) {
// 	if (nth < 1 || total < 2) 
// 		d->track->clear();
// 	else
// 		d->track->setText(QString("%1/%2 ").arg(nth).arg(total));
// 	hideMessage();
// }
// 
// void PlayInfoWidget::showMessage(const QString &msg, int time) {
// 	d->hider.stop();
// 	if (time > 0) {
// 		d->msg->setText(msg);
// 		d->stack->setCurrentIndex(Msg);
// 		d->hider.start(time);
// 	}
// }
// 
// void PlayInfoWidget::hideMessage() {
// 	d->stack->setCurrentIndex(Info);
// }
// 
