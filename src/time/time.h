/**
 * @file    time.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for Time Management
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup time Time
 * @{
 * @defgroup time_handling Time Handling
 * @brief Time handling interface.
 * @{
 */

#ifndef TIME_H
#define TIME_H

/******************************* Include Files *******************************/

#include "autoconf.h"
#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

/**
 * @fn          GetTick(void)
 * @brief       Function that returns how many tick occured since the scheduler started
 * @return      Ticks
 */
extern tick_t GetTick(void);

/**
 * @fn          Sleep(tick_t tick)
 * @brief       Function that puts to sleep the current task.
 * @param[in]   tick    Amount of time the task will be put to sleep.
 * @return      Nothing
 *
 * @note Using tick = 0 will make the task yielding instead.
 */
extern void Sleep(tick_t tick);

/**
 * @fn      SleepPeriodic(void)
 * @brief   Function that puts to sleep the current task until next period
 * @return  Nothing
 */
extern void SleepPeriodic(void);

/**
 * @fn          GetTime(void)
 * @brief       Function that gets time (in CUC format) from RTC
 * @return      CUC formated time
 */
extern time_t GetTime(void);

/**
 * @fn          SetTime(time_t time)
 * @brief       Function that sets RTC from a time value (in CUC format)
 * @param[out]  time    Time formated according to CUC
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_SUCCESSFUL else
 */
extern returnCode_t SetTime(time_t time);

/**
 * @fn          GetTickFreq(void)
 * @brief       Function that gets the kernel tick frequency in Hz
 * @return      Kernel tick frequency in Hz
 */
extern uint32_t GetTickFreq(void);

#endif /* TIME_H */

/**
 * @}
 * @}
 * @}
 */