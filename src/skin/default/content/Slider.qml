import QtQuick 1.0

Item {
	id:root

	property int minimum: 0
	property int maximum: 0
	property int value: 0
	property real __rate: 0

	signal valueChangedByUser()

	function bound(min, val, max) {	return Math.min(max, Math.max(min, val));}
	function clip(val) {return bound(minimum, val, maximum);}

	onValueChanged: {
		if (maximum < minimum)
			value = minimum;
		else if (value > maximum)
			value = maximum
		else if (value < minimum)
			value = minimum
		else {
			__rate = ((value - minimum)/(maximum - minimum))
			handle.x = bound(0, __rate*width - handle.width/2.0, width - handle.width)
		}
	}

	Item {
		id: slider

		anchors {
			left: parent.left; right: parent.right;
			verticalCenter: parent.verticalCenter
		}
		height: 7

		Rectangle {
			id: filled

			anchors {
				top: parent.top
				bottom: parent.bottom
				left: parent.left
				topMargin: 1
				bottomMargin: 1
			}
			width: root.__rate*parent.width
			border.width: 1; border.color: "#66aadd"
			gradient: Gradient {
				GradientStop {position: 0.0; color: "#ffffff"}
				GradientStop {position: 1.0; color: "#cccccc"}
			}
		}
		Rectangle {
			id: unfilled

			anchors {
				top: parent.top; bottom: parent.bottom
				topMargin: 1; bottomMargin: 1
				right: parent.right
			}
			width: (1.0-__rate)*parent.width
			border.width: 1; border.color: "#999999"
			gradient: Gradient {
				GradientStop {position: 0.0; color: "#333333"}
				GradientStop {position: 1.0; color: "#bbbbbb"}
			}
		}

		Rectangle {
			id: handle

			function updateColor(hover, pressed) {
				if (pressed) {
					border.color = "#6ad"
					border.width = 2
					gradient.stops[0].color = "#fff"
					gradient.stops[1].color = "#ccc"
				} else {
					border.width = 1
					if (hover)
						border.color = "#6ad"
					else
						border.color = "#5c5c5c"
					gradient.stops[0].color = "#aaa"
					gradient.stops[1].color = "#999"
				}
			}

			width: 7
			anchors.top: parent.top; anchors.bottom: parent.bottom
			border.width: 1; border.color: "#5c5c5c"
			gradient: Gradient {
				GradientStop {id: stop1; position: 0.0; color: "#aaa"}
				GradientStop {id: stop2; position: 1.0; color: "#999"}
			}

			MouseArea {
				id: handleMouse

				anchors.fill: parent
				hoverEnabled: true
				onHoveredChanged: parent.updateColor(containsMouse, pressed)
				onPressed: parent.updateColor(containsMouse, pressed)
			}
		}
		MouseArea {
			anchors.fill: parent

			onMousePositionChanged: {
				var x = Math.min(Math.max(0, mouseX - handle.width/2.0), width - handle.width)
				var changed = (x != handle.x)
				root.value = root.clip(root.minimum + mouseX/width*(root.maximum - root.minimum));
				if (changed)
					root.valueChangedByUser()
				handle.updateColor(true, pressed)
			}
			onReleased: handle.updateColor(handleMouse.containsMouse, false)
		}
	}
}
