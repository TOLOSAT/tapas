/**
 * @file    initialisation.c
 * @author  Merlin Kooshmanian
 * @brief   Source file initialising tools and HAL
 *
 * @copyright Copyright (c) TOLOSAT 2026
 * SPDX-License-Identifier: Apache-2.0
 */

/******************************* Include Files *******************************/

#include "initialisation.h"
#include "buffers.h"
#include "core/tasks.h"
#include "timers.h"
#include "drivers/common.h"
#include "drivers/peripherals.h"
#include "drivers/memories.h"
#include "drivers/others/ecc.h"
#include "drivers/others/rtc.h"
#include "drivers/others/wdg.h"
#include "fdir/fdir.h"
#include "fdir/context.h"
#include "file-system/fs.h"
#include "platform/cache.h"
#include "platform/mpu.h"
#include "platform/wdg.h"
#include "monitoring/console.h"
#include "monitoring/info.h"
#include "monitoring/indicators.h"
#include "monitoring/monitoring.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @copydoc init
 */
void init(void)
{
    // FDIR Initialisation
    InitFDIR();

    // Cache Initialisation
    InitCache();

    // MPU Initialisation
    InitMPU();

    // HAL Initialisation
    InitHal();

    // BSP Late Initialisation
    BSPLateInit();

    // System LEDs Initialisation
    InitIndicators();

    // Peripherals Initialisation
    InitPeripherals();

    // Memory Initialisation
    InitMemories();

    // RTC Initialisation
    InitRtc();

    // File System Initialisation
    InitFs();

    // Start ECC
    InitEcc();

    // Initialise the context of the kernel
    InitContext();

    // Initialise Console
    InitConsole();

    // Print System Information
    PrintSystemInfo();

    // WARNING : interrupts will be disabled by the following
    // functions. They will be re-reactivated when the OS will
    // be started.

    // System Monitor Initialisation
    InitSYSMON();

    // System Watchdog Initialisation
    InitSYSWDG();

    // Create all tasks
    CreateTasks();

    // Create all buffers
    CreateBuffers();

    // Create all timers
    CreateTimers();

    // Create all user mutexes
    CreateMutexes();
    CreateFsMutexes();
    CreateConsoleMutexes();
    CreatePeripheralsMutexes();
}
