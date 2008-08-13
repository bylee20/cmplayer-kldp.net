//#include "selecttitledialog.h"
//#include "ui/ui_selecttitledialog.h"
//#include <mplayer/dvdinfo.h>
//#include <mplayer/utility.h>
//#include <QTreeWidgetItem>
//
//struct SelectTitleDialog::Data {
//	Ui::Ui_SelectTitleDialog ui;
//	int index;
//};
//
//SelectTitleDialog::SelectTitleDialog(const MPlayer::DVDInfo &dvd, QWidget *parent)
//: QDialog(parent), d(new Data) {
//	d->index = -1;
//	d->ui.setupUi(this);
//	QVector<MPlayer::DVDInfo::Title>::const_iterator it = dvd.titles.begin();
//	QTreeWidgetItem *item = new QTreeWidgetItem(QStringList()
//			<< trUtf8("No.") << trUtf8("길이") << trUtf8("챕터") << trUtf8("앵글") << QString(" "));
//	d->ui.list->setHeaderItem(item);
//	for (; it != dvd.titles.end(); ++it) {
//		QTreeWidgetItem *item = new QTreeWidgetItem(d->ui.list);
//		item->setText(0, QString::number(it->number));
//		item->setText(1, MPlayer::Utility::msecsToString(it->length));
//		item->setText(2, trUtf8("%1 개").arg(it->chapters.size()));
//		item->setText(3, trUtf8("%1 개").arg(it->angles));
//		item->setTextAlignment(0, Qt::AlignHCenter);
//		for (int i=1; i<4; ++i)
//			item->setTextAlignment(i, Qt::AlignRight);
//	}
//	for (int i=0; i<4; ++i) {
//		d->ui.list->headerItem()->setTextAlignment(i, Qt::AlignHCenter);
//		d->ui.list->resizeColumnToContents(i);
//	}
//	connect(d->ui.list, SIGNAL(itemSelectionChanged()), this, SLOT(checkSelection()));
//}
//
//SelectTitleDialog::~SelectTitleDialog() {
//	delete d;
//}
//
//void SelectTitleDialog::checkSelection() {
//	d->ui.play_button->setEnabled(!d->ui.list->selectedItems().isEmpty());
//}
//
//void SelectTitleDialog::accept() {
//	QList<QTreeWidgetItem*> items = d->ui.list->selectedItems();
//	if (!items.isEmpty()) {
//		d->index = d->ui.list->indexOfTopLevelItem(items[0]);
//		QDialog::accept();
//	}
//}
//
//int SelectTitleDialog::titleNumber() const {
//	return d->index + 1;
//}
