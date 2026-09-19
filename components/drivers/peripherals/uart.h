/**
 * @file    uart.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for UART functions
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drivers Drivers
 * @{
 * @defgroup uart UART Driver
 * @brief Abstraction layer for controlling UART buses.
 * @{
 */

#ifndef DRIVERS_PERIPHERALS_UART_H
#define DRIVERS_PERIPHERALS_UART_H

/******************************* Include Files *******************************/

#include "drivers/common.h"
#include "core/irq.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** @brief UART handle struct type redefinition */
typedef UART_HandleTypeDef uartHandleStruct_t;

/** @brief UART peripheral type redefinition (USART1, USART2, ...) */
typedef USART_TypeDef uartPeriph_t;

/** @brief UART baudrate type definition */
typedef uint32_t uartBaudRate_t;

/**
 * @struct  uartConf_t
 * @brief   Struct type definition of a UART configuration
 */
typedef struct
{
    uartPeriph_t *periph;       /**< @brief Pointer to the UART peripheral (USART1, UART2, ...) */
    IRQNo_t irq_no;             /**< @brief UART related interrupt */
    IRQPrio_t irq_prio;         /**< @brief UART related interrupt priority */
    drivingMode_t default_mode; /**< @brief UART driving mode */
    uartBaudRate_t baudrate;    /**< @brief UART instance baudrate */
    clockSource_t clk_src;      /**< @brief UART peripheral clock source */
    DMAConf_t dma_tx;           /**< @brief UART DMA configuration for TX */
    DMAConf_t dma_rx;           /**< @brief UART DMA configuration for TX */
    IOConf_t io_tx;             /**< @brief UART IO configuration for TX */
    IOConf_t io_rx;             /**< @brief UART IO configuration for RX */
    bool is_circular_buffer;    /**< @brief Flag to know if circular buffer mode is enabled or not */
} uartConf_t;

/**
 * @struct  uartInst_t
 * @brief   Struct type definition of a UART descriptor
 */
typedef struct
{
    uartHandleStruct_t handle_struct;               /**< @brief UART handle struct used by HAL */
    drivingMode_t current_mode;                     /**< @brief Current driving mode */
    DMAHandleStruct_t dma_rx_handle_struct;         /**< @brief DMA RX handle struct used by HAL */
    DMAHandleStruct_t dma_tx_handle_struct;         /**< @brief DMA TX handle struct used by HAL */
    DrvCallback_t callback_rx_completed;            /**< @brief Callback when RX is completed */
    DrvCallbackParam_t callback_rx_completed_param; /**< @brief Callback parameter for RX completed */
    DrvCallback_t callback_tx_completed;            /**< @brief Callback when TX is completed */
    DrvCallbackParam_t callback_tx_completed_param; /**< @brief Callback parameter for TX completed */
    const uartConf_t *p_conf;                       /**< @brief Pointer to UART conf */
} uartInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

/**
 * @fn              UartOpen(uartInst_t *uart_inst, const uartConf_t *const uart_conf)
 * @brief           Function that initialise a UART connection
 * @param[in,out]   uart_inst   Instance that contains UART handlers
 * @param[in]       uart_conf   Configuration that contains UART parameters
 * @retval          #RET_SUCCESSFUL if creation succeed
 * @retval          #RET_INVALID_PARAM if UART ref is not available for this board, baudrate or one pointer is null
 */
extern returnCode_t UartOpen(uartInst_t *uart_inst, const uartConf_t *const uart_conf);

/**
 * @fn          UartWrite(uartInst_t *uart_inst, data_t data, length_t length)
 * @brief       Function that write over a UART connection
 * @param[in]   uart_inst   Instance that contains UART parameters and UART Handler
 * @param[in]   data        Message we want to send
 * @param[in]   length      Size of the message we want to send
 * @retval      #RET_SUCCESSFUL if message sent successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if uart timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if uart is still sending previous message
 */
extern returnCode_t UartWrite(uartInst_t *uart_inst, data_t data, length_t length);

/**
 * @fn          UartRead(uartInst_t *uart_inst, data_t data, length_t length)
 * @brief       Function that read over UART connection
 * @param[in]   uart_inst   Instance that contains UART parameters and UART Handler
 * @param[out]  data        Message we want to receive
 * @param[in]   length      Size of the message we want to receive
 * @retval      #RET_SUCCESSFUL if message sent successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if uart timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if uart is still sending previous message
 */
extern returnCode_t UartRead(uartInst_t *uart_inst, data_t data, length_t length);

/**
 * @fn              UartIoctl(uartInst_t *uart_inst, uint32_t cmd, void *data, uint32_t data_size);
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   uart_inst   Instance that contains UART parameters and UART Handler
 * @param[in]       cmd         IO Control command
 * @param[in,out]   data        IO Control command
 * @param[in]       data_size   IO Control data size
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if action cannot be performed because driver is busy
 * @retval          #RET_SUCCESSFUL else
 */
extern returnCode_t UartIoctl(uartInst_t *uart_inst, uint32_t cmd, void *data, uint32_t data_size);

/**
 * @fn              UartClose(uartInst_t *uart_inst)
 * @brief           Function that desinit the UART connection and puts defaults parameters
 * @param[in,out]   uart_inst   Instance that contains UART parameters and UART Handler
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 */
extern returnCode_t UartClose(uartInst_t *uart_inst);

#endif /* DRIVERS_PERIPHERALS_UART_H */

/**
 * @}
 * @}
 * @}
 */