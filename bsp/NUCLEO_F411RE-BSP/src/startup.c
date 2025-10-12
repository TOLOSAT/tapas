/**
 * @file    startup.c
 * @brief   Startup file
 */

/******************************* Include Files *******************************/

#include <stddef.h>
#include <stdint.h>
#include "autoconf.h"
#include "system_stm32f4xx.h"

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
extern uint32_t __data_start__;
extern uint32_t __data_end__;
extern uint32_t __data_start_initialize__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
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
    0,
    0,
    0,
    0,
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
    0,
    Generic_IRQHandler, // EXTI15_10_IRQHandler
    Generic_IRQHandler, // RTC_Alarm_IRQHandler
    Generic_IRQHandler, // OTG_FS_WKUP_IRQHandler
    0,
    0,
    0,
    0,
    Generic_IRQHandler, // DMA1_Stream7_IRQHandler
    0,
    Generic_IRQHandler, // SDIO_IRQHandler
    Generic_IRQHandler, // TIM5_IRQHandler
    Generic_IRQHandler, // SPI3_IRQHandler
    0,
    0,
    0,
    0,
    Generic_IRQHandler, // DMA2_Stream0_IRQHandler
    Generic_IRQHandler, // DMA2_Stream1_IRQHandler
    Generic_IRQHandler, // DMA2_Stream2_IRQHandler
    Generic_IRQHandler, // DMA2_Stream3_IRQHandler
    Generic_IRQHandler, // DMA2_Stream4_IRQHandler
    0,
    0,
    0,
    0,
    0,
    0,
    Generic_IRQHandler, // OTG_FS_IRQHandler
    Generic_IRQHandler, // DMA2_Stream5_IRQHandler
    Generic_IRQHandler, // DMA2_Stream6_IRQHandler
    Generic_IRQHandler, // DMA2_Stream7_IRQHandler
    Generic_IRQHandler, // USART6_IRQHandler
    Generic_IRQHandler, // I2C3_EV_IRQHandler
    Generic_IRQHandler, // I2C3_ER_IRQHandler
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    Generic_IRQHandler, // FPU_IRQHandler
    0,
    0,
    Generic_IRQHandler, // SPI4_IRQHandler
    Generic_IRQHandler, // SPI5_IRQHandler
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
    uint8_t *ptr_flash    = NULL;

    // Then start system initialisation
    SystemInit();

    // Copy .data section from FLASH to RAM
    section_size = (uint32_t)&__data_end__ - (uint32_t)&__data_start__; // cppcheck-suppress misra-c2012-11.4; Exception: memory needs to be addressed
    ptr_ram      = (uint8_t *)&__data_start__;
    ptr_flash    = (uint8_t *)&__data_start_initialize__;
    for (uint32_t i = 0u; i < section_size; i++)
    {
        ptr_ram[i] = ptr_flash[i];
    }

    // Init. the .bss section to zero in SRAM
    section_size = (uint32_t)&__bss_end__ - (uint32_t)&__bss_start__; // cppcheck-suppress misra-c2012-11.4; Exception: memory needs to be addressed
    ptr_ram      = (uint8_t *)&__bss_start__;
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
