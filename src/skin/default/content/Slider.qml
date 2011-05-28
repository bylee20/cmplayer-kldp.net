import QtQuick 1.0
import CMPlayerSkin 0.1
import "../tools"

CSlider {
	id:root

	height: 8

	handle.width: 8
	handle.height: 8
	handle.background.image.plain: "../content/slider-handle.png"
	handle.background.image.hovered: "../content/slider-handle-hovered.png"
	handle.background.image.pressed: "../content/slider-handle-pressed.png"

	filled.height: 6
	filled.border.width.plain: 1
	filled.border.color.plain: "#999"
	filled.border.color.hovered: "#6ad"
	filled.background.gradient.plain: CGradient {
		CGradientStop {position: 0.0; color: "#fff"}
		CGradientStop {position: 1.0; color: "#bbf"}
	}

	unfilled.height: filled.height
	unfilled.border.width.plain: 1
	unfilled.border.color.plain: "#999"
	unfilled.background.gradient.plain: CGradient {
		CGradientStop {position: 0.0; color: "#333"}
		CGradientStop {position: 1.0; color: "#bbb"}
	}
}
