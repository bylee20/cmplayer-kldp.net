#include "subtitleoutput.h"
#include "mediainfo.h"
#include "playengine.h"
#include "info.h"
#include <QtCore/QSet>
#include <QtCore/QRectF>
#include <QtCore/QDir>
#include <backend/subtitleparsers.h>
#include "videooutput.h"
#include <QtCore/QDebug>

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
	connect(d->engine->videoOutput(), SIGNAL(sizeUpdated()), this, SLOT(slotSizeUpdated()));
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

void SubtitleOutput::updateClear() {
	remove();
}

void SubtitleOutput::updateSyncDelay(int msec) {
	if (d->engine) {
		const double sec = static_cast<double>(msec)/1000.0;
		d->engine->tellmp("sub_delay " + QString::number(sec) + " 1");
	}
}

void SubtitleOutput::slotSizeUpdated() {
	updatePos(pos());
}

void SubtitleOutput::updatePos(double pos) {
	if (d->engine) {
		QRectF rect = d->engine->video()->osdRect(true);
		const double fullHeight = rect.bottom() + rect.top();
		const double point = rect.top() + rect.height() * pos;
		const int p = qBound(0, qRound(point/fullHeight*100.0), 100);
		qDebug() << rect << fullHeight << point << p;
		d->engine->tellmp("sub_pos " + QString::number(p) + " 1");
	}
}

void SubtitleOutput::updateCurrentChannel(int index) {
}

void SubtitleOutput::updateStyle(const OsdStyle &style) {
	const double size = qBound(0.0, style.size*100.0, 100.0);
	d->engine->tellmp("sub_scale " + QString::number(size) + " 1");
}

}

}
