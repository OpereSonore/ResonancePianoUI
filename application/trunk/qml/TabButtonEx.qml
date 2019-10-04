import QtQuick 2.6
import QtQuick.Controls 2.0

TabButton {
	id: control

	implicitHeight: parent.height
	implicitWidth: parent.width

	property var image_source

	contentItem: Image {
		id: image
		anchors.fill: control
		source: image_source
		smooth: true
		opacity: checked ? 1.0 : 0.3
		fillMode: Image.Pad
		sourceSize.width: 28
		sourceSize.height: 28
		Rectangle {
			anchors.centerIn: parent
			opacity: checked ? 1.0 : 0.3
			border.color: control_active_color
			border.width: 2
			implicitHeight: 40
			implicitWidth: 40
			color: 'transparent'
			radius: 10
		}
	}

	background: Rectangle {
		color: background_stop_gradient
	}
}
