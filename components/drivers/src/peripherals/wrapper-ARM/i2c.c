/**
 * @file    i2c.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for I2C functions
 *
 * @copyright Copyright (c) TOLOSAT 2026
 * SPDX-License-Identifier: Apache-2.0
 */

/******************************* Include Files *******************************/

#include "drivers/peripherals/i2c.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @copydoc I2cOpen
 */
returnCode_t I2cOpen(i2cInst_t *i2c_inst, const i2cConf_t *const i2c_conf)
{
    (void)(i2c_inst);
    (void)(i2c_conf);
    return RET_SUCCESSFUL;
}

/**
 * @copydoc I2cWrite
 */
returnCode_t I2cWrite(i2cInst_t *i2c_inst, data_t data, length_t length)
{
    (void)(i2c_inst);
    (void)(data);
    (void)(length);
    return RET_SUCCESSFUL;
}

/**
 * @copydoc I2cRead
 */
returnCode_t I2cRead(i2cInst_t *i2c_inst, data_t data, length_t length)
{
    (void)(i2c_inst);
    (void)(data);
    (void)(length);
    return RET_SUCCESSFUL;
}

/**
 * @copydoc I2cIoctl
 */
returnCode_t I2cIoctl(i2cInst_t *i2c_inst, uint32_t cmd, void *data, uint32_t data_size)
{
    (void)(i2c_inst);
    (void)(cmd);
    (void)(data);
    (void)(data_size);
    return RET_SUCCESSFUL;
}

/**
 * @copydoc I2cClose
 */
returnCode_t I2cClose(i2cInst_t *i2c_inst)
{
    (void)(i2c_inst);
    return RET_SUCCESSFUL;
}
