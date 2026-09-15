/**
 * @file    drv_spi.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for SPI functions
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 * @defgroup drv_spi SPI Driver
 * @brief Abstraction layer for controlling SPI buses.
 * @{
 */

#ifndef DRV_SPI_H
#define DRV_SPI_H

/******************************* Include Files *******************************/

#include "drv/common.h"
#include "core/irq.h"

/***************************** Macros Definitions ****************************/

#define SPI_FILL_CHAR 0xffu /**< SPI fill character */

/***************************** Types Definitions *****************************/

/** @brief SPI handle struct type redefinition */
typedef SPI_HandleTypeDef spiHandleStruct_t;

/** @brief SPI peripheral type redefinition (SPI1, SPI2, ...) */
typedef SPI_TypeDef spiPeriph_t;

/** @brief SPI prescaler (used to setup baudrate) type definition */
typedef uint32_t spiPrescaler_t;

/**
 * @enum    spiReadType_t
 * @brief   SPI receive mode type enum
 */
typedef enum
{
    SPI_READ_RX_ONLY = 0u, /**< SPI read does only a RX */
    SPI_READ_TX_RX   = 1u, /**< SPI read does TX and RX */
} spiReadType_t;

/**
 * @struct  spiConf_t
 * @brief   Struct type definition of a SPI configuration
 */
typedef struct
{
    spiPeriph_t *periph;        /**< @brief Pointer to the SPI peripheral (SPI1, SPI2, ...) */
    IRQNo_t irq_no;             /**< @brief SPI related interrupt */
    IRQPrio_t irq_prio;         /**< @brief SPI related interrupt priority */
    drivingMode_t default_mode; /**< @brief SPI driving mode */
    clockSource_t clk_src;      /**< @brief SPI peripheral clock source */
    spiPrescaler_t prescaler;   /**< @brief SPI precaler (used to setup baudrate)*/
    DMAConf_t dma_tx;           /**< @brief SPI DMA configuration for TX */
    DMAConf_t dma_rx;           /**< @brief SPI DMA configuration for TX */
    IOConf_t io_sck;            /**< @brief SPI IO configuration for SCK */
    IOConf_t io_miso;           /**< @brief SPI IO configuration MISO */
    IOConf_t io_mosi;           /**< @brief SPI IO configuration MOSI */
} spiConf_t;

/**
 * @struct  spiInst_t
 * @brief   Struct type definition of a SPI descriptor
 */
typedef struct
{
    spiHandleStruct_t handle_struct;                /**< @brief SPI handle struct used by HAL */
    drivingMode_t current_mode;                     /**< @brief SPI driving mode */
    data_t rxtx_data;                               /**< @brief Data transmitted on the MOSI line when doing a read with extra TX */
    length_t rxtx_data_length;                      /**< @brief Length of the data transmitted on the MOSI line when doing a read with extra TX */
    DMAHandleStruct_t dma_rx_handle_struct;         /**< @brief DMA RX handle struct used by HAL */
    DMAHandleStruct_t dma_tx_handle_struct;         /**< @brief DMA TX handle struct used by HAL */
    DrvCallback_t callback_rx_completed;            /**< @brief Callback when RX is completed */
    DrvCallbackParam_t callback_rx_completed_param; /**< @brief Callback parameter for RX completed */
    DrvCallback_t callback_tx_completed;            /**< @brief Callback when TX is completed */
    DrvCallbackParam_t callback_tx_completed_param; /**< @brief Callback parameter for TX completed */
    const spiConf_t *p_conf;                        /**< @brief Pointer to SPI conf */
} spiInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

/**
 * @fn              SpiOpen(spiInst_t *spi_inst, const spiConf_t *const spi_conf)
 * @brief           Function that initialise a SPI connection
 * @param[in,out]   spi_inst   Instance that contains SPI handlers
 * @param[in]       spi_conf   Configuration that contains SPI parameters
 * @retval          #RET_SUCCESSFUL if creation succeed
 * @retval          #RET_INVALID_PARAM if SPI ref is not available for this board or one pointer is null
 */
extern returnCode_t SpiOpen(spiInst_t *spi_inst, const spiConf_t *const spi_conf);

/**
 * @fn          SpiWrite(spiInst_t *spi_inst, data_t data, length_t length)
 * @brief       Function that write over a SPI connection
 * @param[in]   spi_inst    Instance that contains SPI parameters and SPI Handler
 * @param[in]   data        Message we want to send
 * @param[in]   length      Size of the message we want to sent
 * @retval      #RET_SUCCESSFUL if message sent successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if spi timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if spi is still sending previous message
 */
extern returnCode_t SpiWrite(spiInst_t *spi_inst, data_t data, length_t length);

/**
 * @fn          SpiRead(spiInst_t *spi_inst, data_t data, length_t length)
 * @brief       Function that read over SPI connection
 * @param[in]   spi_inst        Instance that contains SPI parameters and SPI Handler
 * @param[in]   slave_addr      Adress of the slave to which the message will be requested
 * @param[out]  data            Message we want to receive
 * @param[in]   length          Size of the message we want to receive
 * @retval      #RET_SUCCESSFUL if message sent successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if spi timed out before receiving message
 * @retval      #RET_NOT_AVAILABLE if spi is still receiving previous message
 */
extern returnCode_t SpiRead(spiInst_t *spi_inst, data_t data, length_t length);

/**
 * @fn              SpiIoctl(spiInst_t *spi_inst, uint32_t cmd, void *data, uint32_t data_size)
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   spi_inst    Instance that contains SPI parameters and SPI Handler
 * @param[in]       cmd         IO Control command
 * @param[in,out]   data        IO Control command
 * @param[in]       data_size   IO Control data size
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if action cannot be performed because driver is busy
 * @retval          #RET_SUCCESSFUL else
 *
 * @warning This feature is not supported yet so it does nothing
 */
extern returnCode_t SpiIoctl(spiInst_t *spi_inst, uint32_t cmd, void *data, uint32_t data_size);

/**
 * @fn              SpiClose(spiInst_t *spi_inst)
 * @brief           Function that desinit the SPI connection
 * @param[in,out]   spi_inst    Instance that contains SPI parameters and SPI Handler
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 */
extern returnCode_t SpiClose(spiInst_t *spi_inst);

#endif /* DRV_SPI_H */

/**
 * @}
 * @}
 * @}
 */