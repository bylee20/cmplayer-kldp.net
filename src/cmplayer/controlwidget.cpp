#include "controlwidget.h"
#include "sliders.h"
#include "videoplayer.h"
#include <QtGui/QAction>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPainter>
#include <QtGui/QLinearGradient>
#include <QtGui/QPainterPath>
#include <QtGui/QToolButton>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtCore/QDebug>
#include <QtGui/QStackedWidget>
#include <QtCore/QTimer>
#include <core/utility.h>
#include "squeezedlabel.h"

class ControlWidget::Top : public QWidget {
public:
	static const int Info = 0;
	static const int Message = 1;
	SqueezedLabel *source;
	QStackedWidget *stack;
	QLabel *state, *track, *time, *duration, *message;
	int prevSecs;
	QTimer hider;
	int height;
	Top(QWidget *parent = 0)
	: QWidget(parent) {
		QFrame *panel = new QFrame(this);
		panel->setObjectName("panel");
		{
			prevSecs = 0;
			QHBoxLayout *hbox = new QHBoxLayout(panel);
			stack = new QStackedWidget(panel);
			time = new QLabel("00:00:00", panel);
			QLabel *slash = new QLabel("/", panel);
			duration = new QLabel("00:00:00", panel);
			hbox->addWidget(stack);
			hbox->addWidget(time);
			hbox->addWidget(slash);
			hbox->addWidget(duration);

			hbox->setContentsMargins(0, 0, 0, 0);
			hbox->setSpacing(0);

			QWidget *w = new QWidget(stack);
			hbox = new QHBoxLayout(w);
			stack->addWidget(w);
			track = new QLabel(w);
			state = new QLabel(w);
			source = new SqueezedLabel(w);
			hbox->addWidget(track);
			hbox->addWidget(state);
			hbox->addWidget(source);
			stack->addWidget(w);
	
			hbox->setContentsMargins(0, 0, 0, 0);
			hbox->setSpacing(0);

			message = new QLabel(panel);
			stack->addWidget(message);
	
			stack->setCurrentIndex(Info);
			hider.setSingleShot(true);
			
			setStyleSheet("\
				QFrame#panel {\
					margin: 0px;\
					padding: 2px;\
					border: 1px solid #aaa;\
					background: qlineargradient(x1:0, y1:0, x2:0, y2:1\
					, stop:0 #111, stop:0.1 #6ad, stop:0.8 #6ad, stop:1 #fff);\
					border-radius: 3px;\
				}"
			);
		}
		height = panel->sizeHint().height();
		openFile = new QPushButton("OPEN FILE", this);
		openFile->setToolTip(tr("Open File"));
		openUrl = new QPushButton("OPEN URL", this);
		openUrl->setToolTip(tr("Open Url"));
		fullScreen = new QPushButton("FULL SCREEN", this);
		fullScreen->setToolTip(tr("Toggle Full Screen Mode"));
		fullScreen->setCheckable(true);
		playlist = new QPushButton("PLAYLIST", this);
		playlist->setToolTip(tr("Toogle Playlis Visibility"));
		playlist->setCheckable(true);
		
		QHBoxLayout *hbox = new QHBoxLayout(this);
		hbox->setSpacing(0);
		hbox->setContentsMargins(0, 0, 0, 0);
		hbox->addItem(new QSpacerItem(39, 0, QSizePolicy::Fixed, QSizePolicy::Fixed));
		QVBoxLayout *vbox = new QVBoxLayout;
		vbox->addWidget(openFile);
		vbox->addWidget(openUrl);
		hbox->addLayout(vbox);
		hbox->addWidget(panel);
		vbox = new QVBoxLayout;
		vbox->addWidget(playlist);
		vbox->addWidget(fullScreen);
		hbox->addLayout(vbox);
		hbox->addItem(new QSpacerItem(39, 0, QSizePolicy::Fixed, QSizePolicy::Fixed));
		
		speaker = QBrush(qRgb(100, 100, 100), Qt::Dense4Pattern);
	}
	QPushButton *openFile, *openUrl, *playlist, *fullScreen;
private:
	QBrush speaker;
	void paintEvent(QPaintEvent */*event*/) {
		QPainter painter(this);
		QRectF r = rect();
		r.setWidth(35);
		r.moveLeft(2);
		painter.fillRect(r, speaker);
		r.moveRight(width()-2);
		painter.fillRect(r, speaker);
	}
};

class ControlWidget::Middle : public QWidget {
public:
	Middle(VideoPlayer *player, QWidget *parent = 0)
	: QWidget(parent), mute(createButton(this, 10)) {
		QHBoxLayout *hbox = new QHBoxLayout(this);
		hbox->setSpacing(0);
		hbox->setContentsMargins(0, 0, 0, 0);
		setFixedHeight(15);
		setStyleSheet("\
JumpSlider::groove:horizontal {\
	border: 1px solid #6ad;\
	height: 3px;\
	background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #fff, stop:1 #ccc);\
	margin: 0px 2px;\
	padding: 0px;\
}\
JumpSlider::handle:horizontal {\
	background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #aaa, stop:1 #999);\
	border: 1px solid #5c5c5c;\
	width: 5px;\
	margin: -2px 0px;\
	padding: 1px;\
	border-radius: 2px;\
}\
JumpSlider::handle:horizontal:hover {\
	background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #fff, stop:1 #ccc);\
	border: 1px solid #6ad;\
	padding: 1px;\
}\
JumpSlider::handle:horizontal:pressed {\
	background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #fff, stop:1 #ccc);\
	border: 2px solid #6ad;\
	padding: 0px;\
}\
JumpSlider::add-page:horizontal {\
	border: 1px solid #999;\
	height: 3px;\
	background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #333, stop:1 #bbb);\
	margin: 0px 2px;\
	padding: 0px;\
}\
QToolButton {\
	width: 10px;\
	height: 10px;\
}");
		hbox->addWidget(new SeekSlider(player, this));
		hbox->addWidget(mute);
		hbox->addWidget(new VolumeSlider(player, this));
	}
	QToolButton *mute;
};

class ControlWidget::Bottom : public QWidget {
public:
	Bottom(QWidget *parent = 0)
	: QWidget(parent), play(createButton(this)), stop(createButton(this))
	, prev(createButton(this)), next(createButton(this))
	, forward(createButton(this)), backward(createButton(this)) {
		QLinearGradient grad(0.5, 0.0, 0.5, 1.0);
		grad.setColorAt(1.0, qRgb(100, 100, 100));
		grad.setColorAt(0.0, qRgb(0, 0, 0));
		bg = QBrush(grad);
		
		grad = QLinearGradient(0.5, 0.1, 0.5, 0.9);
		grad.setColorAt(1.0, Qt::white);
		grad.setColorAt(0.0, Qt::transparent);
		light = QBrush(grad);

		path.moveTo(0.0, 1.0);
		path.cubicTo(0.05, 0.3, 0.05, 0.3, 0.5, 0.3);
		path.cubicTo(0.95, 0.3, 0.95, 0.3, 1.0, 1.0);
		path.closeSubpath();
		
		QHBoxLayout *hbox = new QHBoxLayout(this);
		hbox->setSpacing(0);
		hbox->setContentsMargins(0, 0, 0, 2);
		hbox->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed));
		hbox->addWidget(prev);
		hbox->addWidget(backward);
		hbox->addWidget(play);
		hbox->addWidget(stop);
		hbox->addWidget(forward);
		hbox->addWidget(next);
		hbox->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed));
	}
	QToolButton *play, *stop, *prev, *next, *forward, *backward;
private:
	void paintEvent(QPaintEvent */*event*/) {
		QPainter painter(this);
		painter.save();
		painter.setRenderHint(QPainter::Antialiasing);
		painter.scale(width(), height());
		painter.setPen(Qt::NoPen);
		painter.fillRect(rect(), bg);
		painter.setOpacity(0.2);
		painter.setBrush(light);
		painter.drawPath(path);
		painter.restore();
	}
	QBrush bg, light;
	QPainterPath path;
};

struct ControlWidget::Data {
	VideoPlayer *player;
	Top *top;
	Middle *middle;
	Bottom *bottom;
	QAction *playlist, *fullScreen;
};

ControlWidget::ControlWidget(VideoPlayer *player, QWidget *parent)
: QWidget(parent), d(new Data) {
	d->player = player;
	d->fullScreen = d->playlist = 0;
	d->top = new Top(this);
	d->middle = new Middle(d->player, this);
	d->bottom = new Bottom(this);
	int bh = d->top->height/2-2;
	QRect r;
	int bf = bh;
	QFont f(font());
	int i=0;
	do {
		bf = bh * (1.0 - i*0.1) + 0.5;
		f.setPixelSize(bf);
		QFontMetrics fm(f);
		r = fm.boundingRect("FULL SCREEN");
		++i;
	} while (r.height() > bh+2);
	setStyleSheet(QString("\
QToolButton {\
	border: none;\
	margin:0px;\
	padding: 2px;\
}\
QToolButton:hover {\
	border: 1px solid #6ad;\
	border-radius: 3px;\
	padding: 1px;\
}\
QToolButton:pressed {\
	border: 2px solid #6ad;\
	border-radius: 3px;\
	padding: 0px;\
}\
QPushButton {\
	border: 1px solid #999;\
	background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #fff, stop:1 #ccc);\
	margin: 0px 2px;\
	padding: 1px;\
	width: %1px;\
	height: %2px;\
	font-size: %3px\
}\
QPushButton:checked {\
	border: 1px solid #999;\
	background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #777, stop:1 #bbb);\
}\
QPushButton:hover {\
	border: 1px solid #6ad;\
	background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #fff, stop:1 #ccc);\
}\
QPushButton:pressed {\
	border: 2px solid #6ad;\
	background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #777, stop:1 #bbb);\
	padding: 0px;\
}\
QPushButton:checked:hover {\
	border: 1px solid #6ad;\
	background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #777, stop:1 #bbb);\
}\
QPushButton:checked:pressed {\
	border: 2px solid #6ad;\
	background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #777, stop:1 #bbb);\
	padding: 0px;\
}").arg(r.width() + 5).arg(bh).arg(bf));

	QWidget *line = new QWidget(this);
	line->setFixedHeight(5);
	line->setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #000, stop:0.1 #000, stop:0.5 #888, stop:0.6 #000, stop:1 #000);");

	QVBoxLayout *vbox = new QVBoxLayout(this);
	vbox->addWidget(line);
	vbox->addWidget(d->top);
	vbox->addWidget(d->middle);
	vbox->addWidget(d->bottom);
	vbox->setSpacing(0);
	vbox->setContentsMargins(0, 0, 0, 0);
// 	setFixedHeight(50);
	setState(Core::Stopped);
	connect(&d->top->hider, SIGNAL(timeout()), this, SLOT(hideMessage()));
	connect(d->player, SIGNAL(stateChanged(Core::State, Core::State))
			, this, SLOT(setState(Core::State)));
	connect(d->player, SIGNAL(currentSourceChanged(const Core::MediaSource&))
			, this, SLOT(setCurrentSource(const Core::MediaSource&)));
	connect(d->player, SIGNAL(durationChanged(int)), this, SLOT(setDuration(int)));
	connect(d->player, SIGNAL(tick(int)), this, SLOT(setPlayTime(int)));
}

ControlWidget::~ControlWidget() {
	delete d;
}

QToolButton *ControlWidget::createButton(QWidget *parent, int size) {
	QToolButton *button = new QToolButton(parent);
	button->setIconSize(QSize(size, size));
	button->setAutoRaise(true);
	button->setFocusPolicy(Qt::NoFocus);
	return button;
}

void ControlWidget::paintEvent(QPaintEvent */*event*/) {
	QPainter painter(this);
	painter.fillRect(rect(), Qt::black);
}

void ControlWidget::connectMute(QAction *action) {
	d->middle->mute->setDefaultAction(action);
}

void ControlWidget::connectPlay(QAction *action) {
	d->bottom->play->setDefaultAction(action);
}

void ControlWidget::connectStop(QAction *action) {
	d->bottom->stop->setDefaultAction(action);
}

void ControlWidget::connectPrevious(QAction *action) {
	d->bottom->prev->setDefaultAction(action);
}

void ControlWidget::connectNext(QAction *action) {
	d->bottom->next->setDefaultAction(action);
}

void ControlWidget::connectForward(QAction *action) {
	d->bottom->forward->setDefaultAction(action);
}

void ControlWidget::connectBackward(QAction *action) {
	d->bottom->backward->setDefaultAction(action);
}

void ControlWidget::showMessage(const QString &msg, int time) {
	d->top->hider.stop();
	if (time > 0) {
		d->top->message->setText(msg);
		d->top->stack->setCurrentIndex(Top::Message);
		d->top->hider.start(time);
	}
}

void ControlWidget::setCurrentSource(const Core::MediaSource &source) {
	d->top->source->setText(source.displayName());
	hideMessage();
}

void ControlWidget::setState(Core::State state) {
	QString text;
	if (state == Core::Stopped)
		text += tr("Stopped");
	else if (state == Core::Finished)
		text += tr("Finished");
	else if (state == Core::Playing)
		text += tr("Playing");
	else
		text += tr("Paused");
	d->top->state->setText("(" + text + ") ");
	hideMessage();
}

void ControlWidget::setDuration(int duration) {
	d->top->duration->setText(Core::Utility::msecsToString(duration));
}

void ControlWidget::setPlayTime(int time) {
	const int secs = time/1000;
	if (d->top->prevSecs != secs) {
		d->top->time->setText(Core::Utility::secsToString(secs));
		d->top->prevSecs = secs;
	}
}

void ControlWidget::setTrackNumber(int nth, int total) {
	if (nth < 1 || total < 2) 
		d->top->track->clear();
	else
		d->top->track->setText(QString("%1/%2 ").arg(nth).arg(total));
	hideMessage();
}

void ControlWidget::hideMessage() {
	d->top->stack->setCurrentIndex(Top::Info);
}

void ControlWidget::togglePlaylist(bool visible) {
	if (d->playlist && d->playlist->isChecked() != visible)
		d->playlist->trigger();
}

void ControlWidget::toggleFullScreen(bool full) {
	if (d->fullScreen && d->fullScreen->isChecked() != full)
		d->fullScreen->trigger();
}

void ControlWidget::connectOpenFile(QAction *action) {
	connect(d->top->openFile, SIGNAL(clicked()), action, SLOT(trigger()));
}

void ControlWidget::connectOpenUrl(QAction *action) {
	connect(d->top->openUrl, SIGNAL(clicked()), action, SLOT(trigger()));
}


void ControlWidget::connectPlaylist(QAction *action) {
	d->playlist = action;
	connect(d->top->playlist, SIGNAL(toggled(bool)), this, SLOT(togglePlaylist(bool)));
	connect(action, SIGNAL(toggled(bool)), d->top->playlist, SLOT(setChecked(bool)));
}

void ControlWidget::connectFullScreen(QAction *action) {
	d->fullScreen = action;
	connect(d->top->fullScreen, SIGNAL(toggled(bool)), this, SLOT(toggleFullScreen(bool)));
	connect(action, SIGNAL(toggled(bool)), d->top->fullScreen, SLOT(setChecked(bool)));
}

