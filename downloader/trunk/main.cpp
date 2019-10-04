/******************************************************************************/
// GNU Lesser General Public License Usage
// This file may be used under the terms of the GNU Lesser
// General Public License version 2.1 as published by the Free Software
// Foundation and appearing in the file LICENSE.LGPL included in the
// packaging of this file.  Please review the following information to
// ensure the GNU Lesser General Public License version 2.1 requirements
// will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
/******************************************************************************/

#include <QSerialPort>

#include "./common/H/pl_net_structs.h"
#include "crc.h"
#include <QThread>

#if defined Q_OS_UNIX
#include <signal.h>
#endif

#include "global.h"

#define PL_NET_COMMAND_WRITE_TIMEOUT          1000
#define PL_NET_STX_TIMEOUT                    1000
#define PL_NET_HEADER_TIMEOUT                 20
#define PL_NET_BODY_TIMEOUT                   100

#define PL_NET_MAX_RETRIES_NUMBER             5

#define LOG(m)                	              qInfo().noquote() << QString("%1\r").arg(m).toLatin1()

/*****************************************************************************/
bool TxCommand(QSerialPort &serial_port, const PL_NET_COMMAND *command, qint64 command_length)
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
bool Rx(QSerialPort &serial_port, char *data, qint64 data_length, int timeout)
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
bool RxResponse(QSerialPort &serial_port, PL_NET_RESPONSE *response, qint64 *response_length)
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
								LOG("BODY crc error");
							}
						}
						else
						{
							LOG("Error receiving BODY");
						}
					}
					else
					{
						LOG("HEADER crc error");
					}
				}
				else
				{
					LOG("HEADER LENGTH/VERSION error");
				}
			}
			else
			{
				LOG("Error receiving HEADER");
			}
		}
		else
		{
			LOG("Error receiving STX");
		}
	}

	return (false);
}

/*****************************************************************************/
bool Transaction(QSerialPort &serial_port, const PL_NET_COMMANDS_BODY *command_body, qint64 command_body_length, PL_NET_RESPONSES_BODY *response_body, qint64 response_body_size,
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
				if (response_length > sizeof(PL_NET_HEADER))
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
bool Upgrade(QSerialPort &serial_port, uint32_t page_id, const uint8_t *page)
/*****************************************************************************/
{
	PL_NET_COMMANDS_BODY  command;
	PL_NET_RESPONSES_BODY response;
	qint64                response_length;
	QString               buffer;

	command.upgrade.cmd_id  = PL_NET_UPGRADE_CMD_ID;
	command.upgrade.page_id = page_id;
	memcpy(command.upgrade.page, page, PL_NET_PAGE_SIZE);

	if (Transaction(serial_port, &command, sizeof(command.upgrade), &response, sizeof(response.upgrade), &response_length))
	{
		if (response.upgrade.result != PL_NET_RESULT_OK)
		{
			buffer = QString("Upgrade result error [%1][%2]").arg(response.upgrade.result).arg(response_length);
			LOG(buffer);
		}
		else if (response_length != sizeof(response.upgrade))
		{
			buffer = QString("Upgrade response length error [%1]").arg(response_length);
			LOG(buffer);
		}
		else
		{
			return (true);
		}
	}
	else
	{
		buffer = QString("Upgrade timeout");
		LOG(buffer);
	}

	return (false);
}

/*****************************************************************************/
bool Reset(QSerialPort &serial_port)
/*****************************************************************************/
{
	PL_NET_COMMANDS_BODY  command;
	PL_NET_RESPONSES_BODY response;
	qint64                response_length;
	QString               buffer;

	command.reset.cmd_id  = PL_NET_RESET_CMD_ID;

	if (Transaction(serial_port, &command, sizeof(command.reset), &response, sizeof(response.reset), &response_length))
	{
		if (response.reset.result != PL_NET_RESULT_OK)
		{
			buffer = QString("Reset result error [%1][%2]").arg(response.reset.result).arg(response_length);
			LOG(buffer);
		}
		else if (response_length != sizeof(response.reset))
		{
			buffer = QString("Reset response length error [%1]").arg(response_length);
			LOG(buffer);
		}
		else
		{
			return (true);
		}
	}
	else
	{
		buffer = QString("Reset timeout");
		LOG(buffer);
	}

	return (false);
}


/******************************************************************************/
int main(int argc, char *argv[])
/******************************************************************************/
{
	QCoreApplication app(argc, argv);

	QString       file_name;
	QByteArray    data;
	int           data_length;
	int           data_sent;
	uint32_t      page_id;
	QSerialPort   serial_port;
	uint8_t       *pointer;
	QString       buffer;
	int           percent;
	int           old_percent;

	if (argc != 2)
	{
		LOG("Parameters error0");
		QThread::usleep(1000000);
		LOG("Parameters error1");
		QThread::usleep(1000000);
		LOG("Parameters error2");
		QThread::usleep(1000000);
		LOG("Parameters error3");
		QThread::usleep(1000000);
		return (-1);
	}

	file_name = QString(argv[1]);

	if (!QFile::exists(file_name))
	{
		LOG("File not exist");
		return (-1);
	}

	QFile file(file_name);

	if (!file.open(QIODevice::ReadOnly))
	{
		LOG("Unable to open file");
		return (-1);
	}

	data = file.readAll();

	file.close();

#if defined Q_OS_UNIX
	serial_port.setPortName("/dev/ttyAMA0");
#else
	serial_port.setPortName("COM1");
#endif

	ASSERT(serial_port.setBaudRate(115200));
	ASSERT(serial_port.setDataBits(QSerialPort::Data8));
	ASSERT(serial_port.setParity(QSerialPort::NoParity));
	ASSERT(serial_port.setStopBits(QSerialPort::OneStop));
	ASSERT(serial_port.setFlowControl(QSerialPort::NoFlowControl));

	if (!serial_port.open(QIODevice::ReadWrite))
	{
		LOG("Unable to open serial port");
		return (-1);
	}


	data_sent = 0;
	page_id = 0;
	percent = 0;
	old_percent = -1;

	pointer = (uint8_t *)data.data();

	data_length = data.length();
	while(data_sent < data_length)
	{
		if (!Upgrade(serial_port, page_id, pointer + data_sent))
		{
			LOG("Error upgrading");
			serial_port.close();

			return (-1);
		}

		data_sent += PL_NET_PAGE_SIZE;
		page_id++;

		percent = ((data_sent * 10) / data_length) * 10;
		if (percent != old_percent)
		{
			buffer = QString("Update firmware %1%").arg(percent, 3);
			LOG(buffer);

			old_percent = percent;
		}
	}

	if (!Reset(serial_port))
	{
		LOG("Error resetting");
		serial_port.close();

		return (-1);
	}

	serial_port.close();
	return (0);
}
