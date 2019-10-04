#ifndef EQUALIZER_H
#define EQUALIZER_H

#include "global.h"

#include <QtCore>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QTimer>

class Equalizer : public QObject
{
	Q_OBJECT

	Q_DISABLE_COPY(Equalizer)
	Q_PROPERTY(double  MainGain             READ GetMainGain           WRITE SetMainGain             NOTIFY mainGainChanged)
	Q_PROPERTY(double  InputGain            READ GetInputGain          WRITE SetInputGain            NOTIFY inputGainChanged)
	Q_PROPERTY(double  LowFreqGain          READ GetLowFreqGain        WRITE SetLowFreqGain          NOTIFY lowFreqGainChanged)
	Q_PROPERTY(double  MidFreqGain          READ GetMidFreqGain        WRITE SetMidFreqGain          NOTIFY midFreqGainChanged)
	Q_PROPERTY(double  HighFreqGain         READ GetHighFreqGain       WRITE SetHighFreqGain         NOTIFY highFreqGainChanged)

	Q_PROPERTY(bool    MicEnable            READ GetMicEnable          WRITE SetMicEnable            NOTIFY micEnableChanged)
	Q_PROPERTY(bool    BalanceEnable        READ GetBalanceEnable      WRITE SetBalanceEnable        NOTIFY balanceEnableChanged)
	Q_PROPERTY(bool    UnbalanceEnable      READ GetUnbalanceEnable    WRITE SetUnbalanceEnable      NOTIFY unbalanceEnableChanged)
	Q_PROPERTY(bool    UsbEnable            READ GetUsbEnable          WRITE SetUsbEnable            NOTIFY usbEnableChanged)
	Q_PROPERTY(bool    BluetoothEnable      READ GetBluetoothEnable    WRITE SetBluetoothEnable      NOTIFY bluetoothEnableChanged)

	Q_PROPERTY(bool    MicShowEq            READ GetMicShowEq          WRITE SetMicShowEq            NOTIFY micShowEqChanged)
	Q_PROPERTY(bool    BalanceShowEq        READ GetBalanceShowEq      WRITE SetBalanceShowEq        NOTIFY balanceShowEqChanged)
	Q_PROPERTY(bool    UnbalanceShowEq      READ GetUnbalanceShowEq    WRITE SetUnbalanceShowEq      NOTIFY unbalanceShowEqChanged)
	Q_PROPERTY(bool    UsbShowEq            READ GetUsbShowEq          WRITE SetUsbShowEq            NOTIFY usbShowEqChanged)
	Q_PROPERTY(bool    BluetoothShowEq      READ GetBluetoothShowEq    WRITE SetBluetoothShowEq      NOTIFY bluetoothShowEqChanged)

public:
	typedef enum
	{
		NO_EQ_ID,
		MIC_EQ_ID,
		BALANCE_EQ_ID,
		UNBALANCE_EQ_ID,
		USB_EQ_ID,
		BLUETOOTH_EQ_ID,

		EQ_ID_NUMBER
	} EQ_ID;

private:
	typedef struct
	{
		double min_gain;
		double min_position;
		double max_gain;
		double max_position;
	} POSITION_GAIN_TABLE;

	typedef enum
	{
		TIMEOUT_EVENT_ID,
		PRESS_EVENT_ID,
	} HANDLE_EVENT_ID;

	typedef enum
	{
		LOW_FREQ_SLIDER_ID,
		MID_FREQ_SLIDER_ID,
		HIGH_FREQ_SLIDER_ID,

		SLIDER_EQ_NUMBER,
	} SLIDER_EQ_ID;

	typedef struct
	{
		int                       state;
		QTimer                    timer;
		double                    *position;
		std::function<void(void)> set_middle;
	} SLIDER_EQ_DATA;

// Signals and Slots
public slots:
	// QML slots
	void onLowFreqHandlePressed(void);
	void onMidFreqHandlePressed(void);
	void onHighFreqHandlePressed(void);

	// Local slots
	void EqChanged(void);
	void LowFreqSliderTimeout(void);
	void MidFreqSliderTimeout(void);
	void HighFreqSliderTimeout(void);

signals:
	// QML signals
	void mainGainChanged(void);
	void inputGainChanged(void);
	void lowFreqGainChanged(void);
	void midFreqGainChanged(void);
	void highFreqGainChanged(void);

	void mainGainAnimatedChanged(void);
	void inputGainAnimatedChanged(void);
	void lowFreqGainAnimatedChanged(void);
	void midFreqGainAnimatedChanged(void);
	void highFreqGainAnimatedChanged(void);

	void micEnableChanged(void);
	void balanceEnableChanged(void);
	void unbalanceEnableChanged(void);
	void usbEnableChanged(void);
	void bluetoothEnableChanged(void);

	void micShowEqChanged(void);
	void balanceShowEqChanged(void);
	void unbalanceShowEqChanged(void);
	void usbShowEqChanged(void);
	void bluetoothShowEqChanged(void);

	void lowFreqGainSetMiddle(void);
	void midFreqGainSetMiddle(void);
	void highFreqGainSetMiddle(void);

	// Local signals

// Functions
public:
	static Equalizer &GetInstance(void);

	~Equalizer(void);

	Equalizer::EQ_ID GetEqId(void);
	void SetEqId(EQ_ID eq_id);

	double GetMainGain(void);
	void SetMainGain(double &position);

	double GetInputGain(void);
	void SetInputGain(double &position);

	double GetLowFreqGain(void);
	void SetLowFreqGain(double &position);

	double GetMidFreqGain(void);
	void SetMidFreqGain(double &position);

	double GetHighFreqGain(void);
	void SetHighFreqGain(double &position);

	bool GetMicEnable(void);
	void SetMicEnable(bool &enable);

	bool GetBalanceEnable(void);
	void SetBalanceEnable(bool &enable);

	bool GetUnbalanceEnable(void);
	void SetUnbalanceEnable(bool &enable);

	bool GetUsbEnable(void);
	void SetUsbEnable(bool &enable);

	bool GetBluetoothEnable(void);
	void SetBluetoothEnable(bool &enable);

	bool GetMicShowEq(void);
	void SetMicShowEq(bool &show_eq);

	bool GetBalanceShowEq(void);
	void SetBalanceShowEq(bool &show_eq);

	bool GetUnbalanceShowEq(void);
	void SetUnbalanceShowEq(bool &show_eq);

	bool GetUsbShowEq(void);
	void SetUsbShowEq(bool &show_eq);

	bool GetBluetoothShowEq(void);
	void SetBluetoothShowEq(bool &show_eq);

private:
	explicit Equalizer(QObject *parent = nullptr);

	void RefreshEq(void);

	double GainToPosition(const POSITION_GAIN_TABLE &table, float gain);
	double GainToPosition(float gain);
	float PositionToGain(const POSITION_GAIN_TABLE &table, double position);
	float PositionToGain(double position);

	void SliderEqEventHandler(SLIDER_EQ_ID id, HANDLE_EVENT_ID event);

// Variables
private:
	static const POSITION_GAIN_TABLE _Equalizer_Table;

	double         _Main_Position;
	double         _Input_Position;
	double         _Low_Freq_Position;
	double         _Mid_Freq_Position;
	double         _High_Freq_Position;

	bool           _Mic_Enable;
	bool           _Balance_Enable;
	bool           _Unbalance_Enable;
	bool           _Usb_Enable;
	bool           _Bluetooth_Enable;

	bool           _Mic_Show_Eq;
	bool           _Balance_Show_Eq;
	bool           _Unbalance_Show_Eq;
	bool           _Usb_Show_Eq;
	bool           _Bluetooth_Show_Eq;

	EQ_ID          _Eq_Id;

	SLIDER_EQ_DATA _Slider_Eq_Data[SLIDER_EQ_NUMBER];

};

#endif // EQUALIZER_H
