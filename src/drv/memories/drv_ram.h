/**
 * @file    drv_ram.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for RAM-emulated memory
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 * @defgroup drv_ram RAM Memory Driver
 * @brief Abstraction layer for controlling an RAM-emulated memory.
 * @{
 */

#ifndef DRV_RAM_H
#define DRV_RAM_H

/******************************* Include Files *******************************/

#include "drv/common.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @struct  ramConf_t
 * @brief   Struct type definition of a RAM-emulated memory configuration
 */
typedef struct
{
    void *base_addr; /**< @brief RAM-emulated memory base address (needs to be 512 bytes aligned) */
    uint32_t size;   /**< @brief RAM-emulated memory size (needs to be 512 bytes aligned) */
} ramConf_t;

/**
 * @struct  ramInst_t
 * @brief   Struct type definition of a RAM-emulated memory descriptor
 */
typedef struct
{
    const ramConf_t *p_conf; /**< @brief Pointer to RAM conf */
} ramInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

/**
 * @fn              RamOpen(ramInst_t *ram_inst, const ramConf_t *const ram_conf)
 * @brief           Function that initialise a RAM memory
 * @param[in,out]   ram_inst   Instance that contains RAM handlers
 * @param[in]       ram_conf   Configuration that contains RAM parameters
 * @retval          #RET_SUCCESSFUL if creation succeed
 * @retval          #RET_INVALID_PARAM if a pointer is null
 */
extern returnCode_t RamOpen(ramInst_t *ram_inst, const ramConf_t *const ram_conf);

/**
 * @fn          RamWrite(ramInst_t *ram_inst, memorySector_t sector, data_t data, length_t length)
 * @brief       Function that writes onto a RAM memory
 * @param[in]   ram_inst    Instance that contains RAM parameters and RAM Handler
 * @param[in]   sector      Sector number from which data will be written
 * @param[in]   data        Pointer from which data will be copied
 * @param[in]   length      Number of sectors that will be written
 * @retval      #RET_SUCCESSFUL if data has been written successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null or parameters are out of range
 */
extern returnCode_t RamWrite(ramInst_t *ram_inst, memorySector_t sector, data_t data, length_t length);

/**
 * @fn          RamRead(ramInst_t *ram_inst, memorySector_t sector, data_t data, length_t length)
 * @brief       Function that read onto an RAM memory
 * @param[in]   ram_inst    Instance that contains RAM parameters and RAM Handler
 * @param[in]   sector      Sector numero from wich data will be read
 * @param[out]  data        Pointer to where data will be copied
 * @param[in]   length      Number of block that will be read
 * @retval      #RET_SUCCESSFUL if data has been written successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null or parameters are out of range
 */
extern returnCode_t RamRead(ramInst_t *ram_inst, memorySector_t sector, data_t data, length_t length);

/**
 * @fn              RamIoctl(ramInst_t *ram_inst, uint32_t cmd, void *data, uint32_t data_size);
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   ram_inst    Instance that contains RAM handlers
 * @param[in]       cmd         IO Control command
 * @param[in,out]   data        IO Control command
 * @param[in]       data_size   IO Control data size
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if action cannot be performed because driver is busy
 * @retval          #RET_SUCCESSFUL else
 */
extern returnCode_t RamIoctl(ramInst_t *ram_inst, uint32_t cmd, void *data, uint32_t data_size);

/**
 * @fn              RamClose(ramInst_t *ram_inst)
 * @brief           Function that desinit the RAM connection
 * @param[in,out]   ram_inst   Instance that contains RAM handlers
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 */
extern returnCode_t RamClose(ramInst_t *ram_inst);

#endif /* DRV_RAM_H */

/**
 * @}
 * @}
 * @}
 */