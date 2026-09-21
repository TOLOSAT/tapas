/**
 * @file    wdg.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for watchdog handling
 *
 * @copyright Copyright (c) TOLOSAT 2026
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup platform Platform
 * @{
 * @defgroup watchdog Watchdog
 * @brief Provides system watchdog handling interface.
 * @{
 */

#ifndef WDG_H
#define WDG_H

/******************************* Include Files *******************************/

#include "kernel_autoconf.h"
#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

/**
 * @fn      InitSYSWDG(void)
 * @brief   Enables System Watchdog
 * @return  Nothing
 */
extern void InitSYSWDG(void);

/**
 * @fn              SYSWDGMain(void)
 * @brief           Main of the SYSWDG task
 */
extern void SYSWDGMain(void);

#endif /* WDG_H */

/**
 * @}
 * @}
 * @}
 */