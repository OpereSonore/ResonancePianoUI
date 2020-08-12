

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
	QTextStream(stdout) << QString("ASSERT(false) in %1 at line %2 %3").arg(file).arg(line).arg(message);
}

/*****************************************************************************/
void Assert::FatalErrorException(const char *file, const int line, const char *message)
/*****************************************************************************/
{
	QTextStream(stdout) << QString("%1 ASSERT_EXCEPTION(%2) in %3 at line %4\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz :")).arg(message).arg(file).arg(line);
}
