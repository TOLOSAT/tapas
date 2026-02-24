/**
 * @file    console.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for Console functions
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup mc Monitoring & Control
 * @{
 * @defgroup console Console
 * @brief Interface for printing and LOGging messages.
 * @{
 */

#ifndef CONSOLE_H
#define CONSOLE_H

/******************************* Include Files *******************************/

#include "autoconf.h"
#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @enum    consoleStatus_t
 * @brief   Console status enum
 */
typedef enum
{
    CONSOLE_NOT_INITIALISED = 0u, /**< Console is not initialised */
    CONSOLE_INITIALISED     = 1u, /**< Console is initialised */
} consoleStatus_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void InitConsole(void);
extern void CreateConsoleMutexes(void);
extern ATTR_CHECK_FORMAT void ConsolePrint(const char *fmt, ...);

#endif /* CONSOLE_H */

/**
 * @}
 * @}
 * @}
 */