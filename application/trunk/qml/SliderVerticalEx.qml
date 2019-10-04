import QtQuick 2.7
import QtQuick.Controls 2.0

Slider {
	id: control

	property real animated_value
	property alias animator: animator
	signal handlePressed

	handle: Rectangle {
		id: handle
		x: control.leftPadding + ((control.availableWidth - width) / 2)
		y: control.topPadding + control.visualPosition * (control.availableHeight - height)
		implicitWidth: 26
		implicitHeight: 26
		radius: 12
		color: handle_color
		border.color: enabled ? control_active_color : control_background_color
		border.width: 2

		MouseArea {
			anchors.fill: parent
			hoverEnabled: true
			propagateComposedEvents: true
			onPressed: {
				control.handlePressed()
				mouse.accepted = false
			}
		}
	}

	background: Rectangle {
		x: control.leftPadding + ((control.availableWidth - width) / 2)
		y: control.topPadding + 0
		implicitWidth: 8
		width: implicitWidth
		height: control.availableHeight
		radius: 12
		color: control_active_color

		Rectangle {
			width: parent.width
			height: control.visualPosition * parent.height
			color: control_background_color
			radius: 12
		}
	}

	PropertyAnimation {
		id: animator
		target: control
		property: "value"
		to: animated_value
		duration: 100
		Component.onCompleted: running = false
	}
}
