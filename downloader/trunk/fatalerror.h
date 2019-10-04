#ifndef FATALERROR_H
#define FATALERROR_H

#include "global.h"

#include <QDateTime>
#include <QString>
#include <QFile>
#include <QDir>
#include <QObject>
#include <QCoreApplication>
#include <QCoreApplication>

class Assert : public QObject
{
	Q_DISABLE_COPY(Assert)

private:
	explicit Assert(QObject *parent = nullptr);
	~Assert();

public:
	static Assert &GetInstance();
	void FatalError(const char *file, const int line, const char *message);
	void FatalErrorException(const char *file, const int line, const char *message);
};

/*****************************************************************************/

#ifdef QT_DEBUG
#include "debugbreak.h"
#define ASSERT(x)\
	do{\
		if (!(x))\
		{\
			debug_break();\
			Assert::GetInstance().FatalError(__FILE__, __LINE__, (#x));\
			qApp->exit(EXIT_FAILURE);\
		}\
	}while(0)

#define ASSERT_EXCEPTION(exception)\
	do{\
		/* Remove warning C4101: 'exception' : unreferenced local variable */ \
		Q_UNUSED(exception)\
		debug_break();\
		Assert::GetInstance().FatalErrorException(__FILE__, __LINE__, exception.what());\
		qApp->exit(EXIT_FAILURE);\
	}while(0)
#else
#define ASSERT(x)\
	do{\
		if (!(x))\
		{\
			Assert::GetInstance().FatalError(__FILE__, __LINE__, (#x));\
			qApp->exit(EXIT_FAILURE);\
		}\
	}while(0)

#define ASSERT_EXCEPTION(exception)\
	do{\
		/* Remove warning C4101: 'exception' : unreferenced local variable */ \
		Q_UNUSED(exception)\
		Assert::GetInstance().FatalErrorException(__FILE__, __LINE__, exception.what());\
		qApp->exit(EXIT_FAILURE);\
	}while(0)
#endif

/*****************************************************************************/

#define STATIC_ASSERT(e)\
	do\
	{\
		enum\
		{\
			assert_static__ = 1 / (int)(e)\
		};\
	}while(0)\

/*****************************************************************************/

#endif

