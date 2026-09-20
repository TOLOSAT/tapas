/**
 * @file    signals.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for signals handling functions
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup core Core
 * @{
 * @defgroup signals Signals
 * @brief Signals handling interface.
 * @{
 */

#ifndef SIGNALS_H
#define SIGNALS_H

/******************************* Include Files *******************************/

#include "autoconf.h"
#include "kernel_types.h"
#include "core/os.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

/**
 * @fn          SendSignal(taskNo_t task, signalMask_t mask)
 * @brief       This function sends signal(s) to a specific task.
 * @param[in]   task    Task to which the signal will be sent
 * @param[in]   mask    Mask defining which signal type(s) will be sent
 * @retval      #RET_INVALID_PARAM if task does not exists
 * @retval      #RET_INVALID_PARAM if mask is null
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t SendSignal(taskNo_t task, signalMask_t mask);

/**
 * @fn          WaitSignal(signalMask_t mask)
 * @brief       This function waits until one of the specified signals is received.
 * @param[in]   mask    Mask defining which signal type(s) will be waited for
 * @retval      #RET_INVALID_PARAM if mask is null
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t WaitSignal(signalMask_t mask);

#endif /* SIGNALS_H */

/**
 * @}
 * @}
 * @}
 */