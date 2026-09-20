/**
 * @file    info.h
 * @author  Merlin Kooshmanian
 * @brief   Header defining system information handling
 *
 * @copyright Copyright (c) TOLOSAT 2026
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup monitoring Monitoring
 * @{
 * @defgroup info System Information
 * @brief Provides hardcoded system information.
 * @{
 */

#ifndef SYSINFO_H
#define SYSINFO_H

/******************************* Include Files *******************************/

#include "autoconf.h"
#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @struct  sytemInfo_t
 * @brief   Struct that contains information about the system
 */
typedef struct
{
    const char *name;                /**< @brief System Name */
    const softwareVersion_t version; /**< @brief System Version */
    const char *build_date;          /**< @brief Build Date */
    const char *build_time;          /**< @brief Build Time */
    const char *target;              /**< @brief Target */
} sytemInfo_t;

/*************************** Variables Declarations **************************/

/**
 * @var     g_system_info
 * @brief   Constante struct containing system information
 */
extern const sytemInfo_t g_system_info;

/**
 * @var     g_program_name
 * @brief   Constant that holds program name
 */
extern const char *g_program_name;

/*************************** Functions Declarations **************************/

/**
 * @fn      PrintSystemInfo(void)
 * @brief   Function that prints all the system information onto the console
 * @return  Nothing
 */
extern void PrintSystemInfo(void);

#endif /* SYSINFO_H */

/**
 * @}
 * @}
 * @}
 */