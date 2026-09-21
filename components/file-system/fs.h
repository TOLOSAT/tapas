/**
 * @file    fs.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TOLOSAT File System functions
 *
 * @copyright Copyright (c) TOLOSAT 2026
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup file-system File System
 * @{
 * @defgroup fs_handling File System Handling
 * @brief File-system handling interface.
 * @{
 */

#ifndef FS_H
#define FS_H

/******************************* Include Files *******************************/

#include <ff.h>
#include <ffconf.h>
#include <ff_gen_drv.h>

#include "kernel_autoconf.h"
#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/**
 * @def     FILE_CONF(file_no)
 * @brief   Get file conf from g_files_conf_table
 */
#define FILE_CONF(file_no) (g_files_conf_table[(file_no) - 1u])

/**
 * @def     FILE_DESC(file_no)
 * @brief   Get file conf from g_files_desc_table
 */
#define FILE_DESC(file_no) (g_files_desc_table[(file_no) - 1u])

/**
 * @def     IS_A_VALID_FILE(file_no)
 * @brief   Indicates if the file_no is valid
 */
#define IS_A_VALID_FILE(file_no) \
    (((file_no) != (fileNo_t)NO_FILE) && ((file_no) < (fileNo_t)CONFIG_MAX_NB_FILES) && (FILE_DESC(file_no).status == DESC_USED))

/***************************** Types Definitions *****************************/

/**
 * @struct  fsInst_t
 * @brief   Struct type definition of a FS instance
 */
typedef struct
{
    char disk_path[4];        /**< @brief FS disk path */
    Diskio_drvTypeDef driver; /**< @brief FS driver */
    FATFS file_system;        /**< @brief FS */
} fsInst_t;

/**
 * @struct  fsFileDesc_t
 * @brief   Struct type of a file descriptor
 */
typedef struct
{
    descStatus_t status; /**< @brief Indicates if the descriptor is free or used */
    FIL temp_file;       /**< @brief Pointer to the temporary file */
} fsFileDesc_t;

/*************************** Variables Declarations **************************/

/**
 * @var     g_files_conf_table
 * @brief   Configuration table where all file configuration are stored
 */
extern const fsFileConf_t g_files_conf_table[CONFIG_MAX_NB_FILES];

/**
 * @var     g_files_desc_table
 * @brief   Descriptor table where all file descriptors are stored
 */
extern fsFileDesc_t g_files_desc_table[CONFIG_MAX_NB_FILES];

/*************************** Functions Declarations **************************/

/**
 * @fn              InitFs(void)
 * @brief           Function that initialise a FS
 * @return          Nothing
 */
extern void InitFs(void);

/**
 * @fn      CreateFsMutexes(void)
 * @brief   Function that allows to postpone mutex initilisation when other mutexes will be initialised.
 */
extern void CreateFsMutexes(void);

/**
 * @fn          FsWrite(fileNo_t file, data_t data, length_t length)
 * @brief       Function that write into a file of the fS
 * @param[in]   file    File reference numero
 * @param[in]   data    Pointer to data which will be written
 * @param[in]   length  Length of data
 * @retval      #RET_INVALID_PARAM if the file is not valid
 * @retval      #RET_INVALID_PARAM if a parameter is null pointer or data length is null
 * @retval      #RET_SUCCESSFUL else
 */
extern returnCode_t FsWrite(fileNo_t file, data_t data, length_t length);

/**
 * @fn          FsRead(fileNo_t file, data_t data, length_t length)
 * @brief       Function that read from a file of the fS
 * @param[in]   file    File reference numero
 * @param[out]  data    Pointer to data which will be read
 * @param[in]   length  Length of data
 * @retval      #RET_INVALID_PARAM if the file is not valid
 * @retval      #RET_INVALID_PARAM if a parameter is null pointer or data length is null
 * @retval      #RET_NOT_AVAILABLE if the section of the file asked does not exist
 * @retval      #RET_SUCCESSFUL else
 */
extern returnCode_t FsRead(fileNo_t file, data_t data, length_t length);

/**
 * @fn              FsIoctl(fileNo_t file, uint32_t cmd, void *data, uint32_t data_size)
 * @brief           Function that adds advanced control to manage a file
 * @param[in]       file        File reference numero
 * @param[in]       cmd         IO Control command
 * @param[in,out]   data        IO Control command
 * @param[in]       data_size   IO Control data length
 * @retval          #RET_INVALID_PARAM if the file is not valid
 * @retval          #RET_INVALID_PARAM if a pointer is null
 * @retval          #RET_SUCCESSFUL else
 */
extern returnCode_t FsIoctl(fileNo_t file, uint32_t cmd, void *data, uint32_t data_size);

/**
 * @fn          DeinitFs(void)
 * @brief       Function that desinit the disk (and FS) connection and puts defaults parameters
 * @retval      #RET_SUCCESSFUL else
 */
extern returnCode_t DeinitFs(void);

#endif /* FS_H */

/**
 * @}
 * @}
 * @}
 */