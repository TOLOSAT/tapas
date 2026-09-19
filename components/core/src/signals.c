/**
 * @file    signals.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for signals handling functions
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

/******************************* Include Files *******************************/

#include "core/signals.h"
#include "core/tasks.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @copydoc SendSignal
 */
returnCode_t SendSignal(taskNo_t task, signalMask_t mask)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    BaseType_t test_value;

    // Check parameter(s)
    if ((IS_A_VALID_TASK(task)) && (mask != 0u))
    {
        // Check if we are in an interrupt context or not
        if (xPortIsInsideInterrupt() == pdFALSE)
        {
            test_value = xTaskNotify(TASK_DESC(task).handle, mask, eSetBits);
            if (test_value != pdPASS)
            {
                KernelPanic();
            }
        }
        else
        {
            BaseType_t higher_priority_task_woken = pdFALSE;

            test_value = xTaskNotifyFromISR(TASK_DESC(task).handle, mask, eSetBits, &higher_priority_task_woken);
            if (test_value != pdPASS)
            {
                KernelPanic();
            }

            // If an higher priority task has been woken, the interrupted
            // task need to yield in order to not create delays.
            portYIELD_FROM_ISR(higher_priority_task_woken);
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @copydoc WaitSignal
 */
returnCode_t WaitSignal(signalMask_t mask)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    BaseType_t test_value;

    // Check parameter(s)
    if ((mask != 0u))
    {
        signalMask_t received_signals = 0u;

        // Wait until one of the specified signals is received.
        do
        {
            // Read currently latched signals without clearing them.
            received_signals = (signalMask_t)ulTaskNotifyValueClear(NULL, 0u);
            if ((received_signals & mask) == 0u)
            {
                // Wait until one of the specified signals is received.
                test_value = xTaskNotifyWait(0u, 0u, &received_signals, portMAX_DELAY);
                if (test_value != pdPASS)
                {
                    KernelPanic();
                }
            }
        } while ((received_signals & mask) == 0u);

        // Clear only the consumed signals and keep the others pending.
        (void)ulTaskNotifyValueClear(NULL, (received_signals & mask));
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}
