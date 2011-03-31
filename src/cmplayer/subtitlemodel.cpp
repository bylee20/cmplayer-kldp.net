#include "subtitlemodel.hpp"
#include <QtGui/QFont>
#include <QtGui/QApplication>
#include <QtCore/QDebug>
#include <QtCore/QTime>
#include <QtCore/QVector>
#include <QtGui/QHeaderView>

struct SubtitleComponentModel::Item {
	Item(): m_end(-1) {}
	Item(c_iterator it): m_end(-1), m_it(it) {}
	int start() {return m_it.key();}
	int end() {return m_end;}
	QString text() const {return m_it->text.toPlain();}
// private:
	int m_end;
	c_iterator m_it;
};

struct SubtitleComponentModel::Data {
	QVector<Item> item;
	const QTime zero;
	int curRow;
	QFont curFont;
	bool visible;
	const Subtitle::Component *comp;
	const Subtitle::Node *pended;
};

SubtitleComponentModel::SubtitleComponentModel(const Subtitle::Component *comp, QObject *parent)
: QAbstractItemModel(parent), d(new Data) {
	d->comp = comp;
	d->curRow = -1;
	d->visible = false;
	d->pended = 0;
	d->curFont = QApplication::font();
	d->curFont.setBold(true);
	d->curFont.setItalic(true);

	d->item.reserve(comp->size());
	c_iterator it = comp->begin();
	for (; it != comp->end(); ++it) {
		if (it->text.hasWords()) {
			it->index = 0;
			d->item.push_back(it);
			break;
		}
	}
	if (!d->item.isEmpty()) {
		for (; it != comp->end(); ++it) {
			if (d->item.last().m_end < 0)
				d->item.last().m_end = it.key();
			if (it->text.hasWords())
				d->item.push_back(it);
			it->index = d->item.size()-1;
		}
	}
}

QString SubtitleComponentModel::name() const {
	return d->comp->name();
}

int SubtitleComponentModel::rowCount(const QModelIndex &parent) const {
	return parent.isValid() ? 0 : d->item.size();
}

int SubtitleComponentModel::columnCount(const QModelIndex &parent) const {
	return parent.isValid() ? 0 : ColumnCount;
}

QVariant SubtitleComponentModel::headerData(int s, Qt::Orientation o, int role) const {
	if (role != Qt::DisplayRole)
		return QVariant();
	if (o == Qt::Horizontal) {
		switch (s) {
		case Start:
			return tr("Start");
		case End:
			return tr("End");
		case Text:
			return tr("Text");
		default:
			return QVariant();
		}
	} else if (o == Qt::Vertical && isValidRow(s)) {
		return s;
	}
	return QVariant();
}

QModelIndex SubtitleComponentModel::index(int row, int column, const QModelIndex &parent) const {
	if (parent.isValid() || !isValidRow(row) || !isValidColumn(column))
		return QModelIndex();
	return createIndex(row, column);
}

QVariant SubtitleComponentModel::data(const QModelIndex &index, int role) const {
	const int row = index.row();
	const int column = index.column();
	if (row < 0 || row >= d->item.size() || column < 0 || column >= 3)
		return QVariant();
	if (role == Qt::DisplayRole) {
		switch (column) {
		case Start:
			return d->zero.addMSecs(d->item[row].start()).toString("hh:mm:ss");
		case End: {
			return d->zero.addMSecs(d->item[row].end()).toString("hh:mm:ss");
		} case Text:
			return d->item[row].text();
		default:
			return QVariant();
		}
	} else if (role == Qt::FontRole && column == Text && row == d->curRow)
		return d->curFont;
	return QVariant();
}

void SubtitleComponentModel::emitDataChanged(int top, int bottom) {
	emit dataChanged(index(top, 0), index(bottom, ColumnCount - 1));
}

void SubtitleComponentModel::emitDataChanged(int row) {
	emitDataChanged(row, row);
}

bool SubtitleComponentModel::isValidRow(int row) const {
	return 0 <= row && row < d->item.size();
}

void SubtitleComponentModel::setVisible(bool visible) {
	if (d->visible != visible) {
		d->visible = visible;
		if (d->visible && d->pended)
			setCurrentNode(d->pended);
	}
}

void SubtitleComponentModel::setCurrentNode(const Subtitle::Node *node) {
	if (!d->visible) {
		d->pended = node;
	} else {
		d->pended = 0;
		const int row = node ? node->index : -1;
		if (d->curRow == row)
			return;
		const int old = d->curRow;
		d->curRow = row;
		if (d->curRow == old + 1 && isValidRow(old) && isValidRow(d->curRow))
			emitDataChanged(old, d->curRow);
		else {
			if (isValidRow(old))
				emitDataChanged(old);
			if (isValidRow(d->curRow))
				emitDataChanged(d->curRow);
		}
		emit currentRowChanged(d->curRow);
	}
}

int SubtitleComponentModel::currentRow() const {
	return d->curRow;
}

/**********************************************************************/

struct SubtitleComponentView::Data {
	SubtitleComponentModel *model;
	bool autoScroll;
};

SubtitleComponentView::SubtitleComponentView(QWidget *parent)
: QTreeView(parent), d(new Data) {
	d->model = 0;
	d->autoScroll = false;
	setAlternatingRowColors(true);
	setRootIsDecorated(false);
}

void SubtitleComponentView::setModelToNull() {
	if (sender() == d->model)
		d->model = 0;
}

void SubtitleComponentView::setModel(QAbstractItemModel *model) {
	if (d->model)
		d->model->disconnect(this);
	d->model = qobject_cast<SubtitleComponentModel*>(model);
	QTreeView::setModel(d->model);
	if (d->model) {
		connect(d->model, SIGNAL(destroyed()), this, SLOT(setModelToNull()));
		connect(d->model, SIGNAL(currentRowChanged(int)), this, SLOT(updateCurrentRow(int)));
		d->model->setVisible(isVisible());
		for (int i=0; i<SubtitleComponentModel::ColumnCount; ++i)
			resizeColumnToContents(i);
	}
}

void SubtitleComponentView::updateCurrentRow(int row) {
	if (!d->model || !d->autoScroll)
		return;
	const QModelIndex idx = d->model->index(row, SubtitleComponentModel::Text);
	if (idx.isValid())
		scrollTo(idx);
}

void SubtitleComponentView::setAutoScrollEnabled(bool enabled) {
	if (d->autoScroll != enabled) {
		d->autoScroll = enabled;
		if (d->autoScroll && d->model)
			updateCurrentRow(d->model->currentRow());
	}
}

void SubtitleComponentView::setTimeVisible(bool visible) {
	setColumnHidden(SubtitleComponentModel::Start, !visible);
	setColumnHidden(SubtitleComponentModel::End, !visible);
}

void SubtitleComponentView::showEvent(QShowEvent *event) {
	QTreeView::showEvent(event);
	if (d->model)
		d->model->setVisible(true);
}

void SubtitleComponentView::hideEvent(QHideEvent *event) {
	QTreeView::hideEvent(event);
	if (d->model)
		d->model->setVisible(false);
}
