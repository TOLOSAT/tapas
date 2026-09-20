/**
 * @file    qspimram.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for QSPI_MRAM flash memory using QSPI bus
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drivers Drivers
 * @{
 * @defgroup qspi_mram QSPI_MRAM Memory Driver
 * @brief Abstraction layer for controlling an QSPI_MRAM flash memory.
 * @{
 */

#ifndef DRIVERS_MEMORIES_QSPIMRAM_H
#define DRIVERS_MEMORIES_QSPIMRAM_H

/******************************* Include Files *******************************/

#include "drivers/common.h"
#include "core/irq.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** @brief QSPI_MRAM handle struct type redefinition */
typedef QSPI_HandleTypeDef qspiHandleStruct_t;

/** @brief QSPI_MRAM peripheral type redefinition (QSPI1, QSPI2, ...) */
typedef QUADSPI_TypeDef qspiPeriph_t;

/**
 * @struct  qspimramConf_t
 * @brief   Struct type definition of a QSPI_MRAM configuration
 */
typedef struct
{
    qspiPeriph_t *periph; /**< @brief Pointer to the QSPI_MRAM peripheral (QSPI1, QSPI2, ...) */
    IRQNo_t irq_no;       /**< @brief QSPI MRAM related interrupt */
    IRQPrio_t irq_prio;   /**< @brief QSPI MRAM related interrupt priority */
    uint32_t clk_src;     /**< @brief QSPI MRAM peripheral clock source */
    uint32_t prescaler;   /**< @brief QSPI MRAM peripheral clock prescaler */
    uint32_t size;        /**< @brief QSPI MRAM size in bytes */
    bool qpi;             /**< @brief QSPI MRAM enable QPI mode (4-4-4) */
    IOConf_t io_sck;      /**< @brief QSPI MRAM IO configuration for SCK */
    IOConf_t io_d0;       /**< @brief QSPI MRAM IO configuration for D0 */
    IOConf_t io_d1;       /**< @brief QSPI MRAM IO configuration for D1 */
    IOConf_t io_d2;       /**< @brief QSPI MRAM IO configuration for D2 */
    IOConf_t io_d3;       /**< @brief QSPI MRAM IO configuration for D3 */
    IOConf_t io_cs;       /**< @brief QSPI MRAM IO configuration for CS */
} qspimramConf_t;

/**
 * @struct  qspimramInst_t
 * @brief   Struct type definition of a QSPI_MRAM descriptor
 */
typedef struct
{
    qspiHandleStruct_t handle_struct; /**< @brief QSPI_MRAM handle struct used by HAL */
    const qspimramConf_t *p_conf;     /**< @brief Pointer to QSPI_MRAM conf */
} qspimramInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

/**
 * @fn              QspiMramOpen(qspimramInst_t *qspimram_inst, const qspimramConf_t *const qspimram_conf)
 * @brief           Function that initialise a QSPI MRAM memory
 * @param[in,out]   qspimram_inst   Instance that contains QSPI MRAM handlers
 * @param[in]       qspimram_conf   Configuration that contains QSPI MRAM parameters
 * @retval          #RET_SUCCESSFUL if creation succeed
 * @retval          #RET_INVALID_PARAM if a pointer is null
 */
extern returnCode_t QspiMramOpen(qspimramInst_t *qspimram_inst, const qspimramConf_t *const qspi_mram_conf);

/**
 * @fn          QspiMramWrite(qspimramInst_t *qspimram_inst, memorySector_t sector, data_t data, length_t length)
 * @brief       Function that writes onto an QSPI MRAM memory
 * @param[in]   qspimram_inst     Instance that contains QSPI MRAM parameters and QSPI MRAM Handler
 * @param[in]   sector      Sector numero from wich data will be read
 * @param[out]  data        Pointer from which data will be copied
 * @param[in]   length      Number of sector that will be read
 * @retval      #RET_SUCCESSFUL if data has been written successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if qspimram timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if qspimram is still sending previous message
 */
extern returnCode_t QspiMramWrite(qspimramInst_t *qspimram_inst, memorySector_t sector, data_t data, length_t length);

/**
 * @fn          QspiMramRead(qspimramInst_t *qspimram_inst, memorySector_t sector, data_t data, length_t length)
 * @brief       Function that read onto an QSPI MRAM memory
 * @param[in]   qspimram_inst     Instance that contains QSPI MRAM parameters and QSPI MRAM Handler
 * @param[in]   sector      Sector numero from wich data will be read
 * @param[out]  data        Pointer to where data will be copied
 * @param[in]   length      Number of sector that will be read
 * @retval      #RET_SUCCESSFUL if data has been read successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if qspimram timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if qspimram is still sending previous message
 */
extern returnCode_t QspiMramRead(qspimramInst_t *qspimram_inst, memorySector_t sector, data_t data, length_t length);

/**
 * @fn              QspiMramIoctl(qspimramInst_t *qspimram_inst, uint32_t cmd, void *data, uint32_t data_size);
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   qspimram_inst    Instance that contains QSPI MRAM handlers
 * @param[in]       cmd             IO Control command
 * @param[in,out]   data            IO Control command
 * @param[in]       data_size       IO Control data size
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if action cannot be performed because driver is busy
 * @retval          #RET_SUCCESSFUL else
 */
extern returnCode_t QspiMramIoctl(qspimramInst_t *qspimram_inst, uint32_t cmd, void *data, uint32_t data_size);

/**
 * @fn              QspiMramClose(qspimramInst_t *qspimram_inst)
 * @brief           Function that desinit the QSPI MRAM connection
 * @param[in,out]   qspimram_inst   Instance that contains QSPI MRAM handlers
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 */
extern returnCode_t QspiMramClose(qspimramInst_t *qspimram_inst);

#endif /* DRIVERS_MEMORIES_QSPIMRAM_H */

/**
 * @}
 * @}
 * @}
 */