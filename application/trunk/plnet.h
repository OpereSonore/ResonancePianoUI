#ifndef PLNET_H
#define PLNET_H

#include "global.h"

#include <QObject>
#include <QThread>
#include <QWaitCondition>
#include <QSerialPort>

#include "./common/H/pl_net_structs.h"
#include "crc.h"

class PlNet : public QThread
{
	Q_OBJECT
	Q_DISABLE_COPY(PlNet)

	Q_PROPERTY(QString FirmwareNameVersion  READ GetFirmwareNameVersion  NOTIFY nameVersionChanged)
	Q_PROPERTY(QString FirmwareVersion      READ GetFirmwareVersion      NOTIFY versionChanged)
	Q_PROPERTY(bool    Connected            READ GetConnected            NOTIFY connectedChanged)

public:
	typedef struct
	{
		uint8_t  cmd_id;
		uint32_t cmd_body_length;
		uint32_t cmd_length;
		uint8_t  rsp_id;
		uint32_t rsp_body_length;
		uint32_t rsp_length;
	}  PL_NET_FIELD;

	// Signals and Slots
public slots:
	// QML slots

	// Local slots
	void SetMainGain(float gain);

	void SetMicEnable(bool enable);
	void SetMicInputGain(float gain);
	void SetMicLowFrequencyGain(float gain);
	void SetMicMidFrequencyGain(float gain);
	void SetMicHighFrequencyGain(float gain);

	void SetBalanceEnable(bool enable);
	void SetBalanceInputGain(float gain);
	void SetBalanceLowFrequencyGain(float gain);
	void SetBalanceMidFrequencyGain(float gain);
	void SetBalanceHighFrequencyGain(float gain);

	void SetUnbalanceEnable(bool enable);
	void SetUnbalanceInputGain(float gain);
	void SetUnbalanceLowFrequencyGain(float gain);
	void SetUnbalanceMidFrequencyGain(float gain);
	void SetUnbalanceHighFrequencyGain(float gain);

	void SetUsbEnable(bool enable);
	void SetUsbInputGain(float gain);
	void SetUsbLowFrequencyGain(float gain);
	void SetUsbMidFrequencyGain(float gain);
	void SetUsbHighFrequencyGain(float gain);

	void SetBluetoothEnable(bool enable);
	void SetBluetoothInputGain(float gain);
	void SetBluetoothLowFrequencyGain(float gain);
	void SetBluetoothMidFrequencyGain(float gain);
	void SetBluetoothHighFrequencyGain(float gain);

signals:
	// QML signals
	void nameVersionChanged(void);
	void versionChanged(void);
	void connectedChanged(void);

	// Local signals
	void EqChanged(void);

	// Functions
public:
	static PlNet &GetInstance(void);

	~PlNet(void);
	void Initialize(const QString &serial_port);
	void run();
	void End(void);

	float GetMainGain(void);

	bool GetMicEnable(void);
	float GetMicInputGain(void);
	float GetMicLowFrequencyGain(void);
	float GetMicMidFrequencyGain(void);
	float GetMicHighFrequencyGain(void);

	bool GetBalanceEnable(void);
	float GetBalanceInputGain(void);
	float GetBalanceLowFrequencyGain(void);
	float GetBalanceMidFrequencyGain(void);
	float GetBalanceHighFrequencyGain(void);

	bool GetUnbalanceEnable(void);
	float GetUnbalanceInputGain(void);
	float GetUnbalanceLowFrequencyGain(void);
	float GetUnbalanceMidFrequencyGain(void);
	float GetUnbalanceHighFrequencyGain(void);

	bool GetUsbEnable(void);
	float GetUsbInputGain(void);
	float GetUsbLowFrequencyGain(void);
	float GetUsbMidFrequencyGain(void);
	float GetUsbHighFrequencyGain(void);

	bool GetBluetoothEnable(void);
	float GetBluetoothInputGain(void);
	float GetBluetoothLowFrequencyGain(void);
	float GetBluetoothMidFrequencyGain(void);
	float GetBluetoothHighFrequencyGain(void);

	QString GetFirmwareNameVersion(void);
	QString GetFirmwareVersion(void);
	bool    GetConnected(void);

	bool EqDataChanged(PL_NET_EQ *eq);

private:
	explicit PlNet(QObject *parent = nullptr);

	bool TxCommand(QSerialPort &serial_port, const PL_NET_COMMAND *command, qint64 command_length);
	bool Rx(QSerialPort &serial_port, char *data, qint64 length, int timeout);
	bool RxResponse(QSerialPort &serial_port, PL_NET_RESPONSE *response, qint64 *response_length);
	bool Transaction(QSerialPort &serial_port, const PL_NET_COMMANDS_BODY *command_body, qint64 command_body_length, PL_NET_RESPONSES_BODY *response_body, qint64 response_body_size,
	                 qint64 *response_body_length);

	bool GetInfo(QSerialPort &serial_port, QString *firmware_name, QString *firmware_version);
	bool GetEq(QSerialPort &serial_port, PL_NET_EQ *eq);
	bool SetEq(QSerialPort &serial_port, const PL_NET_EQ *eq);
	bool GetStatus(QSerialPort &serial_port, PL_NET_SIGNALS_BITS *signals_bits);

	// Variables
private:
	static const PL_NET_FIELD _Pl_Net_Table[];

	bool                      _Connected;

	QMutex                    _Condition_Mutex;
	bool                      _Condition_Flag;
	QWaitCondition            _Condition;

	QString                   _Serial_Port_Name;

	QMutex                    _Eq_Mutex;
	PL_NET_EQ                 _Eq;
	QMutex                    _Signals_Bits_Mutex;
	PL_NET_SIGNALS_BITS       _Signals_Bits;
	QString                   _Firmware_Name;
	QString                   _Firmware_Version;
};

#endif // USBMOUNTPOINT_H
