import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Item {
	width: 700
	height: 400

	property alias background_mouse_area: background_mouse_area
	property alias background_image: background_image

	Image {
		id: background_image
		anchors.fill: parent
		source: "/images/media/music_stand_background.png"
		sourceSize.height: 128
		sourceSize.width: 128
		fillMode: Image.Pad
		smooth: true
		opacity: 0.1
	}

	MouseArea {
		id: background_mouse_area
		anchors.rightMargin: 220
		anchors.leftMargin: 220
		anchors.bottomMargin: 100
		anchors.topMargin: 100
		anchors.fill: parent
	}
}
