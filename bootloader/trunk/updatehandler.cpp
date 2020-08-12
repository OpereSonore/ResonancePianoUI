/******************************************************************************/
// GNU Lesser General Public License Usage
// This file may be used under the terms of the GNU Lesser
// General Public License version 2.1 as published by the Free Software
// Foundation and appearing in the file LICENSE.LGPL included in the
// packaging of this file.  Please review the following information to
// ensure the GNU Lesser General Public License version 2.1 requirements
// will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
/******************************************************************************/

#include "updatehandler.h"

#include <QProcess>

DEFINE_LOG_MODULE_NAME;

#if defined Q_OS_UNIX
#define UPDATE_FOLDER         "/home/pi/pianoLight/update/"

#define UPDATE_FILE           "/home/pi/pianoLight/" APP_MODEL_NAME ".tar.gz"
#define UPDATE_SCRIPT         "/home/pi/pianoLight/update/update.sh"
#define UPDATE_README         "/home/pi/pianoLight/update/readme.txt"
#define ARCHIVED_FILE         "/home/pi/pianoLight/archive/" APP_MODEL_NAME "_%1.tar.gz"
#else
#define UPDATE_FOLDER         "I:/pianoLight/update/"

#define UPDATE_FILE           "I:/pianoLight/" APP_MODEL_NAME ".tar.gz"
#define UPDATE_SCRIPT         "I:/pianoLight/update/update.sh"
#define UPDATE_README         "I:/pianoLight/update/readme.txt"
#define ARCHIVED_FILE         "I:/pianoLight/archive/" APP_MODEL_NAME "_%1.tar.gz"
#endif

/******************************************************************************/
UpdateHandler &UpdateHandler::GetInstance(void)
/******************************************************************************/
{
	static UpdateHandler instance;

	return (instance);
}

/*****************************************************************************/
UpdateHandler::UpdateHandler(QObject *parent) : QObject(parent)
/*****************************************************************************/
{
	SetButtonEnable(NO_BUTTON_ENABLE);

	_ScrollEnable = false;
}

/*****************************************************************************/
UpdateHandler::~UpdateHandler(void)
/*****************************************************************************/
{
}

/*****************************************************************************/
void UpdateHandler::Initialize(void)
/*****************************************************************************/
{
	bool       ok;
	QString    command;
	QFile      readme;
	QByteArray content;
	QString    buffer;
	int        counts;


	try
	{
		ok = false;

		SetButtonEnable(NO_BUTTON_ENABLE);

		if (QFileInfo::exists(UPDATE_FILE))
		{
#if defined Q_OS_UNIX
			command = QString("tar -xvzf %1 -C %2 --overwrite").arg(UPDATE_FILE).arg(UPDATE_FOLDER);
#else
			command = QString("7z x %1 -o%2 -r -aou").arg(UPDATE_FILE).arg(UPDATE_FOLDER);
			ExecuteCommand(command);
			command = QString("7z e %1 -o%2 -r -aou").arg(UPDATE_FILE).arg(UPDATE_FOLDER);
#endif
			ExecuteCommand(command);
			readme.setFileName(UPDATE_README);

			counts = 0;
			while (counts < 200)
			{
				QThread::usleep(10000);
				qApp->processEvents(QEventLoop::AllEvents);
				counts++;
			}

			if (readme.exists() && readme.open(QIODevice::ReadOnly | QIODevice::Text))
			{
				content = readme.readAll();
				buffer = QString(content);
				SetStandardOutput(buffer);
				ok = true;
			}
			else
			{
				buffer = QString("Unable to find %1").arg(UPDATE_README);
				SetStandardOutput(buffer);
			}
		}
		else
		{
			buffer = QString("Unable to find %1").arg(UPDATE_FILE);
			SetStandardOutput(buffer);
		}
	}
	catch (std::exception &e)
	{
		ASSERT_EXCEPTION(e);
	}

	if (ok)
	{
		SetButtonEnable(CANCEL_BUTTON_ENABLE | UPDATE_BUTTON_ENABLE);
	}
	else
	{
		SetButtonEnable(CANCEL_BUTTON_ENABLE);
	}

}

/*****************************************************************************/
void UpdateHandler::ReadStandardOutput(void)
/*****************************************************************************/
{
	int         id;
	QProcess    *process;
	QByteArray  std_out;
	QStringList log_buffer;
	QString     buffer;

	process = (QProcess *)sender();

	std_out = process->readAllStandardOutput();
	log_buffer = QString(std_out).split(QRegExp("[\r\n]"), QString::SkipEmptyParts);

	for (id = 0; id < log_buffer.size(); id++)
	{
		LOG(QString("Process:[%1].Out %2").arg(process->program()).arg(log_buffer[id]));
	}

	buffer = QString(std_out);
	AppendStandardOutput(buffer);
}

/*****************************************************************************/
void UpdateHandler::ReadStandardError(void)
/*****************************************************************************/
{
	int         id;
	QProcess    *process;
	QByteArray  std_err;
	QStringList log_buffer;
	QString     buffer;

	process = (QProcess *)sender();

	std_err = process->readAllStandardError();
	log_buffer = QString(std_err).split(QRegExp("[\r\n]"), QString::SkipEmptyParts);

	for (id = 0; id < log_buffer.size(); id++)
	{
		LOG(QString("Process:[%1].Out %2").arg(process->program()).arg(log_buffer[id]));
	}

	buffer = QString(std_err);
	AppendStandardOutput(buffer);
}

/*****************************************************************************/
void UpdateHandler::ClearStandardOutput(void)
/*****************************************************************************/
{
	_ScrollEnable = false;

	_Standard_Output = "";
	emit standardOutputChanged();
}

/*****************************************************************************/
void UpdateHandler::AppendStandardOutput(QString &buffer)
/*****************************************************************************/
{
	_ScrollEnable = true;

	_Standard_Output += buffer;
	emit standardOutputChanged();
}

/*****************************************************************************/
void UpdateHandler::SetStandardOutput(QString &buffer)
/*****************************************************************************/
{
	_ScrollEnable = false;

	_Standard_Output = buffer;
	emit standardOutputChanged();
}

/*****************************************************************************/
QString UpdateHandler::GetStandardOutput(void)
/*****************************************************************************/
{
	return (_Standard_Output);
}

/*****************************************************************************/
void UpdateHandler::ExecuteCommand (QString &command)
/*****************************************************************************/
{
	QProcess   process;
	QEventLoop loop;
	QString    buffer;

	QObject::connect(&process, SIGNAL(readyReadStandardOutput()), this, SLOT(ReadStandardOutput()));
	QObject::connect(&process, SIGNAL(readyReadStandardError()), this, SLOT(ReadStandardError()));
	QObject::connect(&process, SIGNAL(finished(int, QProcess::ExitStatus)), &loop, SLOT(quit()));
	QObject::connect(&process, SIGNAL(errorOccurred(QProcess::ProcessError)), &loop, SLOT(quit()));

	process.start(command);
	if (process.error() == QProcess::FailedToStart )
	{
		buffer = QString("Process.Error: Unable to statr %1").arg(command);

		LOG(buffer);
		buffer.append("\n");
		AppendStandardOutput(buffer);
	}
	else
	{
		buffer = QString("Process.Start: %1").arg(command);

		LOG(buffer);

		loop.exec();
	}
}

/*****************************************************************************/
void UpdateHandler::cancelButtonClick(void)
/*****************************************************************************/
{
	QString   archived_file_name;
	QDateTime date_time;

	try
	{
		date_time = QDateTime::currentDateTime();
		archived_file_name = QString(ARCHIVED_FILE).arg(date_time.toString("yyyyMMddhhmmsszzz"));
		QFile::rename(UPDATE_FILE, archived_file_name);

		qApp->quit();
	}
	catch (std::exception &e)
	{
		ASSERT_EXCEPTION(e);
	}
}

/*****************************************************************************/
void UpdateHandler::updateButtonClick(void)
/*****************************************************************************/
{
	QString command;
	QString buffer;

	SetButtonEnable(NO_BUTTON_ENABLE);

	try
	{
		if (QFileInfo::exists(UPDATE_SCRIPT))
		{
			buffer = "Starting update...";

			LOG(buffer);
			buffer.append("\n");
			SetStandardOutput(buffer);

			command = QString("sh %1").arg(UPDATE_SCRIPT);
			ExecuteCommand(command);

			buffer = "Upadet done: reboot request";

			LOG(buffer);
			AppendStandardOutput(buffer);
		}
		else
		{
			buffer = QString("Unable to find ").arg(UPDATE_SCRIPT);

			LOG(buffer);
			SetStandardOutput(buffer);
		}
	}
	catch (std::exception &e)
	{
		ASSERT_EXCEPTION(e);
	}

	SetButtonEnable(REBOOT_BUTTON_ENABLE);
}

/*****************************************************************************/
void UpdateHandler::rebootButtonClick(void)
/*****************************************************************************/
{
	QString   archived_file_name;
	QDateTime date_time;

	try
	{
		date_time = QDateTime::currentDateTime();
		archived_file_name = QString(ARCHIVED_FILE).arg(date_time.toString("yyyyMMddhhmmsszzz"));
		QFile::rename(UPDATE_FILE, archived_file_name);
		QFile::remove(UPDATE_FILE);

		qApp->quit();
	}
	catch (std::exception &e)
	{
		ASSERT_EXCEPTION(e);
	}
}

/*****************************************************************************/
void UpdateHandler::SetButtonEnable(int enable)
/*****************************************************************************/
{
	_CancelButtonEnable = ((enable & CANCEL_BUTTON_ENABLE) != 0);
	_RebootButtonEnable = ((enable & REBOOT_BUTTON_ENABLE) != 0);
	_UpdateButtonEnable = ((enable & UPDATE_BUTTON_ENABLE) != 0);

	emit cancelButtonChanged();
	emit rebootButtonChanged();
	emit updateButtonChanged();
}
