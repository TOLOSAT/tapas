/**
 * @file    context.h
 * @author  Théo Bessel
 * @brief   Header file for Context API
 *
 * @copyright Copyright (c) TOLOSAT 2026
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup system System
 * @{
 * @defgroup context Context
 * @brief Interface for saving and restoring kernel context.
 * @{
 */

#ifndef CONTEXT_H
#define CONTEXT_H

/******************************* Include Files *******************************/

#include "kernel_autoconf.h"
#include "kernel_types.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

/**
 * @fn          InitContext(void)
 * @brief       Initialise the context of the kernel
 * @retval      #RET_INVALID_PARAM if an error occurs in the context memory driver
 * @retval      #RET_SUCCESSFUL else
 */
extern void InitContext(void);

/**
 * @fn          ReadContext(context_t *context)
 * @brief       Read the context of the kernel using the context memory driver
 * @param[out]  context Pointer to the context structure
 * @retval      #RET_INVALID_PARAM if an error occurs in the context memory driver
 * @retval      #RET_SUCCESSFUL else
 */
extern returnCode_t ReadContext(context_t *context);

/**
 * @fn          WriteContext(context_t *context)
 * @brief       Save the context of the kernel using the context memory driver
 * @param[in]   context Context structure
 * @retval      #RET_INVALID_PARAM if an error occurs in the context memory driver
 * @retval      #RET_SUCCESSFUL else
 */
extern returnCode_t WriteContext(context_t *context);

/**
 * @fn          EraseContext(void)
 * @brief       Erase the context of the kernel using the context memory driver
 * @retval      #RET_INVALID_PARAM if an error occurs in the context memory driver
 * @retval      #RET_SUCCESSFUL else
 */
extern returnCode_t EraseContext(void);

#endif /* CONTEXT_H */

/**
 * @}
 * @}
 * @}
 */
