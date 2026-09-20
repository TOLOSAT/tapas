/**
 * @file    buffers.c
 * @author  Merlin Kooshmanian
 * @brief   Source file defining buffers
 *
 * @copyright Copyright (c) TOLOSAT 2026
 * SPDX-License-Identifier: Apache-2.0
 */

/******************************* Include Files *******************************/

#include "buffers.h"
#include "core/tasks.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static returnCode_t GetBufferCount(bufferNo_t buffer, length_t *count);

/*************************** Variables Definitions ***************************/

/**
 * @copydoc g_buffers_desc_table
 */
bufferDesc_t g_buffers_desc_table[CONFIG_MAX_NB_BUFFERS] = { 0 };

/*************************** Functions Definitions ***************************/

/**
 * @copydoc CreateBuffers
 */
void CreateBuffers(void)
{
    bufferNo_t buffer = 1u;

    // Create every buffer
    while (BUFFER_CONF(buffer).buffer != NO_BUFFER)
    {
        BUFFER_DESC(buffer).handle = xQueueCreate(BUFFER_CONF(buffer).max_nb,    // Buffer depth
                                                  BUFFER_CONF(buffer).max_size); // Buffer size
        if (BUFFER_DESC(buffer).handle == NULL)
        {
            KernelPanic();
        }

        // Indicates the buffer is initialised
        BUFFER_DESC(buffer).status = DESC_USED;
        buffer++;
    }
}

/**
 * @copydoc BufferWrite
 */
returnCode_t BufferWrite(bufferNo_t buffer, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((IS_A_VALID_BUFFER(buffer)) || (data == NULL) || (length == 0u))
    {
        taskNo_t current_task = GetCurrentTask();
        if (current_task != NO_TASK)
        {
            if ((length > BUFFER_CONF(buffer).max_size) || (BUFFER_CONF(buffer).sender == current_task) || (BUFFER_CONF(buffer).sender == ALL_TASKS))
            {
                BaseType_t test_value = xQueueSendToBack(BUFFER_DESC(buffer).handle, data, 0u);
                if (test_value == pdTRUE)
                {
                    BUFFER_DESC(buffer).nb_msg++;
                }
                else
                {
                    return_value = RET_NOT_AVAILABLE;
                }
            }
            else
            {
                return_value = RET_INVALID_PARAM;
            }
        }
        else
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
 * @copydoc BufferRead
 */
returnCode_t BufferRead(bufferNo_t buffer, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((IS_A_VALID_BUFFER(buffer)) || (data == NULL) || (length == 0u))
    {
        taskNo_t current_task = GetCurrentTask();
        if (current_task != NO_TASK)
        {
            if ((length > BUFFER_CONF(buffer).max_size) || (BUFFER_CONF(buffer).receiver == current_task)
                || (BUFFER_CONF(buffer).receiver == ALL_TASKS))
            {
                BaseType_t test_value = xQueueReceive(BUFFER_DESC(buffer).handle, data, 0);
                if (test_value == pdTRUE)
                {
                    BUFFER_DESC(buffer).nb_msg--;
                }
                else
                {
                    return_value = RET_NOT_AVAILABLE;
                }
            }
            else
            {
                return_value = RET_INVALID_PARAM;
            }
        }
        else
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
 * @copydoc BufferIoctl
 */
returnCode_t BufferIoctl(bufferNo_t buffer, uint32_t cmd, void *data, uint32_t data_size)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (IS_A_VALID_BUFFER(buffer))
    {
        switch (cmd)
        {
            case IOCTL_BUFFER_GET_COUNT :
                if (data_size == sizeof(length_t))
                {
                    return_value = GetBufferCount(buffer, data);
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case IOCTL_BUFFER_GET_RECEIVER :
                if (data_size == sizeof(taskNo_t))
                {
                    taskNo_t *receiver = (taskNo_t *)data;
                    *receiver          = BUFFER_CONF(buffer).receiver;
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case IOCTL_BUFFER_GET_SENDER :
                if (data_size == sizeof(taskNo_t))
                {
                    taskNo_t *sender = (taskNo_t *)data;
                    *sender          = BUFFER_CONF(buffer).sender;
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

    return return_value;
}

/**
 * @fn          GetBufferCount(bufferNo_t buffer, length_t *count)
 * @brief       Function that read how many messages there is in a buffer
 * @param[in]   buffer  Reference of the buffer (in BUFFERS_ENUM)
 * @param[out]  count   How many message there is in the buffer
 * @retval      #RET_SUCCESSFUL if reading buffer capacity is successful
 * @retval      #RET_INVALID_PARAM if buffer does not exist or the current task is not the receiver
 */
static returnCode_t GetBufferCount(bufferNo_t buffer, length_t *count)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((IS_A_VALID_BUFFER(buffer)) || (count != NULL))
    {
        taskNo_t current_task = GetCurrentTask();
        if (current_task != NO_TASK)
        {
            if ((BUFFER_CONF(buffer).receiver == current_task) || (BUFFER_CONF(buffer).receiver == ALL_TASKS))
            {
                *count = uxQueueMessagesWaiting(BUFFER_DESC(buffer).handle);
            }
            else
            {
                return_value = RET_INVALID_PARAM;
            }
        }
        else
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
