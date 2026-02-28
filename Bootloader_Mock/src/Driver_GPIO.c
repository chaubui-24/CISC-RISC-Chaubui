/*
 * Copyright (c) 2023 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Driver_GPIO.h"
#include "HAL_GPIO.h"
#include "Driver_Common.h"

/* Pin mapping */
#define GPIO_MAX_PINS           32U
#define PIN_IS_AVAILABLE(n)     ((n) < GPIO_MAX_PINS)

/* =========================================================================*/
/* INSTANCE 0 (GPIO0): GPIO FOR PORT D                                      */
/* =========================================================================*/

/* Setup GPIO0 Interface */
static int32_t GPIO0_Setup (ARM_GPIO_Pin_t pin, ARM_GPIO_SignalEvent_t cb_event)
{
    int32_t result = ARM_DRIVER_OK;

    if (PIN_IS_AVAILABLE(pin))
    {
        HAL_GPIO0_ClockInit();
        HAL_GPIO0_PinInit(pin);
    }
    else
    {
        result = ARM_GPIO_ERROR_PIN;
    }

    return result;
}

/* Set GPIO Direction */
static int32_t GPIO0_SetDirection (ARM_GPIO_Pin_t pin, ARM_GPIO_DIRECTION direction)
{
    int32_t result = ARM_DRIVER_OK;

    if (PIN_IS_AVAILABLE(pin))
    {
        switch (direction)
        {
            case ARM_GPIO_INPUT:
                HAL_GPIO0_PinDataDirection(pin, 0);
                break;
            case ARM_GPIO_OUTPUT:
                HAL_GPIO0_PinDataDirection(pin, 1);
                break;
            default:
                result = ARM_DRIVER_ERROR_PARAMETER;
                break;
        }
    }
    else
    {
        result = ARM_GPIO_ERROR_PIN;
    }

    return result;
}

/* Set GPIO Output Mode */
static int32_t GPIO0_SetOutputMode (ARM_GPIO_Pin_t pin, ARM_GPIO_OUTPUT_MODE mode)
{
    return ARM_DRIVER_ERROR_UNSUPPORTED;
}

/* Set GPIO Pull Resistor */
static int32_t GPIO0_SetPullResistor (ARM_GPIO_Pin_t pin, ARM_GPIO_PULL_RESISTOR resistor)
{
    return ARM_DRIVER_ERROR_UNSUPPORTED;
}

/* Set GPIO Event Trigger */
static int32_t GPIO0_SetEventTrigger (ARM_GPIO_Pin_t pin, ARM_GPIO_EVENT_TRIGGER trigger)
{
    return ARM_DRIVER_ERROR_UNSUPPORTED;
}

/* Set GPIO Output Level */
static void GPIO0_SetOutput (ARM_GPIO_Pin_t pin, uint32_t val)
{
    if (PIN_IS_AVAILABLE(pin))
    {
        HAL_GPIO0_WritePin(pin, val);
    }
}

/* Get GPIO Input Level */
static uint32_t GPIO0_GetInput (ARM_GPIO_Pin_t pin)
{
    uint32_t result = 0;

    if (PIN_IS_AVAILABLE(pin))
    {
        result = HAL_GPIO0_ReadPin(pin);
    }

    return result;
}

/* GPIO0 Driver access structure */
ARM_DRIVER_GPIO Driver_GPIO0 =
{
    GPIO0_Setup,
    GPIO0_SetDirection,
    GPIO0_SetOutputMode,
    GPIO0_SetPullResistor,
    GPIO0_SetEventTrigger,
    GPIO0_SetOutput,
    GPIO0_GetInput
};

/* =========================================================================*/
/* INSTANCE 1 (GPIO1): GPIO FOR PORT C                                      */
/* =========================================================================*/

/* Setup GPIO1 Interface */
static int32_t GPIO1_Setup (ARM_GPIO_Pin_t pin, ARM_GPIO_SignalEvent_t cb_event)
{
    int32_t result = ARM_DRIVER_OK;

    if (PIN_IS_AVAILABLE(pin))
    {
        HAL_GPIO1_ClockInit();
        HAL_GPIO1_PinInit(pin);
    }
    else
    {
        result = ARM_GPIO_ERROR_PIN;
    }

    return result;
}

/* Set GPIO Direction */
static int32_t GPIO1_SetDirection (ARM_GPIO_Pin_t pin, ARM_GPIO_DIRECTION direction)
{
    int32_t result = ARM_DRIVER_OK;

    if (PIN_IS_AVAILABLE(pin))
    {
        switch (direction)
        {
            case ARM_GPIO_INPUT:
                HAL_GPIO1_PinDataDirection(pin, 0);
                break;
            case ARM_GPIO_OUTPUT:
                HAL_GPIO1_PinDataDirection(pin, 1);
                break;
            default:
                result = ARM_DRIVER_ERROR_PARAMETER;
                break;
        }
    }
    else
    {
        result = ARM_GPIO_ERROR_PIN;
    }

    return result;
}

/* Set GPIO Output Mode */
static int32_t GPIO1_SetOutputMode (ARM_GPIO_Pin_t pin, ARM_GPIO_OUTPUT_MODE mode)
{
    return ARM_DRIVER_ERROR_UNSUPPORTED;
}

/* Set GPIO Pull Resistor */
static int32_t GPIO1_SetPullResistor (ARM_GPIO_Pin_t pin, ARM_GPIO_PULL_RESISTOR resistor)
{
    return ARM_DRIVER_ERROR_UNSUPPORTED;
}

/* Set GPIO Event Trigger */
static int32_t GPIO1_SetEventTrigger (ARM_GPIO_Pin_t pin, ARM_GPIO_EVENT_TRIGGER trigger)
{
    return ARM_DRIVER_ERROR_UNSUPPORTED;
}

/* Set GPIO Output Level */
static void GPIO1_SetOutput (ARM_GPIO_Pin_t pin, uint32_t val)
{
    if (PIN_IS_AVAILABLE(pin))
    {
        HAL_GPIO1_WritePin(pin, val);
    }
}

/* Get GPIO Input Level */
static uint32_t GPIO1_GetInput (ARM_GPIO_Pin_t pin)
{
    uint32_t result = 0;

    if (PIN_IS_AVAILABLE(pin))
    {
        result = HAL_GPIO1_ReadPin(pin);
    }

    return result;
}

/* GPIO1 Driver access structure */
ARM_DRIVER_GPIO Driver_GPIO1 =
{
    GPIO1_Setup,
    GPIO1_SetDirection,
    GPIO1_SetOutputMode,
    GPIO1_SetPullResistor,
    GPIO1_SetEventTrigger,
    GPIO1_SetOutput,
    GPIO1_GetInput
};
