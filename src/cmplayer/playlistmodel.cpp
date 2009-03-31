#include "playlistmodel.h"
#include "recentinfo.h"
#include <QtCore/QUrl>
#include <QtCore/QMimeData>
#include <QtCore/QSet>
#include <QtGui/QFont>
#include <QtCore/QDebug>
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
	connect(d->player, SIGNAL(currentSourceChanged(const Core::MediaSource&))
			, this, SLOT(slotCurrentSourceChanged(const Core::MediaSource&)));
	connect(d->player, SIGNAL(stateChanged(Core::State, Core::State))
			, this, SLOT(updateNext()));
	connect(this, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&))
			, this, SLOT(updateNext()));
	connect(this, SIGNAL(rowCountChanged(int)), this, SLOT(updateNext()));
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

bool PlaylistModel::isRowValid(int row) const {
	return !d->list.isEmpty() && 0 <= row && row < d->list.size();
}

int PlaylistModel::count() const {
	return d->list.size();
}

bool PlaylistModel::swap(int row1, int row2) {
	if (!isRowValid(row1) || !isRowValid(row2))
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
	if (isRowValid(row)) {
		if (d->row == row)
			setCurrentRow(-1);
		emit layoutAboutToBeChanged();
		d->list.removeAt(row);
		emit layoutChanged();
		emit rowCountChanged(d->list.size());
	}
}

void PlaylistModel::play(int row) {
	if (isRowValid(row)) {
		if (d->player->isStopped()) {
			setCurrentRow(row);
			d->player->play(RecentInfo::get()->stoppedTime(d->list[row]));
		} else {
			d->player->setNextSource(d->list[row]);
			if (d->player->hasNextSource())
				d->player->playNext(RecentInfo::get()->stoppedTime(d->list[row]));
		}
	}
}

void PlaylistModel::playNext() {
	play(d->row + 1);
}

void PlaylistModel::playPrevious() {
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
		if (d->loop && !d->list.isEmpty())
			play(0);
	}
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

void PlaylistModel::setCurrentRow(int row, bool setSource) {
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
		if (setSource)
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

void PlaylistModel::insert(int row, const QList<Core::MediaSource> &sources) {
	if (row < 0 || row >= d->list.size())
		row = d->list.size();
	const int begin = row;
	emit layoutAboutToBeChanged();
	for (int i=0; i<sources.size(); ++i) {
		d->list.insert(row, sources[i]);
		++row;
	}
	emit rowCountChanged(d->list.size());
	emit layoutChanged();
	emitDataChanged(begin, row);
}

void PlaylistModel::setPlaylist(const Core::Playlist &list, int current) {
	d->list = list;
	reset();
	emit rowCountChanged(d->list.size());
	d->row = -2;
	setCurrentRow(current);
}

Qt::DropActions PlaylistModel::supportedDropActions() const {
	return Qt::MoveAction;
}

Qt::ItemFlags PlaylistModel::flags(const QModelIndex &index) const {
	Qt::ItemFlags f = QAbstractTableModel::flags(index) | Qt::ItemIsDropEnabled;
	if (index.isValid())
		f |= Qt::ItemIsDragEnabled;
	return f;
}

QStringList PlaylistModel::mimeTypes() const {
	QStringList types;
	types << mimeType();
	return types;
}

QMimeData *PlaylistModel::mimeData(const QModelIndexList &indexes) const {
	QMimeData *mime = new QMimeData();
	QByteArray data;
	QDataStream stream(&data, QIODevice::WriteOnly);
	QSet<int> rows;
	for (int i=0; i<indexes.size(); ++i) {
		const int row = indexes[i].row();
		if (row != -1 && !rows.contains(row)) {
			stream << row << d->list[row].url() << (currentRow() == row);
			rows << row;
		}
	}
	mime->setData(mimeType(), data);
	return mime;
}

bool PlaylistModel::dropMimeData(const QMimeData *data, Qt::DropAction action
		, int row, int /*column*/, const QModelIndex &parent) {
	if (action == Qt::IgnoreAction)
		return true;
	if (!data->hasFormat(mimeType()))
		return false;
	if (row == -1)
		row = parent.isValid() ? parent.row() : rowCount();
	QByteArray encoded = data->data(mimeType());
	QDataStream stream(&encoded, QIODevice::ReadOnly);
	QMap<int, Core::MediaSource> list;
	int curRow = -1;
	while (!stream.atEnd()) {
		QUrl url;
		bool current = false;
		int r = -1;
		stream >> r >> url >> current;
		list.insert(r, Core::MediaSource(url));
		if (current)
			curRow = r;
	}
	if (!list.isEmpty()) {
		emit layoutAboutToBeChanged();
		QMapIterator<int, Core::MediaSource> it(list);
		it.toBack();
		int begin = row;
		while (it.hasPrevious()) {
			const int row = it.previous().key();
			d->list.removeAt(row);
			emitDataChanged(row);
			if (row < begin)
				--begin;
		}
		int end = begin;
		it.toFront();
		QList<int> rows;
		while (it.hasNext()) {
			it.next();
			d->list.insert(end, it.value());
			if (it.key() == curRow)
				setCurrentRow(end);
			rows.push_back(end);
			++end;
		}
		emit layoutChanged();
		emit rowCountChanged(d->list.size());
		emitDataChanged(begin, end);
		emit dropped(rows);
	}
	return true;
}

bool PlaylistModel::setData(const QModelIndex &index, const QVariant &value, int role) {
	return QAbstractTableModel::setData(index, value, role);
}

void PlaylistModel::updateNext() {
	const Core::MediaSource next = d->list.isEmpty()
			? Core::MediaSource() : d->list.value(d->row + 1);
	d->player->setNextSource(next);
}

void PlaylistModel::slotCurrentSourceChanged(const Core::MediaSource &source) {
	setCurrentRow(d->list.indexOf(source), false);
}

