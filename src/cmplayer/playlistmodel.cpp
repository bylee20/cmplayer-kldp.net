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

struct PlaylistModel::Data {
	VideoPlayer *player;
	Core::Playlist list;
	QFont font;
	int row;
	bool loop;
};

PlaylistModel::PlaylistModel(VideoPlayer *player, QObject *parent)
: QAbstractTableModel(parent), d(new Data) {
	d->player = player;
	d->row = -1;
	d->loop = false;
	d->font.setItalic(true);
	d->font.setBold(true);
	connect(d->player, SIGNAL(finished(Core::MediaSource))
	        , this, SLOT(slotFinished(const Core::MediaSource&)));
}

PlaylistModel::~PlaylistModel() {
	delete d;
}

void PlaylistModel::setSource(int row, const Core::MediaSource &source) {
	if (0 <= row && row < d->list.size()) {
		d->list[row] = source;
		emitDataChanged(row);
	} else
		append(source);
}

int PlaylistModel::count() const {
	return d->list.size();
}

bool PlaylistModel::swap(int row1, int row2) {
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

void PlaylistModel::clear() {
	setCurrentRow(-1);
	d->list.clear();
	emit rowCountChanged(0);
	reset();
}

int PlaylistModel::currentRow() const {
	return d->row;
}

void PlaylistModel::remove(int row) {
	if (row < 0 || row >= d->list.size())
		return;
	if (d->row == row)
		setCurrentRow(-1);
	emit layoutAboutToBeChanged();
	d->list.removeAt(row);
	emit layoutChanged();
	emit rowCountChanged(d->list.size());
}

void PlaylistModel::play(int row) {
	if (row < 0 || row >= d->list.size())
		return;
	setCurrentRow(row);
	d->player->play(RecentInfo::get()->stoppedTime(d->list[row]));
}

void PlaylistModel::playNext() {
	if (d->list.isEmpty())
		return;
	play(d->row + 1);
}

void PlaylistModel::playPrevious() {
	if (d->list.isEmpty())
		return;
	play(d->row - 1);
}

void PlaylistModel::setLoopEnabled(bool enabled) {
	d->loop = enabled;
}

bool PlaylistModel::isLoopEnabled() const {
	return d->loop;
}

Core::MediaSource PlaylistModel::source(int row) const {
	return d->list.value(row);
}

int PlaylistModel::row(const Core::MediaSource &source) const {
	return d->list.indexOf(source);
}

void PlaylistModel::slotFinished(const Core::MediaSource &/*source*/) {
	if (d->row == d->list.size() - 1) {
		emit playlistFinished();
		if (!d->loop)
			return;
	}
	playNext();
}

const Core::Playlist &PlaylistModel::playlist() const {
	return d->list;
}

void PlaylistModel::setCurrentFont(const QFont &font) {
	d->font = font;
}

const QFont &PlaylistModel::currentFont() const {
	return d->font;
}

int PlaylistModel::rowCount(const QModelIndex &/*parent*/) const {
	return d->list.size();
}

int PlaylistModel::columnCount(const QModelIndex &/*parent*/) const {
	return 2;
}

QVariant PlaylistModel::headerData(int section, Qt::Orientation orientation, int role) const {
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

QVariant PlaylistModel::data(const QModelIndex &index, int role) const {
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

void PlaylistModel::setCurrentSource(const Core::MediaSource &source) {
	int idx = d->list.indexOf(source);
	if (idx == -1) {
		idx = d->list.size();
		append(source);
	}
	setCurrentRow(idx);
}

void PlaylistModel::setCurrentRow(int row) {
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

void PlaylistModel::append(const QList<Core::MediaSource> &list) {
	emit layoutAboutToBeChanged();
	d->list += list;
	emit rowCountChanged(d->list.size());
	emit layoutChanged();
}

void PlaylistModel::append(const Core::MediaSource &source) {
	insert(rowCount(), source);
}

void PlaylistModel::insert(int row, const Core::MediaSource &source) {
	if (row < 0 || row >= d->list.size())
		row = d->list.size();
	emit layoutAboutToBeChanged();
	d->list.insert(row, source);
	emit rowCountChanged(d->list.size());
	emit layoutChanged();
	emitDataChanged(row);
}

void PlaylistModel::setPlaylist(const Core::Playlist &list, int current) {
	d->list = list;
	reset();
	emit rowCountChanged(d->list.size());
	d->row = -2;
	setCurrentRow(current);
}
