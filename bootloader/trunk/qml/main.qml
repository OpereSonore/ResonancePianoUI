import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.1

ApplicationWindow {
	id: applicationWindow
	visible: true
	width: 800
	height: 480
	property alias flickable_text_box: flickable_text_box
	title: qsTr("Bootloader v1.0.0")

	property string background_start_gradient: "#323232"
	property string background_stop_gradient: "#323232"
	property string frame_border_color: "#bdbebf"
	property string control_active_color: "#21be2b"
	property string text_color: "#21be2b"
	property string control_background_color: "#bdbebf"
	property string handle_color: "#f0f0f0"

	background: GradientEx {
	}

	ColumnLayout {
		anchors.top: parent.top
		anchors.topMargin: 10
		anchors.bottom: parent.bottom
		anchors.bottomMargin: 9
		anchors.right: parent.right
		anchors.rightMargin: 10
		anchors.left: parent.left
		anchors.leftMargin: 10
		spacing: 10

		Flickable {
			id: flickable_text_box
			Layout.preferredHeight: 390
			Layout.fillHeight: false
			Layout.fillWidth: true
			TextArea.flickable: TextArea {
				text: update_handler.StadndardOutput
				wrapMode: TextArea.Wrap
				onTextChanged: (update_handler.ScrollEnable ? cursorPosition
															  = text.length : cursorPosition = 0)
			}

			ScrollIndicator.vertical: ScrollIndicator {
			}
		}
		RowLayout {
			transformOrigin: Item.Center
			Layout.alignment: Qt.AlignRight | Qt.AlignBottom
			ButtonEx {
				id: reboot_button
				text: "Reboot"
				Layout.preferredHeight: 60
				Layout.preferredWidth: 200
				visible: update_handler.RebootEnable
				onClicked: update_handler.rebootButtonClick()
			}
			ButtonEx {
				id: cancel_button
				text: "Cancel"
				Layout.preferredHeight: 60
				Layout.preferredWidth: 200
				visible: update_handler.CancelEnable
				onClicked: update_handler.cancelButtonClick()
			}
			ButtonEx {
				id: update_button
				text: "Update"
				Layout.preferredHeight: 60
				Layout.preferredWidth: 200
				visible: update_handler.UpdateEnable
				onClicked: update_handler.updateButtonClick()
			}
		}
	}
}
