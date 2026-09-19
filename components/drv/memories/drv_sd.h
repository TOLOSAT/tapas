/**
 * @file    drv_sd.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for SD card memory using SDIO bus
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 * @defgroup drv_sd SD Memory Driver
 * @brief Abstraction layer for controlling an SD card memory.
 * @{
 */

#ifndef DRV_SD_H
#define DRV_SD_H

/******************************* Include Files *******************************/

#include "drv/common.h"
#include "core/irq.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** @brief SD handle struct type redefinition */
typedef SD_HandleTypeDef sdHandleStruct_t;

/** @brief SD peripheral type redefinition (SDMMC1, SDMMC2, ...) */
typedef SD_TypeDef sdPeriph_t;

/**
 * @struct  sdConf_t
 * @brief   Struct type definition of a SD configuration
 */
typedef struct
{
    sdPeriph_t *periph;        /**< @brief Pointer to the SD peripheral (SDMMC1, SDMMC2, ...) */
    IRQNo_t irq_no;            /**< @brief SD related interrupt */
    IRQPrio_t irq_prio;        /**< @brief SD related interrupt priority */
    uint32_t clock_edge;       /**< @brief SD clock signal transition */
    uint32_t clock_power_save; /**< @brief SD power save mode */
    uint32_t hw_flow_ctrl;     /**< @brief SD hardware flow control enable/disable */
    uint32_t bus_width;        /**< @brief SD bus width (1, 4, 8) */
    uint32_t clk_src;          /**< @brief SD peripheral clock source */
    uint32_t prescaler;        /**< @brief SD peripheral clock prescaler */
    IOConf_t io_cmd;           /**< @brief SD IO configuration for CMD */
    IOConf_t io_clk;           /**< @brief SD IO configuration for CLK */
    IOConf_t io_d0;            /**< @brief SD IO configuration for D0 */
    IOConf_t io_d1;            /**< @brief SD IO configuration for D1 */
    IOConf_t io_d2;            /**< @brief SD IO configuration for D2 */
    IOConf_t io_d3;            /**< @brief SD IO configuration for D3 */
} sdConf_t;

/**
 * @struct  sdInst_t
 * @brief   Struct type definition of a SD descriptor
 */
typedef struct
{
    sdHandleStruct_t handle_struct; /**< @brief SD handle struct used by HAL */
    const sdConf_t *p_conf;         /**< @brief Pointer to SD conf */
} sdInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

/**
 * @fn              SdOpen(sdInst_t *sd_inst, const sdConf_t *const sd_conf)
 * @brief           Function that initialise a SD memory
 * @param[in,out]   sd_inst   Instance that contains SD handlers
 * @param[in]       sd_conf   Configuration that contains SD parameters
 * @retval          #RET_SUCCESSFUL if creation succeed
 * @retval          #RET_INVALID_PARAM if a pointer is null
 */
extern returnCode_t SdOpen(sdInst_t *sd_inst, const sdConf_t *const sd_conf);

/**
 * @fn          SdWrite(sdInst_t *sd_inst, memorySector_t sector, data_t data, length_t length)
 * @brief       Function that writes onto an SD memory
 * @param[in]   sd_inst     Instance that contains SD parameters and SD Handler
 * @param[in]   sector      Sector numero from wich data will be read
 * @param[out]  data        Pointer from which data will be copied
 * @param[in]   length      Number of block that will be read
 * @retval      #RET_SUCCESSFUL if data has been written successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if sd timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if sd is still sending previous message
 */
extern returnCode_t SdWrite(sdInst_t *sd_inst, memorySector_t sector, data_t data, length_t length);

/**
 * @fn          SdRead(sdInst_t *sd_inst, memorySector_t sector, data_t data, length_t length)
 * @brief       Function that read onto an SD memory
 * @param[in]   sd_inst     Instance that contains SD parameters and SD Handler
 * @param[in]   sector      Sector numero from wich data will be read
 * @param[out]  data        Pointer to where data will be copied
 * @param[in]   length      Number of block that will be read
 * @retval      #RET_SUCCESSFUL if data has been read successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if sd timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if sd is still sending previous message
 */
extern returnCode_t SdRead(sdInst_t *sd_inst, memorySector_t sector, data_t data, length_t length);

/**
 * @fn              SdIoctl(sdInst_t *sd_inst, uint32_t cmd, void *data, uint32_t data_size);
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   sd_inst     Instance that contains SD handlers
 * @param[in]       cmd         IO Control command
 * @param[in,out]   data        IO Control command
 * @param[in]       data_size   IO Control data size
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if action cannot be performed because driver is busy
 * @retval          #RET_SUCCESSFUL else
 */
extern returnCode_t SdIoctl(sdInst_t *sd_inst, uint32_t cmd, void *data, uint32_t data_size);

/**
 * @fn              SdClose(sdInst_t *sd_inst)
 * @brief           Function that desinit the SD connection
 * @param[in,out]   sd_inst   Instance that contains SD handlers
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 */
extern returnCode_t SdClose(sdInst_t *sd_inst);

#endif /* DRV_SD_H */

/**
 * @}
 * @}
 * @}
 */