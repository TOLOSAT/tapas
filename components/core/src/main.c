/**
 * @file    main.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for main
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

/******************************* Include Files *******************************/

#include "main.h"
#include "initialisation.h"
#include "core/os.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @copydoc main
 */
int main(void)
{
    // Initialisation
    init();

    // Start Operating System
    StartOS();

    // Shouldn't go here
    KernelPanic();

    return 0;
}
