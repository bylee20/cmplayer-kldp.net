#include "backendwidget.h"
#include <backend/selectwidget.h>
#include "ui_backendwidget.h"
#include <backend/manager.h>
#include <backend/config.h>
#include <QtCore/QMap>
#include <QtGui/QRadioButton>
#include <QtGui/QButtonGroup>
#include "backend.h"

namespace Pref {

struct BackendWidget::Data {
	Ui::BackendWidget ui;
	BackendPref backend;
};

BackendWidget::BackendWidget(const BackendPref &backend, QWidget *parent)
: QWidget(parent), d(new Data) {
	d->ui.setupUi(this);
	d->backend = backend;
	const Backend::BackendList &backends = d->ui.selector->backends();
	for (int i=0; i<backends.size(); ++i) {
		Backend::BackendObject *obj = backends[i];
		if (obj->fileName() == d->backend.fileName())
			d->ui.selector->setBackend(obj);
		d->ui.tabs->addTab(obj->config()->widget(this), obj->name());
	}
}

BackendWidget::~BackendWidget() {
	delete d;
}

const BackendPref &BackendWidget::backend() const {
	d->backend.setFileName(d->ui.selector->backend()->fileName());
	return d->backend;
}

void BackendWidget::save() {
	const Backend::BackendList &backends = d->ui.selector->backends();
	for (int i=0; i<backends.size(); ++i) {
		Backend::Config *config = backends[i]->config();
		config->update(d->ui.tabs->widget(i));
		config->save();
	}
}

}
