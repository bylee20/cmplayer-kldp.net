#ifndef SUBTITLECOMPONENTMODEL_HPP
#define SUBTITLECOMPONENTMODEL_HPP

#include "subtitle.hpp"
#include <QtCore/QAbstractItemModel>
#include <QtGui/QTreeView>

class SubtitleComponentModel : public QAbstractItemModel {
	Q_OBJECT
public:
	enum Column {Start = 0, End, Text, ColumnCount};
	SubtitleComponentModel(const Subtitle::Component *comp, QObject *parent = 0);
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent =  QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &/*child*/) const {return QModelIndex();}
	void setCurrentNode(const Subtitle::Node *node);
	int currentRow() const;
	bool isValidRow(int row) const;
	bool isValidColumn(int c) const {return 0 <= c && c < ColumnCount;}
	void setVisible(bool visible);
	QString name() const;
signals:
	void currentRowChanged(int row);
private:
	void emitDataChanged(int top, int bottom);
	void emitDataChanged(int row);
	typedef Subtitle::Component::const_iterator c_iterator;
	typedef Subtitle::Component::iterator iterator;
	class Item;
	struct Data;
	Data *d;
};

class SubtitleComponentView : public QTreeView {
	Q_OBJECT
public:
	SubtitleComponentView(QWidget *parent = 0);
	void setModel(QAbstractItemModel *model);
	void setAutoScrollEnabled(bool enabled);
	void setTimeVisible(bool visible);
private slots:
	void updateCurrentRow(int row);
	void setModelToNull();
private:

	void showEvent(QShowEvent *event);
	void hideEvent(QHideEvent *event);
	struct Data;
	Data *d;
};

#endif // SUBTITLEMODEL_HPP
