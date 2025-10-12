/**
 * @file    startup.c
 * @brief   Startup file
 */

/******************************* Include Files *******************************/

#include <stddef.h>
#include <stdint.h>
#include "autoconf.h"
#include "system_CMSDK_CM7.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

extern int main(void);
extern void Default_Handler(void);

// Cortex-M system exceptions
void Reset_Handler(void);
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

// Interrupt Handlers
void Generic_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));

/*************************** Variables Declaration ***************************/

extern uint32_t __stack_end__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
extern uint32_t __kernel_bss_start__;
extern uint32_t __kernel_bss_end__;
extern void (*const isr_vectors[256])(void);

/*************************** Variables Definitions ***************************/

/**
 * @brief ISR Vector Table
 */
__attribute__((section(".isr_vector"))) void (*const isr_vectors[256])(void) = {
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
    Generic_IRQHandler, // UART0RX_Handler
    Generic_IRQHandler, // UART0TX_Handler
    Generic_IRQHandler, // UART1RX_Handler
    Generic_IRQHandler, // UART1TX_Handler
    Generic_IRQHandler, // UART2RX_Handler
    Generic_IRQHandler, // UART2TX_Handler
    Generic_IRQHandler, // GPIO0ALL_Handler
    Generic_IRQHandler, // GPIO1ALL_Handler
    Generic_IRQHandler, // TIMER0_Handler,
    Generic_IRQHandler, // TIMER1_Handler,
    Generic_IRQHandler, // DUALTIMER_Handler,
    Generic_IRQHandler, // SPI_0_1_Handler
    Generic_IRQHandler, // UART_0_1_2_OVF_Handler
    Generic_IRQHandler, // ETHERNET_Handler,
    Generic_IRQHandler, // I2C_Handler
    Generic_IRQHandler, // GPIO2_Handler
    Generic_IRQHandler, // GPIO3_Handler
    Generic_IRQHandler, // UART3RX_Handler
    Generic_IRQHandler, // UART3TX_Handler
    Generic_IRQHandler, // UART4RX_Handler
    Generic_IRQHandler, // UART4TX_Handler
    Generic_IRQHandler, // SPI_2_Handler
    Generic_IRQHandler, // SPI_3_4_Handler
    Generic_IRQHandler, // GPIO0_0_Handler,
    Generic_IRQHandler, // GPIO0_1_Handler,
    Generic_IRQHandler, // GPIO0_2_Handler,
    Generic_IRQHandler, // GPIO0_3_Handler,
    Generic_IRQHandler, // GPIO0_4_Handler,
    Generic_IRQHandler, // GPIO0_5_Handler,
    Generic_IRQHandler, // GPIO0_6_Handler,
    Generic_IRQHandler, // GPIO0_7_Handler,
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

    // Initialise the .bss section with zero
    section_size = (uint32_t)&__kernel_bss_end__ - (uint32_t)&__kernel_bss_start__; // cppcheck-suppress misra-c2012-11.4; Exception: memory needs to
                                                                                    // be addressed
    ptr_ram = (uint8_t *)&__kernel_bss_start__;
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
