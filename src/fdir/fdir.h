/**
 * @file    fdir.h
 * @author  Merlin Kooshmanian
 * @author  Théo Bessel
 * @brief   Header file for Failure Detection, Identification and Recovery (FDIR).
 *
 * @copyright Copyright (c) TOLOSAT 2026
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

#include "autoconf.h"
#include "kernel_types.h"
#include "stacktrace.h"

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

extern void InitFDIR(void);
extern void CheckError(returnCode_t retcode, severityLevel_t severity);
extern void ErrorHandler(void);
extern void KernelPanic(void);
extern void SystemReset(void);

#endif /* FDIR_H */

/**
 * @}
 * @}
 * @}
 */