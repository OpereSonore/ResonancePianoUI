import QtQuick 2.7
import QtQuick.Controls 2.0

Rectangle {
	gradient: Gradient {
		GradientStop {
			position: 0
			color: background_start_gradient
		}
		GradientStop {
			position: 1
			color: background_stop_gradient
		}
	}
}
