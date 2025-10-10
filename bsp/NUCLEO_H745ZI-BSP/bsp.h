/**
 * @file    bsp.h
 * @author  Merlin Kooshmanian
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */
#ifndef BSP_H
#define BSP_H

/******************************* Include Files *******************************/

#include "common_types.h"
#include "stm32h7xx_hal.h"

/***************************** Macros Definitions ****************************/

/* INTERNAL TIMERS */
#define HAL_TIMER_REF                          TIM4
#define HAL_TIMER_IRQ_NO                       TIM4_IRQn
#define MONITORING_TIMER_REF                   TIM3
#define MONITORING_TIMER_IRQ_NO                TIM3_IRQn

/* WATCHDOG CONSTANTS */
#define WATCHDOG_REF                           IWDG1

/* MEMORY REGIONS */
#define BSP_KERNEL_TEXT_REGION_BASE_ADDR       (uint32_t)&__kernel_text_start__
#define BSP_KERNEL_DATA_REGION_BASE_ADDR       (uint32_t)&__kernel_data_start__
#define BSP_KERNEL_RODATA_REGION_BASE_ADDR     (uint32_t)&__kernel_rodata_start__
#define BSP_DMABUFF_REGION_BASE_ADDR           (uint32_t)&__dmabuff_start__

#define BSP_KERNEL_TEXT_REGION_SIZE            ARM_MPU_REGION_SIZE_128KB
#define BSP_KERNEL_DATA_REGION_SIZE            ARM_MPU_REGION_SIZE_128KB
#define BSP_KERNEL_RODATA_REGION_SIZE          ARM_MPU_REGION_SIZE_16KB
#define BSP_DMABUFF_REGION_SIZE                ARM_MPU_REGION_SIZE_32KB

#define BSP_KERNEL_TCM_TEXT_REGION_BASE_ADDR   (uint32_t)&__kernel_itm_text_start__
#define BSP_KERNEL_TCM_DATA_REGION_BASE_ADDR   (uint32_t)&__kernel_itm_data_start__
#define BSP_KERNEL_TCM_RODATA_REGION_BASE_ADDR (uint32_t)&__kernel_itm_rodata_start__

#define BSP_KERNEL_TCM_TEXT_REGION_SIZE        ARM_MPU_REGION_SIZE_64KB
#define BSP_KERNEL_TCM_DATA_REGION_SIZE        ARM_MPU_REGION_SIZE_128KB
#define BSP_KERNEL_TCM_RODATA_REGION_SIZE      ARM_MPU_REGION_SIZE_16KB

/***************************** Types Definitions *****************************/

/*************************** Variables Definitions ***************************/

extern uint32_t __kernel_text_start__;
extern uint32_t __kernel_rodata_start__;
extern uint32_t __kernel_data_start__;
extern uint32_t __dmabuff_start__;

extern uint32_t __kernel_icm_text_start__;
extern uint32_t __kernel_icm_rodata_start__;
extern uint32_t __kernel_icm_data_start__;

/*************************** Functions Declarations **************************/

returnCode_t SystemClock_Config(void);
void BSPLateInit(void);

#endif /* BSP_H */
