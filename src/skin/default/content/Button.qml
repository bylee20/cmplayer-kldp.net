import QtQuick 1.0

Rectangle {
	id: root

	property alias icon:image.source
	property bool checked: false

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

	color: "transparent"
	radius: 3
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
