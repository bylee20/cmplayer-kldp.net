#include "subtitle_parser_p.hpp"
#include "tagiterator.hpp"
#include "global.hpp"
#include <QtCore/QDebug>
#include <list>

//class Block {
//public:
//	virtual ~Block() {qDeleteAll(m_children);}
//	virtual void appendHtml(QString &html) const = 0;
//	virtual bool isBlock() const = 0;
//protected:
//	static bool needToAddSharp(const QStringRef &name, const QStringRef &value) {
//		if (value.size() != 6)
//			return false;
//		if (!TagIterator::opEq(name, "color"))
//			return false;
//		for (int i=0; i<6; ++i) {
//			const ushort ucs = value.at(i).unicode();
//			if (!RichString::isHexNumber(ucs))
//				return false;
//		}
//		return true;
//	}
//	static void appendAttr(QString &html, const TagIterator::Attr &attr) {
//		if (attr.name.isEmpty())
//			return;
//		html += QLatin1Char(' ');
//		html += attr.name;
//		if (!attr.value.isEmpty()) {
//			html += QLatin1Char('=');
//			if (attr.q.unicode() != '\0')
//				html += attr.q;
//			if (needToAddSharp(attr.name, attr.value))
//				html += QLatin1Char('#');
//			html += attr.value;
//			if (attr.q.unicode() != '\0')
//				html += attr.q;
//		}
//	}

//	static inline QStringRef midRef(const QStringRef &ref, int from, int n = -1) {
//		return ref.string()->midRef(ref.position() + from, n);
//	}
//	static inline bool skipSpaces(int &pos, const QStringRef &ref) {
//		if (pos < 0)
//			return false;
//		bool ret = false;
//		for (; pos < ref.size(); ++pos) {
//			const ushort ucs = ref.at(pos).unicode();
//			if (ucs == '\n' || ucs == '\r' || ucs == '\v')
//				continue;
//			if (ucs == ' ' || ucs == '\t') {
//				ret = true;
//				continue;
//			}
//			return ret;
//		}
//		return ret;
//	}
//	QList<Block*> m_children;
//};


//struct TextBlock : public Block {
//	bool isBlock() const {return false;}
//	static TextBlock *make(const QStringRef &text) {
//		TextBlock *block = new TextBlock;
//		block->m_text = text;
//		return block;
//	}
//	void appendHtml(QString &html) const {
//		for (int pos = 0; pos <m_text.size(); ++pos) {
//			const ushort ucs = m_text.at(pos).unicode();
//			if (ucs == '\n' || ucs == '\r' || ucs == '\v')
//				continue;
//			if (ucs == ' ' || ucs == '\t') {
//				static const QChar spc(' ');
//				if (!html.endsWith(spc))
//					html += spc;
//			} else
//				html += ucs;
//		}
//	}
//private:
//	QStringRef m_text;
//};

//void process(QString &html, QString &plain, const QStringRef &text, TagIterator &tag) {

//}

//class TagBlock : public Block {
//public:
//	TagBlock() {m_pair = false; m_block = false;}
//	static TagBlock *make(const QStringRef &text, TagIterator &tag) {
//		if (tag.begin() < 0)
//			return 0;
//		TagBlock *block = new TagBlock;
//		block->m_pair = false;
//		block->m_block = isBlock(tag);
//		block->m_elem = tag.element();
//		const int acount = tag.attributeCount();
//		for (int i=0; i<acount; ++i)
//			block->m_attr.append(tag.attribute(i));
//		if (const char *pair = pairTag(tag)) {
//			block->m_pair = true;
//			for (;;) {
//				const int prev = tag.pos();
//				const int begin = tag.next();
//				if (begin < 0) {
//					if (TextBlock *b = TextBlock::make(midRef(text, prev)))
//						block->m_children.append(b);
//					return block;
//				}
//				if (prev < begin) {
//					if (TextBlock *b = TextBlock::make(midRef(text, prev, begin-prev)))
//						block->m_children.append(b);
//				}
//				if (tag.elementIs(pair))
//					return block;
//				if (tag.elementIs("sync") || tag.elementIs("/body"))
//					return block;
//				if (TagBlock *b = TagBlock::make(text, tag))
//					block->m_children.append(b);
//			}
//		} else {
//			return block;
//		}
//	}
//	void appendHtml(QString &html) const {
//		html += QLatin1Char('<');
//		html += m_elem;
//		for (int i=0; i<m_attr.size(); ++i) {
//			html += QLatin1Char(' ');
//			html += m_attr[i].name;
//			if (!m_attr[i].value.isEmpty()) {
//				html += QLatin1String("=\"");
//				html += m_attr[i].value;
//				html += '"';
//			}
//		}
//		html += QLatin1Char('>');
//		for (int i=0; i<m_children.size(); ++i) {
//			m_children[i]->appendHtml(html);
//		}
//		if (m_pair) {
//			html += QLatin1String("</");
//			html += m_elem;
//			html += QLatin1Char('>');
//		}
//		html += QLatin1Char('\n');
//	}
//	void appendPlain(QString &plain) const {

//	}

//	bool isBlock() const {return m_block;}
//private:
//	static bool isBlock(const TagIterator &tag) {
//		return tag.elementIs("p");
//	}
//	static const char *pairTag(const TagIterator &tag) {
//#define CHECK_TAG(elem) if (tag.elementIs(elem)) {return "/"elem;}
//		CHECK_TAG("p");
//		CHECK_TAG("sync");
//		CHECK_TAG("font");
//		CHECK_TAG("span");
//#undef CHECK_TAG
//		return 0;
//	}
//	QStringRef m_elem;
//	QList<TagIterator::Attr> m_attr;
//	bool m_pair, m_block;
//};

//class SyncBlock : public Block {
//	QList<TagIterator::Attr> m_attr;
//public:
//	bool isBlock() const {return true;}
//	static SyncBlock *make(const QStringRef &text, TagIterator &tag) {
//		if (!tag.elementIs("sync"))
//			return 0;
//		SyncBlock *sync = new SyncBlock;
//		const int acount = tag.attributeCount();
//		for (int i=0; i<acount; ++i)
//			sync->m_attr.append(tag.attribute(i));
//		for (;;) {
//			const int prev = tag.pos();
//			const int begin = tag.next();
//			if (begin < 0) {
//				if (TextBlock *b = TextBlock::make(midRef(text, prev)))
//					sync->m_children.append(b);
//				return sync;
//			}
//			if (prev < begin) {
//				if (TextBlock *b = TextBlock::make(midRef(text, prev, begin-prev)))
//					sync->m_children.append(b);
//			}
//			if (tag.elementIs("/sync") || tag.elementIs("sync") || tag.elementIs("/body"))
//				return sync;
//			if (TagBlock *b = TagBlock::make(text, tag))
//				sync->m_children.append(b);
//		}
//	}
//	void appendHtml(QString &html) const {
//		html += QLatin1String("<span");
//	}
//};


void Subtitle::Parser::Sami::_parse(Subtitle &sub) {
	int sync = -1;
	TagIterator tag(all());
	for (;;) {
		const int pos = tag.next();
		if (pos < 0)
			break;
		if (tag.elementIs("sync")) {
			sync = pos;
			break;
		}
	}
	if (sync < 0)
		return;

	QMap<QString, int> idxes;
	const QString header = all().left(sync);
	for (int i = 0;;) {
		static QRegExp rxLang("\\s*\\.(\\w+)\\s*\\{(.+)\\}");
		i = rxLang.indexIn(header, i);
		if (i < 0)
			break;
		static QRegExp rxParam("\\s*(\\S+)\\s*:\\s*(\\S+)\\s*;\\s*");
		int pos = 0;
		Component comp(name());
		comp.m_lang.m_klass = rxLang.cap(1);
		const QString text = rxLang.cap(2);
		while ((pos = rxParam.indexIn(text, pos)) != -1) {
			const QString key = rxParam.cap(1);
			if (!key.compare("name", Qt::CaseInsensitive))
				comp.m_lang.m_name = rxParam.cap(2);
			else if (!key.compare("lang", Qt::CaseInsensitive))
				comp.m_lang.m_locale = rxParam.cap(2);
			pos += rxParam.matchedLength();
		}
		const int idx = idxes.size();
		idxes.insert(comp.m_lang.m_klass, idx);
		sub.m_comp.push_back(comp);
		i += rxLang.matchedLength();
	}

	for (;;) {
		if (sync < 0)
			break;
		int time = tag.valueToInt("start");
		QString klass = tag.value("class").toString();
		QString text, plain;
		int brIdx = -1;
		std::list<QStringRef> pair;
		for (;;) {
			int a = tag.pos();
			const bool br = tag.elementIs("br");
			if (br) {
				if (!text.isEmpty()) {
					if (brIdx == -1)
						brIdx = text.size();
					appendTo(text, tag);
					plain += '\n';
				}
			} else {
				brIdx = -1;
				appendTo(text, tag);
				if (tag.elementIs("p") || tag.elementIs("sync") || tag.elementIs("span") || tag.elementIs("font"))
					pair.push_back(tag.element());
				else if (!tag.element().isEmpty() && tag.element().at(0) == '/') {
					std::list<QStringRef>::reverse_iterator it;
					for (it=pair.rbegin(); it != pair.rend(); ++it) {
						if (it->compare(RichString::midRef(tag.element(), 1), Qt::CaseInsensitive) == 0) {
							pair.erase(--it.base());
							break;
						}
					}
				}
			}
			const int idx = tag.next();
			const int count = idx < 0 ? -1 : idx - a;
			const QStringRef ref = all().midRef(a, count);
			const int prev = text.size();
			RichString::process(ref, text, plain, false);
			if (brIdx != -1) {
				for (int i=text.size()-1; i>=prev; --i) {
					if (!isspace(text[i].unicode())) {
						brIdx = -1;
						break;
					}
				}
			}
			if (idx < 0) {
				sync = -1;
				break;
			}
			if (klass.isEmpty())
				klass = tag.value("class").toString();
			if (tag.elementIs("sync")) {
				sync = idx;
				break;
			} else if (tag.elementIs("/body")) {
				sync = -1;
				break;
			}
		}
		int chop = 0;
		for (int i=plain.size()-1; i>=0 && isspace(plain[i].unicode()); --i, ++chop) ;
		plain.chop(chop);
		if (brIdx != -1)
			text.chop(text.size() - brIdx);
		std::list<QStringRef>::const_reverse_iterator rit = pair.rbegin();
		for (; rit!=pair.rend(); ++rit) {
			text += QLatin1String("</");
			text += *rit;
			text += QLatin1Char('>');
		}

		Node node;
		node.text = RichString(text, plain);
		QMap<QString, int>::const_iterator it = idxes.find(klass);
		if (it == idxes.end()) {
			const int idx = idxes.size();
			idxes.insert(klass, idx);
			Component comp(name());
			comp.m_lang.m_klass = klass;
			sub.m_comp.push_back(comp);
		}
		if (!node.text.hasWords())
			node.text.clear();
		sub.m_comp[idxes[klass]].insert(time, node);
	}
	for (int i=0; i<sub.m_comp.size(); ++i) {
		Component &comp = sub.m_comp[i];
		if (comp.isEmpty())
			continue;
		const int end = predictEndTime(comp.end()-1);
		if (end >= 0)
			comp[end] = Node();
	}
}

bool Subtitle::Parser::Sami::needToAddSharp(const QStringRef &name, const QStringRef &value) {
	if (value.size() != 6)
		return false;
	if (!TagIterator::opEq(name, "color"))
		return false;
	for (int i=0; i<6; ++i) {
		const ushort ucs = value.at(i).unicode();
		if (!RichString::isHexNumber(ucs))
			return false;
	}
	return true;
}

QString &Subtitle::Parser::Sami::appendTo(QString &rich, const TagIterator &tag) {
	if (tag.element().isEmpty())
		return rich;
	rich += '<';
	if (tag.elementIs("p"))
		rich += QLatin1String("span");
	else
		rich += tag.element();
	for (int i=0; i<tag.attributeCount(); ++i) {
		const TagIterator::Attr &attr = tag.attribute(i);
		rich += ' ';
		rich += attr.name;
		if (!attr.value.isEmpty()) {
			rich += QLatin1Char('=');
			if (attr.q.unicode() != '\0')
				rich += attr.q;
			if (needToAddSharp(attr.name, attr.value))
				rich += QLatin1Char('#');
			rich += attr.value;
			if (attr.q.unicode() != '\0')
				rich += attr.q;
		}
	}
	if (!tag.isOpen())
		rich += '>';
	return rich;
}

QRegExp Subtitle::Parser::TMPlayer::rxLine("^\\s*(\\d?\\d)\\s*:\\s*(\\d\\d)\\s*:\\s*(\\d\\d)\\s*:\\s*(.*)$");

void  Subtitle::Parser::TMPlayer::_parse(Subtitle &sub) {
	sub.append(Component(name()));
	Component &comp = sub.m_comp[0];
	int predictedEnd = -1;
	while (!atEnd()) {
		const QString line = getLine();
		if (rxLine.indexIn(line) == -1)
			continue;
#define T_INT(nth) rxLine.cap(nth).toInt()
		const int time = timeToMSecs(T_INT(1), T_INT(2), T_INT(3));
#undef T_INT
		Subtitle::Node node;
		if (predictedEnd > 0 && time > predictedEnd)
			comp.insert(predictedEnd, node);
		QString text = rxLine.cap(4);
		replaceEntity(text).replace('|', "<br>");
		node.text = RichString(text);
		predictedEnd = predictEndTime(comp.insert(time, node));
	}
}

QRegExp Subtitle::Parser::MicroDVD::rxLine("^\\{(\\d+)\\}\\{(\\d+)\\}(.*)$");

void Subtitle::Parser::MicroDVD::_parse(Subtitle &sub) {
	sub.append(Component(name(), Component::Frame));
	Component &comp = sub.m_comp[0];
	while (!atEnd()) {
		const QString line = getLine();
		if (rxLine.indexIn(line) == -1)
			continue;
		const int start = rxLine.cap(1).toInt();
		const int end = rxLine.cap(2).toInt();
		QString text = rxLine.cap(3).trimmed();
		text.replace('|', "<br>");
		Node node;
		node.text = RichString(text);
		comp.insert(start, node);
		comp.insert(end, Node());
	}
}

void Subtitle::Parser::SubRip::_parse(Subtitle &sub) {
	sub.append(Component(name()));
	Component &comp = sub.m_comp[0];
	QString line;
	while (!atEnd()) {
		line = getLine();
		static QRegExp rxNum("^(\\d+)$");
#define RX_TIME "(\\d\\d):(\\d\\d):(\\d\\d),(\\d\\d\\d)"
		static QRegExp rxTime("^"RX_TIME" --> "RX_TIME"$");
#undef RX_TIME
		if (rxNum.indexIn(line) == -1)
			continue;
		while (!atEnd()) {
			line = getLine();
			if (rxTime.indexIn(line) != -1)
				break;
		}
		if (atEnd())
			return;
#define T_INT(nth) rxTime.cap(nth).toInt()
		const int start = timeToMSecs(T_INT(1), T_INT(2), T_INT(3), T_INT(4));
		const int end = timeToMSecs(T_INT(5), T_INT(6), T_INT(7), T_INT(8));
#undef T_INT
		QString text;
		while (!atEnd()) {
			line = getLine();
			if (line.isEmpty())
				break;
			text += line + '\n';
		}
		text.chop(1);
		text.replace('\n', "<br>");
		Node node;
		node.text = text;
		comp.insert(start, node);
		comp.insert(end, Node());
	}
}
