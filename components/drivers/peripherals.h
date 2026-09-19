/**
 * @file    peripherals.h
 * @author  Merlin Kooshmanian
 * @brief   Header file defining peripherals
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drivers Drivers
 * @{
 * @defgroup peripherals Peripherals Management
 * @brief Abstraction layer for managing peripherals using their corresponding driver.
 * @{
 */

#ifndef PERIPHERALS_H
#define PERIPHERALS_H

/******************************* Include Files *******************************/

#include "autoconf.h"
#include "kernel_types.h"
#include "core/mutex.h"

#include "drivers/common.h"
#include "drivers/peripherals/gpio.h"
#include "drivers/peripherals/i2c.h"
#include "drivers/peripherals/ow.h"
#include "drivers/peripherals/spi.h"
#include "drivers/peripherals/uart.h"

/***************************** Macros Definitions ****************************/

/**
 * @def     PERIPHERAL_CONF(peripheral_no)
 * @brief   Get peripheral conf from g_peripherals_conf_table
 */
#define PERIPHERAL_CONF(peripheral_no) (g_peripherals_conf_table[(peripheral_no) - 1u])

/**
 * @def     PERIPHERAL_DESC(peripheral_no)
 * @brief   Get peripheral conf from g_peripherals_desc_table
 */
#define PERIPHERAL_DESC(peripheral_no) (g_peripherals_desc_table[(peripheral_no) - 1u])

/**
 * @def     IS_A_VALID_PERIPHERAL(peripheral_no)
 * @brief   Indicates if the peripheral_no is valid
 */
#define IS_A_VALID_PERIPHERAL(peripheral_no)                                                                             \
    (((peripheral_no) != (peripheralNo_t)NO_PERIPHERAL) && ((peripheral_no) < (peripheralNo_t)CONFIG_MAX_NB_PERIPHERALS) \
     && (PERIPHERAL_DESC(peripheral_no).status == DESC_USED))

/***************************** Types Definitions *****************************/

/**
 * @enum    peripheralType_t
 * @brief   Peripheral type typedef enum
 */
typedef enum
{
    PERIPHERAL_GPIO = 0u, /**< GPIO type peripheral */
    PERIPHERAL_UART = 1u, /**< UART type peripheral */
    PERIPHERAL_I2C  = 2u, /**< I2C type peripheral */
    PERIPHERAL_SPI  = 3u, /**< SPI type peripheral */
    PERIPHERAL_OW   = 4u, /**< OW type peripheral */
} peripheralType_t;

/**
 * @enum    peripheralSyncMode_t
 * @brief   Peripheral synchronisation mode typedef enum
 */
typedef enum
{
    PERIPHERAL_SYNCHRONOUS  = 0u, /**< Mode synchronous (e.g. interrupt or DMA) */
    PERIPHERAL_ASYNCHRONOUS = 1u, /**< Mode asynchronous (e.g. polling) */
} peripheralSyncMode_t;

/**
 * @enum    peripheralDataFlow_t
 * @brief   Peripheral TX-RX data flow typedef enum
 */
typedef enum
{
    PERIPHERAL_FLOW_COUPLED     = 0u, /**< TX and RX are coupled */
    PERIPHERAL_FLOW_INDEPENDENT = 1u, /**< TX and RX are independent */
} peripheralDataFlow_t;

/**
 * @struct  peripheralConf_t
 * @brief   Struct type of a peripheral configuration
 */
typedef struct
{
    peripheralNo_t peripheral;            /**< @brief Peripheral reference number */
    peripheralType_t type;                /**< @brief Peripheral type (GPIO, UART, I2C, ...) */
    peripheralSyncMode_t synchronisation; /**< @brief Peripheral synchronisation (synchronous, asynchronous) */
    peripheralDataFlow_t flow_type;       /**< @brief Peripheral flow type (TX and RX coupled or independant) */
    const void *p_conf;                   /**< @brief Pointer to the peripheral configuration */
} peripheralConf_t;

/**
 * @struct  peripheralDesc_t
 * @brief   Struct type of a peripheral descriptors
 */
typedef struct
{
    descStatus_t status; /**< @brief Indicates if the descriptor is free or used */
    mutexHandle_t mutex; /**< @brief Peripheral mutex */
    struct
    {
        mutexHandle_t mutex; /**< @brief Peripheral receiving mutex */
        taskNo_t owner;      /**< @brief Peripheral receiving owner */
    } rx;                    /**< @brief Peripheral transmission sub-structure */
    struct
    {
        mutexHandle_t mutex; /**< @brief Peripheral transmitting mutex */
        taskNo_t owner;      /**< @brief Peripheral receiving owner */
    } tx;                    /**< @brief Peripheral reception sub-structure */
    void *p_inst;            /**< @brief Pointer to the peripheral instance */
} peripheralDesc_t;

/*************************** Variables Declarations **************************/

/**
 * @var     g_peripherals_conf_table
 * @brief   Configuration table where all peripherals configurations are stored
 */
extern const peripheralConf_t g_peripherals_conf_table[CONFIG_MAX_NB_PERIPHERALS];

/**
 * @var     g_peripherals_desc_table
 * @brief   Configuration table where all peripherals descriptors are stored
 */
extern peripheralDesc_t g_peripherals_desc_table[CONFIG_MAX_NB_PERIPHERALS];

/*************************** Functions Declarations **************************/

/**
 * @fn      InitPeripherals(void)
 * @brief   Function that initialises the peripherals
 * @return  Nothing
 */
extern void InitPeripherals(void);

/**
 * @fn      extern void CreatePeripheralsMutexes(void)
 * @brief   Function that allows to postpone mutex initilisation when other mutexes will be initialised.
 */
extern void CreatePeripheralsMutexes(void);

/**
 * @fn          PeripheralWrite(peripheralNo_t peripheral, data_t data, length_t length)
 * @brief       Function that writes data to a peripheral
 * @param[in]   peripheral  Peripheral numero
 * @param[in]   data        Data that will be sent to the device
 * @param[in]   length      Length of the data
 * @retval      #RET_INVALID_PARAM if data is a null pointer or peripheral is not valid
 * @retval      #RET_SUCCESSFUL else
 */
extern returnCode_t PeripheralWrite(peripheralNo_t peripheral, data_t data, length_t length);

/**
 * @fn          PeripheralRead(peripheralNo_t peripheral, data_t data, length_t length)
 * @brief       Function that reads data to a peripheral
 * @param[in]   peripheral  Peripheral numero
 * @param[out]  data        Data that will be received to the peripheral
 * @param[in]   length      Length of the data
 * @retval      #RET_INVALID_PARAM if data is a null pointer or peripheral is not valid
 * @retval      #RET_SUCCESSFUL else
 */
extern returnCode_t PeripheralRead(peripheralNo_t peripheral, data_t data, length_t length);

/**
 * @fn              PeripheralIoctl(peripheralNo_t peripheral, uint32_t cmd, void *data, uint32_t data_size)
 * @brief           Function that allows specific control over the peripheral
 * @param[in]       peripheral  Peripheral numero
 * @param[in]       cmd         IO control command
 * @param[in,out]   data        Data related to the command (if any), can be input or output
 * @param[in]       data_size   Data length (if any)
 * @retval          #RET_INVALID_PARAM if peripheral is not valid
 * @retval          #RET_SUCCESSFUL else
 */
extern returnCode_t PeripheralIoctl(peripheralNo_t peripheral, uint32_t cmd, void *data, uint32_t data_size);

#endif /* PERIPHERALS_H */

/**
 * @}
 * @}
 * @}
 */