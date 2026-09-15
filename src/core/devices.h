/**
 * @file    devices.h
 * @author  Merlin Kooshmanian
 * @brief   Header file defining devices
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 * @defgroup devices Devices Management
 * @brief Abstraction layer for managing devices using their corresponding peripheral.
 * @{
 */

#ifndef DEVICES_H
#define DEVICES_H

/******************************* Include Files *******************************/

#include "autoconf.h"
#include "kernel_types.h"
#include "core/buffers.h"
#include "fs/fs.h"
#include "drv/peripherals.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @struct  deviceDesc_t
 * @brief   Struct type of a device descriptors
 */
typedef struct
{
    descStatus_t status; /**< @brief Indicates if the descriptor is free or used */
    deviceType_t type;   /**< @brief Device type (either buffer, file, peripheral) */
    uint32_t resource;   /**< @brief The resource reference (buffer, file, or peripheral) */
} deviceDesc_t;

/*************************** Variables Declarations **************************/

/**
 * @var     g_devices_table
 * @brief   Devices descriptor table
 */
extern deviceDesc_t g_devices_table[CONFIG_MAX_NB_DEVICES];

/*************************** Functions Declarations **************************/

/**
 * @fn          DeviceOpen(deviceNo_t *device, deviceType_t type, uint32_t resource)
 * @brief       Function that creates a device for a given peripheral
 * @param[out]  device      Device numero that has been allocated during the creation
 * @param[in]   type        Device type : either buffer, file or peripheral
 * @param[in]   resource   Buffer, file or peripheral to which to link
 * @retval      #RET_INVALID_PARAM if device is a null pointer or peripheral does not exist
 * @retval      #RET_SUCCESSFUL else
 */
extern returnCode_t DeviceOpen(deviceNo_t *device, deviceType_t type, uint32_t resource);

/**
 * @fn          DeviceWrite(deviceNo_t device, data_t data, length_t length)
 * @brief       Function that writes data to a device
 * @param[in]   device  Device numero
 * @param[in]   data    Data that will be sent to the device
 * @param[in]   length  Length of the data
 * @retval      #RET_INVALID_PARAM if data is a null pointer or device is not valid
 * @retval      #RET_TIMEOUT if writing the device timeouted before sending all data (data sent may be incomplete)
 * @retval      #RET_NOT_AVAILABLE if writing the device is still occuring (data are not fully sent yet)
 * @retval      #RET_SUCCESSFUL else
 */
extern returnCode_t DeviceWrite(deviceNo_t device, data_t data, length_t length);

/**
 * @fn          DeviceRead(deviceNo_t device, data_t data, length_t length)
 * @brief       Function that reads data to a device
 * @param[in]   device  Device numero
 * @param[out]  data    Data that will be received to the device
 * @param[in]   length  Length of the data
 * @retval      #RET_INVALID_PARAM if data is a null pointer or device is not valid
 * @retval      #RET_TIMEOUT if reading the device timeouted before receiving any data
 * @retval      #RET_NOT_AVAILABLE if reading the device is still occuring (data are not available yet)
 * @retval      #RET_SUCCESSFUL else
 */
extern returnCode_t DeviceRead(deviceNo_t device, data_t data, length_t length);

/**
 * @fn              DeviceIoctl(deviceNo_t device, uint32_t cmd, void *data, uint32_t data_size)
 * @brief           Function that allows specific control over the device
 * @param[in]       device      Device numero
 * @param[in]       cmd         IO control command
 * @param[in,out]   data        Data related to the command (if any), can be input or output
 * @param[in]       data_size   Data length (if any)
 * @retval          #RET_INVALID_PARAM if device is not valid
 * @retval          #RET_TIMEOUT if ioctl the device timeouted before receiving any data
 * @retval          #RET_NOT_AVAILABLE if ioctl the device is still occuring
 * @retval          #RET_SUCCESSFUL else
 */
extern returnCode_t DeviceIoctl(deviceNo_t device, uint32_t cmd, void *data, uint32_t data_size);

/**
 * @fn          DeviceClose(deviceNo_t device)
 * @brief       Function that will remove the device
 * @param[in]   device  Device numero
 * @retval      #RET_SUCCESSFUL always
 */
extern returnCode_t DeviceClose(deviceNo_t device);

#endif /* DEVICES_H */

/**
 * @}
 * @}
 * @}
 */