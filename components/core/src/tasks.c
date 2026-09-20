/**
 * @file    tasks.c
 * @author  Merlin Kooshmanian
 * @brief   Source file defining tasks
 *
 * @copyright Copyright (c) TOLOSAT 2026
 * SPDX-License-Identifier: Apache-2.0
 */

/******************************* Include Files *******************************/

#include "core/tasks.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

extern void vInitTaskPrivilege(TaskHandle_t xTask, BaseType_t xRunPrivileged);

/*************************** Variables Definitions ***************************/

/**
 * @copydoc g_tasks_desc_table
 */
taskDesc_t g_tasks_desc_table[CONFIG_MAX_NB_TASKS] = { 0 };

/*************************** Functions Definitions ***************************/

/**
 * @copydoc CreateTasks
 */
void CreateTasks(void)
{
    taskNo_t task = 1u;

    // Create statically every task
    while (TASK_CONF(task).task != NO_TASK)
    {
        // The stack depth is not in bytes but in words (16 bits, 32 bits, 64 bits
        // depending on the architecture), so stack size need to be a multiple of
        // sizeof(StackType_t)
        if ((TASK_CONF(task).stack_size % sizeof(StackType_t)) == 0u)
        {
            // Create task
            TASK_DESC(task).handle = xTaskCreateStatic(TASK_CONF(task).function,                            // Task function
                                                       TASK_CONF(task).name,                                // Task main
                                                       TASK_CONF(task).stack_size / sizeof(StackType_t),    // Task stack size
                                                       NULL,                                                // Task function parameter
                                                       TASK_CONF(task).priority,                            // Task priority
                                                       TASK_CONF(task).p_stack,                             // Task stack
                                                       (StaticTask_t *)pvPortMalloc(sizeof(StaticTask_t))); // Task TCB
            if (TASK_DESC(task).handle == NULL)
            {
                KernelPanic();
            }
            // Set task number in task handle (for easier task recognition)
            vTaskSetTaskNumber(TASK_DESC(task).handle, task);
            // Init task privilege
            vInitTaskPrivilege(TASK_DESC(task).handle, TASK_CONF(task).privilege);
            // Set period
            TASK_DESC(task).period = TASK_CONF(task).default_period;

            // Indicates the task is initialised
            TASK_DESC(task).status = DESC_USED;
            task++;
        }
        else
        {
            KernelPanic();
        }
    }
}

/**
 * @copydoc GetCurrentTask
 */
taskNo_t GetCurrentTask(void)
{
    taskNo_t task = NO_TASK;

    // Get current task number
    task = uxTaskGetTaskNumber(xTaskGetCurrentTaskHandle());
    if (!IS_A_VALID_TASK(task))
    {
        task = NO_TASK;
    }

    return task;
}

/**
 * @copydoc SuspendTask
 */
returnCode_t SuspendTask(taskNo_t task)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (IS_A_VALID_TASK(task))
    {
        // Update task mode for a soft suspension
        TASK_DESC(task).mode = TASK_SUSPENDED;
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @copydoc ResumeTask
 */
returnCode_t ResumeTask(taskNo_t task)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (IS_A_VALID_TASK(task))
    {
        // Update task mode
        TASK_DESC(task).mode = TASK_NOMINAL;

        // Unlock the task
        vTaskResume(TASK_DESC(task).handle);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @copydoc GetTaskPriority
 */
returnCode_t GetTaskPriority(taskNo_t task, taskPriority_t *priority)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (IS_A_VALID_TASK(task))
    {
        *priority = uxTaskPriorityGet(TASK_DESC(task).handle);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @copydoc SetTaskPriority
 */
returnCode_t SetTaskPriority(taskNo_t task, taskPriority_t priority)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (IS_A_VALID_TASK(task))
    {
        vTaskPrioritySet(TASK_DESC(task).handle, priority);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}
