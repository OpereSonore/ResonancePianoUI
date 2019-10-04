#ifndef LOGGER_H
#define LOGGER_H

#include <QDateTime>
#include <QString>
#include <QFile>
#include <QDir>
#include <QObject>
#include <QMutex>
#include <QThreadPool>

#include "modulename.h"

#define DEFINE_LOG_MODULE_NAME  static ModuleName _ModuleName(__FILE__)

#define LOG(m)                  Logger::GetInstance().WriteLog(Logger::LOG,        _ModuleName.Get(), m)
#define LOG_DEBUG(m)            Logger::GetInstance().WriteLog(Logger::LOG_DEBUG,  _ModuleName.Get(), m)
#define LOG_ERROR(m)            Logger::GetInstance().WriteLog(Logger::LOG_ERRORS, _ModuleName.Get(), m)

class LoggerWriter : public QRunnable
{
public:
	explicit LoggerWriter(QString &path, QString &message, QMutex *mutex);

private:
	void run();

private:
	QString _Message;
	QString _File_Path;
	QMutex  *_Mutex;
};

class Logger : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(Logger)

public:
	typedef enum
	{
		LOG,
		LOG_ERRORS,
		LOG_DEBUG,

		LOG_LEVELS_NUMBER
	} LOG_LEVEL;

	// Signals and Slots
public slots:

signals:

	// Functions
public:
	static Logger &GetInstance();
	void Initialize(LOG_LEVEL level);

	void WriteLog(LOG_LEVEL level, QString &module, QString message);

	void End(void);

private:
	explicit Logger(QObject *parent = nullptr);
	~Logger();

	// Variables
private:
	QString   _File_Path;
	LOG_LEVEL _Level;
	QMutex    _Mutex;

	static const char *const _Level_String[];
};

#endif // LOGGER_H
