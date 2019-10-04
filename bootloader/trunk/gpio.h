#ifndef GPIO_H
#define GPIO_H

/******************************************************************************/

#include "global.h"

/******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#if defined Q_OS_UNIX
#include <unistd.h>
#endif

/******************************************************************************/

class Gpio
{
public:
	typedef enum
	{
		IN_DIR,
		OUT_DIR,
	} DIRECTION;

	// Functions
public:
	static Gpio &GetInstance(void);

	~Gpio(void);

	bool Export(int pin);
	bool Unexport(int pin);
	bool Direction(int pin, DIRECTION dir);
	bool Read(int pin);
	bool Write(int pin, bool value);
};

#endif // GPIO_H
