import QtQuick 2.7
import QtQuick.Controls 2.0

Button {
	id: control
	contentItem: Text {
		text: control.text
		font: control.font
		opacity: enabled ? 1.0 : 0.3
		color: text_color
		horizontalAlignment: Text.AlignHCenter
		verticalAlignment: Text.AlignVCenter
		elide: Text.ElideRight
	}

	background: Rectangle {
		color: background_stop_gradient
		opacity: enabled ? 1 : 0.3
		border.color: control.down ? control_active_color : frame_border_color
		border.width: 2
		radius: 8
	}
}
