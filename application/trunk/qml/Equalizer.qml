import QtQuick 2.7
import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

EqualizerForm {
    property double centering_delta: 0.02
    property double centering_value: 0.5
    property double centering_bottom_limit: (centering_value - centering_delta)
    property double centering_top_limit: (centering_value + centering_delta)
    equalizer_mic_enable_switch.onCheckedChanged: {
        equalizer.MicEnable = equalizer_mic_enable_switch.checked
    }
    equalizer_balance_enable_switch.onCheckedChanged: {
        equalizer.BalanceEnable = equalizer_balance_enable_switch.checked
    }
    equalizer_unbalance_enable_switch.onCheckedChanged: {
        equalizer.UnbalanceEnable = equalizer_unbalance_enable_switch.checked
    }
    equalizer_mp3_enable_switch.onCheckedChanged: {
        equalizer.UsbEnable = equalizer_mp3_enable_switch.checked
    }
    equalizer_bluetooth_enable_switch.onCheckedChanged: {
        equalizer.BluetoothEnable = equalizer_bluetooth_enable_switch.checked
    }

    equalizer_mic_eq_button.onClicked: {
        equalizer.MicShowEq = !equalizer.MicShowEq
    }
    equalizer_balance_eq_button.onClicked: {
        equalizer.BalanceShowEq = !equalizer.BalanceShowEq
    }
    equalizer_unbalance_eq_button.onClicked: {
        equalizer.UnbalanceShowEq = !equalizer.UnbalanceShowEq
    }
    equalizer_mp3_eq_button.onClicked: {
        equalizer.UsbShowEq = !equalizer.UsbShowEq
    }
    equalizer_bluetooth_eq_button.onClicked: {
        equalizer.BluetoothShowEq = !equalizer.BluetoothShowEq
    }

    equalizer_input_gain_slider.onPositionChanged: {
        if (!equalizer_input_gain_slider.animator.running)
            equalizer.InputGain = equalizer_input_gain_slider.position
    }
    Connections {
        target: equalizer
        onInputGainAnimatedChanged: {
            equalizer_input_gain_slider.animated_value = equalizer.InputGain
            equalizer_input_gain_slider.animator.running = true
        }
    }

    equalizer_low_freq_gain_slider.onPositionChanged: {
        if (!equalizer_low_freq_gain_slider.animator.running)
            equalizer_low_freq_gain_slider.value = equalizer_low_freq_gain_slider.position
    }
    equalizer_low_freq_gain_slider.onValueChanged: {
        if ((equalizer_low_freq_gain_slider.position > centering_bottom_limit)
                && (equalizer_low_freq_gain_slider.position < centering_top_limit)) {
            equalizer_low_freq_gain_slider.value = centering_value
        }
        if (!equalizer_low_freq_gain_slider.animator.running)
            equalizer.LowFreqGain = equalizer_low_freq_gain_slider.position
    }
    Connections {
        target: equalizer
        onLowFreqGainAnimatedChanged: {
            equalizer_low_freq_gain_slider.animated_value = equalizer.LowFreqGain
            equalizer_low_freq_gain_slider.animator.running = true
        }
    }
    equalizer_low_freq_gain_slider.onHandlePressed: {
        equalizer.onLowFreqHandlePressed()
    }
    Connections {
        target: equalizer
        onLowFreqGainSetMiddle: {
            if (!equalizer_low_freq_gain_slider.pressed) {
                equalizer.LowFreqGain = 0.5
                equalizer_low_freq_gain_slider.animated_value = 0.5
                equalizer_low_freq_gain_slider.animator.running = true
            }
        }
    }
    equalizer_mid_freq_gain_slider.onPositionChanged: {
        if (!equalizer_mid_freq_gain_slider.animator.running)
            equalizer_mid_freq_gain_slider.value = equalizer_mid_freq_gain_slider.position
    }
    equalizer_mid_freq_gain_slider.onValueChanged: {
        if ((equalizer_mid_freq_gain_slider.position > centering_bottom_limit)
                && (equalizer_mid_freq_gain_slider.position < centering_top_limit))
            equalizer_mid_freq_gain_slider.value = centering_value
        if (!equalizer_mid_freq_gain_slider.animator.running)
            equalizer.MidFreqGain = equalizer_mid_freq_gain_slider.position
    }
    Connections {
        target: equalizer
        onMidFreqGainAnimatedChanged: {
            equalizer_mid_freq_gain_slider.animated_value = equalizer.MidFreqGain
            equalizer_mid_freq_gain_slider.animator.running = true
        }
    }
    Connections {
        target: equalizer
        onMidFreqGainSetMiddle: {
            if (!equalizer_mid_freq_gain_slider.pressed) {
                equalizer.MidFreqGain = 0.5
                equalizer_mid_freq_gain_slider.animated_value = 0.5
                equalizer_mid_freq_gain_slider.animator.running = true
            }
        }
    }
    equalizer_mid_freq_gain_slider.onHandlePressed: {
        equalizer.onMidFreqHandlePressed()
    }
    equalizer_high_freq_gain_slider.onPositionChanged: {
        if (!equalizer_high_freq_gain_slider.animator.running)
            equalizer_high_freq_gain_slider.value = equalizer_high_freq_gain_slider.position
    }
    equalizer_high_freq_gain_slider.onValueChanged: {
        if ((equalizer_high_freq_gain_slider.position > centering_bottom_limit)
                && (equalizer_high_freq_gain_slider.position < centering_top_limit))
            equalizer_high_freq_gain_slider.value = centering_value
        if (!equalizer_high_freq_gain_slider.animator.running)
            equalizer.HighFreqGain = equalizer_high_freq_gain_slider.position
    }
    Connections {
        target: equalizer
        onHighFreqGainAnimatedChanged: {
            equalizer_high_freq_gain_slider.animated_value = equalizer.HighFreqGain
            equalizer_high_freq_gain_slider.animator.running = true
        }
    }
    Connections {
        target: equalizer
        onHighFreqGainSetMiddle: {
            if (!equalizer_high_freq_gain_slider.pressed) {
                equalizer.HighFreqGain = 0.5
                equalizer_high_freq_gain_slider.animated_value = 0.5
                equalizer_high_freq_gain_slider.animator.running = true
            }
        }
    }
    equalizer_high_freq_gain_slider.onHandlePressed: {
        equalizer.onHighFreqHandlePressed()
    }
}
