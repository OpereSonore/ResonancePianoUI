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
