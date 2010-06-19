#include "playlistview.hpp"
#include "playengine.hpp"
#include "info.hpp"
#include <QtCore/QFileInfo>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include "playlist.hpp"
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include "pref.hpp"
#include <QtCore>
class PlaylistView::Item {
public:
	Item(const Mrl &mrl) {
		name = new QTableWidgetItem(mrl.fileName());
		location = new QTableWidgetItem(mrl.toString());
		const Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
		name->setFlags(flags);
		location->setFlags(flags);
	}
	void setCurrent(bool current) {
		QFont font = name->font();
		font.setBold(current);
		font.setItalic(current);
		name->setFont(font);
		location->setFont(font);
	}

	Item() {name = location = 0;}
	QTableWidgetItem *name, *location;
};

class PlaylistView::Table : public QTableWidget {
public:
	Table(QWidget *parent = 0): QTableWidget(parent) {
		setSelectionBehavior(SelectRows);
		setSelectionMode(ExtendedSelection);
		setAlternatingRowColors(true);
		setVerticalScrollMode(ScrollPerPixel);
		setHorizontalScrollMode(ScrollPerPixel);
//		setDragEnabled(true);
//		setAcceptDrops(true);
//		setDropIndicatorShown(true);
		setColumnCount(2);
		setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Location"));
	}
	void adjustHeaderSize() {
		resizeColumnsToContents();
		resizeRowsToContents();
	}

private:
//	QVariant
};

struct PlaylistView::Data {
	PlayEngine *engine;
	Table *table;
	Playlist list;
	QList<Item> item;
	int idx;
};

PlaylistView::PlaylistView(PlayEngine *engine, QWidget *parent)
: QWidget(parent), d(new Data) {
	d->engine = engine;
	d->idx = -1;
	d->table = new Table(this);

	QVBoxLayout *vbox = new QVBoxLayout(this);
	vbox->addWidget(d->table);

	connect(d->engine, SIGNAL(mrlChanged(Mrl)), this, SLOT(updateCurrentMrl(Mrl)));
	connect(d->engine, SIGNAL(finished()), this, SLOT(handleFinished()));
}

PlaylistView::~PlaylistView() {
	delete d;
}

void PlaylistView::load(const Mrl &mrl) {
	Playlist list;
	if (mrl.isPlaylist())
		list.load(mrl);
	else {
		const AutoAddFiles mode = Pref::get().autoAddFiles;
		if (mrl.isLocalFile() && mode != DoNotAddFiles) {
			const QFileInfo file(mrl.toLocalFile());
			QStringList filter = Info::videoExtension().toNameFilter()
				+ Info::audioExtension().toNameFilter();
			if (!filter.contains(file.suffix()))
				filter += ("*." + file.suffix());
			const QFileInfoList files = file.dir().entryInfoList(filter
					, QDir::Files, QDir::Name);
			const QString fileName = file.fileName();
			bool prefix = false, suffix = false;
			QFileInfoList::const_iterator it = files.begin();
			for(; it != files.end(); ++it) {
				if (mode != AllFiles) {
					static QRegExp rxs("(\\D*)\\d+(.*)");
					static QRegExp rxt("(\\D*)\\d+(.*)");
					if (rxs.indexIn(fileName) == -1)
						continue;
					if (rxt.indexIn(it->fileName()) == -1)
						continue;
					if (!prefix && !suffix) {
						if (rxs.cap(1) == rxt.cap(1))
							prefix = true;
						else if (rxs.cap(2) == rxt.cap(2))
							suffix = true;
						else
							continue;
					} else if (prefix) {
						if (rxs.cap(1) != rxt.cap(1))
							continue;
					} else if (suffix) {
						if (rxs.cap(2) != rxt.cap(2))
							continue;
					}
				}
				list.append(Mrl::fromLocalFile(it->absoluteFilePath()));
			}
		}
		if (list.isEmpty())
			list.append(mrl);
	}
	setPlaylist(list);
}

const Playlist &PlaylistView::playlist() const {
	return d->list;
}

void PlaylistView::setPlaylist(const Playlist &list) {
	d->list = list;
	d->item.clear();
	d->table->setRowCount(list.size());
	for (int i=0; i<d->list.size(); ++i) {
		Item item(d->list[i]);
		d->table->setItem(i, 0, item.name);
		d->table->setItem(i, 1, item.location);
		d->item.push_back(item);
	}
	d->table->adjustHeaderSize();
}

void PlaylistView::play(const Mrl &mrl) {
	d->engine->stop();
	d->engine->setMrl(mrl);
	d->engine->play();
}

void PlaylistView::playNext() {
	if (d->idx < 0)
		return;
	const int next = d->idx + 1;
	if (next < d->list.size())
		play(d->list[next]);
}

void PlaylistView::playPrevious() {
	if (d->idx < 0 || d->list.isEmpty())
		return;
	const int prev = d->idx - 1;
	if (prev >= 0)
		play(d->list[prev]);
}

void PlaylistView::setCurrentIndex(int idx) {
	if (d->idx == idx || idx >= d->item.size())
		return;
	if (d->idx >= 0)
		d->item[d->idx].setCurrent(false);
	if ((d->idx = idx) >= 0)
		d->item[d->idx].setCurrent(true);
}

void PlaylistView::updateCurrentMrl(const Mrl &mrl) {
	setCurrentIndex(d->list.indexOf(mrl));
}

void PlaylistView::handleFinished() {
	qDebug() << "finished!";
	if (d->idx == d->list.size()-1)
		emit finished();
	else
		playNext();
}
