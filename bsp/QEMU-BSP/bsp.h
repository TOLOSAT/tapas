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
#define HAL_TIMER_PERIPH                   CMSDK_DUALTIMER /**< HAL timer peripheral */
#define HAL_TIMER_IRQ_NO                   DUALTIMER_IRQn  /**< HAL timer interruption numero */
#define MONITORING_TIMER_REF               CMSDK_TIMER0    /**< Monitoring timer peripheral */
#define MONITORING_TIMER_IRQ_NO            TIMER0_IRQn     /**< Monitoring timer interruption numero */

/* MEMORY REGIONS */
#define BSP_KERNEL_TEXT_REGION_BASE_ADDR   (uint32_t)&__kernel_text_start__ /**< Kernel text region base address */
#define BSP_KERNEL_TEXT_REGION_SIZE        ARM_MPU_REGION_SIZE_128KB        /**< Kernel text region size */

#define BSP_KERNEL_DATA_REGION_BASE_ADDR   (uint32_t)&__kernel_data_start__ /**< Kernel data (and bss) region base address */
#define BSP_KERNEL_DATA_REGION_SIZE        ARM_MPU_REGION_SIZE_128KB        /**< Kernel data (and bss) region size */

#define BSP_KERNEL_RODATA_REGION_BASE_ADDR (uint32_t)&__kernel_rodata_start__ /**< Kernel rodata region base address */
#define BSP_KERNEL_RODATA_REGION_SIZE      ARM_MPU_REGION_SIZE_128KB          /**< Kernel rodata region size  */

#define BSP_DMABUFF_REGION_BASE_ADDR       (uint32_t)&__dmabuff_start__ /**< DMA buffer region base address */
#define BSP_DMABUFF_REGION_SIZE            ARM_MPU_REGION_SIZE_32KB     /**< DMA buffer region size */

/*************************** Variables Definitions ***************************/

extern uint32_t __kernel_text_start__;
extern uint32_t __kernel_rodata_start__;
extern uint32_t __kernel_data_start__;
extern uint32_t __dmabuff_start__;

/*************************** Functions Declarations **************************/

void BSPLateInit(void);

#endif /* BSP_H */
