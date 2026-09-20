/**
 * @file    spi.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for SPI functions
 *
 * @copyright Copyright (c) TOLOSAT 2026
 * SPDX-License-Identifier: Apache-2.0
 */

/******************************* Include Files *******************************/

#include "drivers/peripherals/spi.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @copydoc SpiOpen
 */
returnCode_t SpiOpen(spiInst_t *spi_inst, const spiConf_t *const spi_conf)
{
    (void)(spi_inst);
    (void)(spi_conf);
    return RET_SUCCESSFUL;
}

/**
 * @copydoc SpiWrite
 */
returnCode_t SpiWrite(spiInst_t *spi_inst, data_t data, length_t length)
{
    (void)(spi_inst);
    (void)(data);
    (void)(length);
    return RET_SUCCESSFUL;
}

/**
 * @copydoc SpiRead
 */
returnCode_t SpiRead(spiInst_t *spi_inst, data_t data, length_t length)
{
    (void)(spi_inst);
    (void)(data);
    (void)(length);
    return RET_SUCCESSFUL;
}

/**
 * @copydoc SpiIoctl
 */
returnCode_t SpiIoctl(spiInst_t *spi_inst, uint32_t cmd, void *data, uint32_t data_size)
{
    (void)(spi_inst);
    (void)(cmd);
    (void)(data);
    (void)(data_size);
    return RET_SUCCESSFUL;
}

/**
 * @copydoc SpiClose
 */
returnCode_t SpiClose(spiInst_t *spi_inst)
{
    (void)(spi_inst);
    return RET_SUCCESSFUL;
}
