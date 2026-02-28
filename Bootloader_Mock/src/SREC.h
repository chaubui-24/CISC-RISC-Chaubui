/*
 * =========================================================================
 * SREC.h
 * Motorola S-Record Parser Definitions
 * =========================================================================
 */

#ifndef SREC_H_
#define SREC_H_

/*
 * =========================================================================
 * Includes
 * =========================================================================
 */
#include <stdint.h>

/*
 * =========================================================================
 * Macros & Defines
 * =========================================================================
 */
#define SREC_DATA_MAX_SIZE      (64U)

/*
 * =========================================================================
 * Data Types
 * =========================================================================
 */

/* Possible return status after parsing a line */
typedef enum
{
    SREC_OK             = 0U,
    SREC_ERROR_TYPE     = 1U,
    SREC_ERROR_CHECKSUM = 2U,
    SREC_ERROR_FORMAT   = 3U,
    SREC_ERROR_NULL_PTR = 4U
} SREC_Status_t;

/* Info required from a SREC line */
typedef struct
{
    uint32_t address;
    uint8_t  data[SREC_DATA_MAX_SIZE];
    uint8_t  data_length;
    uint8_t  type;
} SREC_Record_t;

/*
 * =========================================================================
 * API Functions
 * =========================================================================
 */
uint8_t         SREC_HexToByte(const uint8_t *hex);
SREC_Status_t   SREC_ParseLine(const uint8_t *line, SREC_Record_t *result);

#endif /* SREC_H_ */
