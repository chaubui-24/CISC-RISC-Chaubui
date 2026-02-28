/*
 * =========================================================================
 * HAL_USART.h
 *
 * Author: Admin
 * =========================================================================
 */

#ifndef HAL_USART_H_
#define HAL_USART_H_

/*
 * =========================================================================
 * Includes
 * =========================================================================
 */
#include <stdint.h>
#include <stdbool.h>
#include "QUEUE.h"

/*
 * =========================================================================
 * Macros & Defines
 * =========================================================================
 */
#define BUFFER_SIZE     100U

/*
 * =========================================================================
 * API Functions
 * =========================================================================
 */
void            HAL_USART_ClockInit       (void);
void            HAL_USART_PinInit         (void);
void            HAL_USART_LPUART_Init     (void);
void            HAL_USART_sendChar        (uint8_t c);
void            HAL_USART_sendString      (const uint8_t *string);
Queue_Status_t  HAL_USART_GetLineFromQueue(uint8_t *buffer);

#endif /* HAL_USART_H_ */
