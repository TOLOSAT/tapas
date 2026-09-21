/**
 * @file    fdir.h
 * @author  Merlin Kooshmanian
 * @author  Théo Bessel
 * @brief   Header file for Failure Detection, Identification and Recovery (FDIR).
 *
 * @copyright Copyright (c) TOLOSAT 2026
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup fdir FDIR
 * @{
 * @defgroup fdir-handling FDIR Handling
 * @brief Failure Detection, Identification and Recovery (FDIR) handling interface.
 * @{
 */

#ifndef FDIR_H
#define FDIR_H

/******************************* Include Files *******************************/

#include "kernel_autoconf.h"
#include "kernel_types.h"
#include "fdir/stacktrace.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @brief General debug information captured during an error.
 */
typedef struct
{
    savedRegisters_t *registers; /**< Pointer to saved CPU registers.     */
    uint32_t cfsr;               /**< Configurable Fault Status Register. */
    uint32_t hfsr;               /**< Hard Fault Status Register.         */
    callStack_t call_stack;      /**< Captured call stack.                */
    uint32_t reboot_origin;      /**< Indicates the reboot origin.        */
} debugInfo_t;

/*************************** Variables Declarations **************************/

extern reportEventCallback_t p_ReportEvent;

/*************************** Functions Declarations **************************/

/**
 *  @fn     InitFDIR(void)
 *  @brief  Function that initialises the FDIR
 */
extern void InitFDIR(void);

/**
 * @fn          CheckError(returnCode_t retcode, severityLevel_t severity)
 * @brief       This function check if an error occured and execute the sanction
 * @param[in]   retcode     Return code of a function.
 * @param[in]   severity    Severity level of the event
 * @return      Nothing
 */
extern void CheckError(returnCode_t retcode, severityLevel_t severity);

/**
 * @fn      ErrorHandler(void)
 * @brief   This function is executed in case of error occurrence.
 * @warning Real FDIR has to be done.
 *
 * Normally this function has to save some context (what happened before error).
 * But currently no real fdir has been done. For debugging purposes, Error handler
 * is just a while loop that hangs processor indefinitly.
 */
extern void ErrorHandler(void);

/**
 * @fn KernelPanic(void)
 * @brief This function is executed in case of error occurrence in kernel space.
 * @warning WIP, not fully implemented now.
 *
 * This function saves the registers and proceeds a StackTrace
 */
extern void KernelPanic(void);

/**
 * @fn      SystemReset(void)
 * @brief   Reset the whole system
 */
extern void SystemReset(void);

#endif /* FDIR_H */

/**
 * @}
 * @}
 * @}
 */