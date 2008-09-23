#include "backend.h"
#include <QtCore/QSettings>

namespace Pref {

void BackendPref::save(QSettings *set) const {
	set->beginGroup("Backend");
	set->setValue("FileName", m_fileName);
	set->endGroup();
}

void BackendPref::load(QSettings *set) {
	set->beginGroup("Backend");
	m_fileName = set->value("FileName", QString()).toString();
	set->endGroup();
}

}
