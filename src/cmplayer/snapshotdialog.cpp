#include "snapshotdialog.h"
#include "ui_snapshotdialog.h"
#include "videoplayer.h"
#include <core/info.h>
#include <QtCore/QFileInfo>
#include <QtGui/QImage>
#include <QtGui/QPixmap>
#include <QtGui/QLabel>
#include <QtGui/QScrollBar>
#include <QtGui/QApplication>
#include <QtGui/QFileDialog>
#include <QtGui/QClipboard>

struct SnapshotDialog::Data {
	Ui::SnapshotDialog ui;
	VideoPlayer *player;
};

SnapshotDialog::SnapshotDialog(QWidget *parent)
: QDialog(parent), d(new Data) {
	d->ui.setupUi(this);
	d->player = 0;
	
	connect(d->ui.zoomIn, SIGNAL(clicked()), d->ui.viewer, SLOT(zoomIn()));
	connect(d->ui.zoomOut, SIGNAL(clicked()), d->ui.viewer, SLOT(zoomOut()));
	connect(d->ui.original, SIGNAL(clicked()), d->ui.viewer, SLOT(zoomOriginal()));
	connect(d->ui.take, SIGNAL(clicked()), this, SLOT(take()));
	connect(d->ui.save, SIGNAL(clicked()), this, SLOT(save()));
// 	connect(d->ui.clip, SIGNAL(clicked()), this, SLOT(copyToClipboard()));
}

SnapshotDialog::~SnapshotDialog() {
	delete d;
}

void SnapshotDialog::save() {
	static const Core::Info::ExtensionList ext = Core::Info::ExtensionList()
			<< "bmp" << "jpg" << "jpeg" << "png" << "ppm" << "tiff" << "xbm" << "xpm";
	const QString filter = tr("Images") + ' ' + ext.toFilter();
	QString file = QFileDialog::getSaveFileName(this, tr("Save File"), QString(), filter);
	if (file.isEmpty())
		return;
	if (!ext.contains(QFileInfo(file).suffix()))
		file += ".png";
	d->ui.viewer->image().save(file);
}

void SnapshotDialog::copyToClipboard() {
	QApplication::clipboard()->setImage(d->ui.viewer->image());
}

void SnapshotDialog::setVideoPlayer(VideoPlayer *player) {
	d->player = player;
	connect(d->player, SIGNAL(snapshotTaken(const QImage&))
			, this, SLOT(setSnapshot(const QImage&)));
}

void SnapshotDialog::take() {
	if (!d->player || d->player->isStopped())
		return;
	d->ui.viewer->setText(tr("Taking a snapshot..."));
	d->ui.take->setEnabled(false);
	d->player->triggerSnapshot();
}

void SnapshotDialog::setSnapshot(const QImage &image) {
	d->ui.take->setEnabled(true);
	if (image.isNull())
		d->ui.viewer->setText(tr("Failed in getting a snapshot!"));
	else
		d->ui.viewer->setImage(image);
	d->ui.save->setEnabled(!image.isNull());
}

struct ImageViewer::Data {
	QImage image;
	QLabel *label;
	double scale;
};

ImageViewer::ImageViewer(QWidget *parent)
: QScrollArea(parent), d(new Data) {
	d->scale = 1.0;
	d->label = new QLabel(this);
	d->label->setBackgroundRole(QPalette::Base);
	d->label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	d->label->setScaledContents(true);
	d->label->setMinimumSize(200, 150);
	d->label->setAlignment(Qt::AlignCenter);
	
	setBackgroundRole(QPalette::Dark);
	setWidget(d->label);
}

ImageViewer::~ImageViewer() {
	delete d;
}

void ImageViewer::setText(const QString &text) {
	d->label->setText(text);
}

void ImageViewer::setImage(const QImage &image) {
	d->image = image;
	d->label->setPixmap(QPixmap::fromImage(image));
	zoomOriginal();
}

void ImageViewer::zoomOriginal() {
	d->label->adjustSize();
	d->scale = 1.0;
	emit scaleChanged(d->scale);
}

void ImageViewer::scale(double factor) {
	if (qAbs(1.0 - factor) < 1.0e-5)
		zoomOriginal();
	else if (d->label->pixmap()) {
		d->scale *= factor;
		d->label->resize(d->scale * d->label->pixmap()->size());
		adjustScrollBar(horizontalScrollBar(), factor);
		adjustScrollBar(verticalScrollBar(), factor);
		emit scaleChanged(d->scale);
	}
}

void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor) {
	scrollBar->setValue(int(factor * scrollBar->value()
			+ ((factor - 1) * scrollBar->pageStep()/2)));
}

const QImage &ImageViewer::image() const {
	return d->image;
}
