#ifndef XINE_SUBTITLEOUTPUT_H
#define XINE_SUBTITLEOUTPUT_H

#include <QObject>
#include <QStringList>

namespace Xine {

class XineStream;							class XineOsd;
class Subtitle;

class SubtitleOutput : public QObject {
	Q_OBJECT
public:
	enum AutoSelect {FirstFile = 0, SameName = 1, AllLoaded = 2, EachLanguage = 3};
	SubtitleOutput(XineStream *stream);
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
	const QList<int> &selectedIndexes() const {return m_selected;}
	void setCurrentChannel(int index);
	int currentChannel() const {return m_curChannel;}
	XineStream *stream() {return m_stream;}
	void setEnableHighQuality(bool enabled);
	XineOsd *osd() {return m_osd;}
	const Subtitle &currentSubtitle() const;
	int syncDelay() const {return m_delay;}
public slots:
	void setInitialPos(double pos);
	void select(int index);
	void deselect(int index);
	void deselectAll();
	void clear() {clear(true);}
	void moveUp();
	void moveDown();
	void move(double percent);
	void setSyncDelay(int msec);
	void addSyncDelay(int msec);
signals:
	void availableChannelsChanged(const QStringList &channels);
	void currentChannelChanged(int index);
	void availableSubtitlesChanged(const QStringList &names);
	void selectedSubtitlesChanged(const QList<int> &indexes);
	void syncDelayChanged(int);
private slots:
	void showSubtitle(int time);
	void update();
private:
	friend class XineStream;
	void updateChannels();
	void updateCurrent();
	void clear(bool emits);
	void renderSubtitle(const QString &text);
	class Data;
	Data *d;
	AutoSelect m_autoSelect;
	QString m_encoding;
	QStringList m_priority;
	XineStream *m_stream;
	XineOsd *m_osd;
	int m_curChannel;
	QList<int> m_selected;
	int m_delay;
	double m_initPos;
};

}

#endif
