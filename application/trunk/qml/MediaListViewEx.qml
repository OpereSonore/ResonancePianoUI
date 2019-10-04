import QtQuick 2.7
import QtQuick.Controls 2.0

ListView {
	id: list
	clip: true

	signal clicked

	property variant media_icons: ["/images/media/back.png", "/images/media/folder.png", "/images/media/music.png", "/images/media/playing.png"]

	delegate: Component {
		Item {
			x: parent.x + 6
			width: parent.width
			height: 30
			Row {
				spacing: 10
				Image {
					width: 30
					height: 30
					source: media_icons[model.icon]
					smooth: true
				}
				LabelEx {
					text: model.name
					font.pointSize: 10
					anchors.verticalCenter: parent.verticalCenter
				}
			}

			MouseArea {
				id: mouse_area
				anchors.fill: parent
				onClicked: {
					list.currentIndex = index
					list.clicked()
				}
			}
		}
	}

	focus: true
}
