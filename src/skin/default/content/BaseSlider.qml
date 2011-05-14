import QtQuick 1.0
import CMPlayerSkin 0.1
import "util.js" as Util

StyleSlider {
	id:root

	onValueChanged: {
		handle_.x = Util.bound(0, rate*width - handle_.width/2.0, width - handle_.width)
		if (mouse.pressed)
			valueChangedByMouse();
	}

	signal valueChangedByMouse()

	MouseRect {
		id: filled_

		anchors.left: parent.left
		anchors.verticalCenter: parent.verticalCenter

		width: rate*parent.width
		height: filled.height

		border.color: filled.border.color
		border.width: filled.border.width
		background.color: filled.background.color
		background.gradient: filled.background.gradient
		background.image: filled.background.image
		background.fillMode: filled.background.fillMode
	}

	MouseRect {
		id: unfilled_

		anchors.right: parent.right
		anchors.verticalCenter: parent.verticalCenter

		width: (1.0 - rate)*parent.width
		height: unfilled.height

		border.color: unfilled.border.color
		border.width: unfilled.border.width
		background.color: unfilled.background.color
		background.gradient: unfilled.background.gradient
		background.image: unfilled.background.image
		background.fillMode: unfilled.background.fillMode
	}

	MouseRect {
		id: handle_

		width: handle.width
		height: handle.height
		anchors.verticalCenter: parent.verticalCenter

		border.color: handle.border.color
		border.width: handle.border.width
		background.color: handle.background.color
		background.gradient: handle.background.gradient
		background.image: handle.background.image
		background.fillMode: handle.background.fillMode
	}

	MouseArea {
		id: mouse
		anchors.fill: parent
		hoverEnabled: false
		onMousePositionChanged: {
			value = minimum + mouseX/width*(maximum - minimum);
			handle_.updateMouse(pressed, true);
		}
		onReleased: handle_.updateMouse(false, handle_.containsMouse)
	}
}
