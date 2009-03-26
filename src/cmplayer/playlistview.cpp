#include "playlistview.h"
#include "playlistmodel.h"
#include <QtCore/QDebug>

PlaylistView::PlaylistView(QWidget *parent)
: QTableView(parent), m_model(0) {
	setSelectionBehavior(SelectRows);
	setSelectionMode(ExtendedSelection);
	setAlternatingRowColors(true);
	setVerticalScrollMode(ScrollPerPixel);
	setHorizontalScrollMode(ScrollPerPixel);
	setDragEnabled(true);
	setAcceptDrops(true);
	setDropIndicatorShown(true);
}

PlaylistView::~PlaylistView() {
}

void PlaylistView::setModel(PlaylistModel *model) {
	if (m_model != model) {
		if (m_model)
			disconnect(m_model, SIGNAL(dropped(const QList<int>&))
					, this, SLOT(selectRows(const QList< int >&)));
		m_model = model;
		QTableView::setModel(m_model);
		if (m_model)
			connect(m_model, SIGNAL(dropped(const QList<int>&))
					, this, SLOT(selectRows(const QList< int >&)));
	}
}

void PlaylistView::selectRows(const QList<int> &rows) {
	if (!m_model)
		return;
	selectionModel()->clear();
	if (!rows.isEmpty()) {
		const QModelIndex begin = m_model->index(rows.first(), 0);
		const QModelIndex end = m_model->index(rows.last(), m_model->columnCount()-1);
		selectionModel()->select(QItemSelection(begin, end), QItemSelectionModel::Select);
	}
}

QList<int> PlaylistView::selectedRows() const {
	QModelIndexList indexes = selectionModel()->selectedIndexes();
	QSet<int> rows;
	for (int i=0; i<indexes.size(); ++i)
		rows << indexes[i].row();
	return rows.toList();
}
