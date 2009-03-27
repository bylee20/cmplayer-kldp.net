#ifndef SNAPSHOTDIALOG_H
#define SNAPSHOTDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QScrollArea>

class VideoPlayer;

class SnapshotDialog : public QDialog {
	Q_OBJECT
public:
	SnapshotDialog(QWidget *parent = 0);
	~SnapshotDialog();
	void setVideoPlayer(VideoPlayer *player);
public slots:
	void take();
private slots:
	void setSnapshot(const QImage &image);
	void save();
	void copyToClipboard();
private:
	struct Data;
	Data *d;
};

class ImageViewer : public QScrollArea {
	Q_OBJECT
public:
	ImageViewer(QWidget *parent = 0);
	~ImageViewer();
	void setText(const QString &text);
	void setImage(const QImage &image);
	void scale(double factor);
	const QImage &image() const;
public slots:
	void zoomIn() {scale(1.25);}
	void zoomOut() {scale(0.8);}
	void zoomOriginal();
signals:
	void scaleChanged(double scale);
private:
	void adjustScrollBar(QScrollBar *scrollBar, double factor);
	struct Data;
	Data *d;
};

#endif
