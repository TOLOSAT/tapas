/**
 * @file    mpu.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for MPU functions
 *
 * @copyright Copyright (c) TOLOSAT 2026
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup platform Platform
 * @{
 * @defgroup mpu MPU
 * @brief Memory Protection Unit interface.
 * @{
 */

#ifndef MPU_H
#define MPU_H

/******************************* Include Files *******************************/

#include "kernel_autoconf.h"
#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

/**
 * @fn     InitMPU(void)
 * @brief  Function that initialises Memory Protection Unit
 *
 * The default cache and shareability strategies are :
 * - For normal memories : non-shareable and cacheable with write back and write and read allocate (i.e. TEX=001, C=1, B=1, S=0)
 * - For devices : shareable and not cacheable (i.e. TEX=000, C=0, B=1, S=1)
 * - For dma buffers : shareable and not cacheable (i.e. TEX=001, C=0, B=0, S=1)
 */
extern void InitMPU(void);

#endif /* MPU_H */

/**
 * @}
 * @}
 * @}
 */