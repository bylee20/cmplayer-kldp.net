#include "wizardpage.h"
#include <backend/filerequester.h>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

namespace Backend {

namespace MPlayer {

struct WizardPage::Data {
	FileRequester *file;
	bool ok;
};

WizardPage::WizardPage(QWidget *parent)
: Backend::WizardPage(parent) {
	d = new Data;
	d->ok = false;
	d->file = new FileRequester("mplayer");
	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(d->file);
	vbox->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
	setLayout(vbox);
	connect(d->file, SIGNAL(validityChanged(bool)), this, SLOT(setComplete(bool)));
	setTitle(trUtf8("MPlayer 설정"));
	setSubTitle(trUtf8("MPlayer 실행 파일의 위치를 지정해주세요."));
}

WizardPage::~WizardPage() {
	delete d;
}

bool WizardPage::isComplete () const {
	return d->ok;
}

void WizardPage::setComplete(bool complete) {
	if (d->ok != complete) {
		d->ok = complete;
		emit completeChanged();
	}
}

}

}
