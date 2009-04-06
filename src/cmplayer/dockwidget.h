// #ifndef LISTDOCK_H
// #define LISTDOCK_H
// 
// #include <QtGui/QDockWidget>
// #include <QtGui/QDialog>
// 
// class QModelIndex;				class PlaylistModel;
// class QComboBox;
// 
// class DockWidget : public QDockWidget {
// 	Q_OBJECT
// public:
// 	DockWidget(PlaylistModel *model, QWidget *parent = 0);
// 	~DockWidget();
// 	void setContentsWidth(int width);
// 	int contentsWidth() const;
// 	void feedback(int diff);
// 	QSize minimumSizeHint() const;
// 	QSize sizeHint() const;
// signals:
// 	void hidingRequested();
// private slots:
// 	void toggleFloat();
// private:
// 	void resizeEvent(QResizeEvent *event);
// 	class TitleBar;
// 	class Frame;
// 	void paintEvent(QPaintEvent *event);
// 	void closeEvent(QCloseEvent *event);
// 	struct Data;
// 	friend struct Data;
// 	Data *d;
// };
// 
// #endif
