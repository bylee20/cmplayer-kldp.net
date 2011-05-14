import QtQuick 1.0
import CMPlayerSkin 0.1

BaseSlider {
	id:root

	height: 7

	handle.width: 7
	handle.height: 7
	handle.border.color.plain: "#5c5c5c"
	handle.border.color.hovered: "#6ad"
	handle.border.color.pressed: "#6ad"
	handle.border.width.plain: 1
	handle.border.width.pressed: 2
	handle.background.gradient.plain: Gradient {
		GradientStop {position: 0.0; color: "#aaa"}
		GradientStop {position: 1.0; color: "#999"}
	}
	handle.background.gradient.pressed: Gradient {
		GradientStop {position: 0.0; color: "#fff"}
		GradientStop {position: 1.0; color: "#ccc"}
	}

	filled.height: 5
	filled.border.width.plain: 1
	filled.border.color.plain: "#6ad"
	filled.background.gradient.plain: Gradient {
		GradientStop {position: 0.0; color: "#fff"}
		GradientStop {position: 1.0; color: "#ccc"}
	}

	unfilled.height: filled.height
	unfilled.border.width.plain: 1
	unfilled.border.color.plain: "#999"
	unfilled.background.gradient.plain: Gradient {
		GradientStop {position: 0.0; color: "#333"}
		GradientStop {position: 1.0; color: "#bbb"}
	}
}
