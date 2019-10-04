import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Item {
	width: 700
	height: 400
	property alias equalizer_high_freq_gain_slider: equalizer_high_freq_gain_slider
	property alias equalizer_mid_freq_gain_slider: equalizer_mid_freq_gain_slider
	property alias equalizer_low_freq_gain_slider: equalizer_low_freq_gain_slider
	property alias equalizer_input_gain_slider: equalizer_input_gain_slider

	property alias equalizer_mic_enable_switch: equalizer_mic_enable_switch
	property alias equalizer_balance_enable_switch: equalizer_balance_enable_switch
	property alias equalizer_unbalance_enable_switch: equalizer_unbalance_enable_switch
	property alias equalizer_mp3_enable_switch: equalizer_mp3_enable_switch
	property alias equalizer_bluetooth_enable_switch: equalizer_bluetooth_enable_switch

	property alias equalizer_mic_eq_button: equalizer_mic_eq_button
	property alias equalizer_balance_eq_button: equalizer_balance_eq_button
	property alias equalizer_unbalance_eq_button: equalizer_unbalance_eq_button
	property alias equalizer_mp3_eq_button: equalizer_mp3_eq_button
	property alias equalizer_bluetooth_eq_button: equalizer_bluetooth_eq_button

	RowLayout {
		anchors.rightMargin: 20
		anchors.leftMargin: 5
		anchors.bottomMargin: 30
		anchors.topMargin: 5
		anchors.fill: parent
		clip: true
		ColumnLayout {
			Layout.columnSpan: 0
			Layout.rowSpan: 0
			Layout.fillHeight: false
			Layout.fillWidth: false
			Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
			anchors.left: parent.left
			anchors.leftMargin: 0
			anchors.bottom: parent.bottom
			anchors.top: parent.top
			clip: true
			spacing: 40
			RowLayout {
				spacing: 20
				Layout.fillHeight: true
				SwitchEx {
					id: equalizer_mic_enable_switch
					checked: equalizer.MicEnable
				}

				Image {
					id: equalizer_mic_enable_image
					anchors.left: equalizer_mic_enable_switch.right
					anchors.leftMargin: -5
					source: equalizer.MicEnable ? "/images/media/plug_on.png" : "/images/media/plug_off.png"
					sourceSize.width: 28
					sourceSize.height: 28
					fillMode: Image.Pad
					smooth: true
				}

				ButtonEx {
					id: equalizer_mic_eq_button
					anchors.left: equalizer_mic_enable_switch.right
					anchors.leftMargin: 60
					Layout.preferredHeight: 40
					Layout.preferredWidth: 40
					background: Image {
						source: equalizer.MicShowEq ? "/images/media/microphone_on.png" : "/images/media/microphone_off.png"
						sourceSize.width: 28
						sourceSize.height: 28
						anchors.fill: parent
						smooth: true
						fillMode: Image.Pad
						Rectangle {
							anchors.fill: parent
							color: 'transparent'
							border.color: equalizer.MicShowEq ? control_active_color : frame_border_color
							border.width: 2
							radius: 40
						}
					}
				}
			}

			RowLayout {
				spacing: 20
				Layout.fillHeight: true
				SwitchEx {
					id: equalizer_balance_enable_switch
					checked: equalizer.BalanceEnable
				}

				Image {
					anchors.left: equalizer_balance_enable_switch.right
					anchors.leftMargin: -5
					source: equalizer.BalanceEnable ? "/images/media/plug_on.png" : "/images/media/plug_off.png"
					sourceSize.width: 28
					sourceSize.height: 28
					fillMode: Image.Pad
					smooth: true
				}

				ButtonEx {
					id: equalizer_balance_eq_button
					anchors.left: equalizer_balance_enable_switch.right
					anchors.leftMargin: 60
					Layout.preferredHeight: 40
					Layout.preferredWidth: 40
					background: Image {
						source: equalizer.BalanceShowEq ? "/images/media/balance_on.png" : "/images/media/balance_off.png"
						sourceSize.width: 28
						sourceSize.height: 28
						anchors.fill: parent
						smooth: true
						fillMode: Image.Pad
						Rectangle {
							anchors.fill: parent
							color: 'transparent'
							border.color: equalizer.BalanceShowEq ? control_active_color : frame_border_color
							border.width: 2
							radius: 40
						}
					}
				}
			}

			RowLayout {
				spacing: 20
				Layout.fillHeight: true
				SwitchEx {
					id: equalizer_unbalance_enable_switch
					checked: equalizer.UnbalanceEnable
				}

				Image {
					anchors.left: equalizer_unbalance_enable_switch.right
					anchors.leftMargin: -5
					source: equalizer.UnbalanceEnable ? "/images/media/plug_on.png" : "/images/media/plug_off.png"
					sourceSize.width: 28
					sourceSize.height: 28
					fillMode: Image.Pad
					smooth: true
				}

				ButtonEx {
					id: equalizer_unbalance_eq_button
					anchors.left: equalizer_unbalance_enable_switch.right
					anchors.leftMargin: 60
					Layout.preferredHeight: 40
					Layout.preferredWidth: 40
					background: Image {
						source: equalizer.UnbalanceShowEq ? "/images/media/unbalance_on.png" : "/images/media/unbalance_off.png"
						sourceSize.width: 28
						sourceSize.height: 28
						anchors.fill: parent
						smooth: true
						fillMode: Image.Pad
						Rectangle {
							anchors.fill: parent
							color: 'transparent'
							border.color: equalizer.UnbalanceShowEq ? control_active_color : frame_border_color
							border.width: 2
							radius: 40
						}
					}
				}
			}

			RowLayout {
				spacing: 20
				Layout.fillHeight: true
				SwitchEx {
					id: equalizer_mp3_enable_switch
					checked: equalizer.UsbEnable
				}
				Image {
					anchors.left: equalizer_mp3_enable_switch.right
					anchors.leftMargin: -5
					source: equalizer.UsbEnable ? "/images/media/plug_on.png" : "/images/media/plug_off.png"
					sourceSize.width: 28
					sourceSize.height: 28
					fillMode: Image.Pad
					smooth: true
				}

				ButtonEx {
					id: equalizer_mp3_eq_button
					anchors.left: equalizer_mp3_enable_switch.right
					anchors.leftMargin: 60
					Layout.preferredHeight: 40
					Layout.preferredWidth: 40
					background: Image {
						source: equalizer.UsbShowEq ? "/images/media/mp3_on.png" : "/images/media/mp3_off.png"
						sourceSize.width: 28
						sourceSize.height: 28
						anchors.fill: parent
						smooth: true
						fillMode: Image.Pad
						Rectangle {
							anchors.fill: parent
							color: 'transparent'
							border.color: equalizer.UsbShowEq ? control_active_color : frame_border_color
							border.width: 2
							radius: 40
						}
					}
				}
			}

			RowLayout {
				spacing: 20
				Layout.fillHeight: true
				SwitchEx {
					id: equalizer_bluetooth_enable_switch
					checked: equalizer.BluetoothEnable
				}
				Image {
					anchors.left: equalizer_bluetooth_enable_switch.right
					anchors.leftMargin: -5
					source: equalizer.BluetoothEnable ? "/images/media/plug_on.png" : "/images/media/plug_off.png"
					sourceSize.width: 28
					sourceSize.height: 28
					fillMode: Image.Pad
					smooth: true
				}

				ButtonEx {
					id: equalizer_bluetooth_eq_button
					anchors.left: equalizer_bluetooth_enable_switch.right
					anchors.leftMargin: 60
					Layout.preferredHeight: 40
					Layout.preferredWidth: 40
					background: Image {
						source: equalizer.BluetoothShowEq ? "/images/media/bluetooth_on.png" : "/images/media/bluetooth_off.png"
						sourceSize.width: 28
						sourceSize.height: 28
						anchors.fill: parent
						smooth: true
						fillMode: Image.Pad
						Rectangle {
							anchors.fill: parent
							color: 'transparent'
							border.color: equalizer.BluetoothShowEq ? control_active_color : frame_border_color
							border.width: 2
							radius: 40
						}
					}
				}
			}
		}

		RowLayout {
			Layout.columnSpan: 0
			Layout.rowSpan: 0
			spacing: 0
			clip: true
			Layout.fillHeight: true
			Layout.fillWidth: true
			Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
			ColumnLayout {
				Layout.fillWidth: true
				Layout.fillHeight: true
				Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
				Image {
					id: equalizer_input_gain_image
					source: "/images/media/gain.png"
					sourceSize.width: 28
					sourceSize.height: 28
					fillMode: Image.Pad
					smooth: true
					Layout.fillHeight: false
					Layout.fillWidth: true
				}

				SliderVerticalEx {
					id: equalizer_input_gain_slider
					enabled: equalizer.MicShowEq || equalizer.BalanceShowEq
							 || equalizer.UnbalanceShowEq || equalizer.UsbShowEq
							 || equalizer.BluetoothShowEq
					rightPadding: 15
					leftPadding: 15
					clip: false
					Layout.fillWidth: false
					Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
					Layout.fillHeight: true
					orientation: Qt.Vertical
					padding: 0
					stepSize: 0
					from: 0
					to: 1
					Component.onCompleted: animated_value = equalizer.InputGain
				}
			}

			RowLayout {
				spacing: 0
				clip: true
				Layout.fillHeight: true
				Layout.fillWidth: false
				Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
				ColumnLayout {
					spacing: 0
					Layout.fillHeight: true
					Layout.fillWidth: true
					Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
					Text {
						text: "+6 dB"
						topPadding: 20
						Layout.fillHeight: true
						font.pointSize: 10
						color: frame_border_color
						Component.onCompleted: topPadding = 30
					}
					Text {
						text: " 0 dB"
						topPadding: 2
						leftPadding: 0
						verticalAlignment: Text.AlignVCenter
						Layout.fillHeight: true
						font.pointSize: 10
						color: frame_border_color
						Component.onCompleted: topPadding = 30
					}
					Text {
						text: "-6 dB"
						bottomPadding: 1
						verticalAlignment: Text.AlignBottom
						Layout.fillHeight: true
						font.pointSize: 10
						color: frame_border_color
						Component.onCompleted: topPadding = 30
					}
				}

				ColumnLayout {
					Layout.fillWidth: true
					Layout.fillHeight: true
					Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
					Image {
						id: equalizer_low_freq_gain_image
						source: "/images/media/low_pass.png"
						sourceSize.width: 28
						sourceSize.height: 28
						fillMode: Image.Pad
						Layout.fillHeight: false
						Layout.fillWidth: true
						smooth: true
					}
					SliderVerticalEx {
						id: equalizer_low_freq_gain_slider
						enabled: equalizer.MicShowEq || equalizer.BalanceShowEq
								 || equalizer.UnbalanceShowEq
								 || equalizer.UsbShowEq
								 || equalizer.BluetoothShowEq
						rightPadding: 15
						leftPadding: 15
						Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
						Layout.fillWidth: false
						Layout.fillHeight: true
						orientation: Qt.Vertical
						padding: 0
						stepSize: 0
						from: 0
						to: 1
						Component.onCompleted: animated_value = equalizer.LowFreqGain
					}
				}

				ColumnLayout {
					Layout.preferredWidth: 30
					spacing: 0
					Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
				}

				ColumnLayout {
					Layout.fillWidth: true
					Layout.fillHeight: true
					Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
					Image {
						id: equalizer_mid_freq_gain_image
						source: "/images/media/band_pass.png"
						sourceSize.width: 28
						sourceSize.height: 28
						fillMode: Image.Pad
						Layout.fillHeight: false
						Layout.fillWidth: true
						smooth: true
					}
					SliderVerticalEx {
						id: equalizer_mid_freq_gain_slider
						enabled: equalizer.MicShowEq || equalizer.BalanceShowEq
								 || equalizer.UnbalanceShowEq
								 || equalizer.UsbShowEq
								 || equalizer.BluetoothShowEq
						rightPadding: 15
						leftPadding: 15
						Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
						Layout.fillWidth: false
						Layout.fillHeight: true
						orientation: Qt.Vertical
						padding: 0
						stepSize: 0
						from: 0
						to: 1
						Component.onCompleted: animated_value = equalizer.MidFreqGain
					}
				}

				ColumnLayout {
					Layout.preferredWidth: 30
					spacing: 0
					Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
				}
				ColumnLayout {
					Layout.fillWidth: true
					Layout.fillHeight: true
					Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
					Image {
						id: equalizer_high_freq_gain_image
						source: "/images/media/high_pass.png"
						sourceSize.width: 28
						sourceSize.height: 28
						fillMode: Image.Pad
						Layout.fillHeight: false
						Layout.fillWidth: true
						smooth: true
					}
					SliderVerticalEx {
						id: equalizer_high_freq_gain_slider
						enabled: equalizer.MicShowEq || equalizer.BalanceShowEq
								 || equalizer.UnbalanceShowEq
								 || equalizer.UsbShowEq
								 || equalizer.BluetoothShowEq
						rightPadding: 15
						leftPadding: 15
						Layout.fillWidth: false
						Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
						Layout.fillHeight: true
						orientation: Qt.Vertical
						padding: 0
						stepSize: 0
						from: 0
						to: 1
						Component.onCompleted: animated_value = equalizer.HighFreqGain
					}
				}
			}
		}
	}
}
