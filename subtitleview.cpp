#include "subtitleview.hpp"
#include "playengine.hpp"
#include "subtitle.hpp"
#include <QtGui/QVBoxLayout>
#include <QtGui/QTreeWidget>
#include <QtGui/QScrollArea>
#include <QtGui/QLabel>

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
	}
	void setComponent(const Subtitle::Component &comp) {
		m_tree->clear();
		m_item.clear();
		m_base = comp.base();
		m_name->setText(comp.name());
		QTreeWidgetItem *prev = 0;
		for (CompConstIt it = comp.begin(); it != comp.end(); ++it) {
			if (prev) {
				prev->setText(End, QString::number(it.key()));
				prev = 0;
			}
			static QRegExp rxTag("<\\s*[a-zA-Z][^>]*>");
			const RichString text = it.value();
			if (!text.hasWords())
				continue;
			QTreeWidgetItem *item = new QTreeWidgetItem;
			item->setText(Start, QString::number(it.key()));
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
	}
private:
	void setCurrent(QTreeWidgetItem *item, bool scroll) {
		if (item == m_current)
			return;
		//			static const QBrush nb(qRgb(200, 200, 255));
		//			static const QBrush ob = item->background(0);
		//			if (d->current) {
		//				d->current->setBackground(Start, ob);
		//				d->current->setBackground(End, ob);
		//				d->current->setBackground(Text, ob);
		//			}
		m_current = item;
		if (scroll)
			m_tree->scrollToItem(item, QTreeWidget::PositionAtCenter);
	}

	QTreeWidget *m_tree;
	QLabel *m_name;
	Subtitle::Component::Base m_base;
	QMap<int, QTreeWidgetItem*> m_item;
	QTreeWidgetItem *m_current;
};


struct SubtitleView::Data {
	QList<CompView*> comp;
	QHBoxLayout *hbox;
	double fps;
	int prev;
};

SubtitleView::SubtitleView(const PlayEngine *engine, QWidget *parent)
: QWidget(parent), d(new Data) {
	d->prev = -1;
	QScrollArea *area = new QScrollArea(this);
	d->hbox = new QHBoxLayout(area);
	d->fps = -1.0;
	QVBoxLayout *vbox = new QVBoxLayout(this);
	vbox->addWidget(area);
	connect(engine, SIGNAL(tick(int)), this, SLOT(updateCurrent(int)));
//	Q
}

SubtitleView::~SubtitleView() {
	delete d;
}

void SubtitleView::setFrameRate(double fps) {
	d->fps = fps;
}

void SubtitleView::setSubtitle(const Subtitle &sub) {
	while (d->comp.size() > sub.count())
		delete d->comp.takeLast();
	while (d->comp.size() < sub.count()) {
		CompView *comp = new CompView;
		d->hbox->addWidget(comp);
		d->comp << comp;
//		view->setStartEndVisible(d->startEnd->isChecked());
	}
	for (int i=0; i<sub.count(); ++i)
		d->comp[i]->setComponent(sub[i]);
}

void SubtitleView::updateCurrent(int time) {
	for (int i=0; i<d->comp.size(); ++i)
		d->comp[i]->setCurrent(time, d->fps, true);
}
