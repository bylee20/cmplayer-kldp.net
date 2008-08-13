#include "subtitle.h"
#include <QSettings>

namespace Pref {

void Subtitle::load(QSettings *set) {
	set->beginGroup("Subtitle");

	set->beginGroup("OsdStyle");
	osdStyle.font = set->value("Font", QFont()).value<QFont>();
	osdStyle.textColor = set->value("TextColor", QColor(Qt::white)).value<QColor>();
	osdStyle.borderColor = set->value("BorderColor", QColor(Qt::black)).value<QColor>();
	osdStyle.scale = static_cast<Xine::XineOsd::Style::Scale>(set->value("Scale"
			, Xine::XineOsd::Style::FitToDiagonal).toInt());
	osdStyle.size = set->value("Size", 0.07).toDouble();
	osdStyle.highQuality = set->value("HighQuality", false).toBool();
	set->endGroup();

	encoding = set->value("Encoding", QString("CP949")).toString();
	initialPos = set->value("InitialPos", 100).toInt();
	priority = set->value("Priority", QStringList()<<"ENCC"<<"EGCC"<<"KRCC"<<"KNCC").toStringList();
	autoLoad = static_cast<AutoLoad>(set->value("AutoLoad",	Contain).toInt());
	autoSelect = static_cast<Xine::SubtitleOutput::AutoSelect>(set->value("AutoSelect",
			Xine::SubtitleOutput::SameName).toInt());
	displayOnMarginWhenFullScreen = set->value("DisplayOnMarginWhenFullScreen", true).toBool();
	set->endGroup();
}

void Subtitle::save(QSettings *set) const {
	set->beginGroup("Subtitle");

	set->beginGroup("OsdStyle");
	set->setValue("Font", osdStyle.font);
	set->setValue("TextColor", osdStyle.textColor);
	set->setValue("BorderColor", osdStyle.borderColor);
	set->setValue("Scale", osdStyle.scale);
	set->setValue("Size", osdStyle.size);
	set->setValue("HighQuality", osdStyle.highQuality);
	set->endGroup();

	set->setValue("Encoding", encoding);
	set->setValue("InitialPos", initialPos);
	set->setValue("Priority", priority);
	set->setValue("AutoLoad", autoLoad);
	set->setValue("AutoSelect", autoSelect);
	set->setValue("DisplayOnMarginWhenFullScreen", displayOnMarginWhenFullScreen);
	set->endGroup();
}

}
