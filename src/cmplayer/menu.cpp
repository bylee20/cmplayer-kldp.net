#include "menu.h"
#include "pref.h"
#include <core/info.h>
#include <core/colorproperty.h>
#include <QtCore/QUrl>
#include <QtCore/QSettings>

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
QHash<QString, QAction*> Menu::acts;

Menu &Menu::create(QWidget *parent) {
	if (obj)
		return *obj;
	Menu *root = new Menu("root", parent);

	Menu *open = root->addMenu("open");
	open->setIcon(QIcon(":/img/document-open.png"));

	QAction *file = open->addAction("file");
	file->setShortcut(Qt::CTRL + Qt::Key_F);
	file->setIcon(QIcon(":/img/video-x-generic.png"));
	file->setData(int('f'));
	QAction *url = open->addAction("url");
	url->setIcon(QIcon(":/img/network-workgroup.png"));
	url->setData(int('u'));
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
	onTop->addActionToGroup("disable", true)->setData("Disable");
	onTop->addActionToGroup("always", true)->setData("Always");
	onTop->addActionToGroup("playing", true)->setData("Playing");
		
	screen->addSeparator();
		
	Menu *size = screen->addMenu("size");
	size->setIcon(QIcon(":/img/transform-scale.png"));
	QAction *to25 = size->addActionToGroup("25%", false);
	QAction *to50 = size->addActionToGroup("50%", false);
	QAction *to100 = size->addActionToGroup("100%", false);
	QAction *to200 = size->addActionToGroup("200%", false);
	QAction *to300 = size->addActionToGroup("300%", false);
	QAction *to400 = size->addActionToGroup("400%", false);
	QAction *toFull = size->addActionToGroup("full", true);
	size->g()->setExclusive(false);
	to25->setData(0.25);
	to50->setData(0.5);
	to100->setData(1.0);
	to200->setData(2.0);
	to300->setData(3.0);
	to400->setData(4.0);
	toFull->setData(-1.0);
	to25->setShortcut(QKeySequence("Shift+`"));
	to50->setShortcut(QKeySequence("`"));
	to100->setShortcut(Qt::Key_1);
	to200->setShortcut(Qt::Key_2);
	to300->setShortcut(Qt::Key_3);
	to400->setShortcut(Qt::Key_4);
	toFull->setShortcuts(QList<QKeySequence>()
			<< Qt::Key_Enter << Qt::Key_Return << Qt::Key_F);

	Menu *aspect = screen->addMenu("aspect");
	aspect->setIcon(QIcon(":/img/zoom-fit-best.png"));
	aspect->addActionToGroup("auto", true)->setData(-1.0);
	aspect->addActionToGroup("4:3", true)->setData(4.0/3.0);
	aspect->addActionToGroup("16:9", true)->setData(16.0/9.0);
	aspect->addActionToGroup("1.85:1", true)->setData(1.85);
	aspect->addActionToGroup("2.35:1", true)->setData(2.35);
		
	Menu *crop = screen->addMenu("crop");
	crop->setIcon(QIcon(":/img/transform-crop-and-resize.png"));
	crop->addActionToGroup("off", true)->setData(-1.0);
	crop->addActionToGroup("4:3", true)->setData(4.0/3.0);
	crop->addActionToGroup("16:9", true)->setData(16.0/9.0);
	crop->addActionToGroup("1.85:1", true)->setData(1.85);
	crop->addActionToGroup("2.35:1", true)->setData(2.35);
	
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
	list->setIcon(QIcon(":/img/view-media-playlist.png"));
	list->setShortcut(Qt::Key_L);
		
	play->addSeparator();
		
	Menu *speed = play->addMenu("speed");
	speed->addActionToGroup("slower", false)->setShortcut(Qt::Key_Minus);
	QAction *reset = speed->addActionToGroup("reset", false);
	reset->setShortcut(Qt::Key_Backspace);
	reset->setData(0);
	QAction *faster = speed->addActionToGroup("faster", false);
	faster->setShortcuts(QList<QKeySequence>() << Qt::Key_Plus << Qt::Key_Equal);

	play->addSeparator();
		
	play->addAction("dvd menu");
		
	play->addSeparator();

	Menu *repeat = play->addMenu("repeat");
	QAction *range = repeat->addActionToGroup("range", false);
	QAction *srange = repeat->addActionToGroup("subtitle", false);
	QAction *quitRepeat = repeat->addActionToGroup("quit", false);
	QAction *advanced = repeat->addActionToGroup("advanced", false);
	range->setShortcut(Qt::Key_R);	
	range->setData(int('r'));
	srange->setShortcut(Qt::Key_E);
	srange->setData(int('s'));
	quitRepeat->setShortcut(Qt::Key_Escape);
	quitRepeat->setData(int('q'));
	advanced->setData(int('a'));
	
	play->addSeparator();
		
	Menu *seek = play->addMenu("seek");
	QAction *forward1 = seek->addActionToGroup("forward1", false);
	QAction *forward2 = seek->addActionToGroup("forward2", false);
	QAction *forward3 = seek->addActionToGroup("forward3", false);
	QAction *backward1 = seek->addActionToGroup("backward1", false);
	QAction *backward2 = seek->addActionToGroup("backward2", false);
	QAction *backward3 = seek->addActionToGroup("backward3", false);
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
		
	subtitle->addActionToGroup("pos up", false, "pos")->setShortcut(Qt::Key_W);
	subtitle->addActionToGroup("pos down", false, "pos")->setShortcut(Qt::Key_S);
	
	subtitle->addSeparator();
		
	subtitle->addActionToGroup("sync add", false, "sync")->setShortcut(Qt::Key_D);
	QAction *syncReset = subtitle->addActionToGroup("sync reset", false, "sync");
	syncReset->setShortcut(Qt::Key_Q);
	syncReset->setData(0);
	subtitle->addActionToGroup("sync sub", false, "sync")->setShortcut(Qt::Key_A);

	Menu *video = root->addMenu("video");
	video->setIcon(QIcon(":/img/games-config-background.png"));
		
	video->addMenu("renderer");
		
	video->addSeparator();
		
	video->addActionToGroup("brightness+", false, "color")->setShortcut(Qt::Key_T);
	video->addActionToGroup("brightness-", false, "color")->setShortcut(Qt::Key_G);
	video->addActionToGroup("contrast+", false, "color")->setShortcut(Qt::Key_Y);
	video->addActionToGroup("contrast-", false, "color")->setShortcut(Qt::Key_H);
	video->addActionToGroup("saturation+", false, "color")->setShortcut(Qt::Key_U);
	video->addActionToGroup("saturation-", false, "color")->setShortcut(Qt::Key_J);
	video->addActionToGroup("hue+", false, "color")->setShortcut(Qt::Key_I);
	video->addActionToGroup("hue-", false, "color")->setShortcut(Qt::Key_K);

	Menu *audio = root->addMenu("audio");
	audio->setIcon(QIcon(":/img/speaker.png"));
		
	audio->addMenu("renderer");
		
	audio->addSeparator();
		
	audio->addMenu("track");
		
	audio->addSeparator();
		
	QAction *volUp = audio->addActionToGroup("volume up", false, "volume");
	volUp->setShortcut(Qt::Key_Up);
	QAction *volDown = audio->addActionToGroup("volume down", false, "volume");
	volDown->setShortcut(Qt::Key_Down);
	QAction *mute = audio->addAction("mute", true);
	mute->setIcon(QIcon(":/img/player-volume.png"));
	mute->setShortcut(Qt::Key_M);
		
	audio->addSeparator();
	
	QAction *ampUp = audio->addActionToGroup("amp up", false, "amp");
	QAction *ampDown = audio->addActionToGroup("amp down", false, "amp");
	ampUp->setShortcut(Qt::CTRL + Qt::Key_Up);
	ampDown->setShortcut(Qt::CTRL + Qt::Key_Down);
		
	QAction *pref = root->addAction("pref");
	pref->setIcon(QIcon(":/img/preferences-system.png"));
// 	root->addAction("help")->setIcon(QIcon(":/img/help-contents.png"));
	QAction *about = root->addAction("about");
	about->setIcon(QIcon(":/img/help-about.png"));
	
	QAction *exit = root->addAction("exit");
	exit->setShortcut(Qt::CTRL + Qt::Key_Q);
	exit->setIcon(QIcon(":/img/application-exit.png"));
	

	root->m_click[OpenFile] = file;
	root->m_click[ToggleFullScreen] = toFull;
	root->m_click[TogglePlayPause] = pause;
	root->m_click[ToggleMute] = mute;
	root->m_click[TogglePlaylist] = list;
	root->m_wheel[Seek1] = WheelActionPair(forward1, backward2);
	root->m_wheel[Seek2] = WheelActionPair(forward2, backward2);
	root->m_wheel[Seek3] = WheelActionPair(forward3, backward2);
	root->m_wheel[NextPrevious] = WheelActionPair(next, prev);
	root->m_wheel[VolumeUpDown] = WheelActionPair(volUp, volDown);
	root->m_wheel[AmpUpDown] = WheelActionPair(ampUp, ampDown);

	root->m_context = new QMenu(parent);
	root->m_context->addAction(file);
	root->m_context->addMenu(open);
	root->m_context->addAction(list);
	root->m_context->addSeparator();
	root->m_context->addMenu(screen);
	root->m_context->addMenu(play);
	root->m_context->addMenu(subtitle);
	root->m_context->addMenu(video);
	root->m_context->addMenu(audio);
	root->m_context->addSeparator();
	root->m_context->addAction(pref);
// 	root->m_context->addAction(help);
	root->m_context->addAction(about);
	root->m_context->addSeparator();
	root->m_context->addAction(exit);
	parent->addActions(root->m_context->actions());

	loadShortcut();
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
		, Core::ColorProperty::Value prop, const QString &text, int step) {
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
	open["url"]->setText(tr("URL"));
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
	repeat["range"]->setText(tr("Set Range to Current Time"));
	repeat["subtitle"]->setText(tr("Repeat Current Subtitle"));
	repeat["quit"]->setText(tr("Quit"));
	repeat["advanced"]->setText(tr("Advanced..."));

	Menu &seek = play("seek");
	seek.setTitle(tr("Seek"));
	const QString forward = tr("Forward %1sec.");
	setActionAttr(seek["forward1"], p->seekingStep1()
			, forward, p->seekingStep1()*0.001, false);
	setActionAttr(seek["forward2"], p->seekingStep2()
			, forward, p->seekingStep2()*0.001, false);
	setActionAttr(seek["forward3"], p->seekingStep3()
			, forward, p->seekingStep3()*0.001, false);
	const QString backward = tr("Backward %1sec.");
	setActionAttr(seek["backward1"], -p->seekingStep1()
			, backward, p->seekingStep1()*0.001, false);
	setActionAttr(seek["backward2"], -p->seekingStep2()
			, backward, p->seekingStep2()*0.001, false);
	setActionAttr(seek["backward3"], -p->seekingStep3()
			, backward, p->seekingStep3()*0.001, false);
	
	Menu &sub = root("subtitle");
	sub.setTitle(tr("Subtitle"));
	Menu &list = sub("list");
	list.setTitle(tr("List"));
	list["open"]->setText(tr("Open"));
	list["clear"]->setText(tr("Clear"));
	list["hide"]->setText(tr("Hide"));
	setActionAttr(sub["pos up"], -p->subtitlePosStep()
			, tr("Up %1%"), p->subtitlePosStep(), false);
	setActionAttr(sub["pos down"], p->subtitlePosStep()
			, tr("Down %1%"), p->subtitlePosStep(), false);
	sub["sync reset"]->setText(tr("Reset Sync"));
	setActionStep(sub["sync add"], sub["sync sub"]
			, tr("Sync %1sec."), p->syncDelayStep(), 0.001);
	
	Menu &video = root("video");
	video.setTitle(tr("Video"));
	video("renderer").setTitle(tr("Renderer"));
	setVideoPropStep(video, "brightness", Core::ColorProperty::Brightness
			, tr("Brightness %1%"), p->brightnessStep());
	setVideoPropStep(video, "saturation", Core::ColorProperty::Saturation
			, tr("Saturation %1%"), p->brightnessStep());
	setVideoPropStep(video, "contrast", Core::ColorProperty::Contrast
			, tr("Contrast %1%"), p->brightnessStep());
	setVideoPropStep(video, "hue", Core::ColorProperty::Hue
			, tr("Hue %1%"), p->brightnessStep());

	Menu &audio = root("audio");
	audio.setTitle(tr("Audio"));
	audio("renderer").setTitle(tr("Renderer"));
	audio("track").setTitle(tr("Track"));
	audio["mute"]->setText(tr("Toggle Mute"));
	setActionStep(audio["volume up"], audio["volume down"]
			, tr("Volume %1%"), p->volumeStep());
	setActionStep(audio["amp up"], audio["amp down"]
			, tr("Amp. %1%"), p->ampStep());

	root["pref"]->setText(tr("Preferences"));
// 	root["help"]->setText(tr("Help"));
	root["about"]->setText(tr("About..."));
	root["exit"]->setText(tr("Exit"));
	
	saveShortcut();
}

QString Menu::configFile() {
	return Core::Info::privatePath() + "/menu.ini";
}

void Menu::saveShortcut() {
	QSettings set(configFile(), QSettings::IniFormat);
	set.beginGroup("Menu");
	QHash<QAction*, QString>::iterator it = keys.begin();
	for (; it != keys.end(); ++it) {
		const QList<QKeySequence> shortcut = it.key()->shortcuts();
		set.beginWriteArray(it.value(), shortcut.size());
		for (int i=0; i<shortcut.size(); ++i) {
			set.setArrayIndex(i);
			set.setValue("Shortcut", shortcut[i]);
		}
		set.endArray();
	}
	set.endGroup();
}

void Menu::loadShortcut() {
	QSettings set(configFile(), QSettings::IniFormat);
	set.beginGroup("Menu");
	QHash<QAction*, QString>::iterator it = keys.begin();
	for (; it != keys.end(); ++it) {
		const int count = set.beginReadArray(it.value());
		QList<QKeySequence> shortcut = it.key()->shortcuts();
		while (shortcut.size() < count)
			shortcut.push_back(QKeySequence());
		for (int i=0; i<count; ++i) {
			set.setArrayIndex(i);
			shortcut[i] = set.value("Shortcut", shortcut[i]).value<QKeySequence>();
		}
		set.endArray();
		it.key()->setShortcuts(shortcut);
	}
	set.endGroup();
}
