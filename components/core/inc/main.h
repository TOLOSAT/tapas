/**
 * @file    main.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for main
 *
 * @copyright Copyright (c) TOLOSAT 2026
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup main Main
 * @brief Software entry point
 * @{
 */

#ifndef MAIN_H
#define MAIN_H

/******************************* Include Files *******************************/

#include "kernel_autoconf.h"
#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

/**
 * @fn      main(void)
 * @brief   Flight Software Main is the entrypoint of the code
 * @return  0
 */
extern int main(void);

#endif /* MAIN_H */

/**
 * @}
 * @}
 */