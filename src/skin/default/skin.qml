import QtQuick 1.0
import CMPlayerSkin 0.1
import "content"

Skin {
	id: skin
	name: "default"

	function updateMediaInfo() {mediaInfo.text = "["+currentMedia.nth+"/"+mediaCount+"] " + currentMedia.info}

	onStorageCreated: {
		storage.hiderTop = 3
		storage.hiderBottom = -1
	}

	Component.onCompleted: {
		var margin = storage.hiderTop
		if (margin > 0) {
			hider.anchors.top = skin.top
			hider.anchors.topMargin = margin
		} else {
			hider.anchors.bottom = skin.bottom
			hider.anchors.bottomMargin = storage.hiderBottom
		}
	}
	Component.onDestruction: {
		storage.hiderBottom = hider.anchors.bottomMargin
		storage.hiderTop = hider.anchors.topMargin
	}

	autoHide: container
	hider.background.image.plain: "content/hider.png"
	hider.background.image.hovered: "content/hider-hovered.png"
	hider.background.image.pressed: "content/hider-pressed.png"
	hider.anchors.left: left
	hider.anchors.leftMargin: 3
	hider.width: 8
	hider.height: 8
	hider.z: 1e10

	hider.dragTopLeft: Qt.point(3, 3)
	hider.dragBottomRight: Qt.point(3, height - hider.height - 3)

	hider.onPressed: hider.anchors.top = hider.anchors.bottom = undefined
	hider.onReleased: {
		if (hider.y + hider.height*0.5 < height*0.5) {
			hider.anchors.topMargin = hider.y
			hider.anchors.bottomMargin = -1
			hider.anchors.top = top
		} else {
			hider.anchors.bottomMargin = height - (hider.y + hider.height)
			hider.anchors.bottom = bottom
			hider.anchors.topMargin = -1
		}
	}

	onDurationChanged: {length.text = "/" + formatMSec(duration, "hh:mm:ss"); seek.maximum = duration}
	onPositionChanged: {pos.text = formatMSec(position, "hh:mm:ss"); seek.value = position}
	onVolumeChanged: volume.value = skin.volume
	onMutedChanged: mute.checked = muted
	onPlayerStateChanged: pause.checked = (playerState == Skin.PlayingState)
	onMediaCountChanged: updateMediaInfo()
	onCurrentMediaChanged: updateMediaInfo()
//	onFullscreenChanged:

	Item {
		id: container

		anchors.fill: parent

		Spacer {
			id: ph_screen

			width: parent.width
			anchors.top: parent.top
			anchors.bottom: controls.top

			onGeometryChanged: screenGeometry = geometry;
		}

		Rectangle {
			id: controls

			width: parent.width; height: 40;
			anchors.bottom: parent.bottom
			color: "#555"

			BorderImage {
				source: "content/background.png"
				anchors.fill: parent
				border.left: 100; border.top: 0
				border.right: 100; border.bottom: 0
			}


			Item {
				id: leftBox

				anchors.top: parent.top; anchors.bottom: parent.bottom
				anchors.topMargin: 4
				anchors.left: parent.left;
				width: 70

				CButton {
					id: pause
					anchors.top: parent.top;
					anchors.topMargin: 3
					anchors.leftMargin: 4
					anchors.left: parent.left
					width: 30
					height: 30

					background.image.plain: checked ? "content/pause.png" : "content/play.png"
					background.image.hovered: checked ? "content/pause-hovered.png" : "content/play-hovered.png"
					background.image.pressed: checked ? "content/pause-pressed.png" : "content/play-pressed.png"
					action: skin.action("menu/play/pause")
				}

				CButton {
					id: backward
					anchors.top: parent.top
					anchors.topMargin: 3
					anchors.left: pause.right
					anchors.leftMargin: 4
					width: 12
					height: 12
					background.image.plain: "content/backward.png"
					background.image.hovered: "content/backward-hovered.png"
					background.image.pressed: "content/backward-pressed.png"
					action: skin.action("menu/play/seek/backward1")
				}
				CButton {
					id: forward
					anchors.top: parent.top
					anchors.topMargin: 3
					anchors.left: backward.right
					anchors.leftMargin: 4
					width: 12
					height: 12
					background.image.plain: "content/forward.png"
					background.image.hovered: "content/forward-hovered.png"
					background.image.pressed: "content/forward-pressed.png"
					action: skin.action("menu/play/seek/forward1")
				}
				CButton {
					id: prev
					anchors.bottomMargin: 4
					anchors.bottom: parent.bottom
					anchors.left: pause.right
					anchors.leftMargin: 4
					width: 12
					height: 12
					background.image.plain: "content/previous.png"
					background.image.hovered: "content/previous-hovered.png"
					background.image.pressed: "content/previous-pressed.png"
					action: skin.action("menu/play/prev")
				}
				CButton {
					id: next
					anchors.bottom: parent.bottom
					anchors.bottomMargin: 4
					anchors.left: prev.right
					anchors.leftMargin: 4
					width: 12
					height: 12
					background.image.plain: "content/next.png"
					background.image.hovered: "content/next-hovered.png"
					background.image.pressed: "content/next-pressed.png"
					action: skin.action("menu/play/next")
				}
			}

			Item {
				id: rightBox

				anchors.top: parent.top;
				anchors.topMargin: 4;
				anchors.bottom: parent.bottom
				anchors.left: leftBox.right
				anchors.right: parent.right;
				anchors.rightMargin: 2

				Item {
					id: lcd

					anchors.top: parent.top
					width: parent.width; height: 24;
					BorderImage {
						source: "content/lcd.png"
						anchors.fill: parent
						border {top: 5; right: 5; bottom: 5; left: 5}
						smooth: true
					}

					Text {
						id: mediaInfo
						anchors {
							top: parent.top; bottom: parent.bottom
							left: parent.left; right: pos.left
							leftMargin: 4; rightMargin: 3
						}
						elide: Text.ElideRight
						font.pixelSize: 11
						color: "black"
						verticalAlignment: Text.AlignVCenter
					}

					Text {
						id: pos

						width: paintedWidth; anchors.right: length.left; height: parent.height
						font.pixelSize: 11
						color: "black"
						verticalAlignment: Text.AlignVCenter
						text: "00:00:00"
					}
					Text {
						id: length

						width: paintedWidth; height: parent.height
						anchors.right: parent.right; anchors.rightMargin: 6
						text: "/00:00:00"
						font.pixelSize: 11
						color: "black"
						verticalAlignment: Text.AlignVCenter
					}
				}

				Item {
					width: parent.width
					anchors.top: lcd.bottom
					anchors.bottom: parent.bottom
					anchors.leftMargin: 1
					anchors.rightMargin: 1
					Slider {
						id: seek
						anchors.left: parent.left

						anchors.right: mute.left
						anchors.rightMargin: 1
						anchors.verticalCenter: parent.verticalCenter
						onValueChangedByUser: skin.seek(value)
					}

					CButton {
						id: mute

						width: 8
						height: 8
						anchors.top: parent.top;
						anchors.bottom: parent.bottom;
						anchors.right: volume.left
						anchors.rightMargin: 1
						action: skin.action("menu/audio/mute")
						background.image.plain: checked ? "./content/speaker-off.png" : "./content/speaker.png";
						background.image.hovered: checked ? "./content/speaker-off-hovered.png" : "./content/speaker-hovered.png"
						background.image.pressed: checked ? "./content/speaker-off-pressed.png" : "./content/speaker-pressed.png"
					}

					Slider {
						id: volume
						maximum: 100
						anchors.right: parent.right
						anchors.verticalCenter: parent.verticalCenter
						width: 70
						onValueChangedByUser: skin.volume = value
					}
				}
			}
		}

	}



}
