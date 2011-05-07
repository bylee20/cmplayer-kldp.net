import QtQuick 1.0

Rectangle {
	id: root

	property alias icon:image.source
	property bool checked: false

	color: "transparent"
	radius: 3

	function __updateBorder() {
		var width = 0
		if (mouse.pressed)
			width = 2
		else if (mouse.containsMouse || checked)
			width = 1
		border.width = width
		if (width > 0)
			border.color = "#6ad"
		else
			border.color = "transparent"
	}

	onCheckedChanged: __updateBorder()

	signal clicked()


	Image {
		id: image

		fillMode: Image.PreserveAspectFit
		smooth: true
		anchors.fill: parent
		anchors.margins: 2
	}

	MouseArea {
		id: mouse

		anchors.fill: parent
		hoverEnabled: true
		onHoveredChanged: parent.__updateBorder()
		onPressed: parent.__updateBorder()
		onReleased: {
			parent.__updateBorder()
			if (containsMouse)
				parent.clicked()
		}
	}
}

//Button {\
//	margin:0px; padding: 2px;\
//}\
//Button#flat {\
//	border: none; border-radius: 3px;\
//}\
//Button#block {\
//	border: 1px solid #999; border-radius: 0px; padding: 1px;\
//	background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #fff, stop:1 #ccc);\
//}\
//	Button#flat:hover, Button#flat:checked, Button#block:hover {\
//	border: 1px solid #6ad; padding: 1px;\
//}\
//Button#flat:pressed, Button#block:pressed {\
//	border: 2px solid #6ad; padding: 0px;\
//}\
//Button#block:checked, Button#block:pressed {\
//	background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #777, stop:1 #bbb);\
//}\
