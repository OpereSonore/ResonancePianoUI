#ifndef GLOBAL_H
#define GLOBAL_H

/*********************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "fatalerror.h"

#include <QDebug>
#include <QObject>

/*********************************************************************************************************************/

#define UNDEFINED      -2

#define _DEF_CAT_2(a, b)                a b
#define DEF_CAT_2(a, b)                 _DEF_CAT_2(a, b)

#define _DEF_CAT_3(a, b, c)             a b c
#define DEF_CAT_3(a, b, c)              _DEF_CAT_3(a, b, c)

#define _DEF_CAT_4(a, b, c, d)          a b c d
#define DEF_CAT_4(a, b, c, d)           _DEF_CAT_4(a, b, c, d)

#define _DEF_CAT_5(a, b, c, d, e)       a b c d e
#define DEF_CAT_5(a, b, c, d, e)        _DEF_CAT_5(a, b, c, d, e)

/*********************************************************************************************************************/

#define ARRAY_SIZE(a)               (sizeof(a) / sizeof(a[0]))
#define ROUND(x, type)              ((type) (((x) >= 0) ? ((x) + 0.5f) : ((x) - 0.5f)))

#define RDIV_UU(a, b)               (((a) + ((b) / 2)) / (b))
#define RDIV_SU(a, b)               ((((a) >= 0) ? ((a) + ((b) / 2)) : ((a) - ((b) / 2))) / (b))

#define RUPDIV_UU(a, b)		        (((a) + (b) - 1) / (b))

#define ABS(a)                      (((a) >= 0) ? (a) : -(a))

#define MIN(a, b)                   (((a) < (b)) ? (a) : (b))
#define MAX(a, b)                   (((a) > (b)) ? (a) : (b))
#define INTERNAL(a, b1, b2)         (((b1) <= (a)) && ((a) <= (b2)))

// Funzioni ottimizzare per il passaggio BIG/LITTLE ENDIAN
#define SWAP_32(x)     x = \
                           ((((x) & 0xFF000000) >> 24) | (((x) & 0x00FF0000) >>  8) | \
                            (((x) & 0x0000FF00) <<  8) | (((x) & 0x000000FF) << 24))

#define _SWAP_32(x)    ((((x) & 0xFF000000) >> 24) | (((x) & 0x00FF0000) >>  8) | \
                        (((x) & 0x0000FF00) <<  8) | (((x) & 0x000000FF) << 24))

#define SWAP_16(x)     x = \
                           ((((x) >> 8) & 0xFF) | (((x) & 0xFF) << 8))

#define _SWAP_16(x)    ((((x) >> 8) & 0xFF) | (((x) & 0xFF) << 8))

/******************************************************************************/

#define UNREFERENCED_PAR(P)     \
	/*lint -save -e527 -e578 -e530 */ \
	{                                 \
		(P) = (P);                    \
	}                                 \
	/*lint -restore */

/******************************************************************************/

#endif // GLOBAL_H
