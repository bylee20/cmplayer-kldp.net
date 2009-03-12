#include "menu.h"
#include "pref.h"
#include <QtCore/QUrl>

void ActionGroup::emitData(QAction *action) {
	const QVariant data = action->data();
	emit triggered(data);
	if (data.type() == QVariant::Int)
		emit triggered(data.toInt());
	else if (data.type() == QVariant::Double)
		emit triggered(data.toDouble());
	else if (data.type() == QVariant::String)
		emit triggered(data.toString());
	else if (data.type() == QVariant::Url)
		emit triggered(data.toUrl());
}

Menu *Menu::obj = 0;
QHash<QAction*, QString> Menu::keys;

Menu &Menu::create(QWidget *parent) {
	if (obj)
		return *obj;
	Menu *root = new Menu("root", parent);

	Menu *open = root->addMenu("open");
	open->setIcon(QIcon(":/img/document-open.png"));

	QAction *file = open->addAction("file");
	file->setShortcut(Qt::CTRL + Qt::Key_F);
	file->setIcon(QIcon(":/img/video-x-generic.png"));

	QAction *dvd = open->addAction("dvd");
	dvd->setIcon(QIcon(":/img/media-optical.png"));
	dvd->setData(QUrl("dvd://"));

	open->addSeparator();

	Menu *recent = open->addMenu("recent");
	recent->setIcon(QIcon(":/img/document-open-recent.png"));
		
	recent->addAction("seperator")->setSeparator(true);
	recent->addAction("clear");
		
	Menu *screen = root->addMenu("screen");
	screen->setIcon(QIcon(":/img/screen.png"));
		
	Menu *onTop = screen->addMenu("on top");
	onTop->setIcon(QIcon(":/img/go-top.png"));
	onTop->addAction("disable", true)->setData("Disable");
	onTop->addAction("always", true)->setData("Always");
	onTop->addAction("playing", true)->setData("Playing");
		
	screen->addSeparator();
		
	Menu *size = screen->addMenu("size");
	size->setIcon(QIcon(":/img/transform-scale.png"));
	QAction *to50 = size->addAction("50%", false);
	QAction *to100 = size->addAction("100%", false);
	QAction *to200 = size->addAction("200%", false);
	QAction *to300 = size->addAction("300%", false);
	QAction *to400 = size->addAction("400%", false);
	QAction *toFull = size->addAction("full", true);
	size->g()->setExclusive(false);
	to50->setData(0.5);
	to100->setData(1.0);
	to200->setData(2.0);
	to300->setData(3.0);
	to400->setData(4.0);
	toFull->setData(-1.0);
	to50->setShortcut(QKeySequence("`"));
	to100->setShortcut(Qt::Key_1);
	to200->setShortcut(Qt::Key_2);
	to300->setShortcut(Qt::Key_3);
	to400->setShortcut(Qt::Key_4);
	QList<QKeySequence> keys;
	keys.append(Qt::Key_Enter);
	keys.append(Qt::Key_Return);
	keys.append(Qt::Key_F);
	toFull->setShortcuts(keys);

	Menu *aspect = screen->addMenu("aspect");
	aspect->setIcon(QIcon(":/img/zoom-fit-best.png"));
	aspect->addAction("auto", true)->setData(-1.0);
	aspect->addAction("4:3", true)->setData(4.0/3.0);
	aspect->addAction("16:9", true)->setData(16.0/9.0);
	aspect->addAction("1.85:1", true)->setData(1.85);
	aspect->addAction("2.35:1", true)->setData(2.35);
		
	Menu *crop = screen->addMenu("crop");
	crop->setIcon(QIcon(":/img/transform-crop-and-resize.png"));
	crop->addAction("off", true)->setData(-1.0);
	crop->addAction("4:3", true)->setData(4.0/3.0);
	crop->addAction("16:9", true)->setData(16.0/9.0);
	crop->addAction("1.85:1", true)->setData(1.85);
	crop->addAction("2.35:1", true)->setData(2.35);
	
	Menu *play = root->addMenu("play");
	play->addMenu("engine");
	play->setIcon(QIcon(":/img/player-time.png"));
		
	play->addSeparator();
		
	QAction *pause = play->addAction("pause");
	pause->setIcon(QIcon(":/img/media-playback-start.png"));
	pause->setShortcut(Qt::Key_Space);
	play->addAction("stop")->setIcon(QIcon(":/img/media-playback-stop.png"));
		
	play->addSeparator();
		
	QAction *prev = play->addAction("prev");
	QAction *next = play->addAction("next");
	QAction *list = play->addAction("list");
	prev->setIcon(QIcon(":/img/media-skip-backward.png"));
	prev->setShortcut(Qt::CTRL + Qt::Key_Left);
	next->setIcon(QIcon(":/img/media-skip-forward.png"));
	next->setShortcut(Qt::CTRL + Qt::Key_Right);
	list->setShortcut(Qt::Key_L);
		
	play->addSeparator();
		
	Menu *speed = play->addMenu("speed");
	speed->addAction("slower", false)->setShortcut(Qt::Key_Minus);
	speed->addAction("reset", false, "reset")->setShortcut(Qt::Key_Backspace);
	speed->addAction("faster", false)->setShortcut(Qt::Key_Plus);

	play->addSeparator();
		
	play->addAction("dvd menu");
		
	play->addSeparator();

	Menu *repeat = play->addMenu("repeat");
	repeat->addAction("range")->setShortcut(Qt::Key_R);
	repeat->addAction("quit")->setShortcut(Qt::Key_Escape);
	repeat->addAction("advanced");

	play->addSeparator();
		
	Menu *seek = play->addMenu("seek");
	QAction *forward1 = seek->addAction("forward1", false);
	QAction *forward2 = seek->addAction("forward2", false);
	QAction *forward3 = seek->addAction("forward3", false);
	QAction *backward1 = seek->addAction("backward1", false);
	QAction *backward2 = seek->addAction("backward2", false);
	QAction *backward3 = seek->addAction("backward3", false);
	forward1->setShortcut(Qt::Key_Right);
	forward2->setShortcut(Qt::Key_PageDown);
	forward3->setShortcut(Qt::Key_End);
	backward1->setShortcut(Qt::Key_Left);
	backward2->setShortcut(Qt::Key_PageUp);
	backward3->setShortcut(Qt::Key_Home);
	
	Menu *subtitle = root->addMenu("subtitle");
	subtitle->setIcon(QIcon(":/img/format-text-bold.png"));
		
	Menu *sList = subtitle->addMenu("list");
	sList->g()->setExclusive(false);
	sList->addAction("open");
	sList->addAction("clear");
	sList->addAction("hide")->setCheckable(true);
		
	subtitle->addSeparator();
		
	subtitle->addAction("pos up", false, "pos")->setShortcut(Qt::Key_W);
	subtitle->addAction("pos down", false, "pos")->setShortcut(Qt::Key_S);
	
	subtitle->addSeparator();
		
	subtitle->addAction("sync add", false, "sync")->setShortcut(Qt::Key_D);
	subtitle->addAction("sync sub", false, "sync")->setShortcut(Qt::Key_A);

	Menu *video = root->addMenu("video");
	video->setIcon(QIcon(":/img/games-config-background.png"));
		
	video->addMenu("renderer");
		
	video->addSeparator();
		
	video->addAction("brightness+", false, "prop")->setShortcut(Qt::Key_T);
	video->addAction("brightness-", false, "prop")->setShortcut(Qt::Key_G);
	video->addAction("contrast+", false, "prop")->setShortcut(Qt::Key_Y);
	video->addAction("contrast-", false, "prop")->setShortcut(Qt::Key_H);
	video->addAction("saturation+", false, "prop")->setShortcut(Qt::Key_U);
	video->addAction("saturation-", false, "prop")->setShortcut(Qt::Key_J);
	video->addAction("hue+", false, "prop")->setShortcut(Qt::Key_I);
	video->addAction("hue-", false, "prop")->setShortcut(Qt::Key_K);

	Menu *audio = root->addMenu("audio");
	audio->setIcon(QIcon(":/img/speaker.png"));
		
	audio->addMenu("renderer");
		
	audio->addSeparator();
		
	audio->addMenu("track");
		
	audio->addSeparator();
		
	audio->addAction("volume up", false, "volume")->setShortcut(Qt::Key_Up);
	audio->addAction("volume down", false, "volume")->setShortcut(Qt::Key_Down);
	QAction *mute = audio->addAction("mute");
	mute->setIcon(QIcon(":/img/player-volume.png"));
	mute->setShortcut(Qt::Key_M);
	mute->setData(0);
	mute->setCheckable(true);
		
	audio->addSeparator();
		
	audio->addAction("amp up", false, "amp")->setShortcut(Qt::CTRL + Qt::Key_Up);
	audio->addAction("amp down", false, "amp")->setShortcut(Qt::CTRL + Qt::Key_Down);
		
	root->addAction("pref")->setIcon(QIcon(":/img/preferences-system.png"));
// 	root->addAction("help")->setIcon(QIcon(":/img/help-contents.png"));
// 	root->addAction("about")->setIcon(QIcon(":/img/help-about.png"));
	
	QAction *exit = root->addAction("exit");
	exit->setShortcut(Qt::CTRL + Qt::Key_Q);
	exit->setIcon(QIcon(":/img/application-exit.png"));
	
	return *(obj = root);
}

template<typename N>
inline static void setActionAttr(QAction *act, const QVariant &data
		, const QString &text, N textValue, bool sign = true) {
	act->setData(data);
	act->setText(text.arg(Menu::toString(textValue, sign)));
}

inline static void setActionStep(QAction *plus, QAction *minus
		, const QString &text, int value, double textRate = -1.0) {
	if (textRate < 0) {
		plus->setText(text.arg(Menu::toString(value)));
		minus->setText(text.arg(Menu::toString(-value)));
	} else {
		plus->setText(text.arg(Menu::toString(value*textRate)));
		minus->setText(text.arg(Menu::toString(-value*textRate)));
	}
	plus->setData(value);
	minus->setData(-value);
}

inline static void setVideoPropStep(Menu &menu, const QString &key
		, Core::VideoProperty prop, const QString &text, int step) {
	setActionAttr(menu[key + "+"], QList<QVariant>() << prop << step, text, step);
	setActionAttr(menu[key + "-"], QList<QVariant>() << prop << -step, text, -step);		
}

void Menu::updatePref() {
	if (!obj)
		return;
	Menu &root = *obj;
	Pref *p = Pref::get();

	Menu &open = root("open");
	open.setTitle(tr("Open"));
	open["file"]->setText(tr("File"));
	open["dvd"]->setText(tr("DVD"));

	Menu &recent = open("recent");
	recent.setTitle(tr("Recent Played"));
	recent["clear"]->setText(tr("Clear"));
	
	Menu &screen = root("screen");
	screen.setTitle(tr("Screen"));
	Menu &onTop = screen("on top");
	onTop.setTitle(tr("Stays On Top"));
	onTop["disable"]->setText(tr("Disable"));
	onTop["always"]->setText(tr("Always"));
	onTop["playing"]->setText(tr("Only Playing"));
		
	Menu &size = screen("size");
	size.setTitle(tr("Size"));
	size["full"]->setText(tr("Full Screen"));

	Menu &aspect = screen("aspect");
	aspect.setTitle(tr("Aspect Ratio"));
	aspect["auto"]->setText(tr("Auto"));
	aspect["4:3"]->setText(tr("4:3 (TV)"));
	aspect["16:9"]->setText(tr("16:9 (HDTV)"));
	aspect["1.85:1"]->setText(tr("1.85:1 (Wide Vision)"));
	aspect["2.35:1"]->setText(tr("2.35:1 (CinemaScope)"));
		
	Menu &crop = screen("crop");
	crop.setTitle(tr("Crop"));
	crop["off"]->setText(tr("Off"));
	crop["4:3"]->setText(tr("4:3 (TV)"));
	crop["16:9"]->setText(tr("16:9 (HDTV)"));
	crop["1.85:1"]->setText(tr("1.85:1 (Wide Vision)"));
	crop["2.35:1"]->setText(tr("2.35:1 (CinemaScope)"));
	
	Menu &play = root("play");
	play.setTitle(tr("Play"));
	play("engine").setTitle(tr("Play Engine"));
	play["pause"]->setText(tr("Play"));
	play["stop"]->setText(tr("Stop"));
		
	play["prev"]->setText(tr("Previous"));
	play["next"]->setText(tr("Next"));
	play["list"]->setText(tr("Toggle Play List"));
		
	Menu &speed = play("speed");
	speed.setTitle(tr("Speed"));
	speed["reset"]->setText(tr("Reset"));
	setActionStep(speed["faster"], speed["slower"], "%1%", p->speedStep());

	play["dvd menu"]->setText(tr("Toggle DVD Menu"));

	Menu &repeat = play("repeat");
	repeat.setTitle(tr("A-B Repeat"));
	repeat["range"]->setText(tr("Set Range"));
	repeat["quit"]->setText(tr("Quit"));
	repeat["advanced"]->setText(tr("Advanced..."));

	Menu &seek = play("seek");
	seek.setTitle(tr("Seek"));
	const QString forward = tr("Forward %1sec.");
	setActionAttr(seek["forward1"], p->seekingStep1(), forward, p->seekingStep1()*0.001, false);
	setActionAttr(seek["forward2"], p->seekingStep2(), forward, p->seekingStep2()*0.001, false);
	setActionAttr(seek["forward3"], p->seekingStep3(), forward, p->seekingStep3()*0.001, false);
	const QString backward = tr("Backward %1sec.");
	setActionAttr(seek["backward1"], -p->seekingStep1(), backward, p->seekingStep1()*0.001, false);
	setActionAttr(seek["backward2"], -p->seekingStep2(), backward, p->seekingStep2()*0.001, false);
	setActionAttr(seek["backward3"], -p->seekingStep3(), backward, p->seekingStep3()*0.001, false);
	
	Menu &sub = root("subtitle");
	sub.setTitle(tr("Subtitle"));
	Menu &list = sub("list");
	list.setTitle(tr("List"));
	list["open"]->setText(tr("Open"));
	list["clear"]->setText(tr("Clear"));
	list["hide"]->setText(tr("Hide"));
	setActionAttr(sub["pos up"], -p->subtitlePosStep(), tr("Up %1%"), p->subtitlePosStep(), false);
	setActionAttr(sub["pos down"], p->subtitlePosStep(), tr("Down %1%"), p->subtitlePosStep(), false);
	setActionStep(sub["sync add"], sub["sync sub"], tr("Sync %1sec."), p->syncDelayStep(), 0.001);
	
	Menu &video = root("video");
	video.setTitle(tr("Video"));
	video("renderer").setTitle(tr("Renderer"));
	setVideoPropStep(video, "brightness", Core::Brightness, tr("Brightness %1%"), p->brightnessStep());
	setVideoPropStep(video, "saturation", Core::Saturation, tr("Saturation %1%"), p->brightnessStep());
	setVideoPropStep(video, "contrast", Core::Contrast, tr("Contrast %1%"), p->brightnessStep());
	setVideoPropStep(video, "hue", Core::Hue, tr("Hue %1%"), p->brightnessStep());

	Menu &audio = root("audio");
	audio.setTitle(tr("Audio"));
	audio("renderer").setTitle(tr("Renderer"));
	audio("track").setTitle(tr("Track"));
	audio["mute"]->setText(tr("Toggle Mute"));
	setActionStep(audio["volume up"], audio["volume down"], tr("Volume %1%"), p->volumeStep());
	setActionStep(audio["amp up"], audio["amp down"], tr("Amp. %1%"), p->ampStep());

	root["pref"]->setText(tr("Preferences"));
// 	root["help"]->setText(tr("Help"));
// 	root["about"]->setText(tr("About..."));
	root["exit"]->setText(tr("Exit"));
}
