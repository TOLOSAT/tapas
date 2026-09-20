/**
 * @file    monitoring.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for system monitoring handling
 *
 * @copyright Copyright (c) TOLOSAT 2026
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup monitoring Monitoring
 * @{
 * @defgroup mon Monitoring
 * @brief Provides system monitoring handling interface.
 * @{
 */

#ifndef MON_H
#define MON_H

/******************************* Include Files *******************************/

#include "autoconf.h"
#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/**
 * @var     g_system_usage
 * @brief   System usage struct
 */
extern systemUsage_t g_system_usage;

/**
 * @var     g_task_usages
 * @brief   Task usage struct
 */
extern taskUsage_t g_task_usages[CONFIG_MAX_NB_TASKS];

/*************************** Functions Declarations **************************/

/**
 * @fn      InitSYSMON(void)
 * @brief   Enables System Monitoring
 * @return  Nothing
 */
extern void InitSYSMON(void);

/**
 * @fn              SYSMONMain(void)
 * @brief           Main of the SYSMON task
 */
extern void SYSMONMain(void);

#endif /* MON_H */

/**
 * @}
 * @}
 * @}
 */