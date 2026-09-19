/**
 * @file    spisd.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for SD card memory over SPI bus
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drivers Drivers
 * @{
 * @defgroup spisd SD Memory Driver over SPI
 * @brief Abstraction layer for controlling an SD card memory over SPI bus.
 * @{
 */

#ifndef DRIVERS_MEMORIES_SPISD_H
#define DRIVERS_MEMORIES_SPISD_H

/******************************* Include Files *******************************/

#include "drivers/common.h"
#include "drivers/peripherals/spi.h"
#include "drivers/peripherals/gpio.h"
#include "core/irq.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @enum    spisdCardType_t
 * @brief   SD card type type enum
 */
typedef enum
{
    NOT_SDCARD  = 0u, /**< @brief Not an SD Card */
    SDCARD_V1   = 1u, /**< @brief Is a SD card v1 */
    SDCARD_V2   = 2u, /**< @brief Is a SD card v2 */
    SDCARD_V2HC = 3u, /**< @brief Is a SD card v2 High Capacity */
} spisdCardType_t;

/**
 * @struct  spisdConf_t
 * @brief   Struct type definition of a SPISD configuration
 */
typedef struct
{
    spiPeriph_t *spi_periph; /**< @brief Pointer to the SPI peripheral (SPI1, SPI2, ...) */
    IRQNo_t irq_no;          /**< @brief SPISD related interrupt */
    IRQPrio_t irq_prio;      /**< @brief SPISD related interrupt priority */
    uint32_t clk_src;        /**< @brief SPI peripheral clock source */
    uint32_t prescaler;      /**< @brief SPI peripheral clock prescaler */
    IOConf_t io_sck;         /**< @brief SPI IO configuration for SCK */
    IOConf_t io_miso;        /**< @brief SPI IO configuration MISO */
    IOConf_t io_mosi;        /**< @brief SPI IO configuration MOSI */
    IOConf_t io_cs;          /**< @brief SPI IO configuration CS */
} spisdConf_t;

/**
 * @struct  spisdInst_t
 * @brief   Struct type definition of a SPISD descriptor
 */
typedef struct
{
    spiHandleStruct_t spi_handle_struct; /**< @brief SPI handle struct used by HAL */
    gpioPort_t *cs_port;                 /**< @brief GPIO CS port */
    gpioPin_t cs_pin;                    /**< @brief GPIO CS pin */
    spisdCardType_t sd_type;             /**< @brief SD card type */
    const spisdConf_t *p_conf;           /**< @brief Pointer to SPISD conf */
} spisdInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

/**
 * @fn              SpiSdOpen(spisdInst_t *spisd_inst, const spisdConf_t *const spisd_conf)
 * @brief           Function that initialise a SPISD memory
 * @param[in,out]   spisd_inst   Instance that contains SPISD handlers
 * @param[in]       spisd_conf   Configuration that contains SPISD parameters
 * @retval          #RET_SUCCESSFUL if creation succeed
 * @retval          #RET_INVALID_PARAM if a pointer is null
 */
extern returnCode_t SpiSdOpen(spisdInst_t *spisd_inst, const spisdConf_t *const spisd_conf);

/**
 * @fn          SpiSdWrite(spisdInst_t *spisd_inst, memorySector_t sector, data_t data, length_t length)
 * @brief       Function that writes onto an SPISD memory
 * @param[in]   spisd_inst     Instance that contains SPISD parameters and SPISD Handler
 * @param[in]   sector      Sector numero from wich data will be read
 * @param[out]  data        Pointer from which data will be copied
 * @param[in]   length      Number of block that will be read
 * @retval      #RET_SUCCESSFUL if data has been written successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if spisd timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if spisd is still sending previous message
 */
extern returnCode_t SpiSdWrite(spisdInst_t *spisd_inst, memorySector_t sector, data_t data, length_t length);

/**
 * @fn          SpiSdRead(spisdInst_t *spisd_inst, memorySector_t sector, data_t data, length_t length)
 * @brief       Function that read onto an SPISD memory
 * @param[in]   spisd_inst     Instance that contains SPISD parameters and SPISD Handler
 * @param[in]   sector      Sector numero from wich data will be read
 * @param[out]  data        Pointer to where data will be copied
 * @param[in]   length      Number of block that will be read
 * @retval      #RET_SUCCESSFUL if data has been read successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if spisd timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if spisd is still sending previous message
 */
extern returnCode_t SpiSdRead(spisdInst_t *spisd_inst, memorySector_t sector, data_t data, length_t length);

/**
 * @fn              SpiSdIoctl(spisdInst_t *spisd_inst, uint32_t cmd, void *data, uint32_t data_size);
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   spisd_inst     Instance that contains SPISD handlers
 * @param[in]       cmd         IO Control command
 * @param[in,out]   data        IO Control command
 * @param[in]       data_size   IO Control data size
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if action cannot be performed because driver is busy
 * @retval          #RET_SUCCESSFUL else
 */
extern returnCode_t SpiSdIoctl(spisdInst_t *spisd_inst, uint32_t cmd, void *data, uint32_t data_size);

/**
 * @fn              SpiSdClose(spisdInst_t *spisd_inst)
 * @brief           Function that desinit the SPISD connection
 * @param[in,out]   spisd_inst   Instance that contains SPISD handlers
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 */
extern returnCode_t SpiSdClose(spisdInst_t *spisd_inst);

#endif /* DRIVERS_MEMORIES_SPISD_H */

/**
 * @}
 * @}
 * @}
 */