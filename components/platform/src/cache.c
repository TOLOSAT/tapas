/**
 * @file    cache.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for CACHE functions
 *
 * @copyright Copyright (c) TOLOSAT 2026
 * SPDX-License-Identifier: Apache-2.0
 */

/******************************* Include Files *******************************/

#include "platform/cache.h"
#include "bsp.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @copydoc InitCache
 */
void InitCache(void)
{
#if defined(CONFIG_CACHE)
    // Enable Instruction Cache
    SCB_EnableICache();

    // Enable Data Cache
    SCB_EnableDCache();

#if !defined(CONFIG_MPU)
    // Force the cache policy to be 'write through'
    SCB->CACR |= SCB_CACR_FORCEWT_Msk;
#endif
#endif
}
