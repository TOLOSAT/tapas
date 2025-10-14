/**
 * @file    startup.c
 * @brief   Startup file
 */

/******************************* Include Files *******************************/

#include <stddef.h>
#include <stdint.h>
#include "autoconf.h"
#include "system_stm32h7xx.h"

/***************************** Macros Definitions ****************************/

/***************************** Type Definitions ****************************/

/**
 * @brief Exception Interrupt Handler Function Prototype
 */
typedef void (*isrVector_t)(void);

/*************************** Functions Declarations **************************/

extern int main(void);
extern void Default_Handler(void);

// Cortex-M system exceptions
void Reset_Handler(void);
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));        /**< @brief NMI Exception Handler */
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));  /**< @brief HardFault Exception Handler */
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));  /**< @brief MemManage Exception Handler */
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));   /**< @brief BusFault Exception Handler */
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler"))); /**< @brief UsageFault Exception Handler */
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));        /**< @brief SVC Exception Handler */
void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));   /**< @brief DebugMon Exception Handler */
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));     /**< @brief PendSV Exception Handler */
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));    /**< @brief SysTick Exception Handler */

// Interrupt Handlers
void Generic_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));

/*************************** Variables Declaration ***************************/

extern uint32_t __stack_end__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
extern uint32_t __tcm_bss_start__;
extern uint32_t __tcm_bss_end__;
extern uint32_t __kernel_bss_start__;
extern uint32_t __kernel_bss_end__;
extern const isrVector_t isr_vectors[256];

/*************************** Variables Definitions ***************************/

/**
 * @brief ISR Vector Table
 */
const isrVector_t isr_vectors[256] __attribute__((section(".isr_vector"))) = {
    (void (*)(void))&__stack_end__,
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0,
    0,
    0,
    0,
    SVC_Handler,
    DebugMon_Handler,
    0,
    PendSV_Handler,
    SysTick_Handler,
    Generic_IRQHandler, // WWDG_IRQHandler
    Generic_IRQHandler, // PVD_IRQHandler
    Generic_IRQHandler, // TAMP_STAMP_IRQHandler
    Generic_IRQHandler, // RTC_WKUP_IRQHandler
    Generic_IRQHandler, // FLASH_IRQHandler
    Generic_IRQHandler, // RCC_IRQHandler
    Generic_IRQHandler, // EXTI0_IRQHandler
    Generic_IRQHandler, // EXTI1_IRQHandler
    Generic_IRQHandler, // EXTI2_IRQHandler
    Generic_IRQHandler, // EXTI3_IRQHandler
    Generic_IRQHandler, // EXTI4_IRQHandler
    Generic_IRQHandler, // DMA1_Stream0_IRQHandler
    Generic_IRQHandler, // DMA1_Stream1_IRQHandler
    Generic_IRQHandler, // DMA1_Stream2_IRQHandler
    Generic_IRQHandler, // DMA1_Stream3_IRQHandler
    Generic_IRQHandler, // DMA1_Stream4_IRQHandler
    Generic_IRQHandler, // DMA1_Stream5_IRQHandler
    Generic_IRQHandler, // DMA1_Stream6_IRQHandler
    Generic_IRQHandler, // ADC_IRQHandler
    Generic_IRQHandler, // FDCAN1_IT0_IRQHandler
    Generic_IRQHandler, // FDCAN2_IT0_IRQHandler
    Generic_IRQHandler, // FDCAN1_IT1_IRQHandler
    Generic_IRQHandler, // FDCAN2_IT1_IRQHandler
    Generic_IRQHandler, // EXTI9_5_IRQHandler
    Generic_IRQHandler, // TIM1_BRK_TIM9_IRQHandler
    Generic_IRQHandler, // TIM1_UP_TIM10_IRQHandler
    Generic_IRQHandler, // TIM1_TRG_COM_TIM11_IRQHandler
    Generic_IRQHandler, // TIM1_CC_IRQHandler
    Generic_IRQHandler, // TIM2_IRQHandler
    Generic_IRQHandler, // TIM3_IRQHandler
    Generic_IRQHandler, // TIM4_IRQHandler
    Generic_IRQHandler, // I2C1_EV_IRQHandler
    Generic_IRQHandler, // I2C1_ER_IRQHandler
    Generic_IRQHandler, // I2C2_EV_IRQHandler
    Generic_IRQHandler, // I2C2_ER_IRQHandler
    Generic_IRQHandler, // SPI1_IRQHandler
    Generic_IRQHandler, // SPI2_IRQHandler
    Generic_IRQHandler, // USART1_IRQHandler
    Generic_IRQHandler, // USART2_IRQHandler
    Generic_IRQHandler, // USART3_IRQHandler
    Generic_IRQHandler, // EXTI15_10_IRQHandler
    Generic_IRQHandler, // RTC_Alarm_IRQHandler
    0,
    Generic_IRQHandler, // TIM8_BRK_TIM12_IRQHandler
    Generic_IRQHandler, // TIM8_UP_TIM13_IRQHandler
    Generic_IRQHandler, // TIM8_TRG_COM_TIM14_IRQHandler
    Generic_IRQHandler, // TIM8_CC_IRQHandler
    Generic_IRQHandler, // DMA1_Stream7_IRQHandler
    Generic_IRQHandler, // FMC_IRQHandler
    Generic_IRQHandler, // SDMMC1_IRQHandler
    Generic_IRQHandler, // TIM5_IRQHandler
    Generic_IRQHandler, // SPI3_IRQHandler
    Generic_IRQHandler, // UART4_IRQHandler
    Generic_IRQHandler, // UART5_IRQHandler
    Generic_IRQHandler, // TIM6_DAC_IRQHandler
    Generic_IRQHandler, // TIM7_IRQHandler
    Generic_IRQHandler, // DMA2_Stream0_IRQHandler
    Generic_IRQHandler, // DMA2_Stream1_IRQHandler
    Generic_IRQHandler, // DMA2_Stream2_IRQHandler
    Generic_IRQHandler, // DMA2_Stream3_IRQHandler
    Generic_IRQHandler, // DMA2_Stream4_IRQHandler
    Generic_IRQHandler, // ETH_IRQHandler
    Generic_IRQHandler, // ETH_WKUP_IRQHandler
    Generic_IRQHandler, // FDCAN_CAL_IRQHandler
    Generic_IRQHandler, // CM7_SEV_IRQHandler
    Generic_IRQHandler, // CM4_SEV_IRQHandler
    0,
    0,
    Generic_IRQHandler, // DMA2_Stream5_IRQHandler
    Generic_IRQHandler, // DMA2_Stream6_IRQHandler
    Generic_IRQHandler, // DMA2_Stream7_IRQHandler
    Generic_IRQHandler, // USART6_IRQHandler
    Generic_IRQHandler, // I2C3_EV_IRQHandler
    Generic_IRQHandler, // I2C3_ER_IRQHandler
    Generic_IRQHandler, // OTG_HS_EP1_OUT_IRQHandler
    Generic_IRQHandler, // OTG_HS_EP1_IN_IRQHandler
    Generic_IRQHandler, // OTG_HS_WKUP_IRQHandler
    Generic_IRQHandler, // OTG_HS_IRQHandler
    Generic_IRQHandler, // DCMI_IRQHandler
    0,
    Generic_IRQHandler, // RNG_IRQHandler
    Generic_IRQHandler, // FPU_IRQHandler
    Generic_IRQHandler, // UART7_IRQHandler
    Generic_IRQHandler, // UART8_IRQHandler
    Generic_IRQHandler, // SPI4_IRQHandler
    Generic_IRQHandler, // SPI5_IRQHandler
    Generic_IRQHandler, // SPI6_IRQHandler
    Generic_IRQHandler, // SAI1_IRQHandler
    Generic_IRQHandler, // LTDC_IRQHandler
    Generic_IRQHandler, // LTDC_ER_IRQHandler
    Generic_IRQHandler, // DMA2D_IRQHandler
    Generic_IRQHandler, // SAI2_IRQHandler
    Generic_IRQHandler, // QUADSPI_IRQHandler
    Generic_IRQHandler, // LPTIM1_IRQHandler
    Generic_IRQHandler, // CEC_IRQHandler
    Generic_IRQHandler, // I2C4_EV_IRQHandler
    Generic_IRQHandler, // I2C4_ER_IRQHandler
    Generic_IRQHandler, // SPDIF_RX_IRQHandler
    Generic_IRQHandler, // OTG_FS_EP1_OUT_IRQHandler
    Generic_IRQHandler, // OTG_FS_EP1_IN_IRQHandler
    Generic_IRQHandler, // OTG_FS_WKUP_IRQHandler
    Generic_IRQHandler, // OTG_FS_IRQHandler
    Generic_IRQHandler, // DMAMUX1_OVR_IRQHandler
    Generic_IRQHandler, // HRTIM1_Master_IRQHandler
    Generic_IRQHandler, // HRTIM1_TIMA_IRQHandler
    Generic_IRQHandler, // HRTIM1_TIMB_IRQHandler
    Generic_IRQHandler, // HRTIM1_TIMC_IRQHandler
    Generic_IRQHandler, // HRTIM1_TIMD_IRQHandler
    Generic_IRQHandler, // HRTIM1_TIME_IRQHandler
    Generic_IRQHandler, // HRTIM1_FLT_IRQHandler
    Generic_IRQHandler, // DFSDM1_FLT0_IRQHandler
    Generic_IRQHandler, // DFSDM1_FLT1_IRQHandler
    Generic_IRQHandler, // DFSDM1_FLT2_IRQHandler
    Generic_IRQHandler, // DFSDM1_FLT3_IRQHandler
    Generic_IRQHandler, // SAI3_IRQHandler
    Generic_IRQHandler, // SWPMI1_IRQHandler
    Generic_IRQHandler, // TIM15_IRQHandler
    Generic_IRQHandler, // TIM16_IRQHandler
    Generic_IRQHandler, // TIM17_IRQHandler
    Generic_IRQHandler, // MDIOS_WKUP_IRQHandler
    Generic_IRQHandler, // MDIOS_IRQHandler
    Generic_IRQHandler, // JPEG_IRQHandler
    Generic_IRQHandler, // MDMA_IRQHandler
    0,
    Generic_IRQHandler, // SDMMC2_IRQHandler
    Generic_IRQHandler, // HSEM1_IRQHandler
    Generic_IRQHandler, // HSEM2_IRQHandler
    Generic_IRQHandler, // ADC3_IRQHandler
    Generic_IRQHandler, // DMAMUX2_OVR_IRQHandler
    Generic_IRQHandler, // BDMA_Channel0_IRQHandler
    Generic_IRQHandler, // BDMA_Channel1_IRQHandler
    Generic_IRQHandler, // BDMA_Channel2_IRQHandler
    Generic_IRQHandler, // BDMA_Channel3_IRQHandler
    Generic_IRQHandler, // BDMA_Channel4_IRQHandler
    Generic_IRQHandler, // BDMA_Channel5_IRQHandler
    Generic_IRQHandler, // BDMA_Channel6_IRQHandler
    Generic_IRQHandler, // BDMA_Channel7_IRQHandler
    Generic_IRQHandler, // COMP1_IRQHandler
    Generic_IRQHandler, // LPTIM2_IRQHandler
    Generic_IRQHandler, // LPTIM3_IRQHandler
    Generic_IRQHandler, // LPTIM4_IRQHandler
    Generic_IRQHandler, // LPTIM5_IRQHandler
    Generic_IRQHandler, // LPUART1_IRQHandler
    Generic_IRQHandler, // WWDG_RST_IRQHandler
    Generic_IRQHandler, // CRS_IRQHandler
    Generic_IRQHandler, // ECC_IRQHandler
    Generic_IRQHandler, // SAI4_IRQHandler
    0,
    Generic_IRQHandler, // HOLD_CORE_IRQHandler
    Generic_IRQHandler, // WAKEUP_PIN_IRQHandler
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

/*************************** Functions Definitions ***************************/

/**
 * @brief Reset Handler. Program goes here when a soft or hard reset has been done
 */
void Reset_Handler(void)
{
    uint32_t section_size = 0u;
    uint8_t *ptr_ram      = NULL;

    // Then start system initialisation
    SystemInit();

    // Initialise the .bss section with zero
    section_size = (uint32_t)&__bss_end__ - (uint32_t)&__bss_start__; // cppcheck-suppress misra-c2012-11.4; Exception: memory needs to be addressed
    ptr_ram      = (uint8_t *)&__bss_start__;
    for (uint32_t i = 0u; i < section_size; i++)
    {
        ptr_ram[i] = 0u;
    }

    // Initialise the .protected_bss section with zero
    section_size = (uint32_t)&__kernel_bss_end__ - (uint32_t)&__kernel_bss_start__; // cppcheck-suppress misra-c2012-11.4; Exception: memory needs to
                                                                                    // be addressed
    ptr_ram = (uint8_t *)&__kernel_bss_start__;
    for (uint32_t i = 0u; i < section_size; i++)
    {
        ptr_ram[i] = 0u;
    }

    // Initialise the .tcm_bss section with zero
    section_size = (uint32_t)&__tcm_bss_end__ - (uint32_t)&__tcm_bss_start__; // cppcheck-suppress misra-c2012-11.4; Exception: memory needs to be
                                                                              // addressed
    ptr_ram = (uint8_t *)&__tcm_bss_start__;
    for (uint32_t i = 0u; i < section_size; i++)
    {
        ptr_ram[i] = 0u;
    }

    // Finally goes to main
    (void)main();
}

/**
 * @brief Default Handler
 */
void Default_Handler(void)
{
    while (1)
    {
        // Do nothing
    }
}
