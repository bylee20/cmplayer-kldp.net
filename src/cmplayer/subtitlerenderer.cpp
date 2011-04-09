#include "subtitlerenderer.hpp"
#include "info.hpp"
#include "mrl.hpp"
#include "pref.hpp"
#include "subtitlemodel.hpp"
#include "osdstyle.hpp"
#include "subtitleview.hpp"
#include <QtCore/QDebug>

struct SubtitleRenderer::Data {
	SubtitleView *view;
	TextOsdRenderer *osd;
	Subtitle sub;
	QVector<Subtitle::Component::const_iterator> prev;
	QVector<SubtitleComponentModel*> model;
	double fps;
	int delay, ms;
	double pos;
	bool visible, empty;
	Subtitle loaded;
	QList<bool> selection;
};

SubtitleRenderer::SubtitleRenderer(): d(new Data) {
	d->empty = d->visible = true;
	d->osd = 0;
	d->fps = 30;
	d->ms = d->delay = 0;
	d->pos = 1.0;
	d->view = 0;
}

SubtitleRenderer::~SubtitleRenderer() {
	delete d;
}

QWidget *SubtitleRenderer::view(QWidget *parent) const {
	if (!d->view) {
		d->view = new SubtitleView(parent);
		d->view->setModel(d->model);
	}
	return d->view;
}

TextOsdRenderer *SubtitleRenderer::osd() const {
	return d->osd;
}

void SubtitleRenderer::setOsd(TextOsdRenderer *osd) {
	d->osd = osd;
}

void SubtitleRenderer::setSubtitle(const Subtitle &subtitle) {
	d->sub = subtitle;
	d->prev.resize(d->sub.size());
	qDeleteAll(d->model);
	d->model.resize(d->sub.size());
	d->empty = true;
	for (int i=0; i<d->sub.size(); ++i) {
		d->prev[i] = d->sub[i].end();
		d->model[i] = new SubtitleComponentModel(&d->sub[i], this);
		if (!d->sub[i].isEmpty())
			d->empty = false;
	}
	if (d->empty)
		clear();
	else
		render(d->ms);
	if (d->view)
		d->view->setModel(d->model);
}

void SubtitleRenderer::setFrameRate(double fps) {
	if (d->fps != fps) {
		for (int i=0; i<d->prev.size(); ++i)
			d->prev[i] = d->sub[i].end();
	}
}

void SubtitleRenderer::render(int ms) {
	d->ms = ms;
	if (!d->visible || d->empty || ms == 0 || !d->osd)
		return;
	bool changed = false;
	for (int i=0; i<d->sub.size(); ++i) {
		Subtitle::Component::const_iterator it = d->sub[i].start(ms - d->delay, d->fps);
		if (it != d->prev[i]) {
			d->prev[i] = it;
			d->model[i]->setCurrentNode(&(*it));
			changed = true;
		}
	}
	if (changed) {
		RichString text;
		for (int i=0; i<d->prev.size(); ++i) {
			if (d->prev[i] != d->sub[i].end())
				text.merge(d->prev[i]->text);
		}
		d->osd->showText(text);
	}
}

void SubtitleRenderer::setVisible(bool visible) {
	if (visible == d->visible)
		return;
	d->visible = visible;
	if (d->visible)
		render(d->ms);
	else
		clear();
}

void SubtitleRenderer::clear() {
	for (int i=0; i<d->prev.size(); ++i)
		d->prev[i] = d->sub[i].end();
	if (d->osd)
		d->osd->clear();
}

const Subtitle &SubtitleRenderer::subtitle() const {
	return d->sub;
}

double SubtitleRenderer::frameRate() const {
	return d->fps;
}

int SubtitleRenderer::delay() const {
	return d->delay;
}

void SubtitleRenderer::unload() {
	d->loaded.clear();
	d->selection.clear();
	setSubtitle(d->loaded);
}

const Subtitle &SubtitleRenderer::loaded() const {
	return d->loaded;
}

QList<bool> SubtitleRenderer::selection() const {
	return d->selection;
}

void SubtitleRenderer::select(int idx, bool selected) {
	if (0 <= idx && idx < d->selection.size()) {
		if (d->selection[idx] != selected) {
			d->selection[idx] = selected;
			applySelection();
		}
	}
}

void SubtitleRenderer::applySelection() {
	Q_ASSERT(d->selection.size() == d->loaded.size());
	const Pref &pref = Pref::get();
	const QStringList priority = pref.subtitlePriority;
	QList<int> order;
	QList<int> indexes;
	QList<int>::iterator it;
	for (int i=0; i<d->selection.size(); ++i) {
		if (d->selection[i])
			indexes.push_back(i);
	}
	for (int i=0; i<priority.size(); ++i) {
		it = indexes.begin();
		while(it != indexes.end()) {
			const QString id = d->loaded[*it].language().id();
			if (id == priority[i]) {
				order.append(*it);
				it = indexes.erase(it);
			} else
				++it;
		}
	}
	order += indexes;
	Subtitle sub;
	for (int i=0; i<order.size(); ++i)
		sub.append(d->loaded[order[i]]);
	setSubtitle(sub);
}

bool SubtitleRenderer::load(const QString &fileName, const QString &enc, bool select) {
	int idx = d->loaded.size();
	Subtitle sub;
	if (!sub.load(fileName, enc))
		return false;
	d->loaded += sub;
	for (int i=idx; i<d->loaded.size(); ++i)
		d->selection.push_back(select);
	Q_ASSERT(d->loaded.size() == d->selection.size());
	applySelection();
	return true;
}

static QList<bool> autoselection(const Mrl &mrl, const Subtitle &loaded) {
	QList<bool> selection;
	QList<int> selected;
	if (loaded.isEmpty() || !mrl.isLocalFile())
		return selection;
	const Pref &p = Pref::get();
	QSet<QString> langSet;
	const QString base = QFileInfo(mrl.toLocalFile()).completeBaseName();
	for (int i=0; i<loaded.size(); ++i) {
		selection.push_back(false);
		bool select = false;
		if (p.subtitleAutoSelect == SameName) {
			select = QFileInfo(loaded[i].fileName()).completeBaseName() == base;
		} else if (p.subtitleAutoSelect == AllLoaded) {
			select = true;
		} else if (p.subtitleAutoSelect == EachLanguage) {
			const QString lang = loaded[i].language().id();
			if ((select = (!langSet.contains(lang))))
				langSet.insert(lang);
		}
		if (select)
			selected.push_back(i);
	}
	if (p.subtitleAutoSelect == SameName
			&& !selected.isEmpty() && !p.subtitleExtension.isEmpty()) {
		for (int i=0; i<selected.size(); ++i) {
			const QString fileName = loaded[selected[i]].fileName();
			const QString suffix = QFileInfo(fileName).suffix().toLower();
			if (p.subtitleExtension == suffix) {
				const int idx = selected[i];
				selected.clear();
				selected.push_back(idx);
				break;
			}
		}
	}
	for (int i=0; i<selected.size(); ++i) {
		selection[selected[i]] = true;
	}
	return selection;
}

int SubtitleRenderer::autoload(const Mrl &mrl, bool autoselect) {
	unload();
	const Pref &pref = Pref::get();
	if (pref.subtitleAutoLoad == NoAutoLoad)
		return 0;
	const QStringList filter = Info::subtitleNameFilter();
	const QFileInfo fileInfo(mrl.toLocalFile());
	const QFileInfoList all = fileInfo.dir().entryInfoList(filter, QDir::Files, QDir::Name);
	const QString base = fileInfo.completeBaseName();
	for (int i=0; i<all.size(); ++i) {
		if (pref.subtitleAutoLoad != SamePath) {
			if (pref.subtitleAutoLoad == Matched) {
				if (base != all[i].completeBaseName())
					continue;
			} else if (!all[i].fileName().contains(base))
				continue;
		}
		Subtitle subtitle;
		if (subtitle.load(all[i].absoluteFilePath(), pref.subtitleEncoding)) {
			d->loaded += subtitle;
		}
	}
	if (autoselect) {
		d->selection = autoselection(mrl, d->loaded);
		applySelection();
	}
	return d->loaded.size();
}

int SubtitleRenderer::start(int pos) const {
	return d->sub.start(pos - d->delay, d->fps);
}

int SubtitleRenderer::end(int pos) const {
	return d->sub.end(pos - d->delay, d->fps);
}

void SubtitleRenderer::setDelay(int delay) {
	if (d->delay != delay) {
		d->delay = delay;
		render(d->ms);
	}
}

bool SubtitleRenderer::hasSubtitle() const {
	return !d->empty;
}

void SubtitleRenderer::setPos(double pos) {
	if (!qFuzzyCompare(pos, d->pos)) {
		d->pos = qBound(0.0, pos, 1.0);
		d->osd->setMargin(0, 1.0 - d->pos, 0, 0);
	}
}

double SubtitleRenderer::pos() const {
	return d->pos;
}
