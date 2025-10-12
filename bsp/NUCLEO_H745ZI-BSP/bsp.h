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
#define HAL_TIMER_PERIPH                       TIM4      /**< HAL timer peripheral */
#define HAL_TIMER_IRQ_NO                       TIM4_IRQn /**< HAL timer interruption numero */
#define MONITORING_TIMER_REF                   TIM3      /**< Monitoring timer peripheral */
#define MONITORING_TIMER_IRQ_NO                TIM3_IRQn /**< Monitoring timer interruption numero */

/* WATCHDOG CONSTANTS */
#define WATCHDOG_PERIPH                        IWDG1 /**< Internal watchdog peripheral */

/* MEMORY REGIONS */
#define BSP_KERNEL_TCM_TEXT_REGION_BASE_ADDR   (uint32_t)&__kernel_tcm_text_start__ /**< Kernel TCM text region base address */
#define BSP_KERNEL_TCM_TEXT_REGION_SIZE        ARM_MPU_REGION_SIZE_64KB             /**< Kernel TCM text region size */

#define BSP_KERNEL_TCM_DATA_REGION_BASE_ADDR   (uint32_t)&__kernel_tcm_data_start__ /**< Kernel TCM data (and bss) region base address */
#define BSP_KERNEL_TCM_DATA_REGION_SIZE        ARM_MPU_REGION_SIZE_128KB            /**< Kernel TCM data (and bss) region size */

#define BSP_KERNEL_TCM_RODATA_REGION_BASE_ADDR (uint32_t)&__kernel_tcm_rodata_start__ /**< Kernel TCM rodata region base address */
#define BSP_KERNEL_TCM_RODATA_REGION_SIZE      ARM_MPU_REGION_SIZE_16KB               /**< Kernel TCM rodata region size */

#define BSP_KERNEL_TEXT_REGION_BASE_ADDR       (uint32_t)&__kernel_text_start__ /**< Kernel text region base address */
#define BSP_KERNEL_TEXT_REGION_SIZE            ARM_MPU_REGION_SIZE_128KB        /**< Kernel text region size */

#define BSP_KERNEL_DATA_REGION_BASE_ADDR       (uint32_t)&__kernel_data_start__ /**< Kernel data (and bss) region base address */
#define BSP_KERNEL_DATA_REGION_SIZE            ARM_MPU_REGION_SIZE_128KB        /**< Kernel data (and bss) region size */

#define BSP_KERNEL_RODATA_REGION_BASE_ADDR     (uint32_t)&__kernel_rodata_start__ /**< Kernel rodata region base address */
#define BSP_KERNEL_RODATA_REGION_SIZE          ARM_MPU_REGION_SIZE_16KB           /**< Kernel rodata region size  */

#define BSP_DMABUFF_REGION_BASE_ADDR           (uint32_t)&__dmabuff_start__ /**< DMA buffer region base address */
#define BSP_DMABUFF_REGION_SIZE                ARM_MPU_REGION_SIZE_32KB     /**< DMA buffer region size */

/***************************** Types Definitions *****************************/

/*************************** Variables Definitions ***************************/

extern uint32_t __kernel_text_start__;
extern uint32_t __kernel_rodata_start__;
extern uint32_t __kernel_data_start__;
extern uint32_t __dmabuff_start__;

extern uint32_t __kernel_tcm_text_start__;
extern uint32_t __kernel_tcm_rodata_start__;
extern uint32_t __kernel_tcm_data_start__;

/*************************** Functions Declarations **************************/

returnCode_t SystemClock_Config(void);
void BSPLateInit(void);

#endif /* BSP_H */
