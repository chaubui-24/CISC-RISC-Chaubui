/*
 * HAL_GPIO.c
 *
 * Author: Admin
 */

#include "HAL_GPIO.h"
#include <stdint.h>
#include <S32K144.h>

/*
 * =========================================================================
 * INSTANCE 0 (GPIO0): GPIO for PORT D
 * =========================================================================
 */

void HAL_GPIO0_ClockInit(void)
{
    IP_PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;
}

void HAL_GPIO0_PinInit(uint32_t pinNumber)
{
    IP_PORTD->PCR[pinNumber] &= ~PORT_PCR_MUX(0x07UL); /* Clear MUX bits */
    IP_PORTD->PCR[pinNumber] |= PORT_PCR_MUX(1UL);     /* Set MUX to GPIO */
}

void HAL_GPIO0_PinDataDirection(uint32_t pinNumber, uint8_t value)
{
    if (value == 1U)
    {
        /* OUTPUT */
        IP_PTD->PDDR |= (1UL << pinNumber);
    }
    else
    {
        /* INPUT */
        IP_PTD->PDDR &= ~(1UL << pinNumber);
    }
}

void HAL_GPIO0_WritePin(uint32_t pinNumber, uint8_t value)
{
    if (value == 1U)
    {
        /* Set Pin Output */
        IP_PTD->PSOR |= (1UL << pinNumber);
    }
    else
    {
        /* Clear Pin Output */
        IP_PTD->PCOR |= (1UL << pinNumber);
    }
}

uint8_t HAL_GPIO0_ReadPin(uint32_t pinNumber)
{
    return ((IP_PTD->PDIR & (1UL << pinNumber)) != 0U) ? 1U : 0U;
}

/*
 * =========================================================================
 * INSTANCE 1 (GPIO1): GPIO for PORT C
 * =========================================================================
 */

void HAL_GPIO1_ClockInit(void)
{
    IP_PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_MASK;
}

void HAL_GPIO1_PinInit(uint32_t pinNumber)
{
    IP_PORTC->PCR[pinNumber] &= ~PORT_PCR_MUX(0x07UL); /* Clear MUX bits */
    IP_PORTC->PCR[pinNumber] |= PORT_PCR_MUX(1UL);     /* Set MUX to GPIO */
}

void HAL_GPIO1_PinDataDirection(uint32_t pinNumber, uint8_t value)
{
    if (value == 1U)
    {
        /* OUTPUT */
        IP_PTC->PDDR |= (1UL << pinNumber);
    }
    else
    {
        /* INPUT */
        IP_PTC->PDDR &= ~(1UL << pinNumber);
    }
}

void HAL_GPIO1_WritePin(uint32_t pinNumber, uint8_t value)
{
    if (value == 1U)
    {
        /* Set Pin Output */
        IP_PTC->PSOR |= (1UL << pinNumber);
    }
    else
    {
        /* Clear Pin Output */
        IP_PTC->PCOR |= (1UL << pinNumber);
    }
}

uint8_t HAL_GPIO1_ReadPin(uint32_t pinNumber)
{
    return ((IP_PTC->PDIR & (1UL << pinNumber)) != 0U) ? 1U : 0U;
}
