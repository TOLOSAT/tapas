/**
 * @file    indicators.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for system indicators (such as LED) handling
 *
 * @copyright Copyright (c) TOLOSAT 2026
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup monitoring Monitoring
 * @{
 * @defgroup indicators Indicators
 * @brief Provides system indicators handling.
 * @{
 */

#ifndef INDICATORS_H
#define INDICATORS_H

/******************************* Include Files *******************************/

#include "autoconf.h"
#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

/**
 * @fn      InitIndicators(void)
 * @brief   Function that initialises the system leds (used for debug)
 * @return  Nothing
 */
extern void InitIndicators(void);

/**
 * @fn      LEDStatToggle(void)
 * @brief   Function that toggles the LED stat
 * @return  Nothing
 */
extern void LEDStatToggle(void);

/**
 * @fn      LEDErrorOn(void)
 * @brief   Function that turns on the LED error (and turn off the LED stat)
 * @return  Nothing
 */
extern void LEDErrorOn(void);

#endif /* INDICATORS_H */

/**
 * @}
 * @}
 * @}
 */