#include "playlistview.hpp"
#include "playengine.hpp"
#include "info.hpp"
#include "playlist.hpp"
#include "dialogs.hpp"
#include "pref.hpp"
#include "controls.hpp"
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMenu>

class PlaylistView::Item {
public:
	Item(const Mrl &mrl) {
		name = new QTableWidgetItem(mrl.displayName());
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
	void setMrl(const Mrl &mrl) {
		name->setText(mrl.displayName());
		location->setText(mrl.toString());
	}

	Item() {name = location = 0;}
	QTableWidgetItem *name, *location;
};

class PlaylistView::Table : public QTableWidget {
public:
	Table(QWidget *parent = 0): QTableWidget(parent) {
		setSelectionBehavior(SelectRows);
//		setSelectionMode(ExtendedSelection);
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
	QMenu *context;
};

PlaylistView::PlaylistView(PlayEngine *engine, QWidget *parent)
: QWidget(parent), d(new Data) {
	d->engine = engine;
	d->idx = -1;
	d->table = new Table(this);
	d->table->setContextMenuPolicy(Qt::CustomContextMenu);

	d->context = new QMenu(this);
	QAction *open = d->context->addAction(tr("Open"));
	QAction *save = d->context->addAction(tr("Save"));
	QAction *add = d->context->addAction(tr("Add"));
	QAction *erase = d->context->addAction(tr("Erase"));
	QAction *up = d->context->addAction(tr("Up"));
	QAction *down = d->context->addAction(tr("Down"));
	QAction *clear = d->context->addAction(tr("Clear"));

	connect(open, SIGNAL(triggered()), this, SLOT(open()));
	connect(save, SIGNAL(triggered()), this, SLOT(save()));
	connect(add, SIGNAL(triggered()), this, SLOT(add()));
	connect(erase, SIGNAL(triggered()), this, SLOT(erase()));
	connect(up, SIGNAL(triggered()), this, SLOT(up()));
	connect(down, SIGNAL(triggered()), this, SLOT(down()));
	connect(clear, SIGNAL(triggered()), this, SLOT(clear()));

	connect(d->table, SIGNAL(customContextMenuRequested(QPoint))
		, this, SLOT(showContextMenu(QPoint)));

	QVBoxLayout *vbox = new QVBoxLayout(this);
	vbox->addWidget(d->table);

	connect(d->engine, SIGNAL(mrlChanged(Mrl)), this, SLOT(updateCurrentMrl(Mrl)));
	connect(d->engine, SIGNAL(finished(Mrl)), this, SLOT(handleFinished()));
	connect(d->table, SIGNAL(cellDoubleClicked(int,int))
		, this, SLOT(handleDoubleClick(int,int)));
}

PlaylistView::~PlaylistView() {
	delete d;
}

void PlaylistView::add() {
	const QString filter = Info::mediaExtensionFilter();
	QStringList files = QFileDialog::getOpenFileNames(this, tr("Open File"), QString(), filter);
	const int from = d->table->rowCount();
	d->table->setRowCount(from + files.size());
	for (int i=0; i<files.size(); ++i) {
		const Mrl mrl = Mrl::fromLocalFile(files[i]);
		d->list << mrl;
		Item item(mrl);
		d->table->setItem(from + i, 0, item.name);
		d->table->setItem(from + i, 1, item.location);
	}
}

void PlaylistView::erase() {
	const int row = d->table->currentRow();
	if (isValidRow(row)) {
		d->table->removeRow(row);
		d->item.removeAt(row);
		d->list.removeAt(row);
		if (row == d->idx)
			setCurrentIndex(-1);
	}
}

void PlaylistView::up() {
	move(true);
}

void PlaylistView::down() {
	move(false);
}

bool PlaylistView::isValidRow(int row) {
	return 0 <= row && row < d->list.size();
}

void PlaylistView::move(bool up) {
	const int row = d->table->currentRow();
	const int target = up ? row-1 : row + 1;
	if (isValidRow(row) && isValidRow(target)) {
		d->list.swap(row, target);
		d->item[row].setMrl(d->list[row]);
		d->item[target].setMrl(d->list[target]);
		if (row == d->idx) {
			setCurrentIndex(target);
		} else if (target == d->idx)
			setCurrentIndex(row);
	}
}

void PlaylistView::clear() {
	d->list.clear();
	d->item.clear();
	d->idx = -1;
	d->table->clear();
}

void PlaylistView::open() {
	QString enc;
	const QString filter = tr("Playlist") +' '+ Info::playlistExtension().toFilter();
	const QString file = EncodingFileDialog::getOpenFileName(this
		, tr("Open File"), QString(), filter, &enc);
	if (!file.isEmpty()) {
		Playlist list;
		list.load(file, enc);
		setPlaylist(list);
	}
}

void PlaylistView::save() {
	if (d->list.isEmpty())
		return;
	QString file = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
			tr("Playlist") + " (*.pls)");
	if (!file.isEmpty()) {
		if (QFileInfo(file).suffix().compare("pls", Qt::CaseInsensitive) != 0)
			file += ".pls";
		d->list.save(file);
	}
}

void PlaylistView::showContextMenu(const QPoint &/*pos*/) {
	d->context->exec(QCursor::pos());
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
	d->idx = -1;
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
	if (d->idx == d->list.size()-1)
		emit finished();
	else
		playNext();
}

void PlaylistView::handleDoubleClick(int row, int /*column*/) {
	if (row < 0 || row >= d->list.size())
		return;
	play(d->list[row]);
}
