#include "favoriteswidget.h"
#include "videoplayer.h"
#include <core/mrl.h>
#include <core/info.h>
#include <QtGui/QTreeWidget>
#include <QtCore/QSettings>
#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QMenu>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QCheckBox>

class FavoritesWidget::Item : public QTreeWidgetItem {
public:
	virtual bool isFolder() const = 0;
	QString name() const {return text(0);}
	void setName(const QString &name) {setText(0, name);}
};

class FavoritesWidget::FolderItem : public FavoritesWidget::Item {
	Q_DECLARE_TR_FUNCTIONS(FavoritesWidget::FolderItem)
public:
	FolderItem(const QString &name = QString()) {
		setName(name);
		setText(1, tr("Folder"));
	}
	bool isFolder() const {return true;}
};

class FavoritesWidget::MrlItem : public FavoritesWidget::Item {
	Q_DECLARE_TR_FUNCTIONS(FavoritesWidget::MrlItem)
public:
	MrlItem() {}
	MrlItem(const Core::Mrl &mrl, const QString &name = QString()) {
		setName(name);
		setMrl(mrl);
	}
	bool isFolder() const {return false;}
	void setMrl(const Core::Mrl &mrl) {
		if (m_mrl != mrl) {
			if (mrl.isPlaylist())
				setText(1, tr("Playlist"));
			else
				setText(1, tr("Media"));
			setText(2, mrl.location());
			m_mrl = mrl;
		}
	}
	const Core::Mrl &mrl() const {return m_mrl;}
private:
	Core::Mrl m_mrl;
};

class FavoritesWidget::ItemDialog : public QDialog {
	Q_DECLARE_TR_FUNCTIONS(FavoritesWidget::ItemDialog)
public:
	ItemDialog(Item *item, QWidget *parent)
	: QDialog(parent), m_folder(item->isFolder()) {init(item);}
	ItemDialog(bool folder, QWidget *parent)
	: QDialog(parent), m_folder(folder) {init();}
	bool isTopLevel() const {return m_top->isChecked();}
	bool isFolder() const {return m_folder;}
	void setLocation(const QString &location) {if (!m_folder) m_loc->setText(location);}
	void setName(const QString &name) {m_name->setText(name);}
	QString name() const {return m_name->text();}
	QString location() const {return m_folder ? QString() : m_loc->text();}
private:
	void init(Item *item = 0) {
		QGridLayout *grid = new QGridLayout;
		m_name = new QLineEdit(this);
		grid->addWidget(new QLabel(tr("Name"), this), 0, 0);
		grid->addWidget(m_name, 0, 1);
		if (!m_folder) {
			m_loc = new QLineEdit(this);
			grid->addWidget(new QLabel(tr("Location"), this), 1, 0);
			grid->addWidget(m_loc, 1, 1);
		}
		m_top = new QCheckBox(tr("Add to top level"), this);
		m_top->setVisible(!item);
		QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel
			, Qt::Horizontal, this);
		QVBoxLayout *vbox = new QVBoxLayout(this);
		vbox->addLayout(grid);
		vbox->addWidget(m_top);
		vbox->addWidget(buttons);
		connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
		connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
		if (item) {
			setName(item->name());
			if (!item->isFolder())
				setLocation(static_cast<MrlItem*>(item)->mrl().location());
		}
	}
	QLineEdit *m_name, *m_loc;
	QCheckBox *m_top;
	bool m_folder;
};

struct FavoritesWidget::Data {
	QTreeWidget *tree;
	QMenu *menu;
	VideoPlayer *player;
	QString config;
};

FavoritesWidget::FavoritesWidget(VideoPlayer *player, QWidget *parent)
: QWidget(parent), d(new Data) {
	setStyleSheet("\
		QTreeView::branch:has-children:!has-siblings:closed,\
		QTreeView::branch:closed:has-children:has-siblings {\
			border-image: none;\
			image: url(:/img/folder.png);\
		}\
		QTreeView::branch:open:has-children:!has-siblings,\
		QTreeView::branch:open:has-children:has-siblings {\
			border-image: none;\
			image: url(:/img/document-open-folder.png);\
		}"
	);
	setContextMenuPolicy(Qt::CustomContextMenu);
	d->player = player;
	d->tree = new QTreeWidget(this);
	d->config = Core::Info::privatePath() + "/favorites.ini";
	QVBoxLayout *vbox = new QVBoxLayout(this);
	vbox->addWidget(d->tree);

	d->menu = new QMenu(this);
	QAction *addNew = d->menu->addAction(tr("Add New Media"));
	QAction *addCur = d->menu->addAction(tr("Add Current Media"));
	QAction *addFolder = d->menu->addAction(tr("Add Folder"));
	QAction *modify = d->menu->addAction(tr("Modify"));
	QAction *erase = d->menu->addAction(tr("Erase"));

	d->tree->setHeaderLabels(QStringList() << tr("Name") << tr("Category") << tr("Location"));
	connect(addNew, SIGNAL(triggered()), this, SLOT(addNew()));
	connect(addCur, SIGNAL(triggered()), this, SLOT(addCurrent()));
	connect(addFolder, SIGNAL(triggered()), this, SLOT(addFolder()));
	connect(erase, SIGNAL(triggered()), this, SLOT(erase()));
	connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContext(QPoint)));
	connect(d->tree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int))
		, this, SLOT(slotDblClick(QTreeWidgetItem*)));
	connect(modify, SIGNAL(triggered()), this, SLOT(modify()));

	load();
}

FavoritesWidget::~FavoritesWidget() {
	save();
	delete d;
}

void FavoritesWidget::addItem(Item *item, bool topLevel) {
	QTreeWidgetItem *parent = d->tree->invisibleRootItem();
	if (!topLevel) {
		Item *item = static_cast<Item*>(d->tree->currentItem());
		if (item)
			parent = item->isFolder() ? item : item->parent();
	}
	if (!parent)
		parent = d->tree->invisibleRootItem();
	parent->addChild(item);
}

void FavoritesWidget::addNew() {
	ItemDialog dlg(false, this);
	if (dlg.exec())
		addItem(new MrlItem(dlg.location(), dlg.name()), dlg.isTopLevel());
}

void FavoritesWidget::addFolder() {
	ItemDialog dlg(true, this);
	if (dlg.exec())
		addItem(new FolderItem(dlg.name()), dlg.isTopLevel());
}

void FavoritesWidget::addCurrent() {
	ItemDialog dlg(false, this);
	const Core::Mrl mrl = d->player->currentSource().mrl();
	dlg.setName(mrl.isLocalFile() ? mrl.fileName() : mrl.toString());
	dlg.setLocation(mrl.location());
	if (dlg.exec())
		addItem(new MrlItem(dlg.location(), dlg.name()), dlg.isTopLevel());
}

void FavoritesWidget::erase() {
	delete d->tree->currentItem();
}

void FavoritesWidget::showContext(const QPoint &pos) {
	d->menu->exec(mapToGlobal(pos));
}

void FavoritesWidget::slotDblClick(QTreeWidgetItem *i) {
	if (!i)
		return;
	Item *item = static_cast<Item*>(i);
	if (item->isFolder())
		item->setExpanded(item->isExpanded());
	else
		emit openRequested(static_cast<MrlItem*>(item)->mrl());
}

void FavoritesWidget::modify(Item *item) {
	if (!item)
		item = static_cast<Item*>(d->tree->currentItem());
	if (item && item != d->tree->invisibleRootItem()) {
		ItemDialog dlg(item, this);
		if (dlg.exec()) {
			item->setName(dlg.name());
			if (!item->isFolder())
				static_cast<MrlItem*>(item)->setMrl(Core::Mrl(dlg.location()));
		}
	}
}

void FavoritesWidget::save() const {
	QSettings set(d->config, QSettings::IniFormat);
	set.beginGroup("Favorites");
	save(d->tree->invisibleRootItem(), &set);
	set.endGroup();
}

void FavoritesWidget::save(QTreeWidgetItem *item, QSettings *set) const {
	if (item != d->tree->invisibleRootItem()) {
		Item *itm = static_cast<Item*>(item);
		set->setValue("Name", itm->name());
		set->setValue("IsFolder", itm->isFolder());
		if (!itm->isFolder())
			set->setValue("Url", static_cast<MrlItem*>(itm)->mrl().url());
	}
	const int count = item->childCount();
	set->beginWriteArray("Items", count);
	for (int i=0; i<count; ++i) {
		set->setArrayIndex(i);
		save(item->child(i), set);
	}
	set->endArray();
}

void FavoritesWidget::load() {
	QSettings set(d->config, QSettings::IniFormat);
	set.beginGroup("Favorites");
	load(d->tree->invisibleRootItem(), &set);
	set.endGroup();;
}

void FavoritesWidget::load(QTreeWidgetItem *parent, QSettings *set) {
	const int count = set->beginReadArray("Items");
	for (int i=0; i<count; ++i) {
		set->setArrayIndex(i);
		const bool isFolder = set->value("IsFolder", false).toBool();
		const QString name = set->value("Name", QString()).toString();
		Item *item = 0;
		if (isFolder) {
			item = new FolderItem(name);
		} else {
			const Core::Mrl mrl = set->value("Url", QUrl()).toUrl();
			item = new MrlItem(mrl, name);
		}
		parent->addChild(item);
		load(item, set);
	}
	set->endArray();
}
