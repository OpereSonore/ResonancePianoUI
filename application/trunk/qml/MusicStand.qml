import QtQuick 2.7

MusicStandForm {
	onFocusChanged: {
		music_stand.onFocusChanged(focus)
	}

	background_mouse_area.onDoubleClicked: {
		music_stand.onDoubleClickEvent()
	}
}
