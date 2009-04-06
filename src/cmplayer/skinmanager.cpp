#include "skinmanager.h"
#include <QtCore/QString>
#include <QtGui/QApplication>

struct SkinManager::Data {
	PanelLayout panel;
};

SkinManager::SkinManager()
: d(new Data) {
	d->panel = static_cast<PanelLayout>(0);
	setPanelLayout(TwoLine);
}

SkinManager::~SkinManager() {
	delete d;
}

SkinManager &SkinManager::get() {
	static SkinManager self;
	return self;
}

void SkinManager::setPanelLayout(PanelLayout layout) {
// 	if (d->panel != layout) {
		if (layout == TwoLine) {
		} else {
			
		}
		d->panel = layout;
// 	}
}

SkinManager::PanelLayout SkinManager::panelLayout() const {
	return d->panel;
}