/*
 * =========================================================================
 * Queue.c
 * Circular Queue implementation for SREC lines
 * =========================================================================
 */

#include "Queue.h"
#include <stddef.h>

/*
 * =========================================================================
 * Private Functions
 * =========================================================================
 */

/* An implementation of strncpy to avoid using it directly from string.h */
static void Queue_CopyString(uint8_t *dest, const uint8_t *src, uint8_t max_len)
{
    uint8_t i = 0U;

    if ((dest != NULL) && (src != NULL))
    {
        /* Copy until max_len - 1 to ensure a space for '\0' at the end */
        while ((i < (max_len - 1U)) && (src[i] != (uint8_t)'\0'))
        {
            dest[i] = src[i];
            i++;
        }
        /* End the string with '\0' */
        dest[i] = (uint8_t)'\0';
    }
}

/*
 * =========================================================================
 * API Implementation
 * =========================================================================
 */

/* Initialize a queue by setting all the fields to 0 */
void Queue_Init(SREC_Queue_t *q)
{
    if (q != NULL)
    {
        q->head = 0U;
        q->tail = 0U;
        q->count = 0U;
    }
}

/* Check if queue is full */
Queue_Status_t Queue_IsFull(const SREC_Queue_t *q)
{
    Queue_Status_t full_status = QUEUE_STATE_NOT_FULL;

    if (q != NULL)
    {
        if (q->count == QUEUE_MAX_ELEMENTS)
        {
            full_status = QUEUE_STATE_FULL;
        }
    }

    return full_status;
}

/* Push a line into queue */
Queue_Status_t Queue_Push(SREC_Queue_t *q, const uint8_t *line)
{
    Queue_Status_t push_status = QUEUE_ERROR;

    /* Check pointers and make sure queue is not null */
    if ((q != NULL) && (line != NULL))
    {
        if (q->count < QUEUE_MAX_ELEMENTS)
        {
            Queue_CopyString(q->data[q->tail], line, SREC_LINE_MAX_LEN);

            /* Calculate new tail with explicit casting to handle wrap-around */
            q->tail = (uint8_t)((q->tail + 1U) % QUEUE_MAX_ELEMENTS);
            q->count++;

            push_status = QUEUE_OK;
        }
    }

    return push_status;
}

/* Pop a line out of queue */
Queue_Status_t Queue_Pop(SREC_Queue_t *q, uint8_t *dest)
{
    Queue_Status_t pop_status = QUEUE_ERROR;

    if ((q != NULL) && (dest != NULL))
    {
        if (q->count > 0U)
        {
            /* Copy data from queue to destination buffer */
            Queue_CopyString(dest, q->data[q->head], SREC_LINE_MAX_LEN);

            /* Calculate new head with explicit casting to handle wrap-around */
            q->head = (uint8_t)((q->head + 1U) % QUEUE_MAX_ELEMENTS);
            q->count--;

            pop_status = QUEUE_OK;
        }
    }

    return pop_status;
}
