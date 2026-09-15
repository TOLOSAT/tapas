/**
 * @file    tasks.h
 * @author  Merlin Kooshmanian
 * @brief   Header file defining tasks
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup core Core
 * @{
 * @defgroup tasks Tasks
 * @brief Tasks handling interface.
 * @{
 */

#ifndef TASKS_H
#define TASKS_H

/******************************* Include Files *******************************/

#include "autoconf.h"
#include "kernel_types.h"
#include "core/os.h"

/***************************** Macros Definitions ****************************/

/**
 * @def     TASK_CONF(task_no)
 * @brief   Get task conf from g_tasks_conf_table
 */
#define TASK_CONF(task_no) (g_tasks_conf_table[(task_no) - 1u])

/**
 * @def     TASK_DESC(task_no)
 * @brief   Get task conf from g_tasks_desc_table
 */
#define TASK_DESC(task_no) (g_tasks_desc_table[(task_no) - 1u])

/**
 * @def     IS_A_VALID_TASK(task_no)
 * @brief   Indicates if the task_no is valid
 */
#define IS_A_VALID_TASK(task_no) \
    (((task_no) != (taskNo_t)NO_TASK) && ((task_no) < (taskNo_t)CONFIG_MAX_NB_TASKS) && (TASK_DESC(task_no).status == DESC_USED))

/***************************** Types Definitions *****************************/

/**
 * @enum    taskMode_t
 * @brief   Task Modes
 */
typedef enum
{
    TASK_NOMINAL   = 0u, /**< Task is in NOMINAL mode */
    TASK_SAFE      = 1u, /**< Task is in SAFE mode */
    TASK_SUSPENDED = 2u, /**< Task is in SUSPENDED mode */
} taskMode_t;

/** @brief Task Handle type */
typedef TaskHandle_t taskHandle_t;

/**
 * @struct  taskDesc_t
 * @brief   Struct type of a task descriptors
 */
typedef struct
{
    descStatus_t status;     /**< @brief Indicates if the descriptor is free or used */
    taskHandle_t handle;     /**< @brief Task handle */
    taskMode_t mode;         /**< @brief Task mode */
    tick_t period;           /**< @brief Task period in ticks */
    tick_t last_wake;        /**< @brief Last time the task was waken in ticks */
    uint32_t syscall_tmp_lr; /**< @brief Store temporarily the LR when executing a syscall */
} taskDesc_t;

/*************************** Variables Declarations **************************/

/**
 * @var     g_tasks_conf_table
 * @brief   Table where all tasks configuration are stored
 */
extern const taskConf_t g_tasks_conf_table[CONFIG_MAX_NB_TASKS];

/**
 * @var     g_tasks_desc_table
 * @brief   Table where all tasks descriptors are stored
 */
extern taskDesc_t g_tasks_desc_table[CONFIG_MAX_NB_TASKS];

/*************************** Functions Declarations **************************/

/**
 * @fn      CreateTasks(void)
 * @brief   Function that initialises the tasks
 * @return  Nothing
 */
extern void CreateTasks(void);

/**
 * @fn          GetCurrentTask(void)
 * @brief       Functions that gets the task no of the current task
 * @return      Current task
 *
 * @note If a task is not registered by the TAPAS API, it means either it's a FreeRTOS internal task or badly initialised task
 */
extern taskNo_t GetCurrentTask(void);

/**
 * @fn          SuspendTask(taskNo_t task)
 * @brief       Function that allow to suspend an active task
 * @param[in]   task    Reference of the task (in TASKS_ENUM)
 * @retval      #RET_SUCCESSFUL if halt is successful
 * @retval      #RET_INVALID_PARAM if task ref does not exist
 */
extern returnCode_t SuspendTask(taskNo_t task);

/**
 * @fn          ResumeTask(taskNo_t task)
 * @brief       Function that allow to resume a suspended tasks
 * @param[in]   task    Reference of the task (in TASKS_ENUM)
 * @retval      #RET_SUCCESSFUL if resume is successful
 * @retval      #RET_INVALID_PARAM if task does not exist
 */
extern returnCode_t ResumeTask(taskNo_t task);

/**
 * @fn          GetTaskPriority(taskNo_t task, taskPriority_t *priority)
 * @brief       Function that allows to get task priority
 * @param[in]   task        Reference of the task (in TASKS_ENUM)
 * @param[out]  priority    Current priority of the task
 * @retval      #RET_SUCCESSFUL if get is successful
 * @retval      #RET_INVALID_PARAM if task does not exist
 */
extern returnCode_t GetTaskPriority(taskNo_t task, taskPriority_t *priority);

/**
 * @fn          SetTaskPriority(taskNo_t task, taskPriority_t priority)
 * @brief       Function that allows to change task priority
 * @param[in]   task        Reference of the task (in TASKS_ENUM)
 * @param[in]   priority    New priority of the task
 * @retval      #RET_SUCCESSFUL if set is successful
 * @retval      #RET_INVALID_PARAM if task does not exist or if priority < IDLE or priority > ISR
 */
extern returnCode_t SetTaskPriority(taskNo_t task, taskPriority_t priority);

#endif /* TASKS_H */

/**
 * @}
 * @}
 * @}
 */