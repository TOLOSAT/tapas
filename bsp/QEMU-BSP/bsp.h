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
#include "cmsdk_hal.h"

/***************************** Macros Definitions ****************************/

/* INTERNAL TIMERS */
#define HAL_TIMER_REF                      CMSDK_DUALTIMER
#define HAL_TIMER_IRQ_NO                   DUALTIMER_IRQn
#define MONITORING_TIMER_REF               CMSDK_TIMER0
#define MONITORING_TIMER_IRQ_NO            TIMER0_IRQn

/* MEMORY REGIONS */
#define BSP_KERNEL_TEXT_REGION_BASE_ADDR   (uint32_t)&__kernel_text_start__
#define BSP_KERNEL_DATA_REGION_BASE_ADDR   (uint32_t)&__kernel_data_start__
#define BSP_KERNEL_RODATA_REGION_BASE_ADDR (uint32_t)&__kernel_rodata_start__
#define BSP_DMABUFF_REGION_BASE_ADDR       (uint32_t)&__dmabuff_start__

#define BSP_KERNEL_TEXT_REGION_SIZE        ARM_MPU_REGION_SIZE_128KB
#define BSP_KERNEL_DATA_REGION_SIZE        ARM_MPU_REGION_SIZE_128KB
#define BSP_KERNEL_RODATA_REGION_SIZE      ARM_MPU_REGION_SIZE_128KB
#define BSP_DMABUFF_REGION_SIZE            ARM_MPU_REGION_SIZE_32KB

/*************************** Variables Definitions ***************************/

extern uint32_t __kernel_text_start__;
extern uint32_t __kernel_rodata_start__;
extern uint32_t __kernel_data_start__;
extern uint32_t __dmabuff_start__;

/*************************** Functions Declarations **************************/

void BSPLateInit(void);

#endif /* BSP_H */
