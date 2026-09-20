/**
 * @file    initialisation.h
 * @author  Merlin Kooshmanian
 * @brief   Header file initialising tools and HAL
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup core Core
 * @{
 * @defgroup init Initialisation
 * @brief System and component initialization routines.
 * @{
 */

#ifndef INITIALISATION_H
#define INITIALISATION_H

/******************************* Include Files *******************************/

#include "autoconf.h"
#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

/**
 * @fn      init(void)
 * @brief   Function that initialise tools and HAL
 */
extern void init(void);

#endif /* INITIALISATION_H */

/**
 * @}
 * @}
 * @}
 */