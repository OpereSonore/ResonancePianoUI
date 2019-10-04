#ifndef _PL_NET_STRUCTS_H
#define _PL_NET_STRUCTS_H

/*****************************************************************************/
// *INDENT-ON*
/*****************************************************************************/

#include <stdint.h>

/*****************************************************************************/

#define PL_NET_CRC_16_INIT_VALUE          0x1177

/*****************************************************************************/

#pragma pack(1)

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/

#define PL_NET_STX            0x02

#define PL_NET_VERSION        0x00

/*****************************************************************************/

#define PL_NET_RESULT_OK      0
#define PL_NET_RESULT_ERROR   1

/*****************************************************************************/

enum
{
	PL_NET_RESET_CMD_ID = 0x00,

	PL_NET_UPGRADE_CMD_ID,

	PL_NET_GET_INFO_CMD_ID,

	PL_NET_GET_EQ_CMD_ID,
	PL_NET_SET_EQ_CMD_ID,

	PL_NET_GET_STATUS_CMD_ID,

	PL_NET_COMMANDS_NUMBER

};

#define PL_NET_RSP_ID_OFFSET      0x80

enum
{
	PL_NET_RESET_RSP_ID = PL_NET_RSP_ID_OFFSET,

	PL_NET_UPGRADE_RSP_ID,

	PL_NET_GET_INFO_RSP_ID,

	PL_NET_GET_EQ_RSP_ID,
	PL_NET_SET_EQ_RSP_ID,

	PL_NET_GET_STATUS_RSP_ID,

	PL_NET_RESPONSES_MAX_ID
};

/*****************************************************************************/

typedef struct
{
	float output_gain;

	struct
	{
		uint8_t enable : 1;
		uint8_t __free : 7;

		float gain; // lineare
		float low;  // lineare
		float mid;  // lineare
		float high; // lineare
	} mic;

	struct
	{
		uint8_t enable : 1;
		uint8_t __free : 7;

		float gain; // lineare
		float low;  // lineare
		float mid;  // lineare
		float high; // lineare
	} xlr;

	struct
	{
		uint8_t enable : 1;
		uint8_t __free : 7;

		float gain; // lineare
		float low;  // lineare
		float mid;  // lineare
		float high; // lineare
	} rca;

	struct
	{
		uint8_t enable : 1;
		uint8_t __free : 7;

		float gain; // lineare
		float low;  // lineare
		float mid;  // lineare
		float high; // lineare
	} mp3;

	struct
	{
		uint8_t enable : 1;
		uint8_t __free : 7;

		float gain; // lineare
		float low;  // lineare
		float mid;  // lineare
		float high; // lineare
	} bt;
} PL_NET_EQ;

typedef struct
{
	uint32_t dsp_init_error           : 1;
	uint32_t dsp_error                : 1;
	uint32_t ext_flash_warning        : 1;
	uint32_t ampli_lf_fault_warning   : 1;
	uint32_t ampli_mf_fault_warning   : 1;
	uint32_t ampli_hf_fault_warning   : 1;
	uint32_t __free                   : 26;
} PL_NET_SIGNALS_BITS;

/*****************************************************************************/

#define PL_NET_RESULT_OK        0

/*****************************************************************************/

typedef struct
{
	uint8_t  stx;
	uint8_t  version;
	uint16_t length;

	uint16_t body_crc;
	uint16_t crc;
} PL_NET_HEADER;

/*****************************************************************************/
/* PL_NET_RESET_CMD_ID                                                       */
/*****************************************************************************/

typedef struct
{
	uint8_t cmd_id;
} PL_NET_RESET_COMMAND;

typedef struct
{
	uint8_t rsp_id;
	uint8_t result; // == 0 ok, != 0 error
} PL_NET_RESET_RESPONSE;

/*****************************************************************************/
/* PL_NET_UPGRADE_CMD_ID                                                     */
/*****************************************************************************/

#define PL_NET_PAGE_SIZE                    256
typedef struct
{
	uint8_t  cmd_id;
	uint32_t page_id;
	uint8_t  page[PL_NET_PAGE_SIZE];
} PL_NET_UPGRADE_COMMAND;

typedef struct
{
	uint8_t  rsp_id;
	uint8_t  result; // == 0 ok, != 0 error
	uint32_t page_id;
} PL_NET_UPGRADE_RESPONSE;

/*****************************************************************************/
/* PL_NET_GET_INFO_CMD_ID                                                    */
/*****************************************************************************/

typedef struct
{
	uint8_t cmd_id;
} PL_NET_GET_INFO_COMMAND;

typedef struct
{
	uint8_t rsp_id;
	uint8_t result; // == 0 ok, != 0 error
	char    firmware_name[16];
	char    firmware_version[10];
} PL_NET_GET_INFO_RESPONSE;

/*****************************************************************************/
/* PL_NET_GET_EQ_CMD_ID                                                    */
/*****************************************************************************/

typedef struct
{
	uint8_t cmd_id;
} PL_NET_GET_EQ_COMMAND;

typedef struct
{
	uint8_t    rsp_id;
	uint8_t    result; // == 0 ok, != 0 error
	PL_NET_EQ  eq;
} PL_NET_GET_EQ_RESPONSE;

/*****************************************************************************/
/* PL_NET_SET_EQ_CMD_ID                                                    */
/*****************************************************************************/

typedef struct
{
	uint8_t    cmd_id;
	PL_NET_EQ  eq;
} PL_NET_SET_EQ_COMMAND;

typedef struct
{
	uint8_t  rsp_id;
	uint8_t  result; // == 0 ok, != 0 error
} PL_NET_SET_EQ_RESPONSE;

/*****************************************************************************/
/* PL_NET_GET_STATUS_CMD_ID                                                  */
/*****************************************************************************/

typedef struct
{
	uint8_t cmd_id;
} PL_NET_GET_STATUS_COMMAND;

typedef struct
{
	uint8_t             rsp_id;
	uint8_t             result; // == 0 ok, != 0 error
	PL_NET_SIGNALS_BITS signals_bits;
} PL_NET_GET_STATUS_RESPONSE;

/*****************************************************************************/

typedef union
{
	uint8_t                        cmd_id;

	PL_NET_RESET_COMMAND           reset;
	PL_NET_UPGRADE_COMMAND         upgrade;
	PL_NET_GET_INFO_COMMAND        get_info;
	PL_NET_GET_EQ_COMMAND          get_eq;
	PL_NET_SET_EQ_COMMAND          set_eq;
	PL_NET_GET_STATUS_COMMAND      get_status;
} PL_NET_COMMANDS_BODY;

typedef struct
{
	PL_NET_HEADER                  header;
	PL_NET_COMMANDS_BODY           body;
} PL_NET_COMMAND;

/*****************************************************************************/

typedef union
{
	uint8_t                        rsp_id;

	PL_NET_RESET_RESPONSE          reset;
	PL_NET_UPGRADE_RESPONSE        upgrade;
	PL_NET_GET_INFO_RESPONSE       get_info;
	PL_NET_GET_EQ_RESPONSE         get_eq;
	PL_NET_SET_EQ_RESPONSE         set_eq;
	PL_NET_GET_STATUS_RESPONSE     get_status;
} PL_NET_RESPONSES_BODY;

typedef struct
{
	PL_NET_HEADER                  header;
	PL_NET_RESPONSES_BODY          body;
} PL_NET_RESPONSE;

#ifdef __cplusplus
}
#endif

#pragma pack()

#endif

