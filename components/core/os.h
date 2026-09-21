/**
 * @file    os.h
 * @author  Merlin Kooshmanian
 * @brief   OS API header file
 *
 * @copyright Copyright (c) TOLOSAT 2026
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef OS_H
#define OS_H

/******************************* Include Files *******************************/

#include <FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <timers.h>

#include "kernel_autoconf.h"
#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/**
 * @brief Syscall Vector Table
 */
extern const uint32_t syscall_vector[NB_SYSCALLS];

/*************************** Functions Declarations **************************/

/**
 * @fn      StartOS(void)
 * @brief   Function that starts the OS
 * @return  Nothing
 */
extern void StartOS(void);

#endif /* OS_H */