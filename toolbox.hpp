#ifndef TOOLBOX_HPP
#define TOOLBOX_HPP

#include <QtGui/QDialog>

//class PlaylistModel;			class VideoPlayer;
class MainWindow;		class PlaylistView;

class ToolBox : public QDialog {
	Q_OBJECT
public:
	ToolBox(/*VideoPlayer *player, PlaylistModel *model, */MainWindow *mainWindow);
	~ToolBox();
	PlaylistView *playlist() const;
signals:
	void hidingRequested();
	void snapRequested();
private slots:
	void changeWidget(int id);
private:
	QRect boxRect() const {return QRect(3, 3, width()-6, height() - 6);}
	void addPage(QWidget *widget, const QString &name, const QString &iconHolder);
	void closeEvent(QCloseEvent *event);
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
	class Frame;
	class ButtonWidget;
	struct Data;
	Data *d;
};


#endif // TOOLBOX_HPP
