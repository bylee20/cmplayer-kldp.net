// #include "dockwidget.h"
// #include "designedbuttons.h"
// #include "playlistwidget.h"
// #include <core/info.h>
// #include <core/playlist.h>
// #include <QtGui/QTabWidget>
// #include <QtGui/QVBoxLayout>
// #include <QtGui/QHBoxLayout>
// #include <QtGui/QPainter>
// #include <QtCore/QDebug>
// #include <QtGui/QLabel>
// #include <QtGui/QPushButton>
// #include <QtGui/QCloseEvent>
// #include "helper.h"
// #include <QtCore/QTimer>
// 
// class DockWidget::TitleBar : public QWidget {
// public:
// 	TitleBar(QWidget *parent): QWidget(parent) {
// 		setStyleSheet("\
// 			QLabel {text-align: center center; color:white; font-size: 9px;}\
// 			ToolButton {width: 9px; height: 9px;}\
// 		");
// 		title = new QLabel(this);
// 		float_ = new ToolButton(QIcon(":/img/window-duplicate-gray.png"), this);
// 		float_->setIconSize(QSize(8, 8));
// 		close = new ToolButton(QIcon(":/img/edit-delete-gray.png"), this);
// 		close->setIconSize(QSize(8, 8));
// 		QHBoxLayout *hbox = new QHBoxLayout(this);
// 		hbox->setContentsMargins(0, 0, 0, 0);
// 		hbox->setSpacing(0);
// 		hbox->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed));
// 		hbox->addWidget(title);
// 		hbox->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed));
// 		hbox->addWidget(float_);
// 		hbox->addWidget(close);
// 	}
// 	ToolButton *float_, *close;
// 	QLabel *title;
// };
// 
// class DockWidget::Frame : public QFrame {
// public:	
// 	Frame(QWidget *parent): QFrame(parent) {
// 	}
// 	void setWidget(QWidget *widget) {
// 		QVBoxLayout *vbox = new QVBoxLayout(this);
// 		vbox->addWidget(widget);
// 		vbox->setContentsMargins(0, 0, 0, 0);
// 	}
// };
// 
// struct DockWidget::Data {
// 	PlaylistWidget *playlist;
// 	TitleBar *titleBar;
// 	Frame *frame;
// 	int feedback;
// };
// 
// 
// DockWidget::DockWidget(PlaylistModel *model, QWidget *parent)
// : QDockWidget(parent), d(new Data) {
// 	d->feedback = 0;
// 	d->titleBar = new TitleBar(this);
// 	d->frame = new Frame(this);
// 	d->playlist = new PlaylistWidget(model, d->frame);
// 	setTitleBarWidget(d->titleBar);
// 	setWidget(d->frame);
// 	d->frame->setWidget(d->playlist);
// 	d->titleBar->title->setText(tr("PLAYLIST"));
// 	connect(d->titleBar->float_, SIGNAL(clicked()), this, SLOT(toggleFloat()));
// 	connect(d->titleBar->close, SIGNAL(clicked()), this, SLOT(close()));
// }
// 
// DockWidget::~DockWidget() {
// 	delete d;
// }
// 
// QSize DockWidget::minimumSizeHint() const {
// 	QSize hint = QDockWidget::minimumSizeHint();
// 	hint.setWidth(300);
// 	qDebug() << "minimumSizeHint";
// 	return hint;
// }
// 
// QSize DockWidget::sizeHint() const {
// 	QSize hint = QDockWidget::sizeHint();
// 	hint.setWidth(300);
// 	qDebug() << "sizeHint";
// 	return hint;
// }
// 
// void DockWidget::toggleFloat() {
// 	setFloating(!isFloating());
// }
// 
// void DockWidget::closeEvent(QCloseEvent *event) {
// 	event->ignore();
// 	emit hidingRequested();
// }
// 

// 
// void DockWidget::setContentsWidth(int width) {
// // 	d->frame->setWidth(width);
// // 	d->tab->setFixedWidth(width);
// }
// 
// int DockWidget::contentsWidth() const {
// 	return d->frame->width();
// }
// 
// void DockWidget::feedback(int diff) {
// 	d->feedback = diff;
// }
// 
// void DockWidget::resizeEvent(QResizeEvent *event) {
// 	QDockWidget::resizeEvent(event);
// 	static int i=0;
// 	qDebug() << ++i << "dock resized" << size();
// }
