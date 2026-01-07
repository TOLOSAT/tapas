/**
 * @file    wdg.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for watchdog handling
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup platform Platform
 * @{
 * @defgroup wdg Watchdog
 * @brief Provides system watchdog handling interface.
 * @{
 */

#ifndef WDG_H
#define WDG_H

/******************************* Include Files *******************************/

#include "autoconf.h"
#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void InitSYSWDG(void);
extern void SYSWDGMain(void);

#endif /* WDG_H */

/**
 * @}
 * @}
 * @}
 */