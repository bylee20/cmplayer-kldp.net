import QtQuick 1.0
import CMPlayerSkin 0.1

StyleButton {
	id: root

	property alias background: bg.background
	property alias border: bg.border
	property alias radius: bg.radius
	property real padding: 0.0
	property bool checked: false

	function updateIcon(checked) {icon_.source = checked ? icon.checked : icon.normal}

	Component.onCompleted: {updateIcon(checked); bg.clicked.connect(clicked)}
	onCheckedChanged: updateIcon(checked)
	onPaddingChanged: icon_.anchors.margins = padding

	signal clicked()

	MouseRect {
		id: bg
		anchors.fill: parent

		Image {
			id: icon_
			fillMode: Image.PreserveAspectFit
			smooth: true
			anchors.fill: parent
		}
	}
}
