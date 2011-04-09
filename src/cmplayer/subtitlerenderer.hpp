#ifndef SUBTITLERENDERER_HPP
#define SUBTITLERENDERER_HPP

#include "subtitle.hpp"
#include "textosdrenderer.hpp"
#include <QtCore/QObject>

class QDialog;			class Mrl;
class SubtitleComponentModel;

class SubtitleRenderer : public QObject {
	Q_OBJECT
public:
	SubtitleRenderer();
	~SubtitleRenderer();
	void setOsd(TextOsdRenderer *osd);
	TextOsdRenderer *osd() const;
	double frameRate() const;
	int delay() const;
	int start(int pos) const;
	int end(int pos) const;
	void setDelay(int delay);
	bool hasSubtitle() const;
	double pos() const;
	void setPos(double pos);
	QWidget *view(QWidget *parent = 0) const;
	void unload();
	int autoload(const Mrl &mrl, bool autoselect = true);
	void select(int idx, bool selected = true);
	void deselct(int idx) {select(idx, false);}
	const Subtitle &loaded() const;
	bool load(const QString &fileName, const QString &enc, bool select);
	QList<bool> selection() const;
public slots:
	void clear();
	void setFrameRate(double frameRate);
	void render(int ms);
	void setVisible(bool visible);
	void setHidden(bool hidden) {setVisible(!hidden);}
private:
	typedef Subtitle::Component Comp;
	typedef Comp::const_iterator CompIt;
	struct Render {
		Render() {comp = 0; model = 0;}
		Render(const Comp &comp);
		~Render();
		const Comp *comp;
		CompIt prev;
		SubtitleComponentModel *model;
	};
	typedef QList<Render*> RenderList;
	void applySelection();
	struct Data;
	Data *d;
};

#endif // SUBTITLERENDERER_HPP
