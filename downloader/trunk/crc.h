#ifndef _CRC_H
#define _CRC_H

/******************************************************************************/

#include "global.h"

/******************************************************************************/

class Crc
{
// Functions
public:
	~Crc(void);

	static Crc &GetInstance(void);

	uint16_t Crc16_1021(const void *data, size_t length, uint16_t start_value);
	uint16_t Crc16_C0C1(const void *data, size_t length, uint16_t start_value);

private:
	Crc(void);
	Crc (const Crc &) = delete;
	Crc &operator= (const Crc &) = delete;
	Crc (Crc &&) = delete;
	Crc &operator=(Crc &&) = delete;

protected:

// Variables
public:

private:
	static const uint16_t Crc16_1021_Table[];
	static const uint16_t Crc16_C0C1_Table[];

protected:

};

#endif

