#include "wizardpages.h"
#include <backend/selectwidget.h>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtCore/QDebug>
#include <QtGui/QLabel>
#include <QtGui/QFontComboBox>
#include "encodingcombobox.h"
#include <QtGui/QGridLayout>


namespace Pref {

IntroPage::IntroPage() {
	setTitle(trUtf8("설정 마법사 시작"));
	QLabel *label = new QLabel(trUtf8("이 마법사를 통해 "
			"CMPlayer를 이용하는데 있어서 꼭 필요하거나 중요한 사항들을 설정하게 될 것입니다."));
	label->setWordWrap(true);
	setWidget(label);
}

BackendPage::BackendPage() {
	ok = false;
	w = new Backend::SelectWidget(this);
	setWidget(w);
	
	setTitle(trUtf8("재생 엔진 선택"));
	setSubTitle(trUtf8("멀티미디어 재생을 위한 엔진을 선택해주세요."));
	
	connect(w, SIGNAL(selected(Backend::FactoryIface*))
			, this, SLOT(slotSelected(Backend::FactoryIface*)));
}

void BackendPage::slotSelected(Backend::FactoryIface *factory) {
	const bool ok = (factory != 0);
	if (this->ok != ok) {
		this->ok = ok;
		emit completeChanged();
	}
}

QString BackendPage::backend() const {
	return w->selectedFileName();
}

SubtitlePage::SubtitlePage() {
	setTitle(trUtf8("자막 설정"));
	setSubTitle(trUtf8("자막을 표시하는데 사용할 글꼴과, 자막 파일을 읽어들일때 이용할 인코딩을 선택해주세요."));
	QGridLayout *grid = new QGridLayout;
	grid->addWidget(new QLabel(trUtf8("글꼴"), this), 0, 0, 1, 1);
	fontCombo = new QFontComboBox(this);
	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addWidget(fontCombo);
	hbox->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
	grid->addLayout(hbox, 0, 1, 1, 1);
	QLabel *label = new QLabel(trUtf8("주) Sans Serif나 Serif와 같은 fontconfig의 "
			"글꼴은 재생엔진에 따라 정상적으로 표시되지 않을 수 있습니다."), this);
	label->setWordWrap(true);
	grid->addWidget(label, 1, 1, 1, 1);
	grid->addWidget(new QLabel(trUtf8("인코딩"), this), 2, 0, 1, 1);
	encCombo = new EncodingComboBox(this);
	hbox = new QHBoxLayout;
	hbox->addWidget(encCombo);
// 	hbox->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
	grid->addLayout(hbox, 2, 1, 1, 1);
	label = new QLabel(trUtf8("주) 일반적으로 한국어가 포함된 자막은 "
			"CP949 혹은 EUC-KR의 인코딩을 이용하는 경우가 많습니다."), this);
	label->setWordWrap(true);
	grid->addWidget(label, 3, 1, 1, 1);
	setLayout(grid);
}

}
 
