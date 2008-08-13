#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractTableModel>

namespace Xine {
class MediaSource;
class PlayList;
class XineStream;
}

class PlayListModel : public QAbstractTableModel {
	Q_OBJECT
public:
	PlayListModel(Xine::XineStream *stream, QObject *parent);
	~PlayListModel();
	void setPlayList(const Xine::PlayList &list);
	void addSources(const QList<Xine::MediaSource> &list);
	void addSource(const Xine::MediaSource &source);
	bool swap(int row1, int row2);
	bool insert(int row, const Xine::MediaSource &source);
	bool setMediaSource(int row, const Xine::MediaSource &source);
	void setCurrentFont(const QFont &font);
	Xine::MediaSource mediaSource(int row) const;
	int currentRow() const;
	Xine::MediaSource currentSource() const;
	const Xine::PlayList &playList() const;
	bool isLoopEnabled() const;
	Xine::MediaSource source(int row) const;
	int row(const Xine::MediaSource &source) const;
	const QFont &currentFont() const;
	int count() const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
public slots:
	void playNext();
	void playPrevious();
	void clear();
	void play(int row);
	void remove(int row);
	void setLoopEnabled(bool enabled);
	void setCurrentRow(int row);
signals:
	void playListFinished();
	void currentRowChanged(int row);
	void rowCountChanged(int count);
private slots:
	void slotFinished();
private:
	struct Data;
	friend struct Data;
	friend class ListUpdateThread;
	Data *d;
};

#endif
