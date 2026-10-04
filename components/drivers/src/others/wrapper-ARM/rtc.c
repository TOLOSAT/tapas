/**
 * @file    rtc.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for RTC functions
 *
 * @copyright Copyright (c) TOLOSAT 2026
 * SPDX-License-Identifier: Apache-2.0
 */

/******************************* Include Files *******************************/

#include "drivers/others/rtc.h"
#include "drivers/others/tim.h"

/***************************** Macros Definitions ****************************/

#define RTC_DEFAULT_YEAR        0u  /**< Default year alias 2000 */
#define RTC_DEFAULT_MONTH       2u  /**< Default month alias february */
#define RTC_DEFAULT_DAY         17u /**< Default day alias 17th */
#define RTC_DEFAULT_HOUR        11u /**< Default hour alias 11h */
#define RTC_DEFAULT_MINUTE      30u /**< Default minute alias 30m */
#define RTC_DEFAULT_SECOND      0u  /**< Default second alias 0 */

#define DAYS_PER_MONTH          30u    /**< Number of days per month (arbitrary set to 30 because it is just for emulation not real RTC)*/
#define HOURS_PER_DAY           24u    /**< Number of hours per day */
#define MINUTES_PER_HOUR        60u    /**< Number of minutes per hour */
#define SECONDS_PER_DAY         86400u /**< Number of second per day */
#define SECONDS_PER_HOUR        3600u  /**< Number of second per hour */
#define SECONDS_PER_MINUTE      60u    /**< Number of second per minute */
#define MILLISECONDS_PER_SECOND 1000u  /**< Number of millisecond per second */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @copydoc InitRtc
 */
void InitRtc(void)
{
    // Does nothing
}

/**
 * @copydoc RtcSetTime
 */
returnCode_t RtcSetTime(const rtcTime_t *rtc_time)
{
    (void)(rtc_time);

    return RET_SUCCESSFUL;
}

/**
 * @copydoc RtcGetTime
 */
returnCode_t RtcGetTime(rtcTime_t *rtc_time)
{
    uint32_t tick       = HAL_GetTick();
    rtc_time->year      = RTC_DEFAULT_YEAR;  // CONSTANT
    rtc_time->month     = RTC_DEFAULT_MONTH; // CONSTANT
    rtc_time->day       = (((tick / MILLISECONDS_PER_SECOND / SECONDS_PER_DAY) + RTC_DEFAULT_DAY - 1u) % DAYS_PER_MONTH) + 1u;
    rtc_time->hour      = ((tick / MILLISECONDS_PER_SECOND / SECONDS_PER_HOUR) + RTC_DEFAULT_HOUR) % HOURS_PER_DAY;
    rtc_time->minute    = ((tick / MILLISECONDS_PER_SECOND / SECONDS_PER_MINUTE) + RTC_DEFAULT_MINUTE) % MINUTES_PER_HOUR;
    rtc_time->second    = ((tick / MILLISECONDS_PER_SECOND) + RTC_DEFAULT_SECOND) % SECONDS_PER_MINUTE;
    rtc_time->subsecond = ((tick % MILLISECONDS_PER_SECOND) << 16) / MILLISECONDS_PER_SECOND;
    return RET_SUCCESSFUL;
}
