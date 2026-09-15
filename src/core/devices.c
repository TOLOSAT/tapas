/**
 * @file    devices.c
 * @author  Merlin Kooshmanian
 * @brief   Source file defining devices
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "core/devices.h"
#include "core/buffers.h"
#include "fs/fs.h"
#include "drv/peripherals.h"
#include "mc/mon.h"
#include "mc/info.h"
#include "fdir/context.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static returnCode_t SystemDeviceWrite(uint32_t sysdev, data_t data, length_t length);
static returnCode_t SystemDeviceRead(uint32_t sysdev, data_t data, length_t length);
static returnCode_t SystemDeviceIoctl(uint32_t sysdev, uint32_t cmd, void *data, uint32_t data_size);

/*************************** Variables Definitions ***************************/

/**
 * @copydoc g_devices_table
 */
deviceDesc_t g_devices_table[CONFIG_MAX_NB_DEVICES] = { 0 };

/*************************** Functions Definitions ***************************/

/**
 * @copydoc DeviceOpen
 */
returnCode_t DeviceOpen(deviceNo_t *device, deviceType_t type, uint32_t resource)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (device != NULL)
    {
        deviceNo_t new_device = 0u;

        // Look for an available device descriptor
        return_value = RET_NOT_AVAILABLE;
        while ((new_device < (deviceNo_t)CONFIG_MAX_NB_DEVICES) && (return_value == RET_NOT_AVAILABLE))
        {
            // Check if descriptor free
            if (g_devices_table[new_device].status == DESC_FREE)
            {
                // Allocate new device
                g_devices_table[new_device].type     = type;
                g_devices_table[new_device].resource = resource;
                g_devices_table[new_device].status   = DESC_USED;
                *device                              = new_device;
                return_value                         = RET_SUCCESSFUL;
            }
            else
            {
                // Continue to look for a free device
                new_device++;
            }
        }

        // If no more device are available.
        if (return_value == RET_NOT_AVAILABLE)
        {
            KernelPanic();
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @copydoc DeviceWrite
 */
returnCode_t DeviceWrite(deviceNo_t device, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((data != NULL) && (device < (deviceNo_t)CONFIG_MAX_NB_DEVICES) && (g_devices_table[device].status != DESC_FREE))
    {
        switch (g_devices_table[device].type)
        {
            case DEVICE_TYPE_BUFFER :
                return_value = BufferWrite(g_devices_table[device].resource, data, length);
                break;
            case DEVICE_TYPE_FILE :
                return_value = FsWrite(g_devices_table[device].resource, data, length);
                break;
            case DEVICE_TYPE_PERIPHERAL :
                return_value = PeripheralWrite(g_devices_table[device].resource, data, length);
                break;
            case DEVICE_TYPE_SYSTEM :
                return_value = SystemDeviceWrite(g_devices_table[device].resource, data, length);
                break;
            default :
                return_value = RET_INVALID_PARAM;
                break;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @copydoc DeviceRead
 */
returnCode_t DeviceRead(deviceNo_t device, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((data != NULL) && (device < (deviceNo_t)CONFIG_MAX_NB_DEVICES) && (g_devices_table[device].status != DESC_FREE))
    {
        switch (g_devices_table[device].type)
        {
            case DEVICE_TYPE_BUFFER :
                return_value = BufferRead(g_devices_table[device].resource, data, length);
                break;
            case DEVICE_TYPE_FILE :
                return_value = FsRead(g_devices_table[device].resource, data, length);
                break;
            case DEVICE_TYPE_PERIPHERAL :
                return_value = PeripheralRead(g_devices_table[device].resource, data, length);
                break;
            case DEVICE_TYPE_SYSTEM :
                return_value = SystemDeviceRead(g_devices_table[device].resource, data, length);
                break;
            default :
                return_value = RET_INVALID_PARAM;
                break;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @copydoc DeviceIoctl
 */
returnCode_t DeviceIoctl(deviceNo_t device, uint32_t cmd, void *data, uint32_t data_size)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((device < (deviceNo_t)CONFIG_MAX_NB_DEVICES) && (g_devices_table[device].status != DESC_FREE))
    {
        switch (g_devices_table[device].type)
        {
            case DEVICE_TYPE_BUFFER :
                return_value = BufferIoctl(g_devices_table[device].resource, cmd, data, data_size);
                break;
            case DEVICE_TYPE_FILE :
                return_value = FsIoctl(g_devices_table[device].resource, cmd, data, data_size);
                break;
            case DEVICE_TYPE_PERIPHERAL :
                return_value = PeripheralIoctl(g_devices_table[device].resource, cmd, data, data_size);
                break;
            case DEVICE_TYPE_SYSTEM :
                return_value = SystemDeviceIoctl(g_devices_table[device].resource, cmd, data, data_size);
                break;
            default :
                return_value = RET_INVALID_PARAM;
                break;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @copydoc DeviceClose
 */
returnCode_t DeviceClose(deviceNo_t device)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Reset device
    g_devices_table[device].resource = 0u;
    g_devices_table[device].status   = DESC_FREE;

    return return_value;
}

/**
 * @fn          SystemDeviceWrite(uint32_t sysdev, data_t data, length_t length)
 * @brief       Function that writes information onto a system device
 * @param[in]   sysdev      System device numero
 * @param[in]   data        Data that will be write onto the system device
 * @param[in]   length      Length of the data
 * @retval      #RET_INVALID_PARAM if data is a null pointer or length is zero
 * @retval      #RET_INVALID_PARAM if the system device does not exist
 * @retval      #RET_NOT_AVAILABLE if cannot write into the system device (because read-only)
 * @retval      #RET_SUCCESSFUL else
 */
static returnCode_t SystemDeviceWrite(uint32_t sysdev, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused for the moment
    (void)(data);
    (void)(length);

    // Check parameter(s)
    if ((data != NULL) && (length != 0u))
    {
        switch (sysdev)
        {
            case SYSDEV_SYSTEM_INFO :
                // System information is read only
                return_value = RET_NOT_AVAILABLE;
                break;
            case SYSDEV_SYSTEM_USAGE :
                // System information is read only
                return_value = RET_NOT_AVAILABLE;
                break;
            case SYSDEV_TASK_USAGES :
                // Task information is read only
                return_value = RET_NOT_AVAILABLE;
                break;
            case SYSDEV_SYSTEM_REBOOT :
                // System reboot is execution only
                return_value = RET_NOT_AVAILABLE;
                break;
            case SYSDEV_SYSTEM_CONTEXT :
                // Check size
                if (length == sizeof(context_t))
                {
                    // Write system context
                    context_t system_context = { 0 };
                    (void)memcpy(&system_context, (void *)data, length);
                    return_value = WriteContext(&system_context);
                }

                break;
            default :
                return_value = RET_INVALID_PARAM;
                break;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SystemDeviceRead(uint32_t sysdev, data_t data, length_t length)
 * @brief       Function that reads information from a system device
 * @param[in]   sysdev      System device numero
 * @param[in]   data        Data that will be read from the system device
 * @param[in]   length      Length of the data
 * @retval      #RET_INVALID_PARAM if data is a null pointer or length is zero
 * @retval      #RET_INVALID_PARAM if the system device does not exist
 * @retval      #RET_SUCCESSFUL else
 */
static returnCode_t SystemDeviceRead(uint32_t sysdev, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((data != NULL) && (length != 0u))
    {
        switch (sysdev)
        {
            case SYSDEV_SYSTEM_INFO :
                // Check size
                if (length == sizeof(sytemInfo_t))
                {
                    (void)memcpy((void *)data, &g_system_info, sizeof(sytemInfo_t));
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case SYSDEV_SYSTEM_USAGE :
                // Check size
                if (length == sizeof(systemUsage_t))
                {
                    (void)memcpy((void *)data, &g_system_usage, sizeof(systemUsage_t));
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case SYSDEV_TASK_USAGES :
                // Check size
                if ((length != 0u) && (length % sizeof(taskUsage_t) == 0u) && (length <= (sizeof(taskUsage_t) * (length_t)CONFIG_MAX_NB_TASKS)))
                {
                    (void)memcpy((void *)data, &g_task_usages, length);
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case SYSDEV_SYSTEM_REBOOT :
                // System reboot is execution only
                return_value = RET_NOT_AVAILABLE;
                break;
            case SYSDEV_SYSTEM_CONTEXT :
                // Check size
                if ((0u < length) && (length <= sizeof(context_t)))
                {
                    // Read system context
                    context_t system_context = { 0 };
                    return_value             = ReadContext(&system_context);

                    (void)memcpy((void *)data, &system_context, length);
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            default :
                return_value = RET_INVALID_PARAM;
                break;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              SystemDeviceIoctl(uint32_t sysdev, uint32_t cmd, void *data, uint32_t data_size)
 * @brief           Function that allows specific control over a system device
 * @param[in]       sysdev      System device numero
 * @param[in]       cmd         IO control command
 * @param[in,out]   data        Data related to the command (if any), can be input or output
 * @param[in]       data_size   Data length (if any)
 * @retval          #RET_INVALID_PARAM if the system device does not exist
 * @retval          #RET_NOT_AVAILABLE if cannot ioctl the system device (because read-only)
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t SystemDeviceIoctl(uint32_t sysdev, uint32_t cmd, void *data, uint32_t data_size)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused for the moment
    (void)(cmd);
    (void)(data);
    (void)(data_size);

    // Check parameter(s)
    switch (sysdev)
    {
        case SYSDEV_SYSTEM_INFO :
            // System information is read only
            return_value = RET_NOT_AVAILABLE;
            break;
        case SYSDEV_SYSTEM_USAGE :
            // System information is read only
            return_value = RET_NOT_AVAILABLE;
            break;
        case SYSDEV_TASK_USAGES :
            // Task information is read only
            return_value = RET_NOT_AVAILABLE;
            break;
        case SYSDEV_SYSTEM_REBOOT :
            NVIC_SystemReset();
            break;
        case SYSDEV_SYSTEM_CONTEXT :
            // Erase the context memory
            return_value = EraseContext();
            break;
        default :
            return_value = RET_INVALID_PARAM;
            break;
    }

    return return_value;
}
