/**
 * @file    drv_tim.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for timers and ticks for HAL
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 * @defgroup drv_tim Timers Driver
 * @brief Abstraction layer for internal timers.
 * @{
 */

#ifndef DRV_TIM_H
#define DRV_TIM_H

/******************************* Include Files *******************************/

#include "drv/common.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** @brief Timer instance type definition */
typedef TIM_HandleTypeDef timerInst_t;

/** @brief Timer peripheral type definition */
typedef TIM_TypeDef timerPeriph_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

/**
 * @brief Monitoring Timer Initialization Function
 */
extern returnCode_t InitMonitoringTimer(void);

/**
 * @brief This function start Monitoring Timer
 */
extern void StartMonitoringTimer(void);

/**
 * @brief This function get the current value of the monitoring tick
 */
extern uint64_t GetMonitoringTick(void);

#endif /* DRV_TIM_H */

/**
 * @}
 * @}
 * @}
 */