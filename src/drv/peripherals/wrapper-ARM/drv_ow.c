/**
 * @file    drv_ow.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for OW functions
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

/******************************* Include Files *******************************/

#include "drv/peripherals/drv_ow.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @copydoc OwOpen
 */
returnCode_t OwOpen(owInst_t *ow_inst, const owConf_t *const ow_conf)
{
    (void)(ow_inst);
    (void)(ow_conf);
    return RET_SUCCESSFUL;
}

/**
 * @copydoc OwWrite
 */
returnCode_t OwWrite(owInst_t *ow_inst, data_t data, length_t length)
{
    (void)(ow_inst);
    (void)(data);
    (void)(length);
    return RET_SUCCESSFUL;
}

/**
 * @copydoc OwRead
 */
returnCode_t OwRead(owInst_t *ow_inst, data_t data, length_t length)
{
    (void)(ow_inst);
    (void)(data);
    (void)(length);
    return RET_SUCCESSFUL;
}

/**
 * @copydoc OwIoctl
 */
returnCode_t OwIoctl(owInst_t *ow_inst, uint32_t cmd, void *data, uint32_t data_size)
{
    (void)(ow_inst);
    (void)(cmd);
    (void)(data);
    (void)(data_size);
    return RET_SUCCESSFUL;
}

/**
 * @copydoc OwClose
 */
returnCode_t OwClose(owInst_t *ow_inst)
{
    (void)(ow_inst);
    return RET_SUCCESSFUL;
}
