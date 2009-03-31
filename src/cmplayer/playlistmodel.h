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
	void insert(int row, const QList<Core::MediaSource> &sources);
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
	Qt::DropActions supportedDropActions() const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QStringList mimeTypes() const;
	QMimeData *mimeData(const QModelIndexList &indexes) const;
	bool dropMimeData(const QMimeData *data, Qt::DropAction action
			, int row, int column, const QModelIndex &parent);
	bool setData(const QModelIndex &index, const QVariant &value, int role);
public slots:
	void setCurrentSource(const Core::MediaSource &source);
	void setCurrentRow(int row, bool setSource = true);
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
	void dropped(const QList<int> &row);
private slots:
// 	void slotCurrentSourceChanged(const Core::MediaSource &source);
	void slotFinished(const Core::MediaSource &source);
// 	void updateNext();
private:
	static QString mimeType() {
		return QString("application/net.xylosper.cmplayer.playlist");
	}
	void emitDataChanged(int from, int to = -1) {
		emit dataChanged(index(from, 0), index(to == -1 ? from : to, columnCount()-1));
	}
	struct Data;
	Data *d;
};

#endif
