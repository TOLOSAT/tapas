/**
 * @file    qspinor.h
 * @author  Théo Bessel
 * @author  Merlin Kooshmanian
 * @brief   Header file for QSPI_NOR flash memory using QSPI bus
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drivers Drivers
 * @{
 * @defgroup qspi_nor QSPI_NOR Memory Driver
 * @brief Abstraction layer for controlling an QSPI_NOR flash memory.
 * @{
 */

#ifndef DRIVERS_MEMORIES_QSPINOR_H
#define DRIVERS_MEMORIES_QSPINOR_H

/******************************* Include Files *******************************/

#include "drivers/common.h"
#include "core/irq.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** @brief QSPI_NOR handle struct type redefinition */
typedef QSPI_HandleTypeDef qspiHandleStruct_t;

/** @brief QSPI_NOR peripheral type redefinition (QSPI1, QSPI2, ...) */
typedef QUADSPI_TypeDef qspiPeriph_t;

/**
 * @struct  qspinorConf_t
 * @brief   Struct type definition of a QSPI_NOR configuration
 */
typedef struct
{
    qspiPeriph_t *periph; /**< @brief Pointer to the QSPI_NOR peripheral (QSPI1, QSPI2, ...) */
    IRQNo_t irq_no;       /**< @brief QSPI NOR related interrupt */
    IRQPrio_t irq_prio;   /**< @brief QSPI NOR related interrupt priority */
    uint32_t clk_src;     /**< @brief QSPI NOR peripheral clock source */
    uint32_t prescaler;   /**< @brief QSPI NOR peripheral clock prescaler */
    uint32_t page_size;   /**< @brief QSPI NOR page size in bytes */
    uint32_t sector_size; /**< @brief QSPI NOR sector size in bytes */
    uint32_t nb_sector;   /**< @brief QSPI NOR number of blocks */
    IOConf_t io_sck;      /**< @brief QSPI NOR IO configuration for SCK */
    IOConf_t io_d0;       /**< @brief QSPI NOR IO configuration for D0 */
    IOConf_t io_d1;       /**< @brief QSPI NOR IO configuration for D1 */
    IOConf_t io_d2;       /**< @brief QSPI NOR IO configuration for D2 */
    IOConf_t io_d3;       /**< @brief QSPI NOR IO configuration for D3 */
    IOConf_t io_cs;       /**< @brief QSPI NOR IO configuration for CS */
} qspinorConf_t;

/**
 * @struct  qspinorInst_t
 * @brief   Struct type definition of a QSPI_NOR descriptor
 */
typedef struct
{
    qspiHandleStruct_t handle_struct; /**< @brief QSPI_NOR handle struct used by HAL */
    const qspinorConf_t *p_conf;      /**< @brief Pointer to QSPI_NOR conf */
} qspinorInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

/**
 * @fn              QspiNorOpen(qspinorInst_t *qspinor_inst, const qspinorConf_t *const qspinor_conf)
 * @brief           Function that initialise a QSPI NOR memory
 * @param[in,out]   qspinor_inst   Instance that contains QSPI NOR handlers
 * @param[in]       qspinor_conf   Configuration that contains QSPI NOR parameters
 * @retval          #RET_SUCCESSFUL if creation succeed
 * @retval          #RET_INVALID_PARAM if a pointer is null
 */
extern returnCode_t QspiNorOpen(qspinorInst_t *qspinor_inst, const qspinorConf_t *const qspi_nor_conf);

/**
 * @fn          QspiNorWrite(qspinorInst_t *qspinor_inst, memorySector_t sector, data_t data, length_t length)
 * @brief       Function that writes onto an QSPI NOR memory
 * @param[in]   qspinor_inst     Instance that contains QSPI NOR parameters and QSPI NOR Handler
 * @param[in]   sector      Sector numero from wich data will be read
 * @param[out]  data        Pointer from which data will be copied
 * @param[in]   length      Number of sector that will be read
 * @retval      #RET_SUCCESSFUL if data has been written successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if qspinor timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if qspinor is still sending previous message
 */
extern returnCode_t QspiNorWrite(qspinorInst_t *qspinor_inst, memorySector_t sector, data_t data, length_t length);

/**
 * @fn          QspiNorRead(qspinorInst_t *qspinor_inst, memorySector_t sector, data_t data, length_t length)
 * @brief       Function that read onto an QSPI NOR memory
 * @param[in]   qspinor_inst     Instance that contains QSPI NOR parameters and QSPI NOR Handler
 * @param[in]   sector      Sector numero from wich data will be read
 * @param[out]  data        Pointer to where data will be copied
 * @param[in]   length      Number of sector that will be read
 * @retval      #RET_SUCCESSFUL if data has been read successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if qspinor timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if qspinor is still sending previous message
 */
extern returnCode_t QspiNorRead(qspinorInst_t *qspinor_inst, memorySector_t sector, data_t data, length_t length);

/**
 * @fn              QspiNorIoctl(qspinorInst_t *qspinor_inst, uint32_t cmd, void *data, uint32_t data_size);
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   qspinor_inst    Instance that contains QSPI NOR handlers
 * @param[in]       cmd             IO Control command
 * @param[in,out]   data            IO Control command
 * @param[in]       data_size       IO Control data size
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if action cannot be performed because driver is busy
 * @retval          #RET_SUCCESSFUL else
 */
extern returnCode_t QspiNorIoctl(qspinorInst_t *qspinor_inst, uint32_t cmd, void *data, uint32_t data_size);

/**
 * @fn              QspiNorClose(qspinorInst_t *qspinor_inst)
 * @brief           Function that desinit the QSPI NOR connection
 * @param[in,out]   qspinor_inst   Instance that contains QSPI NOR handlers
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 */
extern returnCode_t QspiNorClose(qspinorInst_t *qspinor_inst);

#endif /* DRIVERS_MEMORIES_QSPINOR_H */

/**
 * @}
 * @}
 * @}
 */