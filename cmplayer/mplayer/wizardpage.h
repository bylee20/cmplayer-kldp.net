#ifndef BACKEND_MPLAYER_WIZARDPAGE_H
#define BACKEND_MPLAYER_WIZARDPAGE_H

#include <backend/wizardpage.h>

namespace Backend {

namespace MPlayer {

class WizardPage : public Backend::WizardPage {
	Q_OBJECT
public:
	WizardPage(QWidget *parent = 0);
	~WizardPage();
	bool isComplete () const;
private slots:
	void setComplete(bool complete);
private:
	struct Data;
	Data *d;
};

}

}

#endif
