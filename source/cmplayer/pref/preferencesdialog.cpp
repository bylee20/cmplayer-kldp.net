#include "preferencesdialog.h"
#include "preferences.h"
#include "generalwidget.h"
#include "subtitlewidget.h"
#include "interfacewidget.h"
#include "ui/ui_preferencesdialog.h"

namespace Pref {

PreferencesDialog::PreferencesDialog(QWidget *parent)
: QDialog(parent), ui(new Ui::Ui_PreferencesDialog), m_pref(get())
, m_general(new GeneralWidget(m_pref->general(), this))
, m_subtitle(new SubtitleWidget(m_pref->subtitle(), this))
, m_interface(new InterfaceWidget(m_pref->interface(), this)) {
	ui->setupUi(this);
	connect(ui->ok_button, SIGNAL(clicked()), this, SLOT(save()));
	while (ui->stack->count()) {
		QWidget *w = ui->stack->widget(0);
		ui->stack->removeWidget(w);
		delete w;
	}
	ui->stack->addWidget(m_general);
	ui->stack->addWidget(m_subtitle);
	ui->stack->addWidget(m_interface);
}

PreferencesDialog::~PreferencesDialog() {
	delete ui;
}

void PreferencesDialog::save() {
	m_pref->setGeneral(m_general->general());
	m_pref->setSubtitle(m_subtitle->subtitle());
	m_pref->setInterface(m_interface->interface());
	m_pref->save();
	accept();
}

}

