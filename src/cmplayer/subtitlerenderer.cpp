#include "subtitlerenderer.hpp"
#include "info.hpp"
#include "mrl.hpp"
#include "pref.hpp"
#include "subtitlemodel.hpp"
#include "osdstyle.hpp"
#include "subtitleview.hpp"
#include <QtCore/QDebug>

SubtitleRenderer::Render::Render(const Comp &comp) {
	this->comp = &comp;
	this->model = new SubtitleComponentModel(this->comp);
	this->prev = this->comp->end();
}

SubtitleRenderer::Render::~Render() {
	delete this->model;
}

struct SubtitleRenderer::Data {
	SubtitleView *view;
	TextOsdRenderer *osd;
//	Subtitle sub;
//	QVector<CompIt> prev;
//	QVector<SubtitleComponentModel*> model;
	QHash<int, Render*> selected;
	double fps;
	int delay, ms;
	double pos;
	bool visible, empty;
	Subtitle loaded;
	QList<bool> selection;
	RenderList render;
	QVector<SubtitleComponentModel*> model_list() const {
		QVector<SubtitleComponentModel*> list;
		for (int i=0; i<render.size(); ++i)
			list.push_back(render[i]->model);
		return list;
	}
	void reset_prev() {
		for (int i=0; i<render.size(); ++i)
			render[i]->prev = render[i]->comp->end();
	}
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
		d->view->setModel(d->model_list());
	}
	return d->view;
}

TextOsdRenderer *SubtitleRenderer::osd() const {
	return d->osd;
}

void SubtitleRenderer::setOsd(TextOsdRenderer *osd) {
	d->osd = osd;
}

void SubtitleRenderer::setFrameRate(double fps) {
	if (d->fps != fps) {
		d->fps = fps;
		d->reset_prev();
	}
}

void SubtitleRenderer::render(int ms) {
	d->ms = ms;
	if (!d->visible || d->empty || ms == 0 || !d->osd)
		return;
	bool changed = false;
	for (int i=0; i<d->render.size(); ++i) {
		Render &render = *d->render[i];
		CompIt it = render.comp->start(ms - d->delay, d->fps);
		if (it != render.prev) {
			render.prev = it;
			render.model->setCurrentNode(&(*it));
			changed = true;
		}
	}
	if (changed) {
		RichString text;
		for (int i=0; i<d->render.size(); ++i) {
			const Render &render = *d->render[i];
			if (render.prev != render.comp->end()) {
				text.merge(render.prev->text);
			}
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
	d->reset_prev();
	if (d->osd)
		d->osd->clear();
}

//const Subtitle &SubtitleRenderer::subtitle() const {
//	return d->sub;
//}

double SubtitleRenderer::frameRate() const {
	return d->fps;
}

int SubtitleRenderer::delay() const {
	return d->delay;
}

void SubtitleRenderer::unload() {
	qDeleteAll(d->render);
	d->render.clear();
	d->loaded.clear();
	d->selection.clear();
	clear();
	d->empty = true;
	if (d->view)
		d->view->setModel(d->model_list());

	//	d->sub = subtitle;
	//	d->prev.resize(d->sub.size());
	//	qDeleteAll(d->model);
	//	d->model.resize(d->sub.size());
	//	d->empty = true;
	//	for (int i=0; i<d->sub.size(); ++i) {
	//		d->prev[i] = d->sub[i].end();
	//		d->model[i] = new SubtitleComponentModel(&d->sub[i], this);
	//		if (!d->sub[i].isEmpty())
	//			d->empty = false;
	//	}
	//	if (d->empty)
	//		clear();
	//	else
	//		render(d->ms);
	//	if (d->view)
	//		d->view->setModel(d->model);

//	setSubtitle(d->loaded);
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
	for (int i=0; i<d->selection.size(); ++i) {
		if (d->selection[i])
			indexes.push_back(i);
	}
	for (int i=0; i<priority.size(); ++i) {
		QList<int>::iterator it = indexes.begin();
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

	QList<Render*> render;
	QHash<int, Render*> selected;
	d->empty = true;
	for (int i=0; i<order.size(); ++i) {
		const int idx = order[i];
		QHash<int, Render*>::iterator it = d->selected.find(idx);
		if (it == d->selected.end()) {
			Render *r = new Render(d->loaded[idx]);
			render.push_back(r);
			selected.insert(idx, r);
		} else {
			render.push_back(*it);
			selected.insert(idx, *it);
			d->selected.erase(it);
		}
		if (!render.last()->comp->isEmpty())
			d->empty = false;
	}
	QHash<int, Render*>::iterator it = d->selected.begin();
	for (; it != d->selected.end(); ++it)
		delete *it;
	d->render = render;
	d->selected = selected;
	d->reset_prev();
	if (d->empty)
		clear();
	else
		this->render(d->ms);
	if (d->view)
		d->view->setModel(d->model_list());
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

int SubtitleRenderer::start(int time) const {
	int s = -1;
	for (int i=0; i<d->render.size(); ++i) {
		const Comp *comp = d->render[i]->comp;
		const CompIt it = comp->start(time - d->delay, d->fps);
		if (it != comp->end())
			s = qMax(s, comp->isBasedOnFrame() ? Subtitle::msec(it.key(), d->fps) : it.key());
	}
	return s;
}

int SubtitleRenderer::end(int time) const {
	int e = -1;
	for (int i=0; i<d->render.size(); ++i) {
		const Comp *comp = d->render[i]->comp;
		const CompIt it = comp->end(time - d->delay, d->fps);
		if (it != comp->end()) {
			const int t = comp->isBasedOnFrame() ? Subtitle::msec(it.key(), d->fps) : it.key();
			e = e == -1 ? t : qMin(e, t);
		}
	}
	return e;
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
