#include "playengineconfigwidget.h"
#include "backendmanager.h"
#include <core/config.h>
#include <core/info.h>
#include <QtGui/QLabel>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QTableWidget>
#include <QtGui/QHeaderView>

static QLabel *createCenterLabel(const QString &text, QWidget *parent = 0) {
	QLabel *label = new QLabel(text, parent);
	label->setAlignment(Qt::AlignCenter);
	label->setWordWrap(true);
	return label;
}

class PlayEngineConfigWidget::Tab : public QWidget {
public:
	Tab(Core::Config *config, QWidget *parent = 0);
	void apply();
private:
	enum Column {Name = 0, Value = 1, Desc = 2, Count = 3};
	Core::Config *config;
	QTableWidget *table;
};

PlayEngineConfigWidget::Tab::Tab(Core::Config *config, QWidget *parent/* = 0*/)
: QWidget(parent), config(config) {
	QVBoxLayout *vbox = new QVBoxLayout(this);
	if (config) {
		Core::Config::Map map = config->item();
		table = new QTableWidget(map.size(), Count, this);
		table->setHorizontalHeaderLabels(QStringList()
			<< tr("Item") << tr("Value") << tr("Description"));
		table->verticalHeader()->setVisible(false);
		Core::Config::Map::const_iterator it = map.begin();
		const Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
		for (int row = 0; it != map.end(); ++it, ++row) {
			QTableWidgetItem *name = new QTableWidgetItem(it.value().name());
			name->setFlags(flags);
			QTableWidgetItem *value = new QTableWidgetItem;
			value->setFlags(flags | Qt::ItemIsEditable);
			value->setData(Qt::DisplayRole, it.value().data());
			QTableWidgetItem *desc = new QTableWidgetItem(it.value().description());
			desc->setFlags(flags);
			table->setItem(row, Name, name);
			table->setItem(row, Value, value);
			table->setItem(row, Desc, desc);
		}
		vbox->addWidget(table);
	} else {
		QLabel *l =createCenterLabel(
			tr("There is no item to configure."), this);
		vbox->addWidget(l);
	}
}

void PlayEngineConfigWidget::Tab::apply() {
	if (!config)
		return;
	for (int row = 0; row < table->rowCount(); ++row) {
		const QString name = table->item(row, Name)->text();
		const QVariant data = table->item(row, Value)->data(Qt::DisplayRole);
		config->setData(name, data);
	}
	config->apply();
}

struct PlayEngineConfigWidget::Data {
	QTabWidget *tab;
};

PlayEngineConfigWidget::PlayEngineConfigWidget(QWidget *parent)
: QWidget(parent), d(new Data) {
	const BackendMap map = BackendManager::map();
	QVBoxLayout *vbox = new QVBoxLayout(this);
	if (map.isEmpty()) {
		QLabel *label = createCenterLabel(tr("There is no configurable play engine."), this);
		vbox->addWidget(label);
		d->tab = 0;
	} else {
		BackendMap::const_iterator it = map.begin();
		d->tab = new QTabWidget(this);
		for (; it != map.end(); ++it) {
			Tab *tab = new Tab(it.value()->config(), d->tab);
			d->tab->addTab(tab, it.value()->info()->name());
		}
		vbox->addWidget(d->tab);
	}
}

PlayEngineConfigWidget::~PlayEngineConfigWidget() {
	delete d;
}

void PlayEngineConfigWidget::apply() {
	if (d->tab) {
		for (int i=0; i<d->tab->count(); ++i)
			static_cast<Tab*>(d->tab->widget(i))->apply();
	}
}
