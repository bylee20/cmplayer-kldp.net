//#include "richtext.hpp"
//#include <QtCore/QDebug>
//#include <QtGui/QFont>
//#include "tagiterator.hpp"
//#include <QtGui/QPainter>




//struct RootBlock : public Block {
//	void draw(QPainter *painter, const QRectF &rect) {}
//	bool isBlock() const {return true;}
//	QString toHtml() const {
//		QString html;
//		appendHtml(html);
//		return html;
//	}
//	void appendHtml(QString &html) const {
//		for (int i=0; i<m_children.size(); ++i)
//			m_children[i]->appendHtml(html);
//	}
//	static RootBlock *make(const QString &text, const QFont &font, const QPen &pen) {
//		RootBlock *root = new RootBlock;
//		TagIterator tag(text);
//		tag.setPos(0);
//		for (;;) {
//			const int prev = tag.pos();
//			const int begin = tag.next();
//			if (begin < 0) {
//				if (TextBlock *block = TextBlock::make(text.midRef(prev), font))
//					root->m_children.append(block);
//				return root;
//			}
//			if (prev < begin) {
//				if (TextBlock *block = TextBlock::make(text.midRef(prev, begin-prev), font))
//					root->m_children.append(block);
//			}
//			if (TagBlock *block = TagBlock::make(text.midRef(0), font, pen, tag))
//				root->m_children.append(block);
//		}
//		return root;
//	}
//};

//struct RichText::Data {
//	QFont font;
//	RootBlock *root;
//};


//RichText::RichText()
//: d(new Data) {
//	d->root = 0;
//}

//void RichText::setText(const QString &text) {
//	d->root = RootBlock::make(text, QFont(), QPen());
//	qDebug() << "html:" << d->root->toHtml();
//}
