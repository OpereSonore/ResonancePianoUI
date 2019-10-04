/******************************************************************************/
// GNU Lesser General Public License Usage
// This file may be used under the terms of the GNU Lesser
// General Public License version 2.1 as published by the Free Software
// Foundation and appearing in the file LICENSE.LGPL included in the
// packaging of this file.  Please review the following information to
// ensure the GNU Lesser General Public License version 2.1 requirements
// will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
/******************************************************************************/

#include "modulename.h"

/******************************************************************************/
ModuleName::ModuleName(void)
/******************************************************************************/
{
}

/******************************************************************************/
ModuleName::ModuleName (const char *file)
/******************************************************************************/
{
	QFileInfo file_info(file);

	_Module_Name = file_info.fileName().section(".", 0, 0);
}

/******************************************************************************/
ModuleName::~ModuleName (void)
/******************************************************************************/
{
}

/******************************************************************************/
QString &ModuleName::Get(void)
/******************************************************************************/
{
	return (_Module_Name);
}

