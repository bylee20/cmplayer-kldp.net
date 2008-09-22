#include "subtitleoutput.h"
#include "mediainfo.h"
#include "playengine.h"
#include "info.h"
#include <QSet>
#include <QDir>
#include <backend/subtitleparsers.h>
#include <QDebug>

namespace Backend {

namespace MPlayer {

struct SubtitleOutput::Data {
	Data(SubtitleOutput *parent)
	: p(parent), engine(0), tempsub(Info::privatePath() + "/temp.smi")
	, posScale(1.0), loaded(false), onMargin(true), current(QString()) {}
	bool isDisc() {return engine && engine->currentSource().isDisc();}
	SubtitleOutput *p;
	PlayEngine *engine;
	QString tempsub;
	double posScale;
	bool loaded, onMargin;
	QList<Subtitle> subtitles;
	Subtitle current;
	QList<int> selected;
	QStringList priority;
};

SubtitleOutput::SubtitleOutput(PlayEngine *engine)
: Backend::SubtitleOutput(engine), d(new Data(this)) {
	d->engine = engine;
}

SubtitleOutput::~SubtitleOutput() {
	delete d;
}

void SubtitleOutput::remove() {
	if (d->engine) {
		 hide();
		 if (d->engine->tellmp("sub_remove"))
			d->loaded = false;
	}
}

// void SubtitleOutput::moveUp() {
// 	move(d->pos + 0.01/d->posScale);
// }
// 
// void SubtitleOutput::moveDown() {
// 	move(d->pos - 0.01/d->posScale);
// }
// 
// void SubtitleOutput::move(qreal pos) {
// 	d->pos = qBound(0.0, pos, 1.0);
// 	if (d->engine)
// 		d->engine->tellmp("sub_pos "+QString::number(static_cast<int>(d->pos*d->posScale*100)) + " 1");
// }
// 


void SubtitleOutput::show() {
	if (currentSubtitle().isEmpty())
		return;
	if (!SubtitleParsers::save(d->tempsub, currentSubtitle(), encoding()))
		return;
	if (d->engine && d->engine->isRunning()) {
		remove();
		d->engine->tellmp("sub_load \"" + d->tempsub + '\"');
		d->engine->tellmp("sub_select 0");
	}
}

void SubtitleOutput::hide() {
	if (d->engine)
		d->engine->tellmp("sub_select -1");
}

// void SubtitleOutput::setPosScale(qreal scale) {
// 	if (static_cast<int>(d->posScale*100) == static_cast<int>(scale*100))
// 		return;
// 	d->posScale = qBound(0.0, scale, 1.0);
// 	move(d->pos);
// }

// void SubtitleOutput::update() {
// 	if (d->engine && d->engine->isRunning()) {
// 		d->engine->tellmp("sub_scale " + QString::number(d->font.scale) + " 1");
// 		move(d->initPos);
// 		d->engine->tellmp("sub_delay " + QString::number(static_cast<double>(d->delay)/1000.0) + " 1");
// 	}
// }

// void SubtitleOutput::setDisplayOnMarginEnabled(bool enabled) {
// 	d->onMargin = enabled;
// }
// 
// bool SubtitleOutput::isDisplayOnMarginEnabled() const {
// 	return d->onMargin;
// }

void SubtitleOutput::updateClear() {
	remove();
}

void SubtitleOutput::updateSyncDelay(int msec) {
	if (d->engine) {
		const double sec = static_cast<double>(msec)/1000.0;
		d->engine->tellmp("sub_delay " + QString::number(sec) + " 1");
	}
}

void SubtitleOutput::updatePos(double pos) {
}

void SubtitleOutput::updateCurrentChannel(int index) {
}

void SubtitleOutput::updateStyle(const OsdStyle &style) {
	const double size = qBound(0.0, style.size*100.0, 100.0);
	d->engine->tellmp("sub_scale " + QString::number(size) + " 1");
}

}

}
