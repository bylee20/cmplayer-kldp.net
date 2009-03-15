#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QtCore/QAbstractTableModel>
#include <core/namespace.h>

namespace Core {
class MediaSource;				class Playlist;
class PlayEngine;
}

class VideoPlayer;

class PlaylistModel : public QAbstractTableModel {
	Q_OBJECT
public:
	PlaylistModel(VideoPlayer *player, QObject *parent = 0);
	~PlaylistModel();
	void setPlaylist(const Core::Playlist &list, int current = -1);
	void append(const QList<Core::MediaSource> &list);
	void append(const Core::MediaSource &source);
	void insert(int row, const Core::MediaSource &source);
	bool swap(int row1, int row2);
	void setSource(int row, const Core::MediaSource &source);
	void setCurrentFont(const QFont &font);
	Core::MediaSource source(int row) const;
	int currentRow() const;
	const Core::Playlist &playlist() const;
	bool isLoopEnabled() const;
	int row(const Core::MediaSource &source) const;
	const QFont &currentFont() const;
	int count() const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	Core::State state() const;
public slots:
	void setCurrentSource(const Core::MediaSource &source);
	void setCurrentRow(int row);
	void playNext();
	void playPrevious();
	void clear();
	void play(int row);
	void remove(int row);
	void setLoopEnabled(bool enabled);
signals:
	void playlistFinished();
	void currentRowChanged(int row);
	void rowCountChanged(int count);
private slots:
	void slotFinished(const Core::MediaSource &source);
private:
	void emitDataChanged(int row) {
		emit dataChanged(index(row, 0), index(row, columnCount()-1));
	}
	struct Data;
	Data *d;
};

#endif
