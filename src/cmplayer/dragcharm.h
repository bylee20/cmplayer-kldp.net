/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the Graphics Dojo project on Qt Labs.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 or 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef DRAGMOVECHARM_H
#define DRAGMOVECHARM_H

#include <QtCore/QObject>
#include <QtCore/QRect>

class QWidget;

class DragCharm: public QObject {
public:
	DragCharm(QObject *parent = 0);
	~DragCharm();
	void activate(QWidget *widget);
	void deactivate();
	void setRect(const QRect &rect);
	void setBorder(int width);
private:
	enum Bound {Top = 0x01, Bottom = 0x02, Left = 0x04, Right = 0x08
		, TopLeft = Top | Left, TopRight = Top | Right
		, BottomLeft = Bottom | Left, BottomRight = Bottom | Right};
// 	enum Bound {NotBound, TopLeft, Top, TopRight, Left, Right, BottomLeft, Bottom, BottomRight};
	static void setTracking(QWidget *widget);
	bool eventFilter(QObject *object, QEvent *event);
	struct Data;
	Data *d;
};

#endif // DRAGMOVECHARM_H
