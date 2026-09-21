/**
 * @file    console.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for Console functions
 *
 * @copyright Copyright (c) TOLOSAT 2026
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup monitoring Monitoring
 * @{
 * @defgroup console Console
 * @brief Interface for printing and LOGging messages.
 * @{
 */

#ifndef CONSOLE_H
#define CONSOLE_H

/******************************* Include Files *******************************/

#include "kernel_autoconf.h"
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

/**
 * @fn          InitConsole(void)
 * @brief       Initialise the console
 * @return      Nothing
 */
extern void InitConsole(void);

/**
 * @fn      CreateConsoleMutexes(void)
 * @brief   Function that allows to postpone mutex initilisation when other mutexes will be initialised.
 */
extern void CreateConsoleMutexes(void);

/**
 * @fn          ConsolePrint(const char *fmt, ...)
 * @brief       This function prints a string to the console following the
 *              format passed as first argument and the values as other
 *              arguments.
 * @param[in]   fmt The format string
 * @param[in]   ... The argument to replace in the format
 * @return      Nothing
 *
 * The types are checked compile time due to GCC attribute making this function
 * safe to use for the ARMv7 ABI.
 *
 * The currently supported formats are:
 *  - %u -> print usigned int
 *  - %d -> print signed int
 *  - %i -> print signed int
 *  - %x -> print lowercase hex
 *  - %X -> print uppercase hex
 *  - %o -> print octal int
 *  - %s -> print string
 *  - %c -> print char
 */
extern ATTR_CHECK_FORMAT void ConsolePrint(const char *fmt, ...);

#endif /* CONSOLE_H */

/**
 * @}
 * @}
 * @}
 */