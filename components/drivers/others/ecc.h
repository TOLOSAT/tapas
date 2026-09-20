/**
 * @file    ecc.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for ECC functions
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drivers Drivers
 * @{
 * @defgroup ecc ECC Driver
 * @brief Abstraction layer for controlling Error Correction Code (ECC) module.
 * @{
 */

#ifndef DRIVERS_OTHERS_ECC_H
#define DRIVERS_OTHERS_ECC_H

/******************************* Include Files *******************************/

#include "drivers/common.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

#if defined(CONFIG_ECC)
/**
 * @enum    eccRamId_t
 * @brief   Enum defining buffers reference numbers
 */
typedef enum
{
    ECC_AXI_SRAM,
    ECC_ITCM,
    ECC_DTCM0,
    ECC_DTCM1,
    ECC_SRAM1_0,
    ECC_SRAM1_1,
    ECC_SRAM2_0,
    ECC_SRAM2_1,
    ECC_SRAM3,
    ECC_SRAM4,
    ECC_BACKUP_SRAM,
    NB_ECCRAM,
} eccRamId_t;
#endif

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

/**
 * @fn      InitEcc(void)
 * @brief   This function init ECC
 * @return  Nothing
 */
extern void InitEcc(void);

#endif /* DRIVERS_OTHERS_ECC_H */

/**
 * @}
 * @}
 * @}
 */