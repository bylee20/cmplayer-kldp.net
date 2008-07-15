#include "subtitleoutput.h"
#include "mediainfo.h"
#include "mediasource.h"
#include "playengine.h"
#include "informations.h"
#include <QSet>
#include <QDir>
#include "subtitleparsers.h"
#include <QDebug>
#include "dvdinfo.h"

namespace MPlayer {

struct SubtitleOutput::Data {
	Data(SubtitleOutput *parent)
	: p(parent), engine(0), tempsub(Informations::get()->privatePath() + "/temp.smi")
	, delay(0), pos(1.0), initPos(1.0), posScale(1.0), autoScale(FitToWidth), autoSelect(SameName)
	, loaded(false), onMargin(true), current(QString()) {}
	bool load();
	void clear(bool emits, bool reset) {
		p->remove();
		current.clear();
		if (!reset) {
			QMutableListIterator<Subtitle> it(subtitles);
			while(it.hasNext()) {
				if (it.next().type() != Subtitle::Disc)
					it.remove();
			}
		} else
			subtitles.clear();
		selected.clear();
		if (emits) {
			emit p->availableSubtitlesChanged(p->names());
			emit p->selectedSubtitlesChanged(selected);
		}
	}
	void updateCurrent() {
		if (!engine || !engine->isRunning())
			return;
		if (selected.isEmpty()) {
			p->hide();
			return;
		}
		if (selected.size() == 1 && subtitles[selected[0]].type() == Subtitle::Disc)
			current = subtitles[selected[0]];
		else {
			int id = -1;
			for (int i=0; i<subtitles.size(); ++i) {
				if (subtitles[i].type() == Subtitle::LocalFile)
					break;
				id = subtitles[i].internalID();
			}
			QList<int> order;
			QList<int> indexes = selected;
			for (int i=0; i<priority.size(); ++i) {
				QString lang = priority[i];
				QMutableListIterator<int> it(indexes);
				while(it.hasNext()) {
					if (subtitles[it.next()].language() == lang) {
						order.append(it.value());
						it.remove();
					}
				}
			}
			order += indexes;
			current = subtitles[order[0]];
			for (int i=1; i<order.size(); ++i)
				current |= subtitles[order[i]];
			current.setInternalID(++id);
			p->remove();
			if (!SubtitleParsers::save(tempsub, current, encoding))
				return;
		}
		p->show();
	}
	bool isDisc() {return engine && engine->currentSource().isDisc();}
	SubtitleOutput *p;
	PlayEngine *engine;
	QString tempsub;
	qint64 delay;
	qreal pos, initPos, posScale;
	Font font;
	AutoScale autoScale;
	AutoSelect autoSelect;
	QString encoding;
	bool loaded, onMargin;
	QList<Subtitle> subtitles;
	Subtitle current;
	QList<int> selected;
	QStringList priority;
};

bool SubtitleOutput::Data::load() {
	return loaded = (current.type() == Subtitle::Disc
			|| (engine && engine->tellmp("sub_load \"" + tempsub + '\"')));
}

SubtitleOutput::SubtitleOutput(QObject *parent)
: Controller(parent), d(new Data(this)) {
}

SubtitleOutput::~SubtitleOutput() {
	delete d;
}

void SubtitleOutput::setPriority(const QStringList &priority) {
	d->priority = priority;
}

qint64 SubtitleOutput::syncDelay() const {
	return d->delay;
}

qreal SubtitleOutput::pos() const {
	return d->pos;
}

void SubtitleOutput::addSyncDelay(qint64 msec) {
	setSyncDelay(d->delay + msec);
}

void SubtitleOutput::remove() {
	if (d->engine) {
		 hide();
		 if (d->engine->tellmp("sub_remove"))
			d->loaded = false;
	}
}

void SubtitleOutput::clear() {
	d->clear(true, false);
}

void SubtitleOutput::moveUp() {
	move(d->pos + 0.01/d->posScale);
}

void SubtitleOutput::moveDown() {
	move(d->pos - 0.01/d->posScale);
}

void SubtitleOutput::move(qreal pos) {
	d->pos = qBound(0.0, pos, 1.0);
	if (d->engine)
		d->engine->tellmp("sub_pos "+QString::number(static_cast<int>(d->pos*d->posScale*100)) + " 1");
}

void SubtitleOutput::setSyncDelay(qint64 msec) {
	emit syncDelayChanged(d->delay = msec);
	if (d->engine)
		d->engine->tellmp("sub_delay " + QString::number(static_cast<double>(msec)/1000.0) + " 1");
}

const SubtitleOutput::Font &SubtitleOutput::font() const {
	return d->font;
}

void SubtitleOutput::setFont(const Font &font) {
	d->font = font;
}

SubtitleOutput::AutoScale SubtitleOutput::autoScale() const {
	return d->autoScale;
}

void SubtitleOutput::setAutoScale(AutoScale mode) {
	d->autoScale = mode;
}

const QString &SubtitleOutput::encoding() const {
	return d->encoding;
}

void SubtitleOutput::setEncoding(const QString &encoding) {
	d->encoding = encoding;
}

qreal SubtitleOutput::initialPos() const {
	return d->initPos;
}

void SubtitleOutput::setInitialPos(qreal pos) {
	d->initPos = pos;
}

void SubtitleOutput::show() {
	if (d->loaded || d->load())
		d->engine->tellmp("sub_select " + QString::number(d->current.internalID()));
}

void SubtitleOutput::hide() {
	if (d->engine)
		d->engine->tellmp("sub_select -1");
}

void SubtitleOutput::slotFinished() {
	d->loaded = false;
}

void SubtitleOutput::loadDisc() {
	do {
		if (!d->isDisc())
			break;
		const MediaSource source = d->engine->currentSource();
		if (!source.gotInfo() || source.info().dvd().subtitles.isEmpty())
			break;
		d->clear(false, true);
		d->subtitles = source.info().dvd().subtitles;
		emit availableSubtitlesChanged(names());
		d->updateCurrent();
		emit selectedSubtitlesChanged(d->selected);
	return;
	} while(false);
	d->clear(true, true);
}

void SubtitleOutput::load(const QStringList &files) {
	d->clear(false, true);
	for (int i=0; i<files.size(); ++i) {
		QList<Subtitle> subs;
		if (SubtitleParsers::parse(files[i], &subs, d->encoding, 10))
			d->subtitles += subs;
	}
	QSet<QString> langs;
	QString base;
	if (d->engine)
		base = QFileInfo(d->engine->currentSource().filePath()).baseName();
	for (int i=0; i<d->subtitles.size(); ++i) {
		bool select = false;
		switch(d->autoSelect) {
		case SameName:
			select = QFileInfo(d->subtitles[i].filePath()).baseName() == base;
			break;
		case AllLoaded:
			select = true;
			break;
		case EachLanguage:
			if (select = (!langs.contains(d->subtitles[i].language())))
				langs.insert(d->subtitles[i].language());
			break;
		default:
			break;
		}
		if (select)
			d->selected.append(i);
	}
	emit availableSubtitlesChanged(names());
	if (!d->subtitles.isEmpty()) {
		d->updateCurrent();
		emit selectedSubtitlesChanged(d->selected);
	}
}

QStringList SubtitleOutput::names() const {
	QStringList names;
	for (int i=0; i<d->subtitles.size(); ++i)
		names.append(d->subtitles[i].name());
	return names;
}

void SubtitleOutput::appendSubtitles(const QStringList &files, bool display) {
	if (files.isEmpty())
		return;
	int index = d->subtitles.size();
	for (int i=0; i<index; ++i)
		if (d->subtitles[i].type() == Subtitle::Disc)
			d->selected.removeOne(i);
	for (int i=0; i<files.size(); ++i) {
		QList<Subtitle> subs;
		if (!SubtitleParsers::parse(files[i], &subs, d->encoding, 10))
			continue;
		d->subtitles += subs;
		if (display) {
			for (int j=0; j<subs.size(); ++j, ++index)
				d->selected.append(index);
		}
	}
	emit availableSubtitlesChanged(names());
	if (display) {
		d->updateCurrent();
		emit selectedSubtitlesChanged(d->selected);
	}
}

const QList<int> &SubtitleOutput::selectedIndexes() const {
	return d->selected;
}

//void SubtitleOutput::setSelectedIndexes(const QList<int> indexes) {
//	d->selected.clear();
//	bool disc = false;
//	for (int i=0; i<indexes.size(); ++i) {
//		if (i < 0 || i >= d->subtitles.size())
//			continue;
//		Subtitle::Type t = d->subtitles[indexes[i]].type();
//		if (t == Subtitle::Disc && disc)
//			continue;
//		d->selected.append(indexes[i]);
//		if (t == Subtitle::Disc)
//			disc = true;
//	}
//	d->updateCurrent();
//	emit selectedSubtitlesChanged(d->selected);
//}

void SubtitleOutput::select(int index) {
	if (d->selected.contains(index) || index < 0 || index >= d->subtitles.size())
		return;
	if (d->subtitles[index].type() == Subtitle::Disc)
		d->selected.clear();
	else {
		for (int i=0; i<d->subtitles.size(); ++i)
			if (d->subtitles[i].type() == Subtitle::Disc)
				d->selected.removeOne(i);
	}
	d->selected.append(index);
	d->updateCurrent();
	emit selectedSubtitlesChanged(d->selected);
}

void SubtitleOutput::deselect(int index) {
	const int pos = d->selected.indexOf(index);
	if (pos != -1) {
		d->selected.removeAt(pos);
		d->updateCurrent();
		emit selectedSubtitlesChanged(d->selected);
	}
}

void SubtitleOutput::deselectAll() {
	d->selected.clear();
	d->updateCurrent();
	emit selectedSubtitlesChanged(d->selected);
}

//void SubtitleOutput::selectAll() {
//	d->selected.clear();
//	bool disc = false;
//	for (int i=0; i<d->subtitles.size(); ++i) {
//		Subtitle::Type t = d->subtitles[i].type();
//		if (t == Subtitle::Disc && disc)
//			continue;
//		d->selected.append(i);
//		if (t == Subtitle::Disc)
//			disc = true;
//	}
//	d->updateCurrent();
//	emit selectedSubtitlesChanged(d->selected);
//}

const QList<Subtitle> &SubtitleOutput::availableSubtitles() const {
	return d->subtitles;
}

SubtitleOutput::AutoSelect SubtitleOutput::autoSelect() const {
	return d->autoSelect;
}

void SubtitleOutput::setAutoSelect(AutoSelect mode) {
	d->autoSelect = mode;
}

const Subtitle &SubtitleOutput::currentSubtitle() const {
	return d->current;
}

void SubtitleOutput::link(Controller *controller) {
	PlayEngine *engine = qobject_cast<PlayEngine*>(controller);
	if (engine) {
		if (d->engine)
			unlink(d->engine);
		d->engine = engine;
		connect(d->engine, SIGNAL(finished()), this, SLOT(slotFinished()));
		connect(d->engine, SIGNAL(started()), this, SLOT(update()));
		return;
	}
}

void SubtitleOutput::unlink(Controller *controller) {
	if (d->engine && d->engine == controller) {
		disconnect(d->engine, 0, this, 0);
		d->engine = 0;
	}
}

void SubtitleOutput::setPosScale(qreal scale) {
	if (static_cast<int>(d->posScale*100) == static_cast<int>(scale*100))
		return;
	d->posScale = qBound(0.0, scale, 1.0);
	move(d->pos);
}

void SubtitleOutput::update() {
	if (d->engine && d->engine->isRunning()) {
		d->engine->tellmp("sub_scale " + QString::number(d->font.scale) + " 1");
		move(d->initPos);
		d->engine->tellmp("sub_delay " + QString::number(static_cast<double>(d->delay)/1000.0) + " 1");
	}
}

void SubtitleOutput::setDisplayOnMarginEnabled(bool enabled) {
	d->onMargin = enabled;
}

bool SubtitleOutput::isDisplayOnMarginEnabled() const {
	return d->onMargin;
}

}

