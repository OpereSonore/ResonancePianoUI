import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Item {
	width: 700
	height: 400

	property variant media_play_icon: ["/images/media/play.png", "/images/media/pause.png"]
	property variant media_shuffle_icon: ["/images/media/shuffle_off.png", "/images/media/shuffle_on.png"]
	property variant media_repeat_icon: ["/images/media/repeat_off.png", "/images/media/repeat_loop.png", "/images/media/repeat_item_loop.png"]

	property alias media_repeat_button: media_repeat_button
	property alias media_shuffle_button: media_shuffle_button
	property alias media_list_view: media_list_view
	property alias media_list_view_scrollbar: media_list_view_scrollbar
	property alias media_disconnected_text: media_disconnected_text
	property alias media_duration_text: media_duration_text
	property alias media_position_text: media_position_text

	property alias media_info_text: media_info_text
	property alias media_previous_button: media_previous_button
	property alias media_play_button: media_play_button
	property alias media_next_button: media_next_button
	property alias media_state_slider: media_state_slider

	ColumnLayout {
		anchors.fill: parent
		anchors.rightMargin: 20
		anchors.leftMargin: 5
		anchors.bottomMargin: 30
		anchors.topMargin: 5

		RowLayout {
			id: media_list_container
			implicitWidth: parent.width

			MediaListViewEx {
				id: media_list_view
				anchors.fill: parent
				anchors.margins: 10

				model: media_player_data

				ScrollIndicator.vertical: ScrollIndicator {
					id: media_list_view_scrollbar
					contentItem: Rectangle {
						implicitWidth: 6
						implicitHeight: 100
						radius: width / 2
						color: control_active_color
						visible: media_list_view.movingVertically
					}
				}
			}

			Rectangle {
				radius: 12
				border.color: frame_border_color
				border.width: 2
				color: 'transparent'
				anchors.fill: parent
				LabelEx {
					id: media_disconnected_text
					anchors.fill: parent
					horizontalAlignment: Text.AlignHCenter
					verticalAlignment: Text.AlignVCenter
					text: qsTr("Media disconnected")
					font.pointSize: 10
					font.bold: true
					visible: media_player.MediaDisconnected
				}
			}
		}

		RowLayout {
			implicitHeight: 40
			anchors.topMargin: 5
			anchors.bottomMargin: 5

			LabelEx {
				id: media_position_text
				text: media_player.MediaPosition
				width: 80
				height: 30
				Layout.minimumHeight: height
				Layout.preferredHeight: height
				Layout.minimumWidth: width
				Layout.preferredWidth: width
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
				Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
				Layout.rowSpan: 1
				font.pointSize: 9
			}

			MediaSliderEx {
				id: media_state_slider
				implicitWidth: 600
				implicitHeight: 30
				bottomPadding: 10
				topPadding: 10
				enabled: media_player.IsPlaying
				Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
				Layout.fillWidth: true
				value: media_player.MediaProgress
			}

			LabelEx {
				id: media_duration_text
				width: 80
				height: 30
				Layout.minimumHeight: height
				Layout.preferredHeight: height
				Layout.minimumWidth: width
				Layout.preferredWidth: width
				text: media_player.MediaDuration
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
				Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
				Layout.rowSpan: 1
				font.pointSize: 9
			}
			Rectangle {
				radius: 12
				border.color: frame_border_color
				border.width: 2
				color: 'transparent'
				anchors.fill: parent
			}
		}

		RowLayout {
			Layout.fillWidth: true
			implicitHeight: 50
			anchors.topMargin: 5
			anchors.bottomMargin: 5

			LabelEx {
				id: media_info_text
				height: 45
				text: media_player.MediaInfo
				Layout.fillWidth: true
				topPadding: 0
				padding: 0
				clip: true
				Layout.maximumHeight: 45
				Layout.minimumHeight: 45
				Layout.maximumWidth: 450
				Layout.minimumWidth: 450
				font.pointSize: 9
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
				width: 450
				Rectangle {
					radius: 12
					clip: true
					border.color: frame_border_color
					border.width: 2
					color: 'transparent'
					anchors.fill: parent
				}
			}

			ButtonEx {
				id: media_shuffle_button
				Layout.fillWidth: true
				implicitWidth: 25
				implicitHeight: 25
				background: Image {
					source: media_shuffle_icon[media_player.ShuffleIcon]
					anchors.fill: parent
					smooth: true
				}
			}
			ButtonEx {
				id: media_previous_button
				Layout.fillWidth: true
				background: Image {
					source: "/images/media/previous.png"
					sourceSize.height: 45
					sourceSize.width: 45
					anchors.fill: parent
					smooth: true
					Rectangle {
						anchors.fill: parent
						color: 'transparent'
						border.color: media_previous_button.pressed ? control_active_color : frame_border_color
						border.width: 2
						radius: 45
					}
				}
			}
			ButtonEx {
				id: media_play_button
				Layout.fillWidth: true
				background: Image {
					source: media_play_icon[media_player.PlayIcon]
					sourceSize.height: 45
					sourceSize.width: 45
					anchors.fill: parent
					smooth: true
					Rectangle {
						anchors.fill: parent
						color: 'transparent'
						border.color: media_play_button.pressed ? control_active_color : frame_border_color
						border.width: 2
						radius: 45
						smooth: true
					}
				}
			}
			ButtonEx {
				id: media_next_button
				Layout.fillWidth: true
				background: Image {
					source: "/images/media/next.png"
					sourceSize.height: 45
					sourceSize.width: 45
					anchors.fill: parent
					smooth: true
					Rectangle {
						anchors.fill: parent
						color: 'transparent'
						border.color: media_next_button.pressed ? control_active_color : frame_border_color
						border.width: 2
						radius: 45
					}
				}
			}
			ButtonEx {
				id: media_repeat_button
				Layout.fillWidth: false
				implicitWidth: 25
				implicitHeight: 25
				background: Image {
					source: media_repeat_icon[media_player.RepeatIcon]
					anchors.fill: parent
					smooth: true
				}
			}
		}
	}
}
