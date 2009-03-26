#ifndef PLAYLISTVIEW_H
#define PLAYLISTVIEW_H

#include <QtGui/QTableView>

class PlaylistModel;

class PlaylistView : public QTableView {
	Q_OBJECT
public:
	PlaylistView(QWidget *parent = 0);
	~PlaylistView();
	void setModel(PlaylistModel *model);
	QList<int> selectedRows() const;
private slots:
	void selectRows(const QList<int> &rows);
private:
	PlaylistModel *m_model;
};

#endif
