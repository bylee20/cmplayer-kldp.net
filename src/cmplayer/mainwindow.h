#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QSystemTrayIcon>
#include <core/namespace.h>

class RecentStack;				class QUrl;

namespace Core {
class BackendIface;				class MediaSource;
class Playlist;
}

class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	MainWindow(const QUrl &url);
	MainWindow();
	~MainWindow();
	bool isOnTop() const {return windowFlags() & Qt::WindowStaysOnTopHint;}
	static QUrl getUrlFromCommandLine();
public slots:
	void open(const QUrl &url, const QString &enc = QString());
private slots:
	void open();
	void openSubFile();
	void togglePlayPause();
	void seek(int diff);
	void setSyncDelay(int diff);
	void setVolume(int diff);
	void setAmplifyingRate(int diff);
	void setSubtitlePos(int diff);
	void setSpeed(int diff);
	void setVideoSize(double rate);
	void slotSubtitle(QAction *action);
	void setColorProperty(QAction *action);
	void setRenderer(QAction *action);
	void setFullScreen(bool full);
	void showContextMenu(const QPoint &pos);
	void updateSubtitle();
	void clearSubs();
	void slotStateChanged(Core::State state, Core::State old);
	void updatePlaylistInfo();
	void autoLoadSubtitles();
	void toggleDockVisibility();
	void updateRecentActions(const RecentStack &files);
	void updateOnTop();
	void updateRecentSize(int size);
	void slotTracksChanged(const QStringList &tracks);
	void slotCurrentTrackChanged(const QString &track);
	void slotSpusChanged(const QStringList &spus);
	void slotCurrentSpuChanged(const QString &spu);
	void showPrefDialog();
	void slotBackendChanged();
	void slotRepeat(int key);
	void slotHelp();
	void showAbout();
	void hideCursor();
	void slotTrayActivated(QSystemTrayIcon::ActivationReason reason);
	void takeSnapshot();
private:
	typedef QPair<QAction*, QAction*> ActionPair;
	QWidget *createControl(QWidget *parent);
	void commonInitialize();
	void setupUi();
	void closeEvent(QCloseEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void updatePref();
	void saveState();
	void loadState();
	void showEvent(QShowEvent *event);
	void hideEvent(QHideEvent *event);
	void updateWindowTitle();
	void showMessage(const QString &text);
	Core::Playlist open(const Core::MediaSource &source);
	static QIcon defaultIcon();
	template<typename M, typename A>
	static typename A::mapped_type getTriggerAction(uint mod, const M &map
			, const A &act, const typename A::mapped_type &def) {
		typename M::const_iterator it = map.begin();
		for (; it != map.end(); ++it) {
			if (it.key().value() == mod && it.value().first)
				return act[it.value().second.value()];
		}
		return def;
	}
	struct Data;
	Data *d;
	class Acts;
};

#endif
