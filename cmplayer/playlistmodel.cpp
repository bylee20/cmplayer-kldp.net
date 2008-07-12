#include "playlistmodel.h"
#include "recentinfo.h"
#include <QUrl>
#include <QFont>
#include <mplayer/mediasource.h>
#include <mplayer/playlist.h>
#include <mplayer/playengine.h>

struct PlayListModel::Data {
	inline bool isValid(int row) { return row >=0 && row < list.size();}
	bool setCurrentRow(int row) {
		int temp = curRow;
		if (row < 0) {
			curRow = -1;
		} else if (isValid(row) && row != curRow) {
			curRow = row;
		} else
			return false;
		emitDataChanged(temp);
		if (curRow != -1) {
			emitDataChanged(curRow);
			if (engine && engine->currentSource() != list[curRow])
				engine->setCurrentSource(list[curRow]);
		}
		emit p->currentRowChanged(curRow);
		return true;
	}
	void emitDataChanged(int row) {
		emit p->dataChanged(p->index(row, 0), p->index(row, p->columnCount()-1));
	}
	PlayListModel *p;
	MPlayer::PlayList list;
	QFont font;
	MPlayer::PlayEngine *engine;
	int curRow;
	bool loop;
};

PlayListModel::PlayListModel(MPlayer::PlayEngine *engine, QObject *parent)
: QAbstractTableModel(parent), d(new Data()) {
	d->p = this;
	d->engine = engine;
	d->loop = false;
	d->font.setItalic(true);
	d->font.setBold(true);
	connect(engine, SIGNAL(finished()), this, SLOT(slotFinished()));
}

PlayListModel::~PlayListModel() {
	delete d;
}

bool PlayListModel::setMediaSource(int row, const MPlayer::MediaSource &source) {
	if (!d->isValid(row))
		return false;
	d->list[row] = source;
	d->emitDataChanged(row);
	return true;
}

MPlayer::MediaSource PlayListModel::mediaSource(int row) const {
	return d->list.value(row);
}

int PlayListModel::count() const {
	return d->list.size();
}

bool PlayListModel::swap(int row1, int row2) {
	if (d->isValid(row1) && d->isValid(row2)) {
		d->list.swap(row1, row2);
		d->emitDataChanged(row1);
		d->emitDataChanged(row2);
		if (row1 == d->curRow)
			d->setCurrentRow(row2);
		else if (row2 == d->curRow)
			d->setCurrentRow(row1);
		return true;
	} else
		return false;
}

void PlayListModel::addSources(const QList<MPlayer::MediaSource> &list) {
	emit layoutAboutToBeChanged();
	d->list += list;
	emit rowCountChanged(d->list.size());
	emit layoutChanged();
}

void PlayListModel::addSource(const MPlayer::MediaSource &source) {
	insert(rowCount(), source);
}

bool PlayListModel::insert(int row, const MPlayer::MediaSource &source) {
	if (!d->isValid(row))
		return false;
	emit layoutAboutToBeChanged();
	d->list.insert(row, source);
	emit rowCountChanged(d->list.size());
	emit layoutChanged();
	d->emitDataChanged(row);
	return true;
}

void PlayListModel::clear() {
	d->list.clear();
	d->setCurrentRow(-1);
	reset();
}

int PlayListModel::currentRow() const {
	return d->curRow;
}

MPlayer::MediaSource PlayListModel::currentSource() const {
	return source(d->curRow);
}

void PlayListModel::remove(int row) {
	if (!d->isValid(row))
		return;
	if (d->curRow == row)
		d->setCurrentRow(-1);
	emit layoutAboutToBeChanged();
	d->list.removeAt(row);
	emit layoutChanged();
	emit rowCountChanged(d->list.size());
}

void PlayListModel::play(int row) {
	if (!d->isValid(row) || !d->engine)
		return;
	d->setCurrentRow(row);
	d->engine->play(RecentInfo::get()->stoppedTime(d->list[row]));
}

void PlayListModel::playNext() {
	if (d->list.isEmpty())
		return;
	play(d->curRow + 1);
}

void PlayListModel::playPrevious() {
	if (d->list.isEmpty())
		return;
	play(d->curRow - 1);
}

void PlayListModel::setLoopEnabled(bool enabled) {
	d->loop = enabled;
}

bool PlayListModel::isLoopEnabled() const {
	return d->loop;
}

MPlayer::MediaSource PlayListModel::source(int row) const {
	return d->list.value(row);
}

int PlayListModel::row(const MPlayer::MediaSource &source) const {
	return d->list.indexOf(source);
}

void PlayListModel::slotFinished() {
	if (!d->engine || !d->isValid(d->curRow) || d->list[d->curRow] != d->engine->currentSource())
		return;
	if (d->curRow == d->list.size()-1) {
		emit playListFinished();
		if (d->loop)
			d->curRow = -1;
	}
	playNext();
}

void PlayListModel::setPlayList(const MPlayer::PlayList &list) {
	MPlayer::MediaSource curSrc = currentSource();
	d->list = list;
	reset();
	d->setCurrentRow(row(curSrc));
}

const MPlayer::PlayList &PlayListModel::playList() const {
	return d->list;
}

void PlayListModel::setCurrentFont(const QFont &font) {
	d->font = font;
}

const QFont &PlayListModel::currentFont() const {
	return d->font;
}

void PlayListModel::setCurrentRow(int row) {
	d->setCurrentRow(row);
}

int PlayListModel::rowCount(const QModelIndex &/*parent*/) const {
	return d->list.size();
}

int PlayListModel::columnCount(const QModelIndex &/*parent*/) const {
	return 2;
}

QVariant PlayListModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (role != Qt::DisplayRole)
		return QVariant();
	if (orientation == Qt::Horizontal) {
		if (section == 0)
			return trUtf8("이름");
		if (section == 1)
			return trUtf8("위치");
	} else if (orientation == Qt::Vertical)
		return section + 1;
	return QVariant();
}

QVariant PlayListModel::data(const QModelIndex &index, int role) const {
	if (!index.isValid())
		return QVariant();
	const int col = index.column();
	const int row = index.row();
	if (role == Qt::DisplayRole) {
		const MPlayer::MediaSource &source = d->list[row];
		if (col == 0)
			return source.displayName();
		else if (col == 1)
			return source.isLocalFile() ? source.filePath() : source.url().toString();
	} else if (role == Qt::FontRole) {
		if (row == d->curRow)
			return d->font;
	}
	return QVariant();
}
