//#ifndef MPLAYERPLAYLISTMODEL_H
//#define MPLAYERPLAYLISTMODEL_H
//
//#include <QAbstractTableModel>
//
//namespace Xine {
//class MediaSource;
//}
//
//class PlayListModel : public QAbstractTableModel {
//	Q_OBJECT
//public:
//	PlayListModel(MPlayer::PlayEngine *engine, QObject *parent);
//	~PlayListModel();
//	void setPlayList(const MPlayer::PlayList &list);
//	void addSources(const QList<MPlayer::MediaSource> &list);
//	void addSource(const MPlayer::MediaSource &source);
//	bool swap(int row1, int row2);
//	bool insert(int row, const MPlayer::MediaSource &source);
//	bool setMediaSource(int row, const MPlayer::MediaSource &source);
//	void setCurrentFont(const QFont &font);
//	MPlayer::MediaSource mediaSource(int row) const;
//	int currentRow() const;
//	MPlayer::MediaSource currentSource() const;
//	const MPlayer::PlayList &playList() const;
//	bool isLoopEnabled() const;
//	MPlayer::MediaSource source(int row) const;
//	int row(const MPlayer::MediaSource &source) const;
//	const QFont &currentFont() const;
//	int count() const;
//	int rowCount(const QModelIndex &parent = QModelIndex()) const;
//	int columnCount(const QModelIndex &parent = QModelIndex()) const;
//	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
//	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
//public slots:
//	void playNext();
//	void playPrevious();
//	void clear();
//	void play(int row);
//	void remove(int row);
//	void setLoopEnabled(bool enabled);
//	void setCurrentRow(int row);
//signals:
//	void playListFinished();
//	void currentRowChanged(int row);
//	void rowCountChanged(int count);
//private slots:
//	void slotFinished();
//private:
//	struct Data;
//	friend struct Data;
//	friend class ListUpdateThread;
//	Data *d;
//};
//
//#endif
