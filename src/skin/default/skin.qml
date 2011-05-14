import QtQuick 1.0
import CMPlayerSkin 0.1
import "content"
import "content/util.js" as Util

Skin {
	id: skin

	function updateMediaInfo() {mediaInfo.text = "["+currentMedia.nth+"/"+mediaCount+"] " + currentMedia.info}

	name: "default"
	screen.width : width
	screen.anchors.top: top
	screen.anchors.bottom: controls.top
	onStorageCreated: {
		storage.hide = false
	}
	Component.onCompleted: {
		hider.checked = storage.hide
	}
	Component.onDestruction: {
		storage.hide = hider.checked
	}
	onDurationChanged: {length.text = formatMSec(duration, "hh:mm:ss"); seek.maximum = duration}
	onPositionChanged: {pos.text = formatMSec(position, "hh:mm:ss"); seek.value = position}
	onVolumeChanged: volume.value = skin.volume
	onMutedChanged: mute.checked = muted
	onPlayerStateChanged: pause.checked = (playerState == Skin.PlayingState)
	onMediaCountChanged: updateMediaInfo()
	onCurrentMediaChanged: updateMediaInfo()
	onFullscreenChanged: {
		hider.visible = !fullscreen;
		if (fullscreen || hider.checked) {
			controls.visible = false
			screen.anchors.bottom = bottom
		} else {
			controls.visible = true
			screen.anchors.bottom = controls.top
		}
	}

	MouseArea {
		id: mouseTracker

//		function __contains(min, val, max) {return min <= val && val <= max;}
		hoverEnabled: true
		anchors.fill: parent
		onPositionChanged: {
			if (parent.visible && parent.fullscreen) {
				controls.visible = Util.inRange(0, mouseX-controls.pos.x, controls.width)
						&& Util.inRange(0, mouseY-controls.pos.y, controls.height)
				hider.visible = controls.visible
			}
		}
	}

	Rectangle {
		id: controls

		width: parent.width; height: 38; anchors.bottom: parent.bottom
		color: "#555"

		Image {
			anchors.fill: parent
			source: "content/background.svg"
			smooth: true
		}

		Rectangle {
			id: boundary

			width: parent.width; height: 4; anchors.top: parent.top
			gradient: Gradient {
				GradientStop {position: 0.0; color: "#000"}
				GradientStop {position: 0.1; color: "#000"}
				GradientStop {position: 0.5; color: "#888"}
				GradientStop {position: 0.6; color: "#000"}
				GradientStop {position: 1.0; color: "#000"}
			}
		}


		Item {
			id: leftBox

			anchors.top: boundary.bottom; anchors.bottom: parent.bottom
			anchors.left: parent.left; anchors.margins: 1
			width: 2*(height + 1) + 1

			Button {
				id: pause
				anchors.top: parent.top;
				anchors.bottom: parent.bottom;
				anchors.left: parent.left
				width: height
				icon.normal: "content/play.png"
				icon.checked: "content/pause.png"
				onClicked: skin.exec("menu/play/pause")
			}

			Button {
				id: backward
				anchors.top: parent.top
				anchors.left: pause.right
				anchors.leftMargin: 1
				width: pause.width/2-0.5
				height: width
				icon.normal: "content/backward.png"
				onClicked: skin.exec("menu/play/seek/backward1")
			}
			Button {
				id: forward
				anchors.top: parent.top
				anchors.left: backward.right
				anchors.leftMargin: 1
				width: backward.width
				height: backward.height
				icon.normal: "content/forward.png"
				onClicked: skin.exec("menu/play/seek/forward1")
			}
			Button {
				id: prev
				icon.normal: "content/previous.png"
				anchors {bottom: parent.bottom; left: pause.right; leftMargin: 1}
				width: backward.width
				height: backward.height
				onClicked: skin.exec("menu/play/prev")
			}
			Button {
				id: next
				anchors.bottom: parent.bottom
				anchors.left: prev.right
				anchors.leftMargin: 1
				width: backward.width
				height: backward.height
				icon.normal: "content/next.png"
				onClicked: skin.exec("menu/play/next")
			}
		}

		Item {
			id: rightBox

			anchors.top: boundary.bottom;
			anchors.topMargin: 0;
			anchors.bottom: parent.bottom
			anchors.left: leftBox.right
			anchors.right: parent.right;
			anchors.rightMargin: 2

			Rectangle {
				id: lcd

				anchors.top: parent.top
				width: parent.width; height: 22; radius: 3
				border.color: "#aaa"; border.width: 1
				gradient: Gradient {
					GradientStop {position: 0.0; color: "#111"}
					GradientStop {position: 0.1; color: "#6ad"}
					GradientStop {position: 0.8; color: "#6ad"}
					GradientStop {position: 1.0; color: "#fff"}
				}

				Text {
					id: mediaInfo
					anchors {
						top: parent.top; bottom: parent.bottom
						left: parent.left; right: pos.left
						leftMargin: 3; rightMargin: 3
					}
					elide: Text.ElideRight
					font.pixelSize: 11
					verticalAlignment: Text.AlignVCenter
				}

				Text {
					id: pos

					width: paintedWidth; anchors.right: slash.left; height: parent.height
					font.pixelSize: 11
					verticalAlignment: Text.AlignVCenter
					text: "00:00:00"
				}
				Text {
					id: slash

					width: paintedWidth; height: parent.height; anchors.right: length.left
					text: "/"
					font.pixelSize: 11
					verticalAlignment: Text.AlignVCenter
				}
				Text {
					id: length

					width: paintedWidth; height: parent.height
					anchors.right: parent.right; anchors.rightMargin: 3
					text: "00:00:00"
					font.pixelSize: 11
					verticalAlignment: Text.AlignVCenter
				}
			}

			Item {
				width: parent.width
				anchors.top: lcd.bottom
				anchors.bottom: parent.bottom
				Slider {
					id: seek
					anchors.left: parent.left
					anchors.right: mute.left
					anchors.rightMargin: 2
					anchors.verticalCenter: parent.verticalCenter
					onValueChangedByMouse: skin.seek(value)
				}

				Button {
					id: mute
					icon.normal: "./content/speaker.png"
					icon.checked: "./content/speaker-off.png"

					width: 10
					height: 10
					anchors.top: parent.top; anchors.topMargin: 1
					anchors.bottom: parent.bottom; anchors.bottomMargin: 1
					anchors.right: volume.left
					anchors.rightMargin: 2
					onClicked: skin.exec("menu/audio/mute")
				}

				Slider {
					id: volume
					maximum: 100
					anchors.right: parent.right
					anchors.verticalCenter: parent.verticalCenter
					width: 70
					onValueChangedByMouse: skin.volume = value
				}
			}
		}
	}

	Rectangle {
		id: hider

		property alias checked: button.checked

		anchors.margins: 1
		anchors.bottom: parent.bottom;
		anchors.left:parent.left
		width: 8
		height: 8
		radius: 4
		color: "#333"
		border.width: 2
		border.color: "#555"

		onCheckedChanged: {
			controls.visible = !checked
			if (fullscreen || checked)
				screen.anchors.bottom = parent.bottom
			else
				screen.anchors.bottom = controls.top
		}

		Button {
			id: button
			anchors.fill: parent
			radius: parent.radius
			onClicked: checked = !checked;
		}
	}
}
