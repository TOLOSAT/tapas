/**
 * @file    nand.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for NAND memory using FMC
 *
 * @copyright Copyright (c) TOLOSAT 2026
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drivers Drivers
 * @{
 * @defgroup nand NAND Memory Driver
 * @brief Abstraction layer for controlling an NAND memory.
 * @{
 */

#ifndef DRIVERS_MEMORIES_NAND_H
#define DRIVERS_MEMORIES_NAND_H

/******************************* Include Files *******************************/

#include "drivers/common.h"
#include "core/irq.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** @brief NAND handle struct type redefinition */
typedef NAND_HandleTypeDef nandHandleStruct_t;

/** @brief NAND peripheral type redefinition  */
typedef FMC_NAND_TypeDef nandPeriph_t;

/** @brief NAND ID type redefinition */
typedef NAND_IDTypeDef nandId_t;

/**
 * @struct  nandConf_t
 * @brief   Struct type definition of a NAND configuration
 */
typedef struct
{
    nandPeriph_t *periph;     /**< @brief Pointer to the FMC NAND peripheral */
    uint32_t clk_src;         /**< @brief NAND peripheral clock source */
    uint32_t bank;            /**< @brief Pointer to the NAND peripheral (NAND bank) */
    uint32_t wait_feature;    /**< @brief NAND controler active wait or not */
    uint32_t data_width;      /**< @brief NAND data width */
    uint32_t ecc_computation; /**< @brief NAND ECC configuration (enable/disable) */
    uint32_t ecc_page_size;   /**< @brief NAND page size (for ECC computation) */
    uint32_t TCLR_setup_time; /**< @brief Number of clock cycles for the delay between CLE low and RE low */
    uint32_t TAR_setup_time;  /**< @brief Number of clock cycles for the delay between ALE low and RE low */
    uint32_t page_size;       /**< @brief NAND page size in bytes */
    uint32_t spare_area_size; /**< @brief NAND spare area (used for ECC) size in bytes */
    uint32_t block_size;      /**< @brief NAND block size in pages */
    uint32_t nb_block;        /**< @brief NAND number of blocks (total, every plane) */
    uint32_t nb_plane;        /**< @brief NAND number of planes */
    uint32_t plane_size;      /**< @brief NAND plane size in blocks */
    uint32_t extra_cmd;       /**< @brief NAND extra command configuration (enable/disable) */
    uint32_t setup_time;      /**< @brief NAND setup time in clk source tick */
    uint32_t wait_time;       /**< @brief NAND wait time in clk source tick */
    uint32_t hold_time;       /**< @brief NAND hold time in clk source tick */
    uint32_t hiz_time;        /**< @brief NAND high z time in clk source tick */
    IOConf_t io_cle;          /**< @brief NAND IO configuration for CLE */
    IOConf_t io_ale;          /**< @brief NAND IO configuration for ALE */
    IOConf_t io_nce;          /**< @brief NAND IO configuration for NCE */
    IOConf_t io_noe;          /**< @brief NAND IO configuration for NOE */
    IOConf_t io_nwe;          /**< @brief NAND IO configuration for NWE */
    IOConf_t io_nwait;        /**< @brief NAND IO configuration for NWAIT */
    IOConf_t io_d0;           /**< @brief NAND IO configuration for D0 */
    IOConf_t io_d1;           /**< @brief NAND IO configuration for D1 */
    IOConf_t io_d2;           /**< @brief NAND IO configuration for D2 */
    IOConf_t io_d3;           /**< @brief NAND IO configuration for D3 */
    IOConf_t io_d4;           /**< @brief NAND IO configuration for D4 */
    IOConf_t io_d5;           /**< @brief NAND IO configuration for D5 */
    IOConf_t io_d6;           /**< @brief NAND IO configuration for D6 */
    IOConf_t io_d7;           /**< @brief NAND IO configuration for D7 */
} nandConf_t;

/**
 * @struct  nandInst_t
 * @brief   Struct type definition of a NAND descriptor
 */
typedef struct
{
    nandHandleStruct_t handle_struct; /**< @brief NAND handle struct used by HAL */
    nandId_t id;                      /**< @brief NAND ID */
    const nandConf_t *p_conf;         /**< @brief Pointer to NAND conf */
} nandInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

/**
 * @fn              NandOpen(nandInst_t *nand_inst, const nandConf_t *const nand_conf)
 * @brief           Function that initialise a NAND memory
 * @param[in,out]   nand_inst   Instance that contains NAND handlers
 * @param[in]       nand_conf   Configuration that contains NAND parameters
 * @retval          #RET_SUCCESSFUL if creation succeed
 * @retval          #RET_INVALID_PARAM if a pointer is null
 * @retval          #RET_NOT_AVAILABLE if the NAND controller is busy while enabling internal ECC
 * @retval          #RET_TIMEOUT if the NAND controller times out while enabling internal ECC
 * @retval          #RET_ERROR if internal ECC could not be enabled
 */
extern returnCode_t NandOpen(nandInst_t *nand_inst, const nandConf_t *const nand_conf);

/**
 * @fn          NandWrite(nandInst_t *nand_inst, memorySector_t sector, data_t data, length_t length)
 * @brief       Function that writes onto an NAND memory
 * @param[in]   nand_inst   Instance that contains NAND parameters and NAND Handler
 * @param[in]   sector      Sector numero from wich data will be read
 * @param[out]  data        Pointer from which data will be copied
 * @param[in]   length      Number of sector that will be read
 * @retval      #RET_SUCCESSFUL if data has been written successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if nand timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if nand is still sending previous message
 */
extern returnCode_t NandWrite(nandInst_t *nand_inst, memorySector_t sector, data_t data, length_t length);

/**
 * @fn          NandRead(nandInst_t *nand_inst, memorySector_t sector, data_t data, length_t length)
 * @brief       Function that read onto an NAND memory
 * @param[in]   nand_inst     Instance that contains NAND parameters and NAND Handler
 * @param[in]   sector      Sector numero from wich data will be read
 * @param[out]  data        Pointer to where data will be copied
 * @param[in]   length      Number of sector that will be read
 * @retval      #RET_SUCCESSFUL if data has been read successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if nand timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if nand is still sending previous message
 */
extern returnCode_t NandRead(nandInst_t *nand_inst, memorySector_t sector, data_t data, length_t length);

/**
 * @fn              NandIoctl(nandInst_t *nand_inst, uint32_t cmd, void *data, uint32_t data_size);
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   nand_inst     Instance that contains NAND handlers
 * @param[in]       cmd         IO Control command
 * @param[in,out]   data        IO Control command
 * @param[in]       data_size   IO Control data size
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if action cannot be performed because driver is busy
 * @retval          #RET_SUCCESSFUL else
 */
extern returnCode_t NandIoctl(nandInst_t *nand_inst, uint32_t cmd, void *data, uint32_t data_size);

/**
 * @fn              NandClose(nandInst_t *nand_inst)
 * @brief           Function that desinit the NAND connection
 * @param[in,out]   nand_inst   Instance that contains NAND handlers
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 */
extern returnCode_t NandClose(nandInst_t *nand_inst);

#endif /* DRIVERS_MEMORIES_NAND_H */

/**
 * @}
 * @}
 * @}
 */