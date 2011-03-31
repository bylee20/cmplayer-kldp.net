#include "pref_dialog.hpp"
#include "menu.hpp"
#include "ui_pref_dialog.h"
#include "dialogs.hpp"
#include "translator.hpp"
#include "info.hpp"
#include <QtCore/QMap>
#include <QtGui/QStyleFactory>
#include <QtCore/QCoreApplication>
#include <QtGui/QHeaderView>
#include <QtCore/QDebug>

struct Pref::Dialog::Data {
	Ui::Pref_Dialog ui;
	QButtonGroup *shortcuts;
	MouseGroup *dbl, *mdl, *whl;
//	static const QList<Core::MediaType> media;
};

class Pref::Dialog::MouseGroup : public QGroupBox {
	Q_DECLARE_TR_FUNCTIONS(Pref::Dialog::MouseGroup)
public:
	typedef QMap<Qt::KeyboardModifier, int> ItemMap;
	MouseGroup(const QString &title, QWidget *parent = 0)
	: QGroupBox(title, parent) {
		combo[Qt::NoModifier] = new QComboBox(this);
		combo[Qt::ControlModifier] = new QComboBox(this);
		combo[Qt::AltModifier] = new QComboBox(this);
		combo[Qt::ShiftModifier] = new QComboBox(this);

		check[Qt::NoModifier] = new QCheckBox(tr("Plain"), this);
#define MAKE_MOD_CHECK(mod)\
	check[mod] = new QCheckBox(QKeySequence(mod).toString(QKeySequence::NativeText), this);
		MAKE_MOD_CHECK(Qt::ControlModifier);
		MAKE_MOD_CHECK(Qt::AltModifier);
		MAKE_MOD_CHECK(Qt::ShiftModifier);
#undef MAKE_MOD_CHECK

		QGridLayout *grid = new QGridLayout(this);
		grid->addWidget(check[Qt::NoModifier], 0, 0, 1, 1);
		grid->addWidget(check[Qt::ControlModifier], 1, 0, 1, 1);
		grid->addWidget(check[Qt::AltModifier], 2, 0, 1, 1);
		grid->addWidget(check[Qt::ShiftModifier], 3, 0, 1, 1);
		grid->addWidget(combo[Qt::NoModifier], 0, 1, 1, 1);
		grid->addWidget(combo[Qt::ControlModifier], 1, 1, 1, 1);
		grid->addWidget(combo[Qt::AltModifier], 2, 1, 1, 1);
		grid->addWidget(combo[Qt::ShiftModifier], 3, 1, 1, 1);

		QMap<Qt::KeyboardModifier, QComboBox*>::const_iterator it;
		for (it = combo.begin(); it != combo.end(); ++it) {
			QComboBox *combo = it.value();
			combo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
			connect(check[it.key()], SIGNAL(toggled(bool)), combo, SLOT(setEnabled(bool)));
		}
	}
	void addItems(const QList<QPair<QString, int> > &items) {
		QMap<Qt::KeyboardModifier, QComboBox*>::const_iterator it;
		for (it = combo.begin(); it != combo.end(); ++it) {
			for (int i=0; i<items.size(); ++i)
				it.value()->addItem(items[i].first, items[i].second);
		}
	}
	template<typename T>
	void setValues(const T &map) {
		typename T::const_iterator it = map.begin();
		for (; it != map.end(); ++it) {
			const typename T::mapped_type &pair = it.value();
			QComboBox *combo = this->combo[it.key()];
			const int idx = combo->findData(pair.second.value());
			if (idx != -1)
				combo->setCurrentIndex(idx);
			combo->setEnabled(pair.first);
			check[it.key()]->setChecked(pair.first);
		}
	}
	template<typename T>
	T values() const {
		T map;
		QMap<Qt::KeyboardModifier, QComboBox*>::const_iterator it;
		for (it = combo.begin(); it != combo.end(); ++it) {
			typename T::mapped_type pair;
			const QComboBox *combo = it.value();
			pair.first = combo->isEnabled();
			const int data = combo->itemData(combo->currentIndex()).toInt();
			pair.second = static_cast<const typename T::mapped_type::second_type::Type>(data);
			map[it.key()] = pair;
		}
		return map;
	}
private:
	QMap<Qt::KeyboardModifier, QComboBox*> combo;
	QMap<Qt::KeyboardModifier, QCheckBox*> check;
};


class Pref::Dialog::MenuTreeItem : public QTreeWidgetItem {
public:
	enum Column {Discription = 0, Shortcut1, Shortcut2, Shortcut3, Shortcut4};
	MenuTreeItem(int type = Type) : QTreeWidgetItem(type) {}
	MenuTreeItem(QTreeWidget *parent, QAction *action, int type = Type)
		: QTreeWidgetItem(parent, type) {init(action);}
	MenuTreeItem(QTreeWidget *parent, QMenu *menu, int type = Type)
		: QTreeWidgetItem(parent, type) {init(menu->menuAction());}
	MenuTreeItem(MenuTreeItem *parent, QAction *action, int type = Type)
		: QTreeWidgetItem(parent, type) {init(action);}
	MenuTreeItem(MenuTreeItem *parent, QMenu *menu, int type = Type)
		: QTreeWidgetItem(parent, type) {init(menu->menuAction());}
	void addChild(QAction *action){
		if (action->menu() || !Menu::key(action).isEmpty())
			new MenuTreeItem(this, action);
	}
	void addChildren(QMenu *menu){new MenuTreeItem(this, menu);}
	void addChildren(const QList<QAction *> &actions){
		foreach(QAction *act, actions) addChild(act);
	}
	void setAction(QAction *action){takeChildren(); init(action);}
	QAction *action() const {return m_action;}
	bool isMenu() const{return m_action->menu() != 0;}
	bool isSeparator() const{return m_action->isSeparator();}
	const QList<QKeySequence> &shortcuts() const {return m_shortcuts;}
	void setShortcut(Column column, const QKeySequence &shortcut) {
		m_shortcuts[column - 1] = shortcut;
		setText(column, shortcut.toString(QKeySequence::NativeText));
	}
	void applyShortcut() {
		for (int i=0; i<childCount(); ++i)
			static_cast<MenuTreeItem *>(child(i))->applyShortcut();
		m_action->setShortcuts(m_shortcuts);
	}
	void addActionTo(QMenu *parent){
		if (childCount()) {
			QMenu *menu = parent->addMenu(m_action->text());
			menu->setIcon(m_action->icon());
			for (int i=0; i<childCount(); ++i)
				static_cast<MenuTreeItem *>(child(i))->addActionTo(menu);
		} else if (isSeparator())
			parent->addSeparator();
		else
			parent->addAction(m_action);
	}
private:
	void init(QAction *action) {
		m_action = action;
		setIcon(Discription, action->icon());
		if(m_action->menu()) {
			setText(Discription, action->text());
			addChildren(m_action->menu()->actions());
		} else {
			m_shortcuts = m_action->shortcuts();
			while (m_shortcuts.size() < 4)
				m_shortcuts.append(QKeySequence());
			setText(Discription, action->text());
			for (int i=0; i<4; ++i)
				setText(i + 1, m_shortcuts[i].toString(QKeySequence::NativeText));
		}
	}
	QAction *m_action;
	QList<QKeySequence> m_shortcuts;
};

Pref::Dialog::Dialog(QWidget *parent)
: QDialog(parent), d(new Data) {
	d->ui.setupUi(this);
	d->dbl = new MouseGroup(tr("Double Click"), this);
	d->mdl = new MouseGroup(tr("Middle Button Click"), this);
	d->whl = new MouseGroup(tr("Wheel Scroll"), this);

	QVBoxLayout *vbox = static_cast<QVBoxLayout*>(d->ui.mouseWidget->layout());
	vbox->insertWidget(0, d->whl);
	vbox->insertWidget(0, d->mdl);
	vbox->insertWidget(0, d->dbl);

	d->shortcuts = new QButtonGroup(this);
	d->shortcuts->addButton(d->ui.shortcut1, MenuTreeItem::Shortcut1);
	d->shortcuts->addButton(d->ui.shortcut2, MenuTreeItem::Shortcut2);
	d->shortcuts->addButton(d->ui.shortcut3, MenuTreeItem::Shortcut3);
	d->shortcuts->addButton(d->ui.shortcut4, MenuTreeItem::Shortcut4);
	slotCurrentItemChanged(0);
	connect(d->shortcuts, SIGNAL(buttonClicked(int)), this, SLOT(getShortcut(int)));
	connect(d->ui.shortcutTree
			, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*))
			, this, SLOT(slotCurrentItemChanged(QTreeWidgetItem*)));

	const Pref &p = Pref::get();

	const LocaleList &locale = Translator::availableLocales();
	for (int i=0; i<locale.size(); ++i)
		d->ui.locale->addItem(toString(locale[i]), locale[i]);
	setComboIndex(d->ui.locale, p.locale);

	d->ui.autoAdd->addItem(tr("All files in the same path"), AllFiles);
	d->ui.autoAdd->addItem(tr("Files have similar name"), SimilarFiles);
	d->ui.autoAdd->addItem(tr("Do not add any other files"), DoNotAddFiles);
	setComboIndex(d->ui.autoAdd, p.autoAddFiles.value());

	const QStringList subExt = Info::subtitleExtension();
	d->ui.subSelectExt->addItem(tr("All"), QString());
	for (int i=0; i<subExt.size(); ++i)
		d->ui.subSelectExt->addItem(subExt[i], subExt[i]);
	setComboIndex(d->ui.subSelectExt, p.subtitleExtension);

	d->ui.pauseMinimized->setChecked(p.pauseMinimized);
	d->ui.askWhenRecordFound->setChecked(p.askWhenRecordFound);
	d->ui.pauseVideoOnly->setChecked(p.pauseVideoOnly);
	d->ui.startStopped->setChecked(p.rememberStopped);
	d->ui.hideCursor->setChecked(p.hideCursor);
	d->ui.hideDelay->setValue(p.hideDelay/1000);
	d->ui.tray->setChecked(p.enableSystemTray);
	d->ui.hideWhenClosed->setChecked(p.hideClosed);
	d->ui.singleApp->setChecked(p.singleApplication);
	d->ui.disableSS->setChecked(p.disableScreensaver);

	const QStringList styles = QStyleFactory::keys();
	d->ui.winStyle->addItem(tr("Default Style"), QString());
	for (int i = 0; i<styles.size(); ++i)
		d->ui.winStyle->addItem(styles[i], styles[i]);
	setComboIndex(d->ui.winStyle, p.windowStyle);

	QList<QAction *> acts = Menu::get().actions();
	for (int i=0; i<acts.size(); ++i)
		new MenuTreeItem(d->ui.shortcutTree, acts[i]);
	d->ui.shortcutTree->header()->resizeSection(0, 200);

	typedef QPair<QString, int> ItemPair;
	QList<ItemPair> items;
	items << ItemPair(tr("Open File"), OpenFile)
			<< ItemPair(tr("Toggle Full Screen"), ToggleFullScreen)
			<< ItemPair(tr("Toggle Play/Pause"), TogglePlayPause)
			<< ItemPair(tr("Toggle Mute"), ToggleMute);
	d->dbl->addItems(items);
	d->mdl->addItems(items);
	items.clear();
	items << ItemPair(tr("Seek 1 Step"), Seek1)
			<< ItemPair(tr("Seek 2 Step"), Seek2)
			<< ItemPair(tr("Seek 3 Step"), Seek3)
			<< ItemPair(tr("Play Next/Previous"), NextPrevious)
			<< ItemPair(tr("Volum Up/Down"), VolumeUpDown)
			<< ItemPair(tr("Amp. Up/Down"), AmpUpDown);
	d->whl->addItems(items);

	d->dbl->setValues(p.doubleClickMap);
	d->mdl->setValues(p.middleClickMap);
	d->whl->setValues(p.wheelScrollMap);

	d->ui.seek1->setValue(p.seekingStep1/1000);
	d->ui.seek2->setValue(p.seekingStep2/1000);
	d->ui.seek3->setValue(p.seekingStep3/1000);
	d->ui.speed->setValue(p.speedStep);
	d->ui.volume->setValue(p.volumeStep);
	d->ui.amp->setValue(p.ampStep);
	d->ui.subPos->setValue(p.subtitlePosStep);
	d->ui.sync->setValue(p.syncDelayStep*0.001);

	d->ui.autoLoad->addItem(tr("Subtitle whose name is matched the playing file name"), Matched);
	d->ui.autoLoad->addItem(tr("Subtitles whose names contain the playing file name"), Contain);
	d->ui.autoLoad->addItem(tr("Subtitles located in the path of the playing file"), SamePath);
	d->ui.autoLoad->addItem(tr("Do not load any subtitles automatically"), NoAutoLoad);
	setComboIndex(d->ui.autoLoad, p.subtitleAutoLoad.value());
	d->ui.autoSelect->addItem(tr("First subtitle of loaded ones"), FirstFile);
	d->ui.autoSelect->addItem(tr("Subtitle which has the same name as the playing file"), SameName);
	d->ui.autoSelect->addItem(tr("All loaded subtitles"), AllLoaded);
	d->ui.autoSelect->addItem(tr("Each language subtitle"), EachLanguage);
	setComboIndex(d->ui.autoSelect, p.subtitleAutoSelect.value());
	d->ui.encoding->setEncoding(p.subtitleEncoding);
	d->ui.subOsd->setStyle(p.subtitleStyle);
	d->ui.priority->setValues(p.subtitlePriority);
	d->ui.priority->setAddingAndErasingEnabled(true);
	d->ui.subEncAutoDet->setChecked(p.useSubtitleEncodingAutoDetection);
	d->ui.subConf->setValue(p.subtitleEncodingConfidence);
	d->ui.msPerChar->setValue(p.msPerChar);

	connect(d->ui.autoSelect, SIGNAL(currentIndexChanged(int))
			, this, SLOT(checkSubtitleSelect(int)));

	checkSubtitleSelect(d->ui.autoSelect->currentIndex());
}

Pref::Dialog::~Dialog() {
	delete d;
}

void Pref::Dialog::slotCurrentItemChanged(QTreeWidgetItem *it) {
	MenuTreeItem *item = (MenuTreeItem*)it;
	const QList<QAbstractButton*> buttons = d->shortcuts->buttons();
	for (int i=0; i<buttons.size(); ++i)
		buttons[i]->setEnabled(item && !item->isMenu());
}

void Pref::Dialog::getShortcut(int id) {
	MenuTreeItem *item = (MenuTreeItem*)(d->ui.shortcutTree->currentItem());
	if (!item || item->isMenu())
		return;
	GetShortcutDialog dlg(item->shortcuts()[id - 1], this);
	if (dlg.exec())
		item->setShortcut(MenuTreeItem::Column(id), dlg.shortcut());
}

void Pref::Dialog::setComboIndex(QComboBox *combo, const QVariant &value) {
	const int idx = combo->findData(value);
	if (idx != -1)
		combo->setCurrentIndex(idx);
}

QVariant Pref::Dialog::currentComboData(QComboBox *combo) {
	return combo->itemData(combo->currentIndex());
}

void Pref::Dialog::apply() {
	Pref &p = Pref::ref();
	p.locale = currentComboData(d->ui.locale).toLocale();
	p.autoAddFiles = (AutoAddFiles)currentComboData(d->ui.autoAdd).toInt();
	p.pauseMinimized = d->ui.pauseMinimized->isChecked();
	p.pauseVideoOnly = d->ui.pauseVideoOnly->isChecked();
	p.askWhenRecordFound = d->ui.askWhenRecordFound->isChecked();
	p.rememberStopped = d->ui.startStopped->isChecked();
	p.hideCursor = d->ui.hideCursor->isChecked();
	p.hideDelay = d->ui.hideDelay->value()*1000;
	p.enableSystemTray = d->ui.tray->isChecked();
	p.hideClosed = d->ui.hideWhenClosed->isChecked();
	p.singleApplication = d->ui.singleApp->isChecked();
	p.disableScreensaver = d->ui.disableSS->isChecked();
	p.windowStyle = currentComboData(d->ui.winStyle).toString();
	p.subtitleExtension = currentComboData(d->ui.subSelectExt).toString();

	for (int i=0; i<d->ui.shortcutTree->topLevelItemCount(); ++i)
		((MenuTreeItem*)(d->ui.shortcutTree->topLevelItem(i)))->applyShortcut();

	p.doubleClickMap = d->dbl->values<Pref::ClickActionMap>();
	p.middleClickMap = d->mdl->values<Pref::ClickActionMap>();
	p.wheelScrollMap = d->whl->values<Pref::WheelActionMap>();

	p.seekingStep1 = d->ui.seek1->value()*1000;
	p.seekingStep2 = d->ui.seek2->value()*1000;
	p.seekingStep3 = d->ui.seek3->value()*1000;
	p.speedStep = d->ui.speed->value();
	p.volumeStep = d->ui.volume->value();
	p.ampStep = d->ui.amp->value();
	p.subtitlePosStep = d->ui.subPos->value();
	p.syncDelayStep = qRound(d->ui.sync->value()*1000.0);

	p.subtitleAutoLoad = (SubtitleAutoLoad)currentComboData(d->ui.autoLoad).toInt();
	p.subtitleAutoSelect = (SubtitleAutoSelect)currentComboData(d->ui.autoSelect).toInt();
	p.subtitleEncoding = d->ui.encoding->encoding();
	p.subtitleStyle = d->ui.subOsd->style();
	p.subtitlePriority = d->ui.priority->values();
	p.useSubtitleEncodingAutoDetection = d->ui.subEncAutoDet->isChecked();
	p.subtitleEncodingConfidence = d->ui.subConf->value();
	p.msPerChar = d->ui.msPerChar->value();

	p.save();
}

void Pref::Dialog::accept() {
	apply();
	QDialog::accept();
}

void Pref::Dialog::checkSubtitleSelect(int index) {
	const bool enabled = d->ui.autoSelect->itemData(index).toInt() == SameName;
	d->ui.subExtLabel->setEnabled(enabled);
	d->ui.subSelectExt->setEnabled(enabled);
}

void Pref::Dialog::slotMediaItemClicked(QTreeWidgetItem *item, int column) {
	if (column)
		item->setCheckState(column, Qt::Checked);
}

QString Pref::Dialog::toString(const QLocale &locale) {
	QString text;
	bool addName = true;
	switch (locale.language()) {
	case QLocale::C:
		text = tr("System Default Locale");
		addName = false;
		break;
	case QLocale::English:
		text = tr("English");
		break;
	case QLocale::Japanese:
		text = tr("Japanese");
		break;
	case QLocale::Korean:
		text = tr("Korean");
		break;
	default:
		text = QLocale::languageToString(locale.language());
		break;
	}
	if (addName)
		text += " (" + locale.name() + ')';
	return text;
}

