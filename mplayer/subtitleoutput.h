#ifndef MPLAYERSUBTITLEMANAGER_H
#define MPLAYERSUBTITLEMANAGER_H

#include "controller.h"

namespace MPlayer {

class PlayEngine;							class Subtitle;

class SubtitleOutput : public Controller {
	Q_OBJECT
public:
	struct Font {Font(): scale(5.0) {} QString family; double scale;};
	enum AutoScale {NoAutoScale = 0, FitToHeight = 1, FitToWidth = 2, FitToDiagonal = 3};
	enum AutoSelect {FirstFile = 0, SameName = 1, AllLoaded = 2, EachLanguage = 3};
	SubtitleOutput(QObject *parent = 0);
	~SubtitleOutput();
	qint64 syncDelay() const;
	qreal pos() const;
	qreal initialPos() const;
	void setInitialPos(qreal pos);
	const Font &font() const;
	void setFont(const Font &font);
	const Subtitle &currentSubtitle() const;
	void setPriority(const QStringList &priority);
	AutoScale autoScale() const;
	AutoSelect autoSelect() const;
	void setAutoSelect(AutoSelect mode);
	void setAutoScale(AutoScale mode);
	const QString &encoding() const;
	void setEncoding(const QString &encoding);
	void load(const QStringList &files);
	void loadDisc();
	void appendSubtitles(const QStringList &files, bool display = true);
	const QList<int> &selectedIndexes() const;
	QStringList names() const;
	const QList<Subtitle> &availableSubtitles() const;
	void setDisplayOnMarginEnabled(bool enabled);
	bool isDisplayOnMarginEnabled() const;
public slots:
	//void setSelectedIndexes(const QList<int> indexes);
	void select(int index);
	void deselect(int index);
	void deselectAll();
	//void selectAll();
	inline void setVisible(bool visible) {visible ? show() : hide();}
	void show();
	void hide();
	void remove();
	void clear();
	void moveUp();
	void moveDown();
	void move(qreal percent);
	void setSyncDelay(qint64 msec);
	void addSyncDelay(qint64 msec);
signals:
	void availableSubtitlesChanged(const QStringList &names);
	void selectedSubtitlesChanged(const QList<int> &indexes);
	void syncDelayChanged(int);
protected slots:
	void update();
protected:
	void link(Controller *controller);
	void unlink(Controller *controller);
private slots:
	void slotFinished();
private:
	void setPosScale(qreal scale);
	struct Data;
	friend struct Data;
	friend class VideoOutput;
	Data *d;
};

}

#endif
