#include "subtitleview.hpp"
#include "playengine.hpp"
#include "subtitle.hpp"
#include <QtGui/QVBoxLayout>
#include <QtGui/QTreeWidget>
#include <QtGui/QScrollArea>
#include <QtGui/QLabel>
#include <QtCore/QDebug>
#include <QtCore/QTime>
#include <QtGui/QSplitter>
#include <QtGui/QCheckBox>

class SubtitleView::CompView : public QWidget {
	enum Column {Text = 2, Start = 0, End = 1};
	typedef Subtitle::Component::const_iterator CompConstIt;
public:
	CompView(QWidget *parent = 0): QWidget(parent) {
		m_base = Subtitle::Component::Time;
		m_name = new QLabel(this);
		m_tree = new QTreeWidget(this);
		m_current = 0;
		QVBoxLayout *vbox = new QVBoxLayout(this);
		vbox->addWidget(m_name);
		vbox->addWidget(m_tree);
		m_tree->setRootIsDecorated(false);
		m_tree->setHeaderLabels(QStringList() << tr("Start") << tr("End") << tr("Text"));
		setMinimumWidth(200);
	}
	void setComponent(const Subtitle::Component &comp) {
		m_current = 0;
		m_tree->clear();
		m_item.clear();
		m_base = comp.base();
		m_name->setText(comp.name());
		QTreeWidgetItem *prev = 0;
		const QTime zero;
		for (CompConstIt it = comp.begin(); it != comp.end(); ++it) {
			if (prev) {
				prev->setText(End, zero.addMSecs(it.key()).toString("h:mm:ss"));
				prev = 0;
			}
			static QRegExp rxTag("<\\s*[a-zA-Z][^>]*>");
			const RichString text = it.value();
			if (!text.hasWords())
				continue;
			QTreeWidgetItem *item = new QTreeWidgetItem;
			item->setText(Start, zero.addMSecs(it.key()).toString("h:mm:ss"));
			item->setText(Text, text.toPlain());
			m_tree->addTopLevelItem(item);
			m_item[it.key()] = item;
			prev = item;
		}
		m_tree->resizeColumnToContents(Start);
		m_tree->resizeColumnToContents(End);
		m_tree->resizeColumnToContents(Text);
	}
	void setCurrent(int time, double frameRate, bool scroll) {
		if (m_item.isEmpty() || time < 0)
			return;
		int key = time;
		if (m_base == Subtitle::Component::Frame) {
			if (frameRate <= 0.0)
				return;
			key = qRound(time*0.001*frameRate);
		}
		QMap<int, QTreeWidgetItem*>::const_iterator it = m_item.upperBound(key);
		if (it != m_item.begin())
			--it;
		setCurrent(*it, scroll);
		m_defaultFont = m_tree->font();
		m_fontForCurrent = m_defaultFont;
		m_fontForCurrent.setBold(true);
		m_fontForCurrent.setItalic(true);
	}
	void setTimeVisible(bool visible) {
		m_tree->setColumnHidden(Start, !visible);
		m_tree->setColumnHidden(End, !visible);
	}
private:
	void setCurrent(QTreeWidgetItem *item, bool scroll) {
		if (item == m_current)
			return;
		if (m_current) {
			m_current->setFont(Start, m_defaultFont);
			m_current->setFont(End, m_defaultFont);
			m_current->setFont(Text, m_defaultFont);
		}
		m_current = item;
		if (item) {
			m_current->setFont(Start, m_fontForCurrent);
			m_current->setFont(End, m_fontForCurrent);
			m_current->setFont(Text, m_fontForCurrent);
		}
		if (scroll && item)
			m_tree->scrollToItem(item, QTreeWidget::PositionAtCenter);
	}
	QTreeWidget *m_tree;
	QLabel *m_name;
	Subtitle::Component::Base m_base;
	QMap<int, QTreeWidgetItem*> m_item;
	QTreeWidgetItem *m_current;
	QFont m_fontForCurrent;
	QFont m_defaultFont;
};


struct SubtitleView::Data {
	QList<CompView*> comp;
	QSplitter *splitter;
	QCheckBox *timeVisible;
	double fps;
	int prev;
	bool autoScroll;
	bool wait;
};

SubtitleView::SubtitleView(const PlayEngine *engine, QWidget *parent)
: QWidget(parent), d(new Data) {
	d->wait = false;
	d->prev = -1;
	d->autoScroll = true;
	QScrollArea *area = new QScrollArea(this);
	d->splitter = new QSplitter(Qt::Horizontal, area);
	area->setWidget(d->splitter);
	area->setWidgetResizable(true);
	d->fps = -1.0;
	QVBoxLayout *vbox = new QVBoxLayout(this);
	vbox->addWidget(area);
	d->timeVisible = new QCheckBox(tr("Show start/end time"), this);
	vbox->addWidget(d->timeVisible);
	connect(engine, SIGNAL(tick(int)), this, SLOT(updateCurrent(int)));
	connect(d->timeVisible, SIGNAL(toggled(bool)), this, SLOT(setTimeVisible(bool)));
	d->timeVisible->setChecked(true);
	QCheckBox *autoScroll = new QCheckBox(tr("Scroll to current time"), this);
	vbox->addWidget(autoScroll);
	autoScroll->setChecked(true);
	connect(autoScroll, SIGNAL(toggled(bool)), this, SLOT(setAutoScrollEnabled(bool)));
}

SubtitleView::~SubtitleView() {
	delete d;
}

void SubtitleView::setFrameRate(double fps) {
	d->fps = fps;
}

void SubtitleView::setSubtitle(const Subtitle &sub) {
	d->wait = true;
	while (d->comp.size() > sub.count())
		delete d->comp.takeLast();
	while (d->comp.size() < sub.count()) {
		CompView *comp = new CompView;
		d->splitter->addWidget(comp);
		d->comp << comp;
	}
	for (int i=0; i<sub.count(); ++i) {
		d->comp[i]->setComponent(sub[i]);
		d->comp[i]->setTimeVisible(d->timeVisible->isChecked());
	}
	d->wait = false;
	updateCurrent(d->prev);
}

void SubtitleView::updateCurrent(int time) {
	d->prev = time;
	if (!isVisible() || d->wait)
		return;
	for (int i=0; i<d->comp.size(); ++i)
		d->comp[i]->setCurrent(time, d->fps, d->autoScroll);
}

void SubtitleView::setTimeVisible(bool visible) {
	for (int i=0; i<d->comp.size(); ++i)
		d->comp[i]->setTimeVisible(visible);
}

void SubtitleView::showEvent(QShowEvent *event) {
	QWidget::showEvent(event);
	updateCurrent(d->prev);
}

void SubtitleView::setAutoScrollEnabled(bool enabled) {
	if (d->autoScroll == enabled)
		return;
	d->autoScroll = enabled;
	if (d->autoScroll) {
		for (int i=0; i<d->comp.size(); ++i)
			d->comp[i]->setCurrent(d->prev, d->fps, d->autoScroll);
	}
}

