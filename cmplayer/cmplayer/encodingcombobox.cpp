#include "encodingcombobox.h"
#include <QtCore/QRegExp>

struct EncodingComboBox::Data {
	static const QStringList encs;
};

const QStringList EncodingComboBox::Data::encs = QStringList() << "UTF-8" << "Unicode"
		<< QObject::trUtf8("서유럽언어") + " (ISO-8859-1)"
		<< QObject::trUtf8("서유럽언어(유로)") + " (ISO-8859-15)"
		<< QObject::trUtf8("슬라브어/중앙유럽언어") + " (ISO-8859-2)"
		<< QObject::trUtf8("슬라브어/중앙유럽언어(Windows)") + " (CP1250)"
		<< QObject::trUtf8("에스페란토, 갈리시아어, 몰타어, 터키어") + " (ISO-8859-3)"
		<< QObject::trUtf8("구발트어 문자셋") + " (ISO-8859-4)"
		<< QObject::trUtf8("키릴문자") + " (ISO-8859-5)"
		<< QObject::trUtf8("키릴문자(Windows)") + " (CP1251)"
		<< QObject::trUtf8("아랍어") + " (ISO-8859-6)"
		<< QObject::trUtf8("현대 그리스어") + " (ISO-8859-7)"
		<< QObject::trUtf8("터키어") + " (ISO-8859-9)"
		<< QObject::trUtf8("발트어") + " (ISO-8859-13)"
		<< QObject::trUtf8("켈트어") + " (ISO-8859-14)"
		<< QObject::trUtf8("히브리어 문자셋") + " (ISO-8859-8)"
		<< QObject::trUtf8("러시아어") + " (KOI8-R)"
		<< QObject::trUtf8("우크라이나어, 벨로루시어") + " (KOI8-U/RU)"
		<< QObject::trUtf8("간체자 중국어 문자셋") + " (CP936)"
		<< QObject::trUtf8("번체자 중국어 문자셋") + " (BIG5)"
		<< QObject::trUtf8("일본어 문자셋") + " (SHIFT-JIS)"
		<< QObject::trUtf8("한국어 문자셋") + " (CP949)"
		<< QObject::trUtf8("타이 문자셋") + " (CP874)";

EncodingComboBox::EncodingComboBox(QWidget *parent)
: QComboBox(parent) {
	addItems(Data::encs);
	setEditable(true);
}

QString EncodingComboBox::encoding() const {
	QString enc = currentText().trimmed();
	QRegExp rxEnc(".* \\((.*)\\)");
	return (rxEnc.indexIn(enc) == -1) ? enc : rxEnc.cap(1);
}

void EncodingComboBox::setEncoding(const QString &encoding) {
	static const QRegExp rxEncoding(".* \\(" + encoding.toUpper() + "\\)");
	const int idx = Data::encs.indexOf(rxEncoding);
	if (idx != -1)
		setCurrentIndex(idx);
	else
		setEditText(encoding);
}
