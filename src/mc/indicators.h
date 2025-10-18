/**
 * @file    indicators.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for system indicators (such as LED) handling
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup mc Monitoring & Control
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

extern void InitIndicators(void);
extern void LEDStatToggle(void);
extern void LEDErrorOn(void);

#endif /* INDICATORS_H */

/**
 * @}
 * @}
 * @}
 */