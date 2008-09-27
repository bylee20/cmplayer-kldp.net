#ifndef BACKEND_SUBTITLEOUTPUT_H
#define BACKEND_SUBTITLEOUTPUT_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include "osdstyle.h"

namespace Backend {

class PlayEngine;						class Subtitle;

class SubtitleOutput : public QObject {
	Q_OBJECT
public:
	enum AutoSelect {FirstFile = 0, SameName = 1, AllLoaded = 2, EachLanguage = 3};
	SubtitleOutput(PlayEngine *stream);
	virtual ~SubtitleOutput();
	void load(const QStringList &files);
	AutoSelect autoSelect() const;
	void setAutoSelect(AutoSelect mode) {m_autoSelect = mode;}
	void setPriority(const QStringList &priority) {m_priority = priority;}
	const QStringList &priority() const {return m_priority;}
	QStringList names() const;
	void setEncoding(const QString &enc) {m_encoding = enc;}
	const QString &encoding() const {return m_encoding;}
	void appendSubtitles(const QStringList &files, bool display = true);
	void appendSubtitles(const QStringList &files, const QString &encoding, bool display = true);
	const QList<int> &selectedIndexes() const {return m_selected;}
	void setCurrentChannel(int channel);
	int currentChannel() const {return m_curChannel;}
	PlayEngine *engine() {return m_engine;}
	void setEnableHighQuality(bool enabled);
	const Subtitle &currentSubtitle() const;
	int syncDelay() const {return m_delay;}
	void setStyle(const OsdStyle &style) {updateStyle(style); m_style = style;}
	const OsdStyle &style() const {return m_style;}
	double pos() const {return m_pos;}
public slots:
	void setInitialPos(double pos);
	double initialPos() const {return m_initPos;}
	void select(int index);
	void deselect(int index);
	void deselectAll();
	void clear() {clear(true);}
	void moveUp();
	void moveDown();
	void move(double percent);
	void setSyncDelay(int msec);
	void addSyncDelay(int msec);
	virtual void setVisible(bool visible) = 0;
signals:
	void availableChannelsChanged(const QStringList &channels);
	void currentChannelChanged(int index);
	void availableSubtitlesChanged(const QStringList &names);
	void selectedSubtitlesChanged(const QList<int> &indexes);
	void syncDelayChanged(int);
protected:
	virtual void updateClear() = 0;
	virtual void updateSyncDelay(int msec) = 0;
	virtual void updatePos(double pos) = 0;
	virtual void updateCurrentChannel(int index) = 0;
	virtual void updateStyle(const OsdStyle &style) = 0;
	void setChannels(const QStringList &channels);
private slots:
	void update();
private:
	void updateCurrent();
	void clear(bool emits);
	class Data;
	Data *d;
	AutoSelect m_autoSelect;
	QString m_encoding;
	QStringList m_priority;
	PlayEngine *m_engine;
	int m_curChannel;
	QList<int> m_selected;
	OsdStyle m_style;
	int m_delay;
	double m_initPos;
	double m_pos;
};

}

#endif
