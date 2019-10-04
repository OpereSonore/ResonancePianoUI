import QtQuick 2.7
import QtQuick.Controls 2.0

Switch {
	id: control

	indicator: Rectangle {
		implicitWidth: 48
		implicitHeight: 26
		x: control.leftPadding
		y: (parent.height / 2) - (height / 2)
		radius: 12
		color: control.checked ? control_active_color : control_background_color
		border.color: control_active_color
		border.width: 2

		Rectangle {
			x: control.checked ? parent.width - width : 0
			width: 26
			height: 26
			radius: 12
			color: handle_color
			border.width: 2
			border.color: control_active_color
		}
	}

	contentItem: Text {
		text: control.text
		verticalAlignment: Text.AlignVCenter
		font.pointSize: 8
		opacity: 1.0
		color: text_color
		leftPadding: control.indicator.width + control.spacing
	}
}
