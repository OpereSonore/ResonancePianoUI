

#include "equalizer.h"
#include "plnet.h"

DEFINE_LOG_MODULE_NAME;

#define EQUALIZER_MAX_GAIN_dB         0.0
#define EQUALIZER_MIN_GAIN_dB       -96.0

const Equalizer::POSITION_GAIN_TABLE Equalizer::_Equalizer_Table =
{
	-6.0, 0.0,
	 6.0, 1.0,
};

/******************************************************************************/

#define SLIDER_EQ_DOUBLE_CLICK_TIMEOUT              400

/******************************************************************************/
Equalizer &Equalizer::GetInstance(void)
/******************************************************************************/
{
	static Equalizer instance;

	return (instance);
}

/*****************************************************************************/
Equalizer::Equalizer(QObject *parent) : QObject(parent)
/*****************************************************************************/
{
	SLIDER_EQ_DATA *data;

	_Main_Position         = 0.0;
	_Input_Position        = 0.0;
	_Low_Freq_Position     = 0.0;
	_Mid_Freq_Position     = 0.0;
	_High_Freq_Position    = 0.0;

	_Mic_Enable            = false;
	_Balance_Enable        = false;
	_Unbalance_Enable      = false;
	_Usb_Enable            = false;

	_Mic_Show_Eq           = false;
	_Balance_Show_Eq       = false;
	_Unbalance_Show_Eq     = false;
	_Usb_Show_Eq           = false;

	_Eq_Id                 = NO_EQ_ID;

	data = &_Slider_Eq_Data[LOW_FREQ_SLIDER_ID];
	QObject::connect(&data->timer, SIGNAL(timeout()), this, SLOT(LowFreqSliderTimeout()));
	data->set_middle = std::bind(&Equalizer::lowFreqGainSetMiddle, this);
	data->position = &_Low_Freq_Position;
	data->state = 0;


	data = &_Slider_Eq_Data[MID_FREQ_SLIDER_ID];
	QObject::connect(&data->timer, SIGNAL(timeout()), this, SLOT(MidFreqSliderTimeout()));
	data->set_middle = std::bind(&Equalizer::midFreqGainSetMiddle, this);
	data->position = &_Mid_Freq_Position;
	data->state = 0;

	data = &_Slider_Eq_Data[HIGH_FREQ_SLIDER_ID];
	QObject::connect(&data->timer, SIGNAL(timeout()), this, SLOT(HighFreqSliderTimeout()));
	data->set_middle = std::bind(&Equalizer::highFreqGainSetMiddle, this);
	data->position = &_High_Freq_Position;
	data->state = 0;
}

/*****************************************************************************/
Equalizer::~Equalizer(void)
/*****************************************************************************/
{
}

/*****************************************************************************/
double Equalizer::GetMainGain(void)
/*****************************************************************************/
{
	return (_Main_Position);
}

/*****************************************************************************/
void Equalizer::SetMainGain(double &position)
/*****************************************************************************/
{
	if (position == _Main_Position)
	{
		return;
	}

	_Main_Position = position;

	PlNet::GetInstance().SetMainGain(PositionToGain(position));

	emit mainGainChanged();
}

/*****************************************************************************/
double Equalizer::GetInputGain(void)
/*****************************************************************************/
{
	return (_Input_Position);
}

/*****************************************************************************/
void Equalizer::SetInputGain(double &position)
/*****************************************************************************/
{
	float gain;

	if (position == _Input_Position)
	{
		return;
	}

	_Input_Position = position;

	gain = PositionToGain(position);
	switch (_Eq_Id)
	{
		case NO_EQ_ID:
			break;

		case MIC_EQ_ID:
			PlNet::GetInstance().SetMicInputGain(gain);
			break;

		case BALANCE_EQ_ID:
			PlNet::GetInstance().SetBalanceInputGain(gain);
			break;

		case UNBALANCE_EQ_ID:
			PlNet::GetInstance().SetUnbalanceInputGain(gain);
			break;

		case USB_EQ_ID:
			PlNet::GetInstance().SetUsbInputGain(gain);
			break;

		case BLUETOOTH_EQ_ID:
			PlNet::GetInstance().SetBluetoothInputGain(gain);
			break;

		default:
			ASSERT(false);
	}

	emit inputGainChanged();
}

/*****************************************************************************/
double Equalizer::GetLowFreqGain(void)
/*****************************************************************************/
{
	return (_Low_Freq_Position);
}

/*****************************************************************************/
void Equalizer::SetLowFreqGain(double &position)
/*****************************************************************************/
{
	float gain;

	if (position == _Low_Freq_Position)
	{
		return;
	}

	_Low_Freq_Position = position;

	gain = PositionToGain(_Equalizer_Table, position);
	switch (_Eq_Id)
	{
		case NO_EQ_ID:
			break;

		case MIC_EQ_ID:
			PlNet::GetInstance().SetMicLowFrequencyGain(gain);
			break;

		case BALANCE_EQ_ID:
			PlNet::GetInstance().SetBalanceLowFrequencyGain(gain);
			break;

		case UNBALANCE_EQ_ID:
			PlNet::GetInstance().SetUnbalanceLowFrequencyGain(gain);
			break;

		case USB_EQ_ID:
			PlNet::GetInstance().SetUsbLowFrequencyGain(gain);
			break;

		case BLUETOOTH_EQ_ID:
			PlNet::GetInstance().SetBluetoothLowFrequencyGain(gain);
			break;

		default:
			ASSERT(false);
	}

	emit lowFreqGainChanged();
}

/*****************************************************************************/
double Equalizer::GetMidFreqGain(void)
/*****************************************************************************/
{
	return (_Mid_Freq_Position);
}

/*****************************************************************************/
void Equalizer::SetMidFreqGain(double &position)
/*****************************************************************************/
{
	float gain;

	if (position == _Mid_Freq_Position)
	{
		return;
	}

	_Mid_Freq_Position = position;

	gain = PositionToGain(_Equalizer_Table, position);
	switch (_Eq_Id)
	{
		case NO_EQ_ID:
			break;

		case MIC_EQ_ID:
			PlNet::GetInstance().SetMicMidFrequencyGain(gain);
			break;

		case BALANCE_EQ_ID:
			PlNet::GetInstance().SetBalanceMidFrequencyGain(gain);
			break;

		case UNBALANCE_EQ_ID:
			PlNet::GetInstance().SetUnbalanceMidFrequencyGain(gain);
			break;

		case USB_EQ_ID:
			PlNet::GetInstance().SetUsbMidFrequencyGain(gain);
			break;

		case BLUETOOTH_EQ_ID:
			PlNet::GetInstance().SetBluetoothMidFrequencyGain(gain);
			break;

		default:
			ASSERT(false);
	}

	emit midFreqGainChanged();
}

/*****************************************************************************/
double Equalizer::GetHighFreqGain(void)
/*****************************************************************************/
{
	return (_High_Freq_Position);
}

/*****************************************************************************/
void Equalizer::SetHighFreqGain(double &position)
/*****************************************************************************/
{
	float gain;

	if (position == _High_Freq_Position)
	{
		return;
	}

	_High_Freq_Position = position;

	gain = PositionToGain(_Equalizer_Table, position);
	switch (_Eq_Id)
	{
		case NO_EQ_ID:
			break;

		case MIC_EQ_ID:
			PlNet::GetInstance().SetMicHighFrequencyGain(gain);
			break;

		case BALANCE_EQ_ID:
			PlNet::GetInstance().SetBalanceHighFrequencyGain(gain);
			break;

		case UNBALANCE_EQ_ID:
			PlNet::GetInstance().SetUnbalanceHighFrequencyGain(gain);
			break;

		case USB_EQ_ID:
			PlNet::GetInstance().SetUsbHighFrequencyGain(gain);
			break;

		case BLUETOOTH_EQ_ID:
			PlNet::GetInstance().SetBluetoothHighFrequencyGain(gain);
			break;

		default:
			ASSERT(false);
	}

	emit highFreqGainChanged();
}

/*****************************************************************************/
bool Equalizer::GetMicEnable(void)
/*****************************************************************************/
{
	return (_Mic_Enable);
}

/*****************************************************************************/
void Equalizer::SetMicEnable(bool &enable)
/*****************************************************************************/
{
	if (enable == _Mic_Enable)
	{
		return;
	}

	_Mic_Enable = enable;

	PlNet::GetInstance().SetMicEnable(enable);

	emit micEnableChanged();
}

/*****************************************************************************/
bool Equalizer::GetBalanceEnable(void)
/*****************************************************************************/
{
	return (_Balance_Enable);
}

/*****************************************************************************/
void Equalizer::SetBalanceEnable(bool &enable)
/*****************************************************************************/
{
	if (enable == _Balance_Enable)
	{
		return;
	}

	_Balance_Enable = enable;

	PlNet::GetInstance().SetBalanceEnable(enable);

	emit balanceEnableChanged();
}

/*****************************************************************************/
bool Equalizer::GetUnbalanceEnable(void)
/*****************************************************************************/
{
	return (_Unbalance_Enable);
}

/*****************************************************************************/
void Equalizer::SetUnbalanceEnable(bool &enable)
/*****************************************************************************/
{
	if (enable == _Unbalance_Enable)
	{
		return;
	}

	_Unbalance_Enable = enable;

	PlNet::GetInstance().SetUnbalanceEnable(enable);

	emit unbalanceEnableChanged();
}

/*****************************************************************************/
bool Equalizer::GetUsbEnable(void)
/*****************************************************************************/
{
	return (_Usb_Enable);
}

/*****************************************************************************/
void Equalizer::SetUsbEnable(bool &enable)
/*****************************************************************************/
{
	if (enable == _Usb_Enable)
	{
		return;
	}

	_Usb_Enable = enable;

	PlNet::GetInstance().SetUsbEnable(enable);

	emit usbEnableChanged();
}

/*****************************************************************************/
bool Equalizer::GetBluetoothEnable(void)
/*****************************************************************************/
{
	return (_Bluetooth_Enable);
}

/*****************************************************************************/
void Equalizer::SetBluetoothEnable(bool &enable)
/*****************************************************************************/
{
	if (enable == _Bluetooth_Enable)
	{
		return;
	}

	_Bluetooth_Enable = enable;

	PlNet::GetInstance().SetBluetoothEnable(enable);

	emit bluetoothEnableChanged();
}

/*****************************************************************************/
bool Equalizer::GetMicShowEq(void)
/*****************************************************************************/
{
	return (_Mic_Show_Eq);
}

/*****************************************************************************/
void Equalizer::SetMicShowEq(bool &show_eq)
/*****************************************************************************/
{
	if (show_eq == _Mic_Show_Eq)
	{
		return;
	}

	if (show_eq)
	{
		SetEqId(MIC_EQ_ID);
	}
	else
	{
		SetEqId(NO_EQ_ID);
	}
}

/*****************************************************************************/
bool Equalizer::GetBalanceShowEq(void)
/*****************************************************************************/
{
	return (_Balance_Show_Eq);
}

/*****************************************************************************/
void Equalizer::SetBalanceShowEq(bool &show_eq)
/*****************************************************************************/
{
	if (show_eq == _Balance_Show_Eq)
	{
		return;
	}

	if (show_eq)
	{
		SetEqId(BALANCE_EQ_ID);
	}
	else
	{
		SetEqId(NO_EQ_ID);
	}
}

/*****************************************************************************/
bool Equalizer::GetUnbalanceShowEq(void)
/*****************************************************************************/
{
	return (_Unbalance_Show_Eq);
}

/*****************************************************************************/
void Equalizer::SetUnbalanceShowEq(bool &show_eq)
/*****************************************************************************/
{
	if (show_eq == _Unbalance_Show_Eq)
	{
		return;
	}

	if (show_eq)
	{
		SetEqId(UNBALANCE_EQ_ID);
	}
	else
	{
		SetEqId(NO_EQ_ID);
	}
}

/*****************************************************************************/
bool Equalizer::GetUsbShowEq(void)
/*****************************************************************************/
{
	return (_Usb_Show_Eq);
}

/*****************************************************************************/
void Equalizer::SetUsbShowEq(bool &show_eq)
/*****************************************************************************/
{
	if (show_eq == _Usb_Show_Eq)
	{
		return;
	}

	if (show_eq)
	{
		SetEqId(USB_EQ_ID);
	}
	else
	{
		SetEqId(NO_EQ_ID);
	}
}

/*****************************************************************************/
bool Equalizer::GetBluetoothShowEq(void)
/*****************************************************************************/
{
	return (_Bluetooth_Show_Eq);
}

/*****************************************************************************/
void Equalizer::SetBluetoothShowEq(bool &show_eq)
/*****************************************************************************/
{
	if (show_eq == _Bluetooth_Show_Eq)
	{
		return;
	}

	if (show_eq)
	{
		SetEqId(BLUETOOTH_EQ_ID);
	}
	else
	{
		SetEqId(NO_EQ_ID);
	}
}

/*****************************************************************************/
double Equalizer::GainToPosition(const POSITION_GAIN_TABLE &table, float gain)
/*****************************************************************************/
{
	double position;

	position = (double)(table.min_position + ((table.max_position - table.min_position) * (gain - table.min_gain) / ((table.max_gain - table.min_gain))));

	return (position);
}

/**************************************************************************************************/
double Equalizer::GainToPosition(float gain)
/**************************************************************************************************/
{
	if (gain == 0.0f)
	{
		return (1.0f);
	}

	return (pow(10, (gain / 20.0)));
}

/*****************************************************************************/
float Equalizer::PositionToGain(const Equalizer::POSITION_GAIN_TABLE &table, double position)
/*****************************************************************************/
{
	float gain;

	gain = (float)(table.min_gain + ((table.max_gain - table.min_gain) * (position - table.min_position) / ((table.max_position - table.min_position))));

	return (gain);

}

/**************************************************************************************************/
float Equalizer::PositionToGain(double position)
/**************************************************************************************************/
{
	float db_value;

	if (position == 0)
	{
		db_value = EQUALIZER_MIN_GAIN_dB;
	}
	else
	{
		db_value = 20.0 * log10(position);

		if (db_value > EQUALIZER_MAX_GAIN_dB)
		{
			db_value = EQUALIZER_MAX_GAIN_dB;
		}
		else if (db_value < EQUALIZER_MIN_GAIN_dB)
		{
			db_value = EQUALIZER_MIN_GAIN_dB;
		}
	}

	return (db_value);
}

/*****************************************************************************/
void  Equalizer::EqChanged(void)
/*****************************************************************************/
{
	_Main_Position    = GainToPosition(PlNet::GetInstance().GetMainGain());

	_Mic_Enable       = PlNet::GetInstance().GetMicEnable();
	_Balance_Enable   = PlNet::GetInstance().GetBalanceEnable();
	_Unbalance_Enable = PlNet::GetInstance().GetUnbalanceEnable();
	_Usb_Enable       = PlNet::GetInstance().GetUsbEnable();
	_Bluetooth_Enable = PlNet::GetInstance().GetBluetoothEnable();

	emit mainGainAnimatedChanged();

	emit micEnableChanged();
	emit balanceEnableChanged();
	emit unbalanceEnableChanged();
	emit usbEnableChanged();
	emit bluetoothEnableChanged();

	RefreshEq();
}

/*****************************************************************************/
void Equalizer::onLowFreqHandlePressed(void)
/*****************************************************************************/
{
	SliderEqEventHandler(LOW_FREQ_SLIDER_ID, PRESS_EVENT_ID);
}

/*****************************************************************************/
void Equalizer::onMidFreqHandlePressed(void)
/*****************************************************************************/
{
	SliderEqEventHandler(MID_FREQ_SLIDER_ID, PRESS_EVENT_ID);
}

/*****************************************************************************/
void Equalizer::onHighFreqHandlePressed(void)
/*****************************************************************************/
{
	SliderEqEventHandler(HIGH_FREQ_SLIDER_ID, PRESS_EVENT_ID);
}

/*****************************************************************************/
void Equalizer::LowFreqSliderTimeout(void)
/*****************************************************************************/
{
	SliderEqEventHandler(LOW_FREQ_SLIDER_ID, TIMEOUT_EVENT_ID);
}

/*****************************************************************************/
void Equalizer::MidFreqSliderTimeout(void)
/*****************************************************************************/
{
	SliderEqEventHandler(MID_FREQ_SLIDER_ID, TIMEOUT_EVENT_ID);
}

/*****************************************************************************/
void Equalizer::HighFreqSliderTimeout(void)
/*****************************************************************************/
{
	SliderEqEventHandler(HIGH_FREQ_SLIDER_ID, TIMEOUT_EVENT_ID);
}

/*****************************************************************************/
void Equalizer::SliderEqEventHandler(SLIDER_EQ_ID id, HANDLE_EVENT_ID event)
/*****************************************************************************/
{
	SLIDER_EQ_DATA *data;

	ASSERT(id < SLIDER_EQ_NUMBER);

	data = &_Slider_Eq_Data[id];

	switch (data->state)
	{
		case 0:
			if (event == PRESS_EVENT_ID)
			{
				data->timer.start(SLIDER_EQ_DOUBLE_CLICK_TIMEOUT);

				data->state = 1;
			}
			break;

		case 1:
			if (event == PRESS_EVENT_ID)
			{
				data->state = 2;
			}
			else if (event == TIMEOUT_EVENT_ID)
			{
				data->state = 0;
			}
			break;

		case 2:
			if (event == TIMEOUT_EVENT_ID)
			{
				data->set_middle();

				data->state = 0;
			}
			break;

		default:
			ASSERT(false);
	}
}

/*****************************************************************************/
Equalizer::EQ_ID Equalizer::GetEqId(void)
/*****************************************************************************/
{
	return(_Eq_Id);
}

/*****************************************************************************/
void Equalizer::SetEqId(EQ_ID eq_id)
/*****************************************************************************/
{
	_Eq_Id = eq_id;

	RefreshEq();
}

/*****************************************************************************/
void Equalizer::RefreshEq(void)
/*****************************************************************************/
{
	switch (_Eq_Id)
	{
		case NO_EQ_ID:
			_Mic_Show_Eq       = false;
			_Balance_Show_Eq   = false;
			_Unbalance_Show_Eq = false;
			_Usb_Show_Eq       = false;
			_Bluetooth_Show_Eq = false;

			_Input_Position        = 0.0;
			_Low_Freq_Position     = 0.0;
			_Mid_Freq_Position     = 0.0;
			_High_Freq_Position    = 0.0;
			break;

		case MIC_EQ_ID:
			_Mic_Show_Eq       = true;
			_Balance_Show_Eq   = false;
			_Unbalance_Show_Eq = false;
			_Usb_Show_Eq       = false;
			_Bluetooth_Show_Eq = false;

			_Input_Position        = GainToPosition(PlNet::GetInstance().GetMicInputGain());
			_Low_Freq_Position     = GainToPosition(_Equalizer_Table, PlNet::GetInstance().GetMicLowFrequencyGain());
			_Mid_Freq_Position     = GainToPosition(_Equalizer_Table, PlNet::GetInstance().GetMicMidFrequencyGain());
			_High_Freq_Position    = GainToPosition(_Equalizer_Table, PlNet::GetInstance().GetMicHighFrequencyGain());
			break;

		case BALANCE_EQ_ID:
			_Mic_Show_Eq       = false;
			_Balance_Show_Eq   = true;
			_Unbalance_Show_Eq = false;
			_Usb_Show_Eq       = false;
			_Bluetooth_Show_Eq = false;

			_Input_Position        = GainToPosition(PlNet::GetInstance().GetBalanceInputGain());
			_Low_Freq_Position     = GainToPosition(_Equalizer_Table, PlNet::GetInstance().GetBalanceLowFrequencyGain());
			_Mid_Freq_Position     = GainToPosition(_Equalizer_Table, PlNet::GetInstance().GetBalanceMidFrequencyGain());
			_High_Freq_Position    = GainToPosition(_Equalizer_Table, PlNet::GetInstance().GetBalanceHighFrequencyGain());
			break;

		case UNBALANCE_EQ_ID:
			_Mic_Show_Eq       = false;
			_Balance_Show_Eq   = false;
			_Unbalance_Show_Eq = true;
			_Usb_Show_Eq       = false;
			_Bluetooth_Show_Eq = false;

			_Input_Position        = GainToPosition(PlNet::GetInstance().GetUnbalanceInputGain());
			_Low_Freq_Position     = GainToPosition(_Equalizer_Table, PlNet::GetInstance().GetUnbalanceLowFrequencyGain());
			_Mid_Freq_Position     = GainToPosition(_Equalizer_Table, PlNet::GetInstance().GetUnbalanceMidFrequencyGain());
			_High_Freq_Position    = GainToPosition(_Equalizer_Table, PlNet::GetInstance().GetUnbalanceHighFrequencyGain());
			break;

		case USB_EQ_ID:
			_Mic_Show_Eq       = false;
			_Balance_Show_Eq   = false;
			_Unbalance_Show_Eq = false;
			_Usb_Show_Eq       = true;
			_Bluetooth_Show_Eq = false;

			_Input_Position        = GainToPosition(PlNet::GetInstance().GetUsbInputGain());
			_Low_Freq_Position     = GainToPosition(_Equalizer_Table, PlNet::GetInstance().GetUsbLowFrequencyGain());
			_Mid_Freq_Position     = GainToPosition(_Equalizer_Table, PlNet::GetInstance().GetUsbMidFrequencyGain());
			_High_Freq_Position    = GainToPosition(_Equalizer_Table, PlNet::GetInstance().GetUsbHighFrequencyGain());
			break;

		case BLUETOOTH_EQ_ID:
			_Mic_Show_Eq       = false;
			_Balance_Show_Eq   = false;
			_Unbalance_Show_Eq = false;
			_Usb_Show_Eq       = false;
			_Bluetooth_Show_Eq = true;

			_Input_Position        = GainToPosition(PlNet::GetInstance().GetBluetoothInputGain());
			_Low_Freq_Position     = GainToPosition(_Equalizer_Table, PlNet::GetInstance().GetBluetoothLowFrequencyGain());
			_Mid_Freq_Position     = GainToPosition(_Equalizer_Table, PlNet::GetInstance().GetBluetoothMidFrequencyGain());
			_High_Freq_Position    = GainToPosition(_Equalizer_Table, PlNet::GetInstance().GetBluetoothHighFrequencyGain());
			break;

		default:
			ASSERT(false);
	}

	emit inputGainAnimatedChanged();
	emit lowFreqGainAnimatedChanged();
	emit midFreqGainAnimatedChanged();
	emit highFreqGainAnimatedChanged();

	emit micShowEqChanged();
	emit balanceShowEqChanged();
	emit unbalanceShowEqChanged();
	emit usbShowEqChanged();
	emit bluetoothShowEqChanged();
}
