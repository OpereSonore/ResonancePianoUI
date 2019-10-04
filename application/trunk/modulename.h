#ifndef MODULENAME_H
#define MODULENAME_H

/******************************************************************************/

#include "global.h"

/******************************************************************************/

class ModuleName
{
// Functions
public:
	~ModuleName(void);

	ModuleName(const char *file);

	QString &Get(void);

private:
	ModuleName(void);

protected:

// Variables
public:

private:
	QString _Module_Name;

protected:

};

#endif // MODULENAME_H
