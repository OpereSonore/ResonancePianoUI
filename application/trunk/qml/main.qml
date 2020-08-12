import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.1

ApplicationWindow {
	id: applicationWindow
	visible: true
	width: 800
	height: 480
	title: qsTr("Pianolight v1.2.0")

	property string background_start_gradient: "#323232"
	property string background_stop_gradient: "#323232"
	property string frame_border_color: "#bdbebf"
	property string control_active_color: "#21be2b"
	property string text_color: "#21be2b"
	property string control_background_color: "#bdbebf"
	property string handle_color: "#f0f0f0"

	background: GradientEx {}

	Connections {
		target: assert
		onOpenAssertDialog: {
			assert_dialog.text = message
			assert_dialog.open()
		}
	}

	MessageDialog {
		id: assert_dialog
		title: "Fatal error"
		text: "ASSERT(false) in [HERE FILE NAME] at [HERE LINE]"
		icon: StandardIcon.Critical
		modality: Qt.WindowModal
		Component.onCompleted: visible = false
		standardButtons: StandardButton.Abort
		onButtonClicked: {
			assert.Close()
		}
	}

	Connections {
		target: update_handler
		onOpenUpdateDialog: {
			update_dialog.open()
		}
	}

	MessageDialog {
		id: update_dialog
		title: "Update found"
		text: "Update found on usb key: Would you like to apply it?"
		icon: StandardIcon.Warning
		modality: Qt.WindowModal
		Component.onCompleted: visible = false
		standardButtons: StandardButton.Yes | StandardButton.No
		onYes: {
			update_handler.updateDialogResponse(true)
		}
		onNo: {
			update_handler.updateDialogResponse(false)
		}
	}

	RowLayout {
		id: top_frame
		x: 0
		y: 0
		width: parent.width
		height: 30

		Image {
			anchors.left: parent.left
			source: pl_net.Connected ? "/images/media/connected.png" : "/images/media/disconnected.png"
			sourceSize.width: 30
			sourceSize.height: 30
			fillMode: Image.Pad
			smooth: true
		}

		LabelEx {
			anchors.centerIn: parent
			//            text: Qt.application.name + " " + Qt.application.version
			text: Qt.application.name
		}

		/*
		LabelEx {
			anchors.right: parent.right
			text: pl_net.FirmwareVersion
		}
*/
	}

	RowLayout {
		id: central_frame
		x: top_frame.x
		y: top_frame.height
		width: 800
		height: 400

		SwipeView {
			id: swipe
			clip: true
			width: 700
			height: parent.height
			currentIndex: tab_bar.currentIndex
			Equalizer {}

			Media {}

			MusicStand {
				id: music_stand_form
			}

			/* Abilitato per inibire lo swipe delle pagine */
			Component.onCompleted: swipe.contentItem.interactive = false
		}

		ColumnLayout {
			Image {
				id: main_volume_image
				source: music_stand_form.focus ? "/images/media/volume_disable.png" : "/images/media/volume_enable.png"
				opacity: music_stand_form.focus ? 0.3 : 1.0
				anchors.topMargin: 5
				sourceSize.width: 28
				sourceSize.height: 28
				Layout.fillHeight: false
				Layout.fillWidth: true
				fillMode: Image.Pad
				smooth: true
			}
			SliderVerticalEx {
				id: main_volume
				bottomPadding: 30
				topPadding: 5
				enabled: !music_stand_form.focus
				opacity: music_stand_form.focus ? 0.3 : 1.0
				Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
				Layout.fillWidth: true
				Layout.fillHeight: true
				orientation: Qt.Vertical
				padding: 0
				stepSize: 0
				from: 0
				to: 1
				onPositionChanged: value = position
				onValueChanged: equalizer.MainGain = position
			}
			Connections {
				target: equalizer
				onMainGainAnimatedChanged: {
					main_volume.animated_value = equalizer.MainGain
					main_volume.animator.running = true
				}
			}
		}
	}

	footer: TabBarEx {
		x: top_frame.x
		y: top_frame.height + central_frame.height
		width: parent.width
		height: 60
		enabled: music_stand.SwipeEnable

		opacity: music_stand.SwipeEnable ? 1.0 : 0.3
		id: tab_bar

		currentIndex: swipe.currentIndex

		background: Rectangle {
			color: background_stop_gradient
		}

		TabButtonEx {
			image_source: "/images/media/equalizer.png"
		}
		TabButtonEx {
			image_source: "/images/media/mp3.png"
		}
		TabButtonEx {
			image_source: "/images/media/music_stand.png"
		}
	}
}
