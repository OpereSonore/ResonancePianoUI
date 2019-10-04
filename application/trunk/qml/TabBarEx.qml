import QtQuick 2.7
import QtQuick.Controls 2.0

TabBar {
	id: control

	implicitWidth: Math.max(
					   background ? background.implicitWidth : 0,
									(contentItem.implicitWidth + leftPadding + rightPadding))
	implicitHeight: Math.max(
						background ? background.implicitHeight : 0,
									 (contentItem.implicitHeight + topPadding + bottomPadding))

	spacing: 1

	contentItem: PathView {
		implicitWidth: 200
		implicitHeight: 48

		model: control.contentModel

		interactive: false
		snapMode: PathView.NoSnap
		movementDirection: PathView.Positive
		highlightMoveDuration: 100

		path: Path {
			startX: control.count ? (control.availableWidth / control.count / 2) : 0
			startY: control.availableHeight / 2
			PathLine {
				x: control.count ? control.availableWidth
								   + (control.availableWidth / control.count / 2) : 0
				y: (control.availableHeight / 2)
			}
		}
	}

	background: Rectangle {
	}
}
