#include "pref_widget.hpp"
#include "translator.hpp"
#include "info.hpp"
#include "dialogs.hpp"
#include <QtGui/QColorDialog>
#include <QtGui/QFontDialog>
#include <QtGui/QStyleFactory>
#include <QtGui/QGroupBox>
#include "ui_pref_widget.h"
#include "menu.hpp"

struct Pref::Widget::Data {
	Ui::Pref_Widget ui;
	QButtonGroup *shortcuts;
	MouseGroup *dbl, *mdl, *whl;

};

class Pref::Widget::MouseGroup : public QWidget {
	Q_DECLARE_TR_FUNCTIONS(Pref::Widget::MouseGroup)
public:
	typedef QMap<Qt::KeyboardModifier, int> ItemMap;
	MouseGroup(QWidget *parent = 0)
	: QWidget(parent) {
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

		grid->setMargin(0);

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


class Pref::Widget::MenuTreeItem : public QTreeWidgetItem {
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

Pref::Widget::Widget(QWidget *parent)
: QWidget(parent), d(new Data) {
	d->ui.setupUi(this);

	d->dbl = new MouseGroup;
	d->mdl = new MouseGroup;
	d->whl = new MouseGroup;
	QFrame *line = 0;
	d->ui.mouse_form->addRow(tr("Double Click:"), d->dbl);
	line = new QFrame;
	line->setFrameShadow(QFrame::Plain);
	line->setFrameShape(QFrame::HLine);
	d->ui.mouse_form->addRow(line);
	d->ui.mouse_form->addRow(tr("Middle Click:"), d->mdl);
	line = new QFrame;
	line->setFrameShadow(QFrame::Plain);
	line->setFrameShape(QFrame::HLine);
	d->ui.mouse_form->addRow(line);
	d->ui.mouse_form->addRow(tr("Wheel Scroll:"), d->whl);

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
	setComboIndex(d->ui.autoAdd, p.auto_add_files.value());

	const QStringList subExt = Info::subtitleExt();
	d->ui.subSelectExt->addItem(tr("All"), QString());
	for (int i=0; i<subExt.size(); ++i)
		d->ui.subSelectExt->addItem(subExt[i], subExt[i]);
	setComboIndex(d->ui.subSelectExt, p.sub_ext);

	d->ui.pauseMinimized->setChecked(p.pause_minimized);
	d->ui.askWhenRecordFound->setChecked(p.ask_record_found);
	d->ui.pauseVideoOnly->setChecked(p.pause_video_only);
	d->ui.startStopped->setChecked(p.remember_stopped);
	d->ui.hideCursor->setChecked(p.hide_cursor);
	d->ui.hideDelay->setValue(p.hide_delay/1000);
	d->ui.tray->setChecked(p.enable_system_tray);
	d->ui.hideWhenClosed->setChecked(p.hide_rather_close);
	d->ui.singleApp->setChecked(p.single_app);
	d->ui.disableSS->setChecked(p.disable_screensaver);

	d->ui.blur_c->setValue(p.blur_kern_c);
	d->ui.blur_n->setValue(p.blur_kern_n);
	d->ui.blur_d->setValue(p.blur_kern_d);
	d->ui.sharpen_c->setValue(p.sharpen_kern_c);
	d->ui.sharpen_n->setValue(p.sharpen_kern_n);
	d->ui.sharpen_d->setValue(p.sharpen_kern_d);
	d->ui.min_luma->setValue(p.adjust_contrast_min_luma);
	d->ui.max_luma->setValue(p.adjust_contrast_max_luma);
	d->ui.auto_contrast_th->setValue(p.auto_contrast_threshold);

	const QStringList styles = QStyleFactory::keys();
	d->ui.winStyle->addItem(tr("Default Style"), QString());
	for (int i = 0; i<styles.size(); ++i)
		d->ui.winStyle->addItem(styles[i], styles[i]);
	setComboIndex(d->ui.winStyle, p.window_style);

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

	d->dbl->setValues(p.double_click_map);
	d->mdl->setValues(p.middle_click_map);
	d->whl->setValues(p.wheel_scroll_map);

	d->ui.seek1->setValue(p.seek_step1/1000);
	d->ui.seek2->setValue(p.seek_step2/1000);
	d->ui.seek3->setValue(p.seek_step3/1000);
	d->ui.speed->setValue(p.speed_step);
	d->ui.volume->setValue(p.volume_step);
	d->ui.amp->setValue(p.amp_step);
	d->ui.subPos->setValue(p.sub_pos_step);
	d->ui.sync->setValue(p.sync_delay_step*0.001);

	d->ui.autoLoad->addItem(tr("Subtitle whose name is matched the playing file name"), Matched);
	d->ui.autoLoad->addItem(tr("Subtitles whose names contain the playing file name"), Contain);
	d->ui.autoLoad->addItem(tr("Subtitles located in the path of the playing file"), SamePath);
	d->ui.autoLoad->addItem(tr("Do not load any subtitles automatically"), NoAutoLoad);
	setComboIndex(d->ui.autoLoad, p.sub_autoload.value());
	d->ui.autoSelect->addItem(tr("First subtitle of loaded ones"), FirstFile);
	d->ui.autoSelect->addItem(tr("Subtitle which has the same name as the playing file"), SameName);
	d->ui.autoSelect->addItem(tr("All loaded subtitles"), AllLoaded);
	d->ui.autoSelect->addItem(tr("Each language subtitle"), EachLanguage);
	setComboIndex(d->ui.autoSelect, p.sub_autoselect.value());
	d->ui.encoding->setEncoding(p.sub_enc);
	d->ui.priority->setValues(p.sub_priority);
	d->ui.priority->setAddingAndErasingEnabled(true);
	d->ui.subEncAutoDet->setChecked(p.sub_enc_autodetection);
	d->ui.subConf->setValue(p.sub_enc_confidence);
	d->ui.msPerChar->setValue(p.ms_per_char);

	d->ui.sub_font_label->setFont(p.sub_style.font);
	d->ui.sub_font_label->setText(p.sub_style.font.family());
	setColorLabel(d->ui.sub_color_fg_label, p.sub_style.color_fg);
	setColorLabel(d->ui.sub_color_bg_label, p.sub_style.color_bg);
	d->ui.sub_auto_size->addItem(tr("Fit to Diagonal"), OsdStyle::FitToDiagonal);
	d->ui.sub_auto_size->addItem(tr("Fit to Height"), OsdStyle::FitToHeight);
	d->ui.sub_auto_size->addItem(tr("Fit to Width"), OsdStyle::FitToWidth);
	setComboIndex(d->ui.sub_auto_size, p.sub_style.auto_size);
	d->ui.sub_size_scale->setValue(p.sub_style.text_scale*100.);

	connect(d->ui.sub_font_button, SIGNAL(clicked()), this, SLOT(setSubFont()));
	connect(d->ui.sub_color_fg_button, SIGNAL(clicked()), this, SLOT(setSubColor()));
	connect(d->ui.sub_color_bg_button, SIGNAL(clicked()), this, SLOT(setSubColor()));

	connect(d->ui.autoSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(checkSubtitleSelect(int)));
	checkSubtitleSelect(d->ui.autoSelect->currentIndex());
}

Pref::Widget::~Widget() {
	delete d;
}

QSize Pref::Widget::sizeHint() const {
	int width = 0;
	int height = 0;
	for (int i=0; i<d->ui.stack->count(); ++i) {
		const QSize hint = d->ui.stack->widget(i)->sizeHint();
		width = qMax(width, hint.width());
		height = qMax(height, hint.height());
	}
	return QSize(width, height);
}

void Pref::Widget::setColorLabel(QLabel *label, const QColor &color) {
	QPalette p = label->palette();
	p.setColor(QPalette::Window, color);
	p.setColor(QPalette::Button, color);
#define INVERT(value) (255 - value)
	const QColor inverted(INVERT(color.red()), INVERT(color.green()), INVERT(color.blue()));
#undef INVERT
	p.setColor(QPalette::WindowText, inverted);
	p.setColor(QPalette::ButtonText, inverted);
	label->setPalette(p);
	label->setText(color.name());
}

void Pref::Widget::setSubColor() {
	QLabel *label = 0;
	if (sender() == d->ui.sub_color_fg_button)
		label = d->ui.sub_color_fg_label;
	else if (sender() == d->ui.sub_color_bg_button)
		label = d->ui.sub_color_bg_label;
	else
		return;
	const QColor before = label->palette().color(QPalette::Window);
	const QColor color = QColorDialog::getColor(before, this);
	if (color.isValid())
		setColorLabel(label, color);
}

void Pref::Widget::setSubFont() {
	bool ok = false;
	const QFont before = d->ui.sub_font_label->font();
	QFont font = QFontDialog::getFont(&ok, before, this, QString(), QFontDialog::DontUseNativeDialog);
	if (ok) {
		font.setPointSize(before.pointSize());
		font.setPixelSize(before.pixelSize());
		d->ui.sub_font_label->setFont(font);
		d->ui.sub_font_label->setText(font.family());
	}
}

//const OsdStyle &OsdStyle::Widget::style() const {
//	const int scale = d->ui.scale->currentIndex();
//	if (scale != -1)
//		d->style.scale = (OsdStyle::AutoSize)(d->ui.scale->itemData(scale).toInt());
//	d->style.text_scale = d->ui.size->value()/100.;
//	return d->style;
//}


void Pref::Widget::slotCurrentItemChanged(QTreeWidgetItem *it) {
	MenuTreeItem *item = (MenuTreeItem*)it;
	const QList<QAbstractButton*> buttons = d->shortcuts->buttons();
	for (int i=0; i<buttons.size(); ++i)
		buttons[i]->setEnabled(item && !item->isMenu());
}

void Pref::Widget::getShortcut(int id) {
	MenuTreeItem *item = (MenuTreeItem*)(d->ui.shortcutTree->currentItem());
	if (!item || item->isMenu())
		return;
	GetShortcutDialog dlg(item->shortcuts()[id - 1], this);
	if (dlg.exec())
		item->setShortcut(MenuTreeItem::Column(id), dlg.shortcut());
}

void Pref::Widget::setComboIndex(QComboBox *combo, const QVariant &value) {
	const int idx = combo->findData(value);
	if (idx != -1)
		combo->setCurrentIndex(idx);
}

QVariant Pref::Widget::currentComboData(QComboBox *combo) {
	return combo->itemData(combo->currentIndex());
}

void Pref::Widget::apply() {
	Pref &p = Pref::ref();
	p.locale = currentComboData(d->ui.locale).toLocale();
	p.auto_add_files = (AutoAddFiles)currentComboData(d->ui.autoAdd).toInt();
	p.pause_minimized = d->ui.pauseMinimized->isChecked();
	p.pause_video_only = d->ui.pauseVideoOnly->isChecked();
	p.ask_record_found = d->ui.askWhenRecordFound->isChecked();
	p.remember_stopped = d->ui.startStopped->isChecked();
	p.hide_cursor = d->ui.hideCursor->isChecked();
	p.hide_delay = d->ui.hideDelay->value()*1000;

	p.blur_kern_c = d->ui.blur_c->value();
	p.blur_kern_n = d->ui.blur_n->value();
	p.blur_kern_d = d->ui.blur_d->value();
	p.sharpen_kern_c = d->ui.sharpen_c->value();
	p.sharpen_kern_n = d->ui.sharpen_n->value();
	p.sharpen_kern_d = d->ui.sharpen_d->value();
	p.adjust_contrast_min_luma = d->ui.min_luma->value();
	p.adjust_contrast_max_luma = d->ui.max_luma->value();
	p.auto_contrast_threshold = d->ui.auto_contrast_th->value();

	p.enable_system_tray = d->ui.tray->isChecked();
	p.hide_rather_close = d->ui.hideWhenClosed->isChecked();
	p.single_app = d->ui.singleApp->isChecked();
	p.disable_screensaver = d->ui.disableSS->isChecked();
	p.window_style = currentComboData(d->ui.winStyle).toString();
	p.sub_ext = currentComboData(d->ui.subSelectExt).toString();

	for (int i=0; i<d->ui.shortcutTree->topLevelItemCount(); ++i)
		((MenuTreeItem*)(d->ui.shortcutTree->topLevelItem(i)))->applyShortcut();

	p.double_click_map = d->dbl->values<Pref::ClickActionMap>();
	p.middle_click_map = d->mdl->values<Pref::ClickActionMap>();
	p.wheel_scroll_map = d->whl->values<Pref::WheelActionMap>();

	p.seek_step1 = d->ui.seek1->value()*1000;
	p.seek_step2 = d->ui.seek2->value()*1000;
	p.seek_step3 = d->ui.seek3->value()*1000;
	p.speed_step = d->ui.speed->value();
	p.volume_step = d->ui.volume->value();
	p.amp_step = d->ui.amp->value();
	p.sub_pos_step = d->ui.subPos->value();
	p.sync_delay_step = qRound(d->ui.sync->value()*1000.0);

	p.sub_autoload = (SubtitleAutoLoad)currentComboData(d->ui.autoLoad).toInt();
	p.sub_autoselect = (SubtitleAutoSelect)currentComboData(d->ui.autoSelect).toInt();
	p.sub_enc = d->ui.encoding->encoding();

	p.sub_style.font = d->ui.sub_font_label->font();
	p.sub_style.color_fg = d->ui.sub_color_fg_label->palette().color(QPalette::Window);
	p.sub_style.color_bg = d->ui.sub_color_bg_label->palette().color(QPalette::Window);
	p.sub_style.auto_size = (OsdStyle::AutoSize)currentComboData(d->ui.sub_auto_size).toInt();
	p.sub_style.text_scale = d->ui.sub_size_scale->value()/100.0;

	p.sub_priority = d->ui.priority->values();
	p.sub_enc_autodetection = d->ui.subEncAutoDet->isChecked();
	p.sub_enc_confidence = d->ui.subConf->value();
	p.ms_per_char = d->ui.msPerChar->value();

	p.save();
}

void Pref::Widget::checkSubtitleSelect(int index) {
	const bool enabled = d->ui.autoSelect->itemData(index).toInt() == SameName;
	d->ui.subExtLabel->setEnabled(enabled);
	d->ui.subSelectExt->setEnabled(enabled);
}

void Pref::Widget::slotMediaItemClicked(QTreeWidgetItem *item, int column) {
	if (column)
		item->setCheckState(column, Qt::Checked);
}

QString Pref::Widget::toString(const QLocale &locale) {
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

int Pref::Widget::pageCount() const {
	return d->ui.stack->count();
}

QString Pref::Widget::pageName(int idx) const {
	return d->ui.stack->widget(idx)->property("name").toString();
}

QIcon Pref::Widget::pageIcon(int idx) const {
	return d->ui.stack->widget(idx)->property("icon").value<QIcon>();
}

void Pref::Widget::setCurrentPage(int idx) const {
	d->ui.stack->setCurrentIndex(idx);
}

int Pref::Widget::currentPage() const {
	return d->ui.stack->currentIndex();
}
