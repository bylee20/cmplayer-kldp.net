#include "subtitle.h"
#include <QtCore/QSettings>

namespace Pref {

void Subtitle::load(QSettings *set) {
	set->beginGroup("Subtitle");

	set->beginGroup("OsdStyle");
	d->style.font = set->value("Font", QFont()).value<QFont>();
	QColor color = set->value("TextColor", QColor(Qt::white)).value<QColor>();
	d->style.text_color = color;
	color = set->value("BorderColor", QColor(Qt::black)).value<QColor>();
	d->style.border_color = color;
	int value = set->value("Scale", Backend::OsdStyle::FitToDiagonal).toInt();
	d->style.scale = static_cast<Backend::OsdStyle::Scale>(value);
	d->style.size = set->value("Size", 0.07).toDouble();
	d->style.high_quality = set->value("HighQuality", false).toBool();
	set->endGroup();

	d->encoding = set->value("Encoding", QString("CP949")).toString();
	d->initialPos = set->value("InitialPos", 100).toInt();
	const QStringList prior = QStringList()<<"ENCC"<<"EGCC"<<"KRCC"<<"KNCC";
	d->priority = set->value("Priority", prior).toStringList();
	value = set->value("AutoLoad",	Contain).toInt();
	d->autoLoad = static_cast<AutoLoad>(value);
	value = set->value("AutoSelect", Backend::SubtitleOutput::SameName).toInt();
	d->autoSelect = static_cast<Backend::SubtitleOutput::AutoSelect>(value);
	set->endGroup();
}

void Subtitle::save(QSettings *set) const {
	set->beginGroup("Subtitle");

	set->beginGroup("OsdStyle");
	set->setValue("Font", d->style.font);
	set->setValue("TextColor", d->style.text_color);
	set->setValue("BorderColor", d->style.border_color);
	set->setValue("Scale", d->style.scale);
	set->setValue("Size", d->style.size);
	set->setValue("HighQuality", d->style.high_quality);
	set->endGroup();

	set->setValue("Encoding", d->encoding);
	set->setValue("InitialPos", d->initialPos);
	set->setValue("Priority", d->priority);
	set->setValue("AutoLoad", d->autoLoad);
	set->setValue("AutoSelect", d->autoSelect);
	set->endGroup();
}

}
