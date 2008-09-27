#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QtCore/QAbstractTableModel>

namespace Backend {
class MediaSource;				class PlayList;
class PlayEngine;
}

class PlayListModel : public QAbstractTableModel {
	Q_OBJECT
public:
	PlayListModel(QObject *parent = 0);
	~PlayListModel();
	void setPlayEngine(Backend::PlayEngine *engine);
	void setPlayList(const Backend::PlayList &list);
	void addSources(const QList<Backend::MediaSource> &list);
	void addSource(const Backend::MediaSource &source);
	bool swap(int row1, int row2);
	bool insert(int row, const Backend::MediaSource &source);
	bool setMediaSource(int row, const Backend::MediaSource &source);
	void setCurrentFont(const QFont &font);
	Backend::MediaSource mediaSource(int row) const;
	int currentRow() const;
	Backend::MediaSource currentSource() const;
	const Backend::PlayList &playList() const;
	bool isLoopEnabled() const;
	Backend::MediaSource source(int row) const;
	int row(const Backend::MediaSource &source) const;
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
	void slotFinished(const Backend::MediaSource &source);
private:
	struct Data;
	friend struct Data;
	friend class ListUpdateThread;
	Data *d;
};

#endif
