/**
 * @file    kernel.h
 * @author  Merlin Kooshmanian
 * @brief   Header including all kernel headers
 *
 * @copyright Copyright (c) TOLOSAT 2026
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @defgroup kernel Kernel
 * @{
 */

#ifndef KERNEL_H
#define KERNEL_H

/******************************* Include Files *******************************/

#include "kernel_types.h"
#include "utils/endianness.h"
#include "utils/time_conversions.h"

/***************************** Macros Definitions ****************************/

/**
 * @cond    IGNORE_DOXYGEN
 * @note    Syscall redefinition to make the user think they use the kernel function
 */
#define CheckError      sys_CheckError      /**< CheckError syscall redefinition */
#define Sleep           sys_Sleep           /**< Sleep syscall redefinition */
#define SleepPeriodic   sys_SleepPeriodic   /**< SleepPeriodic syscall redefinition */
#define GetTick         sys_GetTick         /**< GetTick syscall redefinition */
#define GetTime         sys_GetTime         /**< GetTime syscall redefinition */
#define SetTime         sys_SetTime         /**< SetTime syscall redefinition */
#define GetTickFreq     sys_GetTickFreq     /**< GetTickFreq syscall redefinition */
#define DeviceOpen      sys_DeviceOpen      /**< DeviceOpen syscall redefinition */
#define DeviceWrite     sys_DeviceWrite     /**< DeviceWrite syscall redefinition */
#define DeviceRead      sys_DeviceRead      /**< DeviceRead syscall redefinition */
#define DeviceIoctl     sys_DeviceIoctl     /**< DeviceIoctl syscall redefinition */
#define DeviceClose     sys_DeviceClose     /**< DeviceClose syscall redefinition */
#define GetCurrentTask  sys_GetCurrentTask  /**< GetCurrentTask syscall redefinition */
#define SuspendTask     sys_SuspendTask     /**< SuspendTask syscall redefinition */
#define ResumeTask      sys_ResumeTask      /**< ResumeTask syscall redefinition */
#define GetTaskPriority sys_GetTaskPriority /**< GetTaskPriority syscall redefinition */
#define SetTaskPriority sys_SetTaskPriority /**< SetTaskPriority syscall redefinition */
#define AcquireMutex    sys_AcquireMutex    /**< AcquireMutex syscall redefinition */
#define ReleaseMutex    sys_ReleaseMutex    /**< ReleaseMutex syscall redefinition */
#define SendSignal      sys_SendSignal      /**< SendSignal syscall redefinition */
#define WaitSignal      sys_WaitSignal      /**< WaitSignal syscall redefinition */
#define StartTimer      sys_StartTimer      /**< StartTimer syscall redefinition */
#define PauseTimer      sys_PauseTimer      /**< PauseTimer syscall redefinition */
#define ResumeTimer     sys_ResumeTimer     /**< ResumeTimer syscall redefinition */
#define SetTimer        sys_SetTimer        /**< ResumeTimer syscall redefinition */
#define ConsolePrint    sys_ConsolePrint    /**< ConsolePrint syscall redefinition */
/** @endcond */

/*************************** Functions Declarations **************************/

/**
 * @fn      sys_CheckError(returnCode_t retcode, severityLevel_t severity)
 * @brief   Syscall declaration for CheckError
 */
extern void CheckError(returnCode_t retcode, severityLevel_t severity);

/**
 * @fn      sys_Sleep(tick_t tick)
 * @brief   Syscall declaration for Sleep
 */
extern void Sleep(tick_t tick);

/**
 * @fn      sys_SleepPeriodic(void)
 * @brief   Syscall declaration for SleepPeriodic
 */
extern void SleepPeriodic(void);

/**
 * @fn      sys_GetTick(void)
 * @brief   Syscall declaration for GetTick
 */
extern tick_t GetTick(void);

/**
 * @fn      sys_GetTime(void)
 * @brief   Syscall declaration for GetTime
 */
extern time_t GetTime(void);

/**
 * @fn      sys_SetTime(time_t time)
 * @brief   Syscall declaration for SetTime
 */
extern returnCode_t SetTime(time_t time);

/**
 * @fn      sys_GetTickFreq(void)
 * @brief   Syscall declaration for GetTickFreq
 */
extern uint32_t GetTickFreq(void);

/**
 * @fn      sys_DeviceOpen(deviceNo_t *device, deviceType_t type, uint32_t resource)
 * @brief   Syscall declaration for DeviceOpen
 */
extern returnCode_t DeviceOpen(deviceNo_t *device, deviceType_t type, uint32_t resource);

/**
 * @fn      sys_DeviceWrite(deviceNo_t device, data_t data, length_t length)
 * @brief   Syscall declaration for DeviceWrite
 */
extern returnCode_t DeviceWrite(deviceNo_t device, data_t data, length_t length);

/**
 * @fn      sys_DeviceRead(deviceNo_t device, data_t data, length_t length)
 * @brief   Syscall declaration for DeviceRead
 */
extern returnCode_t DeviceRead(deviceNo_t device, data_t data, length_t length);

/**
 * @fn      sys_DeviceIoctl(deviceNo_t device, uint32_t cmd, void *data, uint32_t data_size)
 * @brief   Syscall declaration for DeviceIoctl
 */
extern returnCode_t DeviceIoctl(deviceNo_t device, uint32_t cmd, void *data, uint32_t data_size);

/**
 * @fn      sys_DeviceClose(deviceNo_t device)
 * @brief   Syscall declaration for DeviceClose
 */
extern returnCode_t DeviceClose(deviceNo_t device);

/**
 * @fn      sys_GetCurrentTask(void)
 * @brief   Syscall declaration for GetCurrentTask
 */
extern taskNo_t GetCurrentTask(void);

/**
 * @fn      sys_SuspendTask(taskNo_t task)
 * @brief   Syscall declaration for SuspendTask
 */
extern returnCode_t SuspendTask(taskNo_t task);

/**
 * @fn      sys_ResumeTask(taskNo_t task)
 * @brief   Syscall declaration for ResumeTask
 */
extern returnCode_t ResumeTask(taskNo_t task);

/**
 * @fn      sys_GetTaskPriority(taskNo_t task, taskPriority_t *priority)
 * @brief   Syscall declaration for GetTaskPriority
 */
extern returnCode_t GetTaskPriority(taskNo_t task, taskPriority_t *priority);

/**
 * @fn      sys_SetTaskPriority(taskNo_t task, taskPriority_t priority)
 * @brief   Syscall declaration for SetTaskPriority
 */
extern returnCode_t SetTaskPriority(taskNo_t task, taskPriority_t priority);

/**
 * @fn      sys_AcquireMutex(mutexNo_t mutex)
 * @brief   Syscall declaration for AcquireMutex
 */
extern returnCode_t AcquireMutex(mutexNo_t mutex);

/**
 * @fn      sys_ReleaseMutex(mutexNo_t mutex)
 * @brief   Syscall declaration for ReleaseMutex
 */
extern returnCode_t ReleaseMutex(mutexNo_t mutex);

/**
 * @fn      sys_SendSignal(taskNo_t task, signalMask_t mask)
 * @brief   Syscall declaration for SendSignal
 */
extern returnCode_t SendSignal(taskNo_t task, signalMask_t mask);

/**
 * @fn      sys_WaitSignal(signalMask_t mask)
 * @brief   Syscall declaration for WaitSignal
 */
extern returnCode_t WaitSignal(signalMask_t mask);

/**
 * @fn      sys_StartTimer(timerNo_t timer)
 * @brief   Syscall declaration for StartTimer
 */
extern returnCode_t StartTimer(timerNo_t timer);

/**
 * @fn      sys_PauseTimer(timerNo_t timer)
 * @brief   Syscall declaration for PauseTimer
 */
extern returnCode_t PauseTimer(timerNo_t timer);

/**
 * @fn      sys_ResumeTimer(timerNo_t timer)
 * @brief   Syscall declaration for ResumeTimer
 */
extern returnCode_t ResumeTimer(timerNo_t timer);

/**
 * @fn      sys_SetTimer(timerNo_t timer, tick_t period, timerMode_t mode)
 * @brief   Syscall declaration for SetTimer
 */
extern returnCode_t SetTimer(timerNo_t timer, tick_t period, timerMode_t mode);

/**
 * @fn      sys_ConsolePrint(const char *fmt, ...)
 * @brief   Syscall declaration for ConsolePrint
 */
extern ATTR_CHECK_FORMAT void ConsolePrint(const char *fmt, ...);

#endif /* KERNEL_H */

/**
 * @}
 */