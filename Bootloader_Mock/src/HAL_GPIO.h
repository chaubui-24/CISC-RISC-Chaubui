/*
 * HAL_GPIO.h
 *
 * Author: Admin
 */

#ifndef HAL_GPIO_H_
#define HAL_GPIO_H_

/*
 * =========================================================================
 * Includes
 * =========================================================================
 */
#include <stdint.h>

/*
 * =========================================================================
 * Instance 0 (GPIO0): GPIO for PORT D
 * =========================================================================
 */
void    HAL_GPIO0_ClockInit       (void);
void    HAL_GPIO0_PinInit         (uint32_t pinNumber);
void    HAL_GPIO0_PinDataDirection(uint32_t pinNumber, uint8_t value);
void    HAL_GPIO0_WritePin        (uint32_t pinNumber, uint8_t value);
uint8_t HAL_GPIO0_ReadPin         (uint32_t pinNumber);

/*
 * =========================================================================
 * Instance 1 (GPIO1): GPIO for PORT C
 * =========================================================================
 */
void    HAL_GPIO1_ClockInit       (void);
void    HAL_GPIO1_PinInit         (uint32_t pinNumber);
void    HAL_GPIO1_PinDataDirection(uint32_t pinNumber, uint8_t value);
void    HAL_GPIO1_WritePin        (uint32_t pinNumber, uint8_t value);
uint8_t HAL_GPIO1_ReadPin         (uint32_t pinNumber);

#endif /* HAL_GPIO_H_ */
