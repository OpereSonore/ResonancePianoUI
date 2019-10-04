import QtQuick 2.7
import QtQuick.Controls 2.0

Slider {
	id: control

	handle: Rectangle {
		x: control.leftPadding + (control.visualPosition * (control.availableWidth - width))
		y: control.topPadding + (control.availableHeight / 2) - (height / 2)
		implicitWidth: 1
		implicitHeight: 1
		radius: 12
		color: handle_color
		border.color: control_active_color
		border.width: 2
	}

	background: Rectangle {
		x: control.leftPadding
		y: control.topPadding + (control.availableHeight / 2) - (height / 2)
		implicitHeight: 8
		width: control.availableWidth
		height: implicitHeight
		radius: 12
		color: control_background_color

		Rectangle {
			width: control.visualPosition * parent.width
			height: parent.height
			color: control_active_color
			radius: 12
		}
	}
}
