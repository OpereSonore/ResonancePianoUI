import QtQuick 2.7

MediaForm {

	media_shuffle_button.onClicked: {
		media_player.onShuffleClicked()
	}
	media_previous_button.onClicked: {
		media_player.onPreviousClicked()
	}
	media_play_button.onClicked: {
		media_player.onPlayClicked()
	}
	media_next_button.onClicked: {
		media_player.onNextClicked()
	}
	media_state_slider.onValueChanged: {
		media_player.MediaProgress = media_state_slider.value
	}
	media_state_slider.onPressedChanged: {
		media_player.onMediaProgressPressed(media_state_slider.pressed)
	}
	media_list_view.onClicked: {
		media_player.onMediaPlayerListViewChanged(media_list_view.currentIndex)
	}
	media_repeat_button.onClicked: {
		media_player.onRepeatClicked()
	}
	Connections {
		target: media_player
		onResetMediaList: {
			media_list_view.currentIndex = -1
		}
	}
}
