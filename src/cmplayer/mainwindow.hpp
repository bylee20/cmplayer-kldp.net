#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QtGui/QMainWindow>
#include <QtGui/QSystemTrayIcon>
#include "global.hpp"

class Mrl;		class PlayEngine;
class AudioTrack;

class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	MainWindow();
	~MainWindow();
	PlayEngine *engine() const;
public slots:
	void openMrl(const Mrl &mrl, const QString &enc = QString());
private slots:
	void setVolumeNormalized(bool norm);
	void openFile();
	void openDvd();
	void openUrl();
	void togglePlayPause();
	void showContextMenu(const QPoint &pos);
	void updateMrl(const Mrl &mrl);
	void seek(int diff);
	void setVolume(int volume);
	void setMuted(bool muted);
	void setVideoSize(double times);
	void updateState(MediaState state, MediaState old);
	void setColorProperty(QAction *action);
	void setSpeed(int speed);
	void setAmp(int amp);
	void doRepeat(int key);
	void moveSubtitle(int dy);
	void toggleToolBox();
	void clearSubtitles();
	void updateSubtitle(QAction *action);
	void handleFinished();
	void setSyncDelay(int diff);
	void updateRecentActions(const QList<Mrl> &list);
	void setPref();
	void hideCursor();
	void handleTray(QSystemTrayIcon::ActivationReason reason);
	void setCurrentAudioTrack(int id);
	void openSubFile();
	void updateAudioTrackInfo(const QList<AudioTrack> &tracks);
private:
	static QIcon defaultIcon();
	void appendSubFiles(const QStringList &files, bool checked, const QString &enc);
	void loadState();
	void saveState();
	void closeEvent(QCloseEvent *event);
	void applyPref();
	void showEvent(QShowEvent *event);
	void hideEvent(QHideEvent *event);
	void setFullScreen(bool full);
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
	void mouseMoveEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void dropEvent(QDropEvent *event);
	class VideoScreen;
	void doSubtitleAutoLoad();
	void doSubtitleAutoSelection();
	void updateSubtitle();
	void showMessage(const QString &message, int last = 2500);
	void showMessage(const QString &cmd, int value
		, const QString &unit, bool sign = false, int last = 2500);
	void showMessage(const QString &cmd, const QString &description, int last = 2500);
	void showMessage(const QString &cmd, double value
		, const QString &unit, bool sign = false, int last = 2500);
	void showMessage(const QString &cmd, bool value, int last = 2500);
	void setupUi();
	struct Data;
	Data *d;
};

#endif // MAINWINDOW_HPP
