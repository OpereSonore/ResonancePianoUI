/******************************************************************************/
// GNU Lesser General Public License Usage
// This file may be used under the terms of the GNU Lesser
// General Public License version 2.1 as published by the Free Software
// Foundation and appearing in the file LICENSE.LGPL included in the
// packaging of this file.  Please review the following information to
// ensure the GNU Lesser General Public License version 2.1 requirements
// will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
/******************************************************************************/

#include "plnet.h"

DEFINE_LOG_MODULE_NAME;

/******************************************************************************/

#define PL_NET_COMMAND_WRITE_TIMEOUT          1000
#define PL_NET_STX_TIMEOUT                    1000
#define PL_NET_HEADER_TIMEOUT                 20
#define PL_NET_BODY_TIMEOUT                   100

#define PL_NET_MAX_RETRIES_NUMBER             3

#define PL_NET_WAIT_EQ_TIME                   10

/******************************************************************************/

const PlNet::PL_NET_FIELD PlNet::_Pl_Net_Table[] =
{
	{
		PL_NET_RESET_CMD_ID,
		sizeof(PL_NET_RESET_COMMAND), sizeof(PL_NET_RESET_COMMAND)   + sizeof(PL_NET_HEADER),
		PL_NET_RESET_RSP_ID,
		sizeof(PL_NET_RESET_RESPONSE), sizeof(PL_NET_RESET_RESPONSE) + sizeof(PL_NET_HEADER),
	},

	{
		PL_NET_UPGRADE_CMD_ID,
		sizeof(PL_NET_UPGRADE_COMMAND),  sizeof(PL_NET_UPGRADE_COMMAND)  + sizeof(PL_NET_HEADER),
		PL_NET_UPGRADE_RSP_ID,
		sizeof(PL_NET_UPGRADE_RESPONSE), sizeof(PL_NET_UPGRADE_RESPONSE) + sizeof(PL_NET_HEADER),
	},

	{
		PL_NET_GET_INFO_CMD_ID,
		sizeof(PL_NET_GET_INFO_COMMAND),  sizeof(PL_NET_GET_INFO_COMMAND)  + sizeof(PL_NET_HEADER),
		PL_NET_GET_INFO_RSP_ID,
		sizeof(PL_NET_GET_INFO_RESPONSE), sizeof(PL_NET_GET_INFO_RESPONSE) + sizeof(PL_NET_HEADER),
	},

	{
		PL_NET_GET_EQ_CMD_ID,
		sizeof(PL_NET_GET_EQ_COMMAND),  sizeof(PL_NET_GET_EQ_COMMAND)  + sizeof(PL_NET_HEADER),
		PL_NET_GET_EQ_RSP_ID,
		sizeof(PL_NET_GET_EQ_RESPONSE), sizeof(PL_NET_GET_EQ_RESPONSE) + sizeof(PL_NET_HEADER),
	},

	{
		PL_NET_SET_EQ_CMD_ID,
		sizeof(PL_NET_SET_EQ_COMMAND),  sizeof(PL_NET_SET_EQ_COMMAND)  + sizeof(PL_NET_HEADER),
		PL_NET_SET_EQ_RSP_ID,
		sizeof(PL_NET_SET_EQ_RESPONSE), sizeof(PL_NET_SET_EQ_RESPONSE) + sizeof(PL_NET_HEADER),
	},

	{
		PL_NET_GET_STATUS_CMD_ID,
		sizeof(PL_NET_GET_STATUS_COMMAND),  sizeof(PL_NET_GET_STATUS_COMMAND)  + sizeof(PL_NET_HEADER),
		PL_NET_GET_STATUS_RSP_ID,
		sizeof(PL_NET_GET_STATUS_RESPONSE), sizeof(PL_NET_GET_STATUS_RESPONSE) + sizeof(PL_NET_HEADER),
	},
};

/******************************************************************************/
PlNet &PlNet::GetInstance(void)
/******************************************************************************/
{
	static PlNet instance;

	return (instance);
}

/*****************************************************************************/
PlNet::PlNet(QObject *parent) : QThread(parent)
/*****************************************************************************/
{
	uint                id;
	const PL_NET_FIELD  *field;

	_Condition_Flag = false;
	_Connected      = false;

	STATIC_ASSERT(ARRAY_SIZE(_Pl_Net_Table) == PL_NET_COMMANDS_NUMBER);

	for (id = 0; id < ARRAY_SIZE(_Pl_Net_Table); id++)
	{
		field = &_Pl_Net_Table[id];

		ASSERT(field->cmd_id == id);
		ASSERT(field->rsp_id == (id + PL_NET_RSP_ID_OFFSET));

		ASSERT(sizeof(PL_NET_HEADER) == (field->cmd_length) - (field->cmd_body_length));
		ASSERT(sizeof(PL_NET_HEADER) == (field->rsp_length) - (field->rsp_body_length));
	}
}

/*****************************************************************************/
PlNet::~PlNet(void)
/*****************************************************************************/
{
}

/*****************************************************************************/
void PlNet::End(void)
/*****************************************************************************/
{
	_Condition_Mutex.lock();
	requestInterruption();
	_Condition.wakeOne();
	_Condition_Mutex.unlock();

	wait();
}

/*****************************************************************************/
void PlNet::Initialize(const QString &serail_port)
/*****************************************************************************/
{
	_Serial_Port_Name = serail_port;

	start();
}

/*****************************************************************************/
bool PlNet::TxCommand(QSerialPort &serial_port, const PL_NET_COMMAND *command, qint64 command_length)
/*****************************************************************************/
{
	char     *pointer;
	qint64   length;
	qint64   written;

	length  = command_length;
	pointer = (char *)command;

	do
	{
		written = serial_port.write(pointer, length);
		if ((written == 0) || (written == -1))
		{
			return (false);
		}

		ASSERT((written > 0) && (written <= length));

		pointer += written;
		length  -= written;

	}
	while (length > 0);

	return (serial_port.waitForBytesWritten(PL_NET_COMMAND_WRITE_TIMEOUT));
}

/*****************************************************************************/
bool PlNet::Rx(QSerialPort &serial_port, char *data, qint64 data_length, int timeout)
/*****************************************************************************/
{
	char   *pointer;
	qint64 remained;
	qint64 readed;

	pointer  = data;
	remained = data_length;

	do
	{
		readed = serial_port.read(pointer, remained);
		if (readed == -1)
		{
			return (false);
		}

		ASSERT(readed <= remained);

		pointer  += readed;
		remained -= readed;

		if (remained == 0)
		{
			return (true);
		}
	} while (serial_port.waitForReadyRead(timeout));

	return (false);
}

/*****************************************************************************/
bool PlNet::RxResponse(QSerialPort &serial_port, PL_NET_RESPONSE *response, qint64 *response_length)
/*****************************************************************************/
{
	char      *rx_pointer;
	qint64    rx_length;
	uint16_t  crc;

	*response_length = 0;

	rx_pointer = (char *)&response->header.stx;
	rx_length  = sizeof(response->header.stx);

	while (Rx(serial_port, (char *)&response->header.stx, sizeof(response->header.stx), PL_NET_STX_TIMEOUT))
	{
		if (response->header.stx == PL_NET_STX)
		{
			rx_pointer = ((char *)&response->header.stx) + sizeof(response->header.stx);
			rx_length  = sizeof(response->header) - sizeof(response->header.stx);
			if (Rx(serial_port, rx_pointer, rx_length, PL_NET_HEADER_TIMEOUT))
			{
				if ((response->header.version == PL_NET_VERSION) &&
					(response->header.length  <= sizeof(response->body)))
				{
					crc = Crc::GetInstance().Crc16_1021(&response->header, (sizeof(response->header) - sizeof(response->header.crc)), PL_NET_CRC_16_INIT_VALUE);
					if (crc == response->header.crc)
					{
						rx_pointer = (char *)&response->body;
						rx_length  = response->header.length;
						if (Rx(serial_port, rx_pointer, rx_length, PL_NET_BODY_TIMEOUT))
						{
							crc = Crc::GetInstance().Crc16_1021(&response->body, response->header.length, PL_NET_CRC_16_INIT_VALUE);
							if (crc == response->header.body_crc)
							{
								*response_length = sizeof(response->header) + response->header.length;
								return (true);
							}
							else
							{
								LOG_DEBUG("BODY crc error");
							}
						}
						else
						{
							LOG_DEBUG("Error receiving BODY");
						}
					}
					else
					{
						LOG_DEBUG("HEADER crc error");
					}
				}
				else
				{
					LOG_DEBUG("HEADER LENGTH/VERSION error");
				}
			}
			else
			{
				LOG_DEBUG("Error receiving HEADER");
			}
		}
		else
		{
			LOG_DEBUG("Error receiving STX");
		}
	}

	return (false);
}

/*****************************************************************************/
bool PlNet::Transaction(QSerialPort &serial_port, const PL_NET_COMMANDS_BODY *command_body, qint64 command_body_length, PL_NET_RESPONSES_BODY *response_body, qint64 response_body_size,
						qint64 *response_body_length)
/*****************************************************************************/
{
	PL_NET_COMMAND   command;
	qint64           command_length;
	PL_NET_RESPONSE  response;
	qint64           response_length;
	int              retries;

	uint16_t         crc;

	ASSERT(command_body          != NULL);
	ASSERT(response_body         != NULL);
	ASSERT(response_body_length  != NULL);
	ASSERT(command_body_length   <= sizeof(command.body));

	*response_body_length = 0;

	ASSERT(command_body->cmd_id <  PL_NET_COMMANDS_NUMBER);

	command.header.stx     = PL_NET_STX;
	command.header.version = PL_NET_VERSION;
	command.header.length  = command_body_length;

	memcpy(&command.body, command_body, command_body_length);

	crc = Crc::GetInstance().Crc16_1021(&command.body, command_body_length, PL_NET_CRC_16_INIT_VALUE);

	command.header.body_crc = crc;

	crc = Crc::GetInstance().Crc16_1021(&command.header, (sizeof(command.header) - sizeof(command.header.crc)), PL_NET_CRC_16_INIT_VALUE);
	command.header.crc = crc;

	command_length = sizeof(command.header) + command_body_length;

	*response_body_length = 0;

	retries = 0;
	while (retries < PL_NET_MAX_RETRIES_NUMBER)
	{
		if (TxCommand(serial_port, &command, command_length))
		{
			if (RxResponse(serial_port, &response, &response_length))
			{
				if ((response.body.rsp_id == _Pl_Net_Table[command.body.cmd_id].rsp_id) &&
					(response_length      >  sizeof(PL_NET_HEADER)))
				{
					*response_body_length = (response_length - sizeof(PL_NET_HEADER));
					if (response_body_size >= (*response_body_length))
					{
						memcpy(response_body, &response.body, (*response_body_length));
						return (true);
					}
				}
			}
		}

		retries++;
	};

	return (false);
}

/*****************************************************************************/
bool PlNet::GetInfo(QSerialPort &serial_port, QString *firmware_name, QString *firmware_version)
/*****************************************************************************/
{
	PL_NET_COMMANDS_BODY  command;
	PL_NET_RESPONSES_BODY response;
	qint64                response_length;
	QString               buffer;

	command.get_info.cmd_id = PL_NET_GET_INFO_CMD_ID;

	if (Transaction(serial_port, &command, sizeof(command.get_info), &response, sizeof(response.get_info), &response_length))
	{
		if (response_length == sizeof(response.get_info))
		{
			response.get_info.firmware_name[sizeof(response.get_info.firmware_name) - 1] = '\0';
			*firmware_name = QString::fromLatin1(response.get_info.firmware_name);

			response.get_info.firmware_version[sizeof(response.get_info.firmware_version) - 1] = '\0';
			*firmware_version = QString::fromLatin1(response.get_info.firmware_version);

			return (true);
		}
		else
		{
			buffer = QString("GetInfo response length error [%1]").arg(response_length);
			LOG_DEBUG(buffer);
		}
	}
	else
	{
		buffer = QString("GetInfo timeout");
		LOG_DEBUG(buffer);
	}

	*firmware_name = "";
	*firmware_version = "";

	return (false);
}

/*****************************************************************************/
bool PlNet::GetEq(QSerialPort &serial_port, PL_NET_EQ *eq)
/*****************************************************************************/
{
	PL_NET_COMMANDS_BODY  command;
	PL_NET_RESPONSES_BODY response;
	qint64                response_length;
	QString               buffer;

	ASSERT(eq != NULL);

	command.get_eq.cmd_id = PL_NET_GET_EQ_CMD_ID;

	if (Transaction(serial_port, &command, sizeof(command.get_eq), &response, sizeof(response.get_eq), &response_length))
	{
		if (response.get_eq.result != PL_NET_RESULT_OK)
		{
			buffer = QString("GetEq result error [%1][%2]").arg(response.get_eq.result).arg(response_length);
			LOG_DEBUG(buffer);
		}
		else if (response_length != sizeof(response.get_eq))
		{
			buffer = QString("GetEq response length error [%1]").arg(response_length);
			LOG_DEBUG(buffer);
		}
		else
		{
			*eq = response.get_eq.eq;

			return (true);
		}
	}
	else
	{
		buffer = QString("GetEq timeout");
		LOG_DEBUG(buffer);
	}

	return (false);
}

/*****************************************************************************/
bool PlNet::SetEq(QSerialPort &serial_port, const PL_NET_EQ *eq)
/*****************************************************************************/
{
	PL_NET_COMMANDS_BODY  command;
	PL_NET_RESPONSES_BODY response;
	qint64                response_length;
	QString               buffer;

	command.set_eq.cmd_id = PL_NET_SET_EQ_CMD_ID;
	command.set_eq.eq     = *eq;

	if (Transaction(serial_port, &command, sizeof(command.set_eq), &response, sizeof(response.set_eq), &response_length))
	{
		if (response.set_eq.result != PL_NET_RESULT_OK)
		{
			buffer = QString("SetEq result error [%1][%2]").arg(response.set_eq.result).arg(response_length);
			LOG_DEBUG(buffer);
		}
		else if (response_length != sizeof(response.set_eq))
		{
			buffer = QString("SetEq response length error [%1]").arg(response_length);
			LOG_DEBUG(buffer);
		}
		else
		{
			return (true);
		}
	}
	else
	{
		buffer = QString("SetEq timeout");
		LOG_DEBUG(buffer);
	}

	return (false);
}

/*****************************************************************************/
bool PlNet::GetStatus(QSerialPort &serial_port, PL_NET_SIGNALS_BITS *signals_bits)
/*****************************************************************************/
{
	PL_NET_COMMANDS_BODY  command;
	PL_NET_RESPONSES_BODY response;
	qint64                response_length;
	QString               buffer;

	command.get_status.cmd_id = PL_NET_GET_STATUS_CMD_ID;

	if (Transaction(serial_port, &command, sizeof(command.get_status), &response, sizeof(response.get_status), &response_length))
	{
		if (response.get_status.result != PL_NET_RESULT_OK)
		{
			buffer = QString("GetStatus result error [%1][%2]").arg(response.get_status.result).arg(response_length);
			LOG_DEBUG(buffer);
		}
		else if (response_length != sizeof(response.get_status))
		{
			buffer = QString("GetStatus response length error [%1]").arg(response_length);
			LOG_DEBUG(buffer);
		}
		else
		{
			*signals_bits = response.get_status.signals_bits;

			return (true);
		}
	}
	else
	{
		buffer = QString("GetStatus timeout");
		LOG_DEBUG(buffer);
	}

	return (false);
}

/*****************************************************************************/
bool PlNet::EqDataChanged(PL_NET_EQ *eq)
/*****************************************************************************/
{
	bool result;

	QMutexLocker condition_locker(&_Condition_Mutex);

	result = _Condition_Flag;
	if (!result)
	{
		result = _Condition.wait(&_Condition_Mutex, PL_NET_WAIT_EQ_TIME);
	}

	if (result)
	{
		QMutexLocker locker(&_Eq_Mutex);
		_Condition_Flag = false;

		*eq = _Eq;
	}

	return (result);
}

/*****************************************************************************/
QString PlNet::GetFirmwareNameVersion(void)
/*****************************************************************************/
{
	if ((_Firmware_Name == "") || (_Firmware_Version == ""))
		return("");
	else
		return (_Firmware_Name + " - " + _Firmware_Version);
}

/*****************************************************************************/
QString PlNet::GetFirmwareVersion(void)
/*****************************************************************************/
{
	if ((_Firmware_Name == "") || (_Firmware_Version == ""))
		return("");
	else
		return (_Firmware_Version);
}

/*****************************************************************************/
bool PlNet::GetConnected(void)
/*****************************************************************************/
{
	return(_Connected);
}

/*****************************************************************************/
void PlNet::run(void)
/*****************************************************************************/
{
	QSerialPort          serial_port;
	uint                 state;
	PL_NET_EQ            eq;
	PL_NET_SIGNALS_BITS  signals_bits;

	LOG("Thread started");

	serial_port.setPortName(_Serial_Port_Name);
	ASSERT(serial_port.setBaudRate(115200));
	ASSERT(serial_port.setDataBits(QSerialPort::Data8));
	ASSERT(serial_port.setParity(QSerialPort::NoParity));
	ASSERT(serial_port.setStopBits(QSerialPort::OneStop));
	ASSERT(serial_port.setFlowControl(QSerialPort::NoFlowControl));

	if (!serial_port.open(QIODevice::ReadWrite))
	{
		LOG_ERROR("Unable to open" + _Serial_Port_Name);
		return;
	}

	_Connected = false;
	emit connectedChanged();

	state = 0;
	while (!isInterruptionRequested())
	{
		switch (state)
		{
			case 0:
				if (GetInfo(serial_port, &_Firmware_Name, &_Firmware_Version))
				{
					_Connected = true;

					emit connectedChanged();
					emit nameVersionChanged();
					emit versionChanged();

					state = 1;
				}
				break;

			case 1:
				if (GetEq(serial_port, &eq))
				{
					_Eq_Mutex.lock();
					_Eq = eq;
					_Eq_Mutex.unlock();

					emit EqChanged();

					state = 2;
				}
				else
				{
					state = 10;
				}
				break;

			case 2:
				if (EqDataChanged(&eq))
				{
					state = 3;
				}
				else
				{
					state = 4;
				}
				break;

			case 3:
				if (SetEq(serial_port, &eq))
				{
					state = 2;
				}
				else
				{
					state = 10;
				}
				break;

			case 4:
				if (GetStatus(serial_port, &signals_bits))
				{
					_Signals_Bits_Mutex.lock();
					_Signals_Bits = signals_bits;
					_Signals_Bits_Mutex.unlock();

					state = 2;
				}
				else
				{
					state = 10;
				}
				break;

			case 10:
				_Connected = false;

				emit connectedChanged();

				state = 0;
				break;

			default:
				ASSERT(false);


		}
	}

	serial_port.close();

	LOG("Thread stopped");

	quit();
}

/*****************************************************************************/
void PlNet::SetMainGain(float gain)
/*****************************************************************************/
{
	QMutexLocker condition_locker(&_Condition_Mutex);
	QMutexLocker eq_locker(&_Eq_Mutex);

	_Eq.output_gain = gain;

	_Condition_Flag	= true;
	_Condition.wakeOne();
}

/*****************************************************************************/
float PlNet::GetMainGain(void)
/*****************************************************************************/
{
	QMutexLocker eq_locker(&_Eq_Mutex);

	return (_Eq.output_gain);
}

/*****************************************************************************/
void PlNet::SetMicEnable(bool enable)
/*****************************************************************************/
{
	QMutexLocker condition_locker(&_Condition_Mutex);
	QMutexLocker eq_locker(&_Eq_Mutex);

	_Eq.mic.enable = enable ? 1 : 0;

	_Condition_Flag	= true;
	_Condition.wakeOne();
}

/*****************************************************************************/
bool PlNet::GetMicEnable(void)
/*****************************************************************************/
{
	QMutexLocker eq_locker(&_Eq_Mutex);

	return (_Eq.mic.enable);
}

/*****************************************************************************/
void PlNet::SetMicInputGain(float gain)
/*****************************************************************************/
{
	QMutexLocker condition_locker(&_Condition_Mutex);
	QMutexLocker eq_locker(&_Eq_Mutex);

	_Eq.mic.gain = gain;

	_Condition_Flag	= true;
	_Condition.wakeOne();
}

/*****************************************************************************/
float PlNet::GetMicInputGain(void)
/*****************************************************************************/
{
	QMutexLocker eq_locker(&_Eq_Mutex);

	return (_Eq.mic.gain);
}

/*****************************************************************************/
void PlNet::SetMicLowFrequencyGain(float gain)
/*****************************************************************************/
{
	QMutexLocker condition_locker(&_Condition_Mutex);
	QMutexLocker eq_locker(&_Eq_Mutex);

	_Eq.mic.low = gain;

	_Condition_Flag	= true;
	_Condition.wakeOne();
}

/*****************************************************************************/
float PlNet::GetMicLowFrequencyGain(void)
/*****************************************************************************/
{
	QMutexLocker eq_locker(&_Eq_Mutex);

	return (_Eq.mic.low);
}

/*****************************************************************************/
void PlNet::SetMicMidFrequencyGain(float gain)
/*****************************************************************************/
{
	QMutexLocker condition_locker(&_Condition_Mutex);
	QMutexLocker eq_locker(&_Eq_Mutex);

	_Eq.mic.mid = gain;

	_Condition_Flag	= true;
	_Condition.wakeOne();
}

/*****************************************************************************/
float PlNet::GetMicMidFrequencyGain(void)
/*****************************************************************************/
{
	QMutexLocker eq_locker(&_Eq_Mutex);

	return (_Eq.mic.mid);
}

/*****************************************************************************/
void PlNet::SetMicHighFrequencyGain(float gain)
/*****************************************************************************/
{
	QMutexLocker condition_locker(&_Condition_Mutex);
	QMutexLocker eq_locker(&_Eq_Mutex);

	_Eq.mic.high = gain;

	_Condition_Flag	= true;
	_Condition.wakeOne();
}

/*****************************************************************************/
float PlNet::GetMicHighFrequencyGain(void)
/*****************************************************************************/
{
	QMutexLocker eq_locker(&_Eq_Mutex);

	return (_Eq.mic.high);
}

/*****************************************************************************/
void PlNet::SetBalanceEnable(bool enable)
/*****************************************************************************/
{
	QMutexLocker condition_locker(&_Condition_Mutex);
	QMutexLocker eq_locker(&_Eq_Mutex);

	_Eq.xlr.enable = enable ? 1 : 0;

	_Condition_Flag	= true;
	_Condition.wakeOne();
}

/*****************************************************************************/
bool PlNet::GetBalanceEnable(void)
/*****************************************************************************/
{
	QMutexLocker eq_locker(&_Eq_Mutex);

	return (_Eq.xlr.enable);
}

/*****************************************************************************/
void PlNet::SetBalanceInputGain(float gain)
/*****************************************************************************/
{
	QMutexLocker condition_locker(&_Condition_Mutex);
	QMutexLocker eq_locker(&_Eq_Mutex);

	_Eq.xlr.gain = gain;

	_Condition_Flag	= true;
	_Condition.wakeOne();
}

/*****************************************************************************/
float PlNet::GetBalanceInputGain(void)
/*****************************************************************************/
{
	QMutexLocker eq_locker(&_Eq_Mutex);

	return (_Eq.xlr.gain);
}

/*****************************************************************************/
void PlNet::SetBalanceLowFrequencyGain(float gain)
/*****************************************************************************/
{
	QMutexLocker condition_locker(&_Condition_Mutex);
	QMutexLocker eq_locker(&_Eq_Mutex);

	_Eq.xlr.low = gain;

	_Condition_Flag	= true;
	_Condition.wakeOne();
}

/*****************************************************************************/
float PlNet::GetBalanceLowFrequencyGain(void)
/*****************************************************************************/
{
	QMutexLocker eq_locker(&_Eq_Mutex);

	return (_Eq.xlr.low);
}

/*****************************************************************************/
void PlNet::SetBalanceMidFrequencyGain(float gain)
/*****************************************************************************/
{
	QMutexLocker condition_locker(&_Condition_Mutex);
	QMutexLocker eq_locker(&_Eq_Mutex);

	_Eq.xlr.mid = gain;

	_Condition_Flag	= true;
	_Condition.wakeOne();
}

/*****************************************************************************/
float PlNet::GetBalanceMidFrequencyGain(void)
/*****************************************************************************/
{
	QMutexLocker eq_locker(&_Eq_Mutex);

	return (_Eq.xlr.mid);
}

/*****************************************************************************/
void PlNet::SetBalanceHighFrequencyGain(float gain)
/*****************************************************************************/
{
	QMutexLocker condition_locker(&_Condition_Mutex);
	QMutexLocker eq_locker(&_Eq_Mutex);

	_Eq.xlr.high = gain;

	_Condition_Flag	= true;
	_Condition.wakeOne();
}

/*****************************************************************************/
float PlNet::GetBalanceHighFrequencyGain(void)
/*****************************************************************************/
{
	QMutexLocker eq_locker(&_Eq_Mutex);

	return (_Eq.xlr.high);
}

/*****************************************************************************/
void PlNet::SetUnbalanceEnable(bool enable)
/*****************************************************************************/
{
	QMutexLocker condition_locker(&_Condition_Mutex);
	QMutexLocker eq_locker(&_Eq_Mutex);

	_Eq.rca.enable = enable;

	_Condition_Flag	= true;
	_Condition.wakeOne();
}

/*****************************************************************************/
bool PlNet::GetUnbalanceEnable(void)
/*****************************************************************************/
{
	QMutexLocker eq_locker(&_Eq_Mutex);

	return (_Eq.rca.enable);
}

/*****************************************************************************/
void PlNet::SetUnbalanceInputGain(float gain)
/*****************************************************************************/
{
	QMutexLocker condition_locker(&_Condition_Mutex);
	QMutexLocker eq_locker(&_Eq_Mutex);

	_Eq.rca.gain = gain;

	_Condition_Flag	= true;
	_Condition.wakeOne();
}

/*****************************************************************************/
float PlNet::GetUnbalanceInputGain(void)
/*****************************************************************************/
{
	QMutexLocker eq_locker(&_Eq_Mutex);

	return (_Eq.rca.gain);
}

/*****************************************************************************/
void PlNet::SetUnbalanceLowFrequencyGain(float gain)
/*****************************************************************************/
{
	QMutexLocker condition_locker(&_Condition_Mutex);
	QMutexLocker eq_locker(&_Eq_Mutex);

	_Eq.rca.low = gain;

	_Condition_Flag	= true;
	_Condition.wakeOne();
}

/*****************************************************************************/
float PlNet::GetUnbalanceLowFrequencyGain(void)
/*****************************************************************************/
{
	QMutexLocker eq_locker(&_Eq_Mutex);

	return (_Eq.rca.low);
}

/*****************************************************************************/
void PlNet::SetUnbalanceMidFrequencyGain(float gain)
/*****************************************************************************/
{
	QMutexLocker condition_locker(&_Condition_Mutex);
	QMutexLocker eq_locker(&_Eq_Mutex);

	_Eq.rca.mid = gain;

	_Condition_Flag	= true;
	_Condition.wakeOne();
}

/*****************************************************************************/
float PlNet::GetUnbalanceMidFrequencyGain(void)
/*****************************************************************************/
{
	QMutexLocker eq_locker(&_Eq_Mutex);

	return (_Eq.rca.mid);
}

/*****************************************************************************/
void PlNet::SetUnbalanceHighFrequencyGain(float gain)
/*****************************************************************************/
{
	QMutexLocker condition_locker(&_Condition_Mutex);
	QMutexLocker eq_locker(&_Eq_Mutex);

	_Eq.rca.high = gain;

	_Condition_Flag	= true;
	_Condition.wakeOne();
}

/*****************************************************************************/
float PlNet::GetUnbalanceHighFrequencyGain(void)
/*****************************************************************************/
{
	QMutexLocker eq_locker(&_Eq_Mutex);

	return (_Eq.rca.high);
}

/*****************************************************************************/
void PlNet::SetUsbEnable(bool enable)
/*****************************************************************************/
{
	QMutexLocker condition_locker(&_Condition_Mutex);
	QMutexLocker eq_locker(&_Eq_Mutex);

	_Eq.mp3.enable = enable;

	_Condition_Flag	= true;
	_Condition.wakeOne();
}

/*****************************************************************************/
bool PlNet::GetUsbEnable(void)
/*****************************************************************************/
{
	QMutexLocker eq_locker(&_Eq_Mutex);

	return (_Eq.mp3.enable);
}

/*****************************************************************************/
void PlNet::SetUsbInputGain(float gain)
/*****************************************************************************/
{
	QMutexLocker condition_locker(&_Condition_Mutex);
	QMutexLocker eq_locker(&_Eq_Mutex);

	_Eq.mp3.gain = gain;

	_Condition_Flag	= true;
	_Condition.wakeOne();
}

/*****************************************************************************/
float PlNet::GetUsbInputGain(void)
/*****************************************************************************/
{
	QMutexLocker eq_locker(&_Eq_Mutex);

	return (_Eq.mp3.gain);
}

/*****************************************************************************/
void PlNet::SetUsbLowFrequencyGain(float gain)
/*****************************************************************************/
{
	QMutexLocker condition_locker(&_Condition_Mutex);
	QMutexLocker eq_locker(&_Eq_Mutex);

	_Eq.mp3.low = gain;

	_Condition_Flag	= true;
	_Condition.wakeOne();
}

/*****************************************************************************/
float PlNet::GetUsbLowFrequencyGain(void)
/*****************************************************************************/
{
	QMutexLocker eq_locker(&_Eq_Mutex);

	return (_Eq.mp3.low);
}

/*****************************************************************************/
void PlNet::SetUsbMidFrequencyGain(float gain)
/*****************************************************************************/
{
	QMutexLocker condition_locker(&_Condition_Mutex);
	QMutexLocker eq_locker(&_Eq_Mutex);

	_Eq.mp3.mid = gain;

	_Condition_Flag	= true;
	_Condition.wakeOne();
}

/*****************************************************************************/
float PlNet::GetUsbMidFrequencyGain(void)
/*****************************************************************************/
{
	QMutexLocker eq_locker(&_Eq_Mutex);

	return (_Eq.mp3.mid);
}

/*****************************************************************************/
void PlNet::SetUsbHighFrequencyGain(float gain)
/*****************************************************************************/
{
	QMutexLocker condition_locker(&_Condition_Mutex);
	QMutexLocker eq_locker(&_Eq_Mutex);

	_Eq.mp3.high = gain;

	_Condition_Flag	= true;
	_Condition.wakeOne();
}

/*****************************************************************************/
float PlNet::GetUsbHighFrequencyGain(void)
/*****************************************************************************/
{
	QMutexLocker eq_locker(&_Eq_Mutex);

	return (_Eq.mp3.high);
}

/*****************************************************************************/
void PlNet::SetBluetoothEnable(bool enable)
/*****************************************************************************/
{
	QMutexLocker condition_locker(&_Condition_Mutex);
	QMutexLocker eq_locker(&_Eq_Mutex);

	_Eq.bt.enable = enable;

	_Condition_Flag	= true;
	_Condition.wakeOne();
}

/*****************************************************************************/
bool PlNet::GetBluetoothEnable(void)
/*****************************************************************************/
{
	QMutexLocker eq_locker(&_Eq_Mutex);

	return (_Eq.bt.enable);
}

/*****************************************************************************/
void PlNet::SetBluetoothInputGain(float gain)
/*****************************************************************************/
{
	QMutexLocker condition_locker(&_Condition_Mutex);
	QMutexLocker eq_locker(&_Eq_Mutex);

	_Eq.bt.gain = gain;

	_Condition_Flag	= true;
	_Condition.wakeOne();
}

/*****************************************************************************/
float PlNet::GetBluetoothInputGain(void)
/*****************************************************************************/
{
	QMutexLocker eq_locker(&_Eq_Mutex);

	return (_Eq.bt.gain);
}

/*****************************************************************************/
void PlNet::SetBluetoothLowFrequencyGain(float gain)
/*****************************************************************************/
{
	QMutexLocker condition_locker(&_Condition_Mutex);
	QMutexLocker eq_locker(&_Eq_Mutex);

	_Eq.bt.low = gain;

	_Condition_Flag	= true;
	_Condition.wakeOne();
}

/*****************************************************************************/
float PlNet::GetBluetoothLowFrequencyGain(void)
/*****************************************************************************/
{
	QMutexLocker eq_locker(&_Eq_Mutex);

	return (_Eq.bt.low);
}

/*****************************************************************************/
void PlNet::SetBluetoothMidFrequencyGain(float gain)
/*****************************************************************************/
{
	QMutexLocker condition_locker(&_Condition_Mutex);
	QMutexLocker eq_locker(&_Eq_Mutex);

	_Eq.bt.mid = gain;

	_Condition_Flag	= true;
	_Condition.wakeOne();
}

/*****************************************************************************/
float PlNet::GetBluetoothMidFrequencyGain(void)
/*****************************************************************************/
{
	QMutexLocker eq_locker(&_Eq_Mutex);

	return (_Eq.bt.mid);
}

/*****************************************************************************/
void PlNet::SetBluetoothHighFrequencyGain(float gain)
/*****************************************************************************/
{
	QMutexLocker condition_locker(&_Condition_Mutex);
	QMutexLocker eq_locker(&_Eq_Mutex);

	_Eq.bt.high = gain;

	_Condition_Flag	= true;
	_Condition.wakeOne();
}

/*****************************************************************************/
float PlNet::GetBluetoothHighFrequencyGain(void)
/*****************************************************************************/
{
	QMutexLocker eq_locker(&_Eq_Mutex);

	return (_Eq.bt.high);
}

