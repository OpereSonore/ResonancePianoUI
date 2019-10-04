/******************************************************************************/
// GNU Lesser General Public License Usage
// This file may be used under the terms of the GNU Lesser
// General Public License version 2.1 as published by the Free Software
// Foundation and appearing in the file LICENSE.LGPL included in the
// packaging of this file.  Please review the following information to
// ensure the GNU Lesser General Public License version 2.1 requirements
// will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
/******************************************************************************/

#include "fatalerror.h"

#define FILE_NAME    "fatal_error"
#define ROTATE_SIZE  9999999

/*****************************************************************************/
Assert &Assert::GetInstance()
/*****************************************************************************/
{
	static Assert instance;

	return (instance);
}

/*****************************************************************************/
Assert::Assert(QObject *parent) : QObject(parent)
/*****************************************************************************/
{
	_File_Path = QCoreApplication::applicationDirPath() + "/log/" FILE_NAME ".log";
}

/*****************************************************************************/
Assert::~Assert()
/*****************************************************************************/
{

}

/*****************************************************************************/
void Assert::FatalError(const char *file, const int line, const char *message)
/*****************************************************************************/
{
	QString file_error_message;
	QString dialog_error_message;

	dialog_error_message = QString("ASSERT(false) in %1 at line %2").arg(file).arg(line);
	file_error_message = QString("%1 ASSERT(%2) in %3 at line %4\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz :")).arg(message).arg(file).arg(line);

	WriteFile(file_error_message);

	_Wait_Close_Condition = false;
	emit openAssertDialog(dialog_error_message);

	while (!_Wait_Close_Condition)
	{
		qApp->processEvents(QEventLoop::AllEvents);
	}
}

/*****************************************************************************/
void Assert::FatalErrorException(const char *file, const int line, const char *message)
/*****************************************************************************/
{
	QString error_message;

	error_message = QString("%1 ASSERT_EXCEPTION(%2) in %3 at line %4\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz :")).arg(message).arg(file).arg(line);

	WriteFile(error_message);

	_Wait_Close_Condition = false;
	emit openAssertDialog(error_message);

	while (!_Wait_Close_Condition)
	{
		qApp->processEvents(QEventLoop::AllEvents);
	}
}

/*****************************************************************************/
void Assert::WriteFile(QString message)
/*****************************************************************************/
{
	QFile file;
	QString old_log_file_name;
	QDateTime date_time;

	try
	{
		date_time = QDateTime::currentDateTime();

		file.setFileName(_File_Path);
		if (file.open(QFile::WriteOnly | QFile::Append | QFile::Text))
		{
			if (file.size() >= ROTATE_SIZE)
			{
				old_log_file_name = QCoreApplication::applicationDirPath() + "/log/" FILE_NAME "_" + date_time.toString("yyyyMMddhhmmsszzz") + ".log";
				QFile::copy(_File_Path, old_log_file_name);

				file.resize(0);
			}

			file.write(message.toStdString().c_str());
			file.close();
		}
	}
	catch (...)
	{
	}
}

/*****************************************************************************/
void Assert::Close(void)
/*****************************************************************************/
{
	_Wait_Close_Condition = true;
}
