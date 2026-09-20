/**
 * @file    bsp.h
 * @author  Merlin Kooshmanian
 *
 * @copyright Copyright (c) TOLOSAT 2026
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef BSP_H
#define BSP_H

/******************************* Include Files *******************************/

#include "common_types.h"
#include "stm32f4xx_hal.h"

/***************************** Macros Definitions ****************************/

/* INTERNAL TIMERS */
#define HAL_TIMER_PERIPH        TIM4      /**< HAL timer peripheral */
#define HAL_TIMER_IRQ_NO        TIM4_IRQn /**< HAL timer interruption numero */
#define MONITORING_TIMER_REF    TIM3      /**< Monitoring timer peripheral */
#define MONITORING_TIMER_IRQ_NO TIM3_IRQn /**< Monitoring timer interruption numero */

/* WATCHDOG CONSTANTS */
#define WATCHDOG_PERIPH         IWDG /**< Internal watchdog peripheral */

/***************************** Types Definitions *****************************/

/*************************** Functions Declarations **************************/

/**
 * @fn      SystemClock_Config
 * @brief   System Clock Configuration
 */
returnCode_t SystemClock_Config(void);

/**
 * @fn      BSPLateInit(void)
 * @brief   This function will initialise some BSP specifique peripherals
 * @return  Nothing
 */
void BSPLateInit(void);

#endif /* BSP_H */
