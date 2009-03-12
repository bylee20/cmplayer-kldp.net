#include "playlistmodel.h"
#include "recentinfo.h"
#include <QtCore/QUrl>
#include <QDebug>
#include <QTime>
#include <QtGui/QFont>
#include <core/mediasource.h>
#include <core/playlist.h>
#include <core/playengine.h>
#include "videoplayer.h"

struct PlayListModel::Data {
	VideoPlayer *player;
	Core::PlayList list;
	QFont font;
	int row;
	bool loop;
};

PlayListModel::PlayListModel(VideoPlayer *player, QObject *parent)
: QAbstractTableModel(parent), d(new Data) {
	d->player = player;
	d->row = -1;
	d->loop = false;
	d->font.setItalic(true);
	d->font.setBold(true);
	connect(d->player, SIGNAL(finished(Core::MediaSource))
	        , this, SLOT(slotFinished(const Core::MediaSource&)));
}

PlayListModel::~PlayListModel() {
	delete d;
}

void PlayListModel::setSource(int row, const Core::MediaSource &source) {
	if (0 <= row && row < d->list.size()) {
		d->list[row] = source;
		emitDataChanged(row);
	} else
		append(source);
}

int PlayListModel::count() const {
	return d->list.size();
}

bool PlayListModel::swap(int row1, int row2) {
	if (row1 < 0 || row1 >= d->list.size() || row2 < 0 || row2 >= d->list.size())
		return false;
	d->list.swap(row1, row2);
	if (row1 == d->row)
		emit currentRowChanged(d->row = row2);
	else if (row2 == d->row)
		emit currentRowChanged(d->row = row1);
	emitDataChanged(row1);
	emitDataChanged(row2);
	return true;
}

void PlayListModel::clear() {
	setCurrentRow(-1);
	d->list.clear();
	emit rowCountChanged(0);
	reset();
}

int PlayListModel::currentRow() const {
	return d->row;
}

void PlayListModel::remove(int row) {
	if (row < 0 || row >= d->list.size())
		return;
	if (d->row == row)
		setCurrentRow(-1);
	emit layoutAboutToBeChanged();
	d->list.removeAt(row);
	emit layoutChanged();
	emit rowCountChanged(d->list.size());
}

void PlayListModel::play(int row) {
	if (row < 0 || row >= d->list.size())
		return;
	setCurrentRow(row);
	d->player->play(RecentInfo::get()->stoppedTime(d->list[row]));
}

void PlayListModel::playNext() {
	if (d->list.isEmpty())
		return;
	play(d->row + 1);
}

void PlayListModel::playPrevious() {
	if (d->list.isEmpty())
		return;
	play(d->row - 1);
}

void PlayListModel::setLoopEnabled(bool enabled) {
	d->loop = enabled;
}

bool PlayListModel::isLoopEnabled() const {
	return d->loop;
}

Core::MediaSource PlayListModel::source(int row) const {
	return d->list.value(row);
}

int PlayListModel::row(const Core::MediaSource &source) const {
	return d->list.indexOf(source);
}

void PlayListModel::slotFinished(const Core::MediaSource &/*source*/) {
	if (d->row == d->list.size() - 1) {
		emit playListFinished();
		if (!d->loop)
			return;
	}
	playNext();
}

const Core::PlayList &PlayListModel::playList() const {
	return d->list;
}

void PlayListModel::setCurrentFont(const QFont &font) {
	d->font = font;
}

const QFont &PlayListModel::currentFont() const {
	return d->font;
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
			return tr("Name");
		if (section == 1)
			return tr("Location");
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
		const Core::MediaSource &source = d->list[row];
		if (col == 0)
			return source.displayName();
		else if (col == 1)
			return source.isLocalFile() ? source.filePath() : source.url().toString();
	} else if (role == Qt::FontRole) {
		if (row == d->row)
			return d->font;
	}
	return QVariant();
}

void PlayListModel::setCurrentSource(const Core::MediaSource &source) {
	int idx = d->list.indexOf(source);
	if (idx == -1) {
		idx = d->list.size();
		append(source);
	}
	setCurrentRow(idx);
}

void PlayListModel::setCurrentRow(int row) {
	if (row < 0 || row >= d->list.size())
		row = -1;
	if (row == d->row)
		return;
	const int old = d->row;
	emit currentRowChanged(d->row = row);
	if (old != -1)
		emitDataChanged(old);
	if (d->row != -1) {
		emitDataChanged(d->row);
		d->player->setCurrentSource(d->list[d->row]);
	}
}

void PlayListModel::append(const QList<Core::MediaSource> &list) {
	emit layoutAboutToBeChanged();
	d->list += list;
	emit rowCountChanged(d->list.size());
	emit layoutChanged();
}

void PlayListModel::append(const Core::MediaSource &source) {
	insert(rowCount(), source);
}

void PlayListModel::insert(int row, const Core::MediaSource &source) {
	if (row < 0 || row >= d->list.size())
		row = d->list.size();
	emit layoutAboutToBeChanged();
	d->list.insert(row, source);
	emit rowCountChanged(d->list.size());
	emit layoutChanged();
	emitDataChanged(row);
}

void PlayListModel::setPlayList(const Core::PlayList &list, int current) {
	d->list = list;
	reset();
	emit rowCountChanged(d->list.size());
	d->row = -2;
	setCurrentRow(current);
}
