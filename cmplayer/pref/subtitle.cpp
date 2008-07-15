#include "subtitle.h"
#include <QSettings>

namespace Pref {

void Subtitle::load(QSettings *set) {
	set->beginGroup("Subtitle");
//	font.family = set->value("FontFamily", QString()).toString();
	encoding = set->value("Encoding", QString("CP949")).toString();
//	font.scale = set->value("FontScale", 5.0).toDouble();
	initialPos = set->value("InitialPos", 100).toInt();
	priority = set->value("Priority", QStringList()<<"ENCC"<<"EGCC"<<"KRCC"<<"KNCC").toStringList();
//	autoScale = static_cast<MPlayer::SubtitleOutput::AutoScale>(set->value("AutoScale",
//			MPlayer::SubtitleOutput::FitToHeight).toInt());
	autoLoad = static_cast<AutoLoad>(set->value("AutoLoad",	Contain).toInt());
//	autoSelect = static_cast<MPlayer::SubtitleOutput::AutoSelect>(set->value("AutoSelect",
//			MPlayer::SubtitleOutput::SameName).toInt());
	displayOnMarginWhenFullScreen = set->value("DisplayOnMarginWhenFullScreen", true).toBool();
	set->endGroup();
}

void Subtitle::save(QSettings *set) const {
	set->beginGroup("Subtitle");
//	set->setValue("FontFamily", font.family);
	set->setValue("Encoding", encoding);
//	set->setValue("AutoScale", autoScale);
//	set->setValue("FontScale", font.scale);
	set->setValue("InitialPos", initialPos);
	set->setValue("Priority", priority);
	set->setValue("AutoLoad", autoLoad);
//	set->setValue("AutoSelect", autoSelect);
	set->setValue("DisplayOnMarginWhenFullScreen", displayOnMarginWhenFullScreen);
	set->endGroup();
}

}
