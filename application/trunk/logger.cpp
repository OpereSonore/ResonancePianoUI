

#include "logger.h"

#define FILE_NAME    "pianoLight"
#define ROTATE_SIZE  9999999
#define END_TIMEOUT  10000

/*****************************************************************************/

const char *const Logger::_Level_String[] =
{
	"[LOG]",
	"[ERRORS]",
	"[DEBUG]"
};

/*****************************************************************************/
LoggerWriter::LoggerWriter(QString &path, QString &message, QMutex *mutex)
/*****************************************************************************/
{
	_File_Path = path;
	_Message   = message;
	_Mutex     = mutex;
}

/*****************************************************************************/
void LoggerWriter::run(void)
/*****************************************************************************/
{
	QFile     file;
	QString   old_log_file_name;
	QDateTime date_time;

	try
	{
		QMutexLocker locker(_Mutex);

		file.setFileName(_File_Path);
		if (file.open(QFile::WriteOnly | QFile::Append | QFile::Text))
		{
			if (file.size() >= ROTATE_SIZE)
			{
				date_time = QDateTime::currentDateTime();

				old_log_file_name = QCoreApplication::applicationDirPath() + "/log/" FILE_NAME "_" + date_time.toString("yyyyMMddhhmmsszzz") + ".log";
				QFile::copy(_File_Path, old_log_file_name);

				file.resize(0);
			}

			file.write(_Message.toStdString().c_str());
			file.close();
		}
	}
	catch (...)
	{
	}
}

/*****************************************************************************/
Logger &Logger::GetInstance()
/*****************************************************************************/
{
	static Logger instance;

	return (instance);
}

/*****************************************************************************/
Logger::Logger(QObject *parent) : QObject(parent)
/*****************************************************************************/
{
	_File_Path = QCoreApplication::applicationDirPath() + "/log/" FILE_NAME ".log";
	QThreadPool::globalInstance()->setMaxThreadCount(1);
}

/*****************************************************************************/
Logger::~Logger()
/*****************************************************************************/
{
}

/*****************************************************************************/
void Logger::Initialize(LOG_LEVEL level)
/*****************************************************************************/
{
	QString module;
	QString message;

	ASSERT(level < LOG_LEVELS_NUMBER);

	_Level = level;

	module = "logger";
	message = QString("----------------------------------------%1----------------------------------------").arg(_Level_String[_Level]);

	WriteLog(LOG, module, message);
}

/*****************************************************************************/
void Logger::End(void)
/*****************************************************************************/
{
	QThreadPool::globalInstance()->waitForDone(END_TIMEOUT);
}

/*****************************************************************************/
void Logger::WriteLog(LOG_LEVEL level, QString &module, QString message)
/*****************************************************************************/
{
	QDateTime date_time;
	QString log_message;

	if (level <= _Level)
	{
		date_time = QDateTime::currentDateTime();
		log_message = QString("%1 [%2]: %3\n").arg(date_time.toString("yyyy-MM-dd hh:mm:ss.zzz")).arg(module).arg(message);

		LoggerWriter *writer = new LoggerWriter(_File_Path, log_message, &_Mutex);
		writer->setAutoDelete(true);
		// QThreadPool prende il controllo di LoggerWriter e lo cancella autonomamente
		QThreadPool::globalInstance()->start(writer);
	}
}

