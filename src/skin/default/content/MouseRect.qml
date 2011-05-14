import QtQuick 1.0
import CMPlayerSkin 0.1

Rect {
	id: root

	property alias radius: bg.radius
	property real padding: 0.0
	property alias containsMouse: mouse.containsMouse
	property alias pressed: mouse.pressed

	function updateMouse(pressed, hovered) {
		if (pressed) {
			bg.color = background.color.pressed
			bg.gradient = background.gradient.pressed
			bg.border.color = border.color.pressed
			bg.border.width = border.width.pressed
		} else if (hovered) {
			bg.color = background.color.hovered
			bg.gradient = background.gradient.hovered
			bg.border.color = border.color.hovered
			bg.border.width = border.width.hovered
		} else {
			bg.color = background.color.plain
			bg.gradient = background.gradient.plain
			bg.border.color = border.color.plain
			bg.border.width = border.width.plain
		}
	}

	Component.onCompleted: {updateMouse(mouse.pressed, mouse.containsMouse)}
	background.onFillModeChanged: image_.fillMode = background.fillMode

	signal clicked()

	Rectangle {
		id: bg
		anchors.fill: parent

		Image {
			id: image_
			smooth: true
			anchors.fill: parent
		}

		MouseArea {
			id: mouse
			anchors.fill: parent
			hoverEnabled: true
			onHoveredChanged: updateMouse(pressed, containsMouse)
			onPressedChanged: updateMouse(pressed, containsMouse)
			onReleased: if (containsMouse) root.clicked()
		}
	}
}
