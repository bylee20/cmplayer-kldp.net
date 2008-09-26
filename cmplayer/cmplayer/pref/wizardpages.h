#ifndef PREF_WIZARDPAGES_H
#define PREF_WIZARDPAGES_H

#include <backend/wizardpage.h>

class QFontComboBox;			class EncodingComboBox;

namespace Backend {
class FactoryIface;				class SelectWidget;
}

namespace Pref {

class IntroPage : public Backend::WizardPage {
	Q_OBJECT
public:
	IntroPage();
};

class BackendPage : public Backend::WizardPage {
	Q_OBJECT
public:
	BackendPage();
	bool isComplete () const {return ok;}
	QString backend() const;
private slots:
	void slotSelected(Backend::FactoryIface *factory);
private:
	Backend::SelectWidget *w;
	bool ok;
};

class SubtitlePage : public Backend::WizardPage {
	Q_OBJECT
public:
	SubtitlePage();
private:
	QFontComboBox *fontCombo;
	EncodingComboBox *encCombo;
};

class FinishPage : public Backend::WizardPage {
	Q_OBJECT
public:
	FinishPage();
private:
};

}

#endif

