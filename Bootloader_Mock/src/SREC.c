/*
 * =========================================================================
 * SREC.c
 * Motorola S-Record Parser Implementation
 * =========================================================================
 */

#include "SREC.h"
#include <stdint.h>
#include <stddef.h>

/*
 * =========================================================================
 * Helper Functions
 * =========================================================================
 */

/* Turn two Hex characters into a byte of data */
uint8_t SREC_HexToByte(const uint8_t *hex)
{
    uint8_t result = 0U;
    uint8_t i      = 0U;

    if (hex != NULL)
    {
        for (i = 0U; i < 2U; i++)
        {
            /* Type cast and Left shift 4 bit */
            result = (uint8_t)((uint8_t)(result << 4U) & 0xF0U);

            if ((hex[i] >= (uint8_t)'0') && (hex[i] <= (uint8_t)'9'))
            {
                result = (uint8_t)(result | (uint8_t)(hex[i] - (uint8_t)'0'));
            }
            else if ((hex[i] >= (uint8_t)'A') && (hex[i] <= (uint8_t)'F'))
            {
                result = (uint8_t)(result | (uint8_t)((uint8_t)(hex[i] - (uint8_t)'A') + 10U));
            }
            else if ((hex[i] >= (uint8_t)'a') && (hex[i] <= (uint8_t)'f'))
            {
                result = (uint8_t)(result | (uint8_t)((uint8_t)(hex[i] - (uint8_t)'a') + 10U));
            }
            else
            {
                /* Do nothing */
            }
        }
    }

    return result;
}

/*
 * =========================================================================
 * API Implementation
 * =========================================================================
 */

/* Parse a SREC line in order to get the desirable data */
SREC_Status_t SREC_ParseLine(const uint8_t* line, SREC_Record_t *result)
{
    SREC_Status_t status         = SREC_OK;
    uint8_t       address_length = 0U;
    uint8_t       bytecount      = 0U;
    uint8_t       checksum       = 0U;
    uint8_t       i              = 0U;
    uint8_t       b              = 0U;

    if ((line == NULL) || (result == NULL))
    {
        status = SREC_ERROR_NULL_PTR;
    }
    else if (line[0] != (uint8_t)'S')
    {
        status = SREC_ERROR_TYPE;
    }
    else
    {
        /* Parse Record Type */
        result->type = (uint8_t)(line[1] - (uint8_t)'0');

        /* Determine Address Length based on Record Type */
        if ((result->type == 0U) || (result->type == 1U) || (result->type == 9U))
        {
            address_length = 2U;
        }
        else if ((result->type == 2U) || (result->type == 8U))
        {
            address_length = 3U;
        }
        else if ((result->type == 3U) || (result->type == 7U))
        {
            address_length = 4U;
        }
        else
        {
            status = SREC_ERROR_FORMAT;
        }

        /* Proceed if type is valid */
        if (status == SREC_OK)
        {
            uint8_t record_checksum = 0U; /* Declaration moved to top of block */

            bytecount       = SREC_HexToByte(&line[2]);
            checksum        = bytecount;
            result->address = 0U;

            /* Parse Address */
            for (i = 0U; i < address_length; i++)
            {
                b = SREC_HexToByte(&line[4U + (i * 2U)]);
                result->address = (uint32_t)((result->address << 8U) | (uint32_t)b);
                checksum = (uint8_t)(checksum + b);
            }

            /* Parse Data */
            result->data_length = (uint8_t)((uint8_t)(bytecount - address_length) - 1U);

            for (i = 0U; i < result->data_length; i++)
            {
                b = SREC_HexToByte(&line[4U + (address_length * 2U) + (i * 2U)]);
                result->data[i] = b;
                checksum = (uint8_t)(checksum + b);
            }

            /* Verify Checksum */
            /* Checksum is at the end of the line (after Type, ByteCount, Address, and Data) */
            record_checksum = SREC_HexToByte(&line[4U + (address_length * 2U) + (result->data_length * 2U)]);

            /* The calculated checksum is the 1's complement of the sum of the bytes */
            if (((uint8_t)(checksum ^ 0xFFU)) != record_checksum)
            {
                status = SREC_ERROR_CHECKSUM;
            }
        }
    }

    return status;
}
