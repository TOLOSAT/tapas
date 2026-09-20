/**
 * @file    rtc.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for RTC functions
 *
 * @copyright Copyright (c) TOLOSAT 2026
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drivers Drivers
 * @{
 * @defgroup rtc RTC Driver
 * @brief Abstraction layer for controlling Real-Time Clocks (RTC).
 * @{
 */

#ifndef DRIVERS_OTHERS_RTC_H
#define DRIVERS_OTHERS_RTC_H

/******************************* Include Files *******************************/

#include "drivers/common.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @struct  rtcTime_t
 * @brief   Struct type definition for Time storage struct
 */
typedef struct
{
    uint8_t year;       /**< RTC year */
    uint8_t month;      /**< RTC month */
    uint8_t day;        /**< RTC day */
    uint8_t hour;       /**< RTC hour */
    uint8_t minute;     /**< RTC minute */
    uint8_t second;     /**< RTC second */
    uint16_t subsecond; /**< RTC 16 bits subsecond fraction (0x0001 = 2^(-16) s) */
} rtcTime_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

/**
 * @fn      InitRtc(void)
 * @brief   Function that initialise RTC
 * @return  Nothing
 *
 * @warning     RTC is not supported by CMSDK so it is emulated by HAL tick
 */
extern void InitRtc(void);

/**
 * @fn          RtcSetTime(const rtcTime_t *rtc_time)
 * @brief       Function that sets time from RTC
 * @param[in]   rtc_time    Value of RTC time we want to set
 * @retval      #RET_SUCCESSFUL always
 *
 * @warning     RTC is not supported by CMSDK so it is emulated by HAL tick
 */
extern returnCode_t RtcSetTime(const rtcTime_t *rtc_time);

/**
 * @fn          RtcGetTime(rtcTime_t *rtc_time)
 * @brief       Function that gets time from RTC
 * @param[out]  rtc_time    Value to RTC time we want to read
 * @retval      #RET_SUCCESSFUL always
 *
 * @warning     RTC is not supported by CMSDK so it is emulated by HAL tick
 */
extern returnCode_t RtcGetTime(rtcTime_t *rtc_time);

#endif /* DRIVERS_OTHERS_RTC_H */

/**
 * @}
 * @}
 * @}
 */