/**
 * @file    drv_nand.h
 * @author  Merlin Kooshmanian
 * @brief   Source file for NAND memory using FMC
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

#if defined(STM32H7)

/******************************* Include Files *******************************/

#include <string.h>

#include "drv/memories.h"
#include "drv/memories/drv_nand.h"
#include "core/irq.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

#define NAND_TIMEOUT                    1000u          /**< NAND Timeout for ST HAL */
#define NAND_MAX_BLOCK_SIZE_BYTES       262144u        /**< NAND block maximum size used for write buffering (correspond to 64 pages of 4096 bytes) */
#define NAND_INVALID_BLOCK_NUMBER       ((uint32_t)-1) /**< NAND Invalid block id (used to default the value of nand_write_buffer_block) */
#define NAND_CMD_SET_FEATURE            ((uint8_t)0xEFu) /**< NAND SET FEATURE command */
#define NAND_CMD_GET_FEATURE            ((uint8_t)0xEEu) /**< NAND GET FEATURE command */
#define NAND_FEATURE_ARRAY_OP_MODE      ((uint8_t)0x90u) /**< NAND Feature 'array operation mode' */
#define NAND_FEATURE_ECC_ENABLE         ((uint8_t)0x08u) /**< NAND Feature 'array operation mode' ECC enable bit */
#define NAND_CMD_GET_STATUS             ((uint8_t)0x70u) /**< NAND GET Status command */
#define NAND_STATUS_ECC_UNCORRECTABLE   ((uint8_t)0x01u) /**< More than 8 bit errors detected */
#define NAND_STATUS_ECC_CORRECTION_MASK ((uint8_t)0x18u) /**< ECC correction level mask, bits 4:3 */
#define NAND_STATUS_ECC_NO_ERROR        ((uint8_t)0x00u) /**< No bit error detected */
#define NAND_STATUS_ECC_1_TO_3          ((uint8_t)0x08u) /**< 1 to 3 bit errors corrected */
#define NAND_STATUS_ECC_4_TO_6          ((uint8_t)0x10u) /**< 4 to 6 bit errors corrected */
#define NAND_STATUS_ECC_7_TO_8          ((uint8_t)0x18u) /**< 7 to 8 bit errors corrected */

/***************************** Types Definitions *****************************/

/**
 * @struct  nandFeatureData_t
 * @brief   Struct type definition of a NAND Feature data
 */
typedef struct
{
    uint8_t p1; /**< @brief NAND feature parameters 1 */
    uint8_t p2; /**< @brief NAND feature parameters 2 */
    uint8_t p3; /**< @brief NAND feature parameters 3 */
    uint8_t p4; /**< @brief NAND feature parameters 4 */
} nandFeatureData_t;

/**
 * @enum  nandEccStatus_t
 * @brief   Enum type definition of NAND ECC status for a page
 */
typedef enum
{
    NAND_ECC_NO_ERROR,         /**< Page had no ECC error */
    NAND_ECC_CORRECTED_1_TO_3, /**< Page had 1 to 3 bits error but it has been corrected */
    NAND_ECC_CORRECTED_4_TO_6, /**< Page had 4 to 6 bits error but it has been corrected */
    NAND_ECC_CORRECTED_7_TO_8, /**< Page had 7 to 8 bits error but it has been corrected */
    NAND_ECC_UNCORRECTABLE     /**< Page had more than 8 bits error and can't be corrected */
} nandEccStatus_t;

/*************************** Functions Declarations **************************/

static NAND_AddressTypeDef NandLinearToAddress(nandInst_t *nand_inst, uint32_t linear_address);
static returnCode_t NandInitClock(nandInst_t *nand_inst, const nandConf_t *const nand_conf);
static returnCode_t NandDeInitClock(nandInst_t *nand_inst);
static returnCode_t NandSetupIOs(nandInst_t *nand_inst, const nandConf_t *const nand_conf);
static returnCode_t NandEnableECC(nandInst_t *nand_inst);
static returnCode_t NandGetFeature(nandInst_t *nand_inst, uint8_t feature_address, nandFeatureData_t *feature_data);
static returnCode_t NandSetFeature(nandInst_t *nand_inst, uint8_t feature_address, const nandFeatureData_t *feature_data);
static returnCode_t NandReadPage(nandInst_t *nand_inst, memorySector_t sector, data_t data, nandEccStatus_t *ecc_status);
static returnCode_t NandReadStatus(nandInst_t *nand_inst, uint8_t *status);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              NandOpen(nandInst_t *nand_inst, const nandConf_t *const nand_conf)
 * @brief           Function that initialise a NAND memory
 * @param[in,out]   nand_inst   Instance that contains NAND handlers
 * @param[in]       nand_conf   Configuration that contains NAND parameters
 * @retval          #RET_SUCCESSFUL if creation succeed
 * @retval          #RET_INVALID_PARAM if a pointer is null
 * @retval          #RET_NOT_AVAILABLE if the NAND controller is busy while enabling internal ECC
 * @retval          #RET_TIMEOUT if the NAND controller times out while enabling internal ECC
 * @retval          #RET_ERROR if internal ECC could not be enabled
 */
returnCode_t NandOpen(nandInst_t *nand_inst, const nandConf_t *const nand_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((nand_inst != NULL) && (nand_conf != NULL))
    {
        // Init peripheral clock
        return_value = NandInitClock(nand_inst, nand_conf);
        if (return_value == RET_SUCCESSFUL)
        {
            // Setup IOs
            return_value = NandSetupIOs(nand_inst, nand_conf);
            if (return_value == RET_SUCCESSFUL)
            {
                FMC_NAND_PCC_TimingTypeDef timing = { 0 };
                // Setup NAND
                nand_inst->handle_struct.Instance                  = nand_conf->periph;
                nand_inst->handle_struct.Init.NandBank             = nand_conf->bank;
                nand_inst->handle_struct.Init.Waitfeature          = nand_conf->wait_feature;
                nand_inst->handle_struct.Init.MemoryDataWidth      = nand_conf->data_width;
                nand_inst->handle_struct.Init.EccComputation       = nand_conf->ecc_computation;
                nand_inst->handle_struct.Init.ECCPageSize          = nand_conf->ecc_page_size;
                nand_inst->handle_struct.Init.TCLRSetupTime        = nand_conf->TCLR_setup_time;
                nand_inst->handle_struct.Init.TARSetupTime         = nand_conf->TAR_setup_time;
                nand_inst->handle_struct.Config.PageSize           = nand_conf->page_size;
                nand_inst->handle_struct.Config.SpareAreaSize      = nand_conf->spare_area_size;
                nand_inst->handle_struct.Config.BlockSize          = nand_conf->block_size;
                nand_inst->handle_struct.Config.BlockNbr           = nand_conf->nb_block;
                nand_inst->handle_struct.Config.PlaneNbr           = nand_conf->nb_plane;
                nand_inst->handle_struct.Config.PlaneSize          = nand_conf->plane_size;
                nand_inst->handle_struct.Config.ExtraCommandEnable = nand_conf->extra_cmd;
                // Timing
                timing.SetupTime     = nand_conf->setup_time;
                timing.WaitSetupTime = nand_conf->wait_time;
                timing.HoldSetupTime = nand_conf->hold_time;
                timing.HiZSetupTime  = nand_conf->hiz_time;
                // HAL NAND initialization
                HAL_StatusTypeDef test_hal = HAL_NAND_Init(&nand_inst->handle_struct, &timing, &timing);
                if (test_hal == HAL_OK)
                {
                    // Link the conf pointer
                    nand_inst->p_conf = nand_conf;

                    // Reset NAND
                    test_hal = HAL_NAND_Reset(&nand_inst->handle_struct);
                    if (test_hal == HAL_OK)
                    {
                        // Then wait 1 ms to be sure NAND is in a stable state
                        HAL_Delay(1u);
                        // Then get ID
                        test_hal = HAL_NAND_Read_ID(&nand_inst->handle_struct, &nand_inst->id);
                        if (test_hal == HAL_OK)
                        {
                            // Finally enable internal ECC
                            return_value = NandEnableECC(nand_inst);
                        }
                        else
                        {
                            KernelPanic();
                        }
                    }
                    else
                    {
                        KernelPanic();
                    }
                }
                else
                {
                    KernelPanic();
                }
            }
            else
            {
                KernelPanic();
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
 * @fn          NandWrite(nandInst_t *nand_inst, memorySector_t sector, data_t data, length_t length)
 * @brief       Function that writes onto an NAND memory
 * @param[in]   nand_inst   Instance that contains NAND parameters and NAND Handler
 * @param[in]   sector      Sector numero from wich data will be read
 * @param[out]  data        Pointer from which data will be copied
 * @param[in]   length      Number of sector that will be read
 * @retval      #RET_SUCCESSFUL if data has been written successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if nand timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if nand is still sending previous message
 */
returnCode_t NandWrite(nandInst_t *nand_inst, memorySector_t sector, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // NAND Write Buffer
    static uint8_t __attribute__((section(".nandbuff"))) nand_write_buffer[NAND_MAX_BLOCK_SIZE_BYTES] = { 0 };
    // Variable to store the block numero currently in RAM
    static uint32_t nand_write_buffer_block = NAND_INVALID_BLOCK_NUMBER;

    // Check parameter(s)
    if ((nand_inst != NULL) && (nand_inst->p_conf != NULL) && (length != 0u) && (data != NULL))
    {
        // Check bounds
        if ((nand_inst->p_conf->page_size != 0u) && (nand_inst->p_conf->block_size != 0u)
            && (nand_inst->p_conf->block_size <= (NAND_MAX_BLOCK_SIZE_BYTES / nand_inst->p_conf->page_size)))
        {
            const uint32_t sector_count = nand_inst->p_conf->nb_block * nand_inst->p_conf->block_size;

            if (((uint32_t)sector < sector_count) && (length <= (sector_count - (uint32_t)sector)))
            {
                uint32_t first_page_offset = sector % nand_inst->p_conf->block_size;
                uint32_t number_blocks     = (first_page_offset + length + nand_inst->p_conf->block_size - 1u) / nand_inst->p_conf->block_size;
                uint32_t written_pages     = 0u;

                // For each block
                for (uint32_t i = 0u; i < number_blocks; i++)
                {
                    uint32_t current_sector     = (uint32_t)sector + written_pages;
                    uint32_t offset             = current_sector % nand_inst->p_conf->block_size;
                    uint32_t size               = nand_inst->p_conf->block_size - offset;
                    uint32_t current_block      = current_sector / nand_inst->p_conf->block_size;
                    uint32_t block_start_sector = current_block * nand_inst->p_conf->block_size;
                    HAL_StatusTypeDef test_hal  = HAL_OK;

                    if (size > (length - written_pages))
                    {
                        size = length - written_pages;
                    }

                    NAND_AddressTypeDef block_addr = NandLinearToAddress(nand_inst, block_start_sector);

                    // Verify if the buffer to write in is not the current block in RAM
                    if (nand_write_buffer_block != current_block)
                    {
                        // Get the content of the block to the write buffer
                        test_hal = HAL_NAND_Read_Page_8b(&nand_inst->handle_struct, &block_addr, nand_write_buffer, nand_inst->p_conf->block_size);
                    }

                    // If everything is OK, we can write to the NAND
                    if (test_hal == HAL_OK)
                    {
                        // Update current block numero with the one in RAM
                        nand_write_buffer_block = current_block;

                        // Copy the new data into the write buffer
                        (void)memcpy(&nand_write_buffer[offset * nand_inst->p_conf->page_size], &data[written_pages * nand_inst->p_conf->page_size],
                                     (size_t)size * nand_inst->p_conf->page_size);

                        // Erase the block
                        test_hal = HAL_NAND_Erase_Block(&nand_inst->handle_struct, &block_addr);
                        if (test_hal == HAL_OK)
                        {
                            // Write back the block with new content
                            test_hal =
                                HAL_NAND_Write_Page_8b(&nand_inst->handle_struct, &block_addr, nand_write_buffer, nand_inst->p_conf->block_size);
                            if (test_hal != HAL_OK)
                            {
                                KernelPanic();
                            }
                        }
                        else
                        {
                            KernelPanic();
                        }
                    }
                    else
                    {
                        KernelPanic();
                    }

                    written_pages += size;
                }
            }
            else
            {
                return_value = RET_INVALID_PARAM;
            }
        }
        else
        {
            return_value = RET_INVALID_PARAM;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          NandRead(nandInst_t *nand_inst, memorySector_t sector, data_t data, length_t length)
 * @brief       Function that read onto an NAND memory
 * @param[in]   nand_inst     Instance that contains NAND parameters and NAND Handler
 * @param[in]   sector      Sector numero from wich data will be read
 * @param[out]  data        Pointer to where data will be copied
 * @param[in]   length      Number of sector that will be read
 * @retval      #RET_SUCCESSFUL if data has been read successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if nand timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if nand is still sending previous message
 */
returnCode_t NandRead(nandInst_t *nand_inst, memorySector_t sector, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((nand_inst != NULL) && (length != 0u) && (data != NULL))
    {
        uint32_t page = 0u;
        while ((page < length) && (return_value == RET_SUCCESSFUL))
        {
            nandEccStatus_t ecc_status;
            return_value = NandReadPage(nand_inst, sector + page, &data[page * nand_inst->p_conf->page_size], &ecc_status);
            page++;

            // Decode ECC status
            if ((return_value == RET_SUCCESSFUL) &&
                (ecc_status == NAND_ECC_UNCORRECTABLE))
            {
                return_value = RET_ERROR; // TO DO : implement a proper FTL
            }
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              NandIoctl(nandInst_t *nand_inst, uint32_t cmd, void *data, uint32_t data_size);
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   nand_inst     Instance that contains NAND handlers
 * @param[in]       cmd         IO Control command
 * @param[in,out]   data        IO Control command
 * @param[in]       data_size   IO Control data size
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if action cannot be performed because driver is busy
 * @retval          #RET_SUCCESSFUL else
 */
returnCode_t NandIoctl(nandInst_t *nand_inst, uint32_t cmd, void *data, uint32_t data_size)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (nand_inst != NULL)
    {
        switch (cmd)
        {
            case IOCTL_MEMORY_GET_STATUS :
                if (data_size == sizeof(memoryStatus_t))
                {
                    HAL_NAND_StateTypeDef status = HAL_NAND_GetState(&nand_inst->handle_struct);
                    if (status == HAL_NAND_STATE_READY)
                    {
                        *(memoryStatus_t *)data = MEMORY_READY;
                    }
                    else
                    {
                        *(memoryStatus_t *)data = MEMORY_NO_DISK;
                    }
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case IOCTL_MEMORY_SYNC :
                // Sync is not required for this NAND card driver, so do nothing
                break;
            case IOCTL_MEMORY_GET_SECTOR_COUNT :
                if (data_size == sizeof(memorySectorCount_t))
                {
                    *(memorySectorCount_t *)data = (uint32_t)(nand_inst->p_conf->nb_block * nand_inst->p_conf->block_size);
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case IOCTL_MEMORY_GET_SECTOR_SIZE :
                if (data_size == sizeof(memorySectorSize_t))
                {
                    *(memorySectorSize_t *)data = nand_inst->p_conf->page_size;
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case IOCTL_MEMORY_GET_BLOCK_SIZE :
                if (data_size == sizeof(memoryBlockSize_t))
                {
                    *(memoryBlockSize_t *)data = nand_inst->p_conf->block_size;
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
 * @fn              NandClose(nandInst_t *nand_inst)
 * @brief           Function that desinit the NAND connection
 * @param[in,out]   nand_inst   Instance that contains NAND handlers
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 */
returnCode_t NandClose(nandInst_t *nand_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (nand_inst != NULL)
    {
        HAL_NAND_DeInit(&nand_inst->handle_struct);
        (void)NandDeInitClock(nand_inst);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @brief   Converts a linear page address into a NAND physical address.
 * @param[in,out]   nand_inst   Instance that contains NAND handlers
 * @param[in]  linear_address  Linear address in number of pages
 * @return     NAND_AddressTypeDef structure with Page, Block, Plane
 */
static NAND_AddressTypeDef NandLinearToAddress(nandInst_t *nand_inst, uint32_t linear_address)
{
    NAND_AddressTypeDef addr;

    addr.Page  = linear_address % nand_inst->p_conf->block_size;
    addr.Block = (linear_address / nand_inst->p_conf->block_size) % nand_inst->p_conf->plane_size;
    addr.Plane = (linear_address / (nand_inst->p_conf->block_size * nand_inst->p_conf->plane_size));

    return addr;
}

/**
 * @fn              NandInitClock(nandInst_t *nand_inst, const nandConf_t *const nand_conf)
 * @brief           Function that setups NAND peripheral clock
 * @param[in,out]   nand_inst   Instance that contains NAND handlers
 * @param[in]       nand_conf   Configuration that contains NAND parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_ERROR if the clock initialisation failed
 */
static returnCode_t NandInitClock(nandInst_t *nand_inst, const nandConf_t *const nand_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused
    (void)(nand_inst);

    // Check parameter(s)
    if ((nand_inst != NULL) && (nand_conf != NULL))
    {
#if defined(STM32H7)
        RCC_PeriphCLKInitTypeDef nand_peripheral_clock_settings = { 0 };
        nand_peripheral_clock_settings.PeriphClockSelection     = RCC_PERIPHCLK_FMC;
        nand_peripheral_clock_settings.FmcClockSelection        = nand_conf->clk_src;
        if (HAL_RCCEx_PeriphCLKConfig(&nand_peripheral_clock_settings) == HAL_OK)
        {
            __HAL_RCC_FMC_CLK_ENABLE();
        }
        else
        {
            return_value = RET_ERROR;
        }
#elif defined(STM32F4)
        __HAL_RCC_FMC_CLK_ENABLE();
#else
#error
#endif /* STM32H7 | STM32F4 */
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              NandDeInitClock(nandInst_t *nand_inst)
 * @brief           Function that disables NAND peripheral clock
 * @param[in,out]   nand_inst   Instance that contains NAND handlers
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_ERROR if the clock initialisation failed
 */
static returnCode_t NandDeInitClock(nandInst_t *nand_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (nand_inst != NULL)
    {
        (void)(nand_inst);
        __HAL_RCC_FMC_CLK_DISABLE();
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              NandSetupIOs(nandInst_t *nand_inst, const nandConf_t *const nand_conf)
 * @brief           Function that setups IOs
 * @param[in,out]   nand_inst   Instance that contains NAND handlers
 * @param[in]       nand_conf   Configuration that contains NAND parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if IT is not available for this NAND
 */
static returnCode_t NandSetupIOs(nandInst_t *nand_inst, const nandConf_t *const nand_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused
    (void)(nand_inst);

    // Check parameter(s)
    if ((nand_inst != NULL) && (nand_conf != NULL))
    {
        // First Init CLE IO
        return_value = SetupIO(&nand_conf->io_cle);

        // Init ALE IO
        if (return_value == RET_SUCCESSFUL)
        {
            return_value = SetupIO(&nand_conf->io_ale);
        }

        // Init NCE IO
        if (return_value == RET_SUCCESSFUL)
        {
            return_value = SetupIO(&nand_conf->io_nce);
        }

        // Init NOE IO
        if (return_value == RET_SUCCESSFUL)
        {
            return_value = SetupIO(&nand_conf->io_noe);
        }

        // Init NWE IO
        if (return_value == RET_SUCCESSFUL)
        {
            return_value = SetupIO(&nand_conf->io_nwe);
        }

        // Init NWAIT IO
        if (return_value == RET_SUCCESSFUL)
        {
            return_value = SetupIO(&nand_conf->io_nwait);
        }

        // Init D0 IO
        if (return_value == RET_SUCCESSFUL)
        {
            return_value = SetupIO(&nand_conf->io_d0);
        }

        // Init D1 IO
        if (return_value == RET_SUCCESSFUL)
        {
            return_value = SetupIO(&nand_conf->io_d1);
        }

        // Init D2 IO
        if (return_value == RET_SUCCESSFUL)
        {
            return_value = SetupIO(&nand_conf->io_d2);
        }

        // Init D3 IO
        if (return_value == RET_SUCCESSFUL)
        {
            return_value = SetupIO(&nand_conf->io_d3);
        }

        // Init D4 IO
        if (return_value == RET_SUCCESSFUL)
        {
            return_value = SetupIO(&nand_conf->io_d4);
        }

        // Init D5 IO
        if (return_value == RET_SUCCESSFUL)
        {
            return_value = SetupIO(&nand_conf->io_d5);
        }

        // Init D6 IO
        if (return_value == RET_SUCCESSFUL)
        {
            return_value = SetupIO(&nand_conf->io_d6);
        }

        // Init D7 IO
        if (return_value == RET_SUCCESSFUL)
        {
            return_value = SetupIO(&nand_conf->io_d7);
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              NandEnableECC(nandInst_t *nand_inst)
 * @brief           Enables internal NAND ECC
 * @param[in,out]   nand_inst   Instance that contains NAND handlers
 * @retval          #RET_INVALID_PARAM if nand_inst is a NULL pointer
 * @retval          #RET_NOT_AVAILABLE when NAND controller is busy
 * @retval          #RET_TIMEOUT if NAND controller timed out
 * @retval          #RET_ERROR if internal ECC could not be enabled
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t NandEnableECC(nandInst_t *nand_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (nand_inst != NULL)
    {
        nandFeatureData_t feature_data = { 0 };

        // First read current feature "Array Operation Mode"
        return_value = NandGetFeature(nand_inst, NAND_FEATURE_ARRAY_OP_MODE, &feature_data);
        if (return_value == RET_SUCCESSFUL)
        {
            // Enable ECC
            feature_data.p1 |= NAND_FEATURE_ECC_ENABLE;
            // Reserved parameters must be written as zero
            feature_data.p2 = 0u;
            feature_data.p3 = 0u;
            feature_data.p4 = 0u;

            // Then write back feature
            return_value = NandSetFeature(nand_inst, NAND_FEATURE_ARRAY_OP_MODE, &feature_data);
            if (return_value == RET_SUCCESSFUL)
            {
                // Check the feature has been successfully written
                return_value = NandGetFeature(nand_inst, NAND_FEATURE_ARRAY_OP_MODE, &feature_data);
                if (return_value == RET_SUCCESSFUL)
                {
                    if ((feature_data.p1 & NAND_FEATURE_ECC_ENABLE) == 0u)
                    {
                        return_value = RET_ERROR;
                    }
                }
            }
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              NandGetFeature(nandInst_t *nand_inst, uint8_t feature_address, nandFeatureData_t *feature_data)
 * @brief           Read NAND's feature register
 * @param[in,out]   nand_inst       Instance that contains NAND handlers
 * @param[in]       feature_address NAND feature address
 * @param[out]      feature_data    Feature data that has been read
 * @retval          #RET_INVALID_PARAM if nand_inst or feature_data is a null pointer
 * @retval          #RET_NOT_AVAILABLE if NAND is already in use
 * @retval          #RET_TIMEOUT if the NAND times out during operation
 * @retval          #RET_ERROR if the NAND controller is in an invalid state
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t NandGetFeature(nandInst_t *nand_inst, uint8_t feature_address, nandFeatureData_t *feature_data)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((nand_inst != NULL) && (feature_data != NULL))
    {
        NAND_HandleTypeDef *hnand = &nand_inst->handle_struct;
        uint32_t tickstart;
        uint32_t nand_status;

        if (hnand->State == HAL_NAND_STATE_READY)
        {
            // Update the NAND controller state
            hnand->State = HAL_NAND_STATE_BUSY;

            // Enter GET FEATURES mode
            *(__IO uint8_t *)((uint32_t)(NAND_DEVICE | CMD_AREA)) = NAND_CMD_GET_FEATURE;
            __DSB();

            // Select feature address
            *(__IO uint8_t *)((uint32_t)(NAND_DEVICE | ADDR_AREA)) = feature_address;
            __DSB();

            // Wait until the NAND isn't busy anymore
            tickstart = HAL_GetTick();
            do
            {
                nand_status = HAL_NAND_Read_Status(hnand);
                if ((nand_status != NAND_READY) && ((HAL_GetTick() - tickstart) >= NAND_TIMEOUT))
                {
                    return_value = RET_TIMEOUT;
                }
            } while ((nand_status != NAND_READY) && (return_value == RET_SUCCESSFUL));

            // Read if no timeout occurs
            if (return_value == RET_SUCCESSFUL)
            {
                // Get the feature data
                *(__IO uint8_t *)((uint32_t)(NAND_DEVICE | CMD_AREA)) = NAND_CMD_AREA_A;
                __DSB();
                feature_data->p1 = *(__IO uint8_t *)NAND_DEVICE;
                feature_data->p2 = *(__IO uint8_t *)NAND_DEVICE;
                feature_data->p3 = *(__IO uint8_t *)NAND_DEVICE;
                feature_data->p4 = *(__IO uint8_t *)NAND_DEVICE;

                // Reset NAND state
                hnand->State = HAL_NAND_STATE_READY;
            }
            else
            {
                hnand->State = HAL_NAND_STATE_ERROR;
            }
        }
        else if (hnand->State == HAL_NAND_STATE_BUSY)
        {
            return_value = RET_NOT_AVAILABLE;
        }
        else
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              NandSetFeature(nandInst_t *nand_inst, uint8_t feature_address, const nandFeatureData_t *feature_data)
 * @brief           Write NAND's feature register
 * @param[in,out]   nand_inst       Instance that contains NAND handlers
 * @param[in]       feature_address NAND Feature Address
 * @param[in]       feature_data    Feature data that will be written
 * @retval          #RET_INVALID_PARAM if nand_inst or feature_data is a null pointer
 * @retval          #RET_NOT_AVAILABLE if NAND is already in use
 * @retval          #RET_TIMEOUT if the NAND times out during operation
 * @retval          #RET_ERROR if the NAND controller is in an invalid state
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t NandSetFeature(nandInst_t *nand_inst, uint8_t feature_address, const nandFeatureData_t *feature_data)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((nand_inst != NULL) && (feature_data != NULL))
    {
        NAND_HandleTypeDef *hnand = &nand_inst->handle_struct;
        uint32_t tickstart;
        uint32_t nand_status;

        if (hnand->State == HAL_NAND_STATE_READY)
        {
            // Update the NAND controller state
            hnand->State = HAL_NAND_STATE_BUSY;

            // Enter SET FEATURES mode
            *(__IO uint8_t *)((uint32_t)(NAND_DEVICE | CMD_AREA)) = NAND_CMD_SET_FEATURE;
            __DSB();

            // Select feature address
            *(__IO uint8_t *)((uint32_t)(NAND_DEVICE | ADDR_AREA)) = feature_address;
            __DSB();

            // Wait tADL before writing the first feature parameter (70 ns minimum)
            HAL_Delay(1u);

            // Write feature
            *(__IO uint8_t *)NAND_DEVICE = feature_data->p1;
            __DSB();
            *(__IO uint8_t *)NAND_DEVICE = feature_data->p2;
            __DSB();
            *(__IO uint8_t *)NAND_DEVICE = feature_data->p3;
            __DSB();
            *(__IO uint8_t *)NAND_DEVICE = feature_data->p4;
            __DSB();

            // Wait until the NAND isn't busy anymore
            tickstart = HAL_GetTick();
            do
            {
                nand_status = HAL_NAND_Read_Status(hnand);
                if ((nand_status != NAND_READY) && ((HAL_GetTick() - tickstart) >= NAND_TIMEOUT))
                {
                    return_value = RET_TIMEOUT;
                }
            } while ((nand_status != NAND_READY) && (return_value == RET_SUCCESSFUL));

            if (return_value == RET_SUCCESSFUL)
            {
                // Update the NAND controller state
                hnand->State = HAL_NAND_STATE_READY;
            }
            else
            {
                hnand->State = HAL_NAND_STATE_ERROR;
            }
        }
        else if (hnand->State == HAL_NAND_STATE_BUSY)
        {
            return_value = RET_NOT_AVAILABLE;
        }
        else
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              NandReadPage(nandInst_t *nand_inst, memorySector_t sector, data_t data, nandEccStatus_t *ecc_status)
 * @brief           Reads one NAND page and reports its internal ECC status
 * @param[in,out]   nand_inst   Instance that contains NAND handlers
 * @param[in]       sector      Linear sector number of the page to read
 * @param[out]      data        Buffer receiving one complete NAND page
 * @param[out]      ecc_status  Internal ECC result associated with the page
 * @retval          #RET_INVALID_PARAM if a parameter is invalid
 * @retval          #RET_NOT_AVAILABLE if the NAND controller is busy
 * @retval          #RET_TIMEOUT if the page read times out
 * @retval          #RET_ERROR if the page or status read fails
 * @retval          #RET_SUCCESSFUL if the page and its ECC status were read
 */
static returnCode_t NandReadPage(nandInst_t *nand_inst, memorySector_t sector, data_t data, nandEccStatus_t *ecc_status)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((nand_inst != NULL) && (data != NULL) && (ecc_status != NULL))
    {
        // Get sector's NAND adress
        NAND_AddressTypeDef nand_addr = NandLinearToAddress(nand_inst, sector);

        // Read Page
        HAL_StatusTypeDef test_hal = HAL_NAND_Read_Page_8b(&nand_inst->handle_struct, &nand_addr, data, 1u);
        if (test_hal == HAL_OK)
        {
            uint8_t raw_status;

            // Read Status
            return_value = NandReadStatus(nand_inst, &raw_status);
            if (return_value == RET_SUCCESSFUL)
            {
                if ((raw_status & NAND_STATUS_ECC_UNCORRECTABLE) != 0u)
                {
                    *ecc_status = NAND_ECC_UNCORRECTABLE;
                }
                else
                {
                    switch (raw_status & NAND_STATUS_ECC_CORRECTION_MASK)
                    {
                        case NAND_STATUS_ECC_NO_ERROR :
                            *ecc_status = NAND_ECC_NO_ERROR;
                            break;

                        case NAND_STATUS_ECC_1_TO_3 :
                            *ecc_status = NAND_ECC_CORRECTED_1_TO_3;
                            break;

                        case NAND_STATUS_ECC_4_TO_6 :
                            *ecc_status = NAND_ECC_CORRECTED_4_TO_6;
                            break;

                        case NAND_STATUS_ECC_7_TO_8 :
                            *ecc_status = NAND_ECC_CORRECTED_7_TO_8;
                            break;

                        default :
                            return_value = RET_ERROR;
                            break;
                    }
                }
            }
        }
        else if (test_hal == HAL_BUSY)
        {
            return_value = RET_NOT_AVAILABLE;
        }
        else if (test_hal == HAL_TIMEOUT)
        {
            return_value = RET_TIMEOUT;
        }
        else
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              NandReadStatus(nandInst_t *nand_inst, uint8_t *status)
 * @brief           Read NAND status register
 * @param[in,out]   nand_inst   Instance that contains NAND handlers
 * @param[out]      status      Feature data that has been read
 * @retval          #RET_INVALID_PARAM if nand_inst or status is a null pointer
 * @retval          #RET_NOT_AVAILABLE if NAND is already in use
 * @retval          #RET_TIMEOUT if the NAND times out during operation
 * @retval          #RET_ERROR if the NAND controller is in an invalid state
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t NandReadStatus(nandInst_t *nand_inst, uint8_t *status)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((nand_inst != NULL) && (status != NULL))
    {
        NAND_HandleTypeDef *hnand = &nand_inst->handle_struct;

        if (hnand->State == HAL_NAND_STATE_READY)
        {
            // Update the NAND controller state
            hnand->State = HAL_NAND_STATE_BUSY;

            // Enter GET FEATURES mode
            *(__IO uint8_t *)((uint32_t)(NAND_DEVICE | CMD_AREA)) = NAND_CMD_STATUS;
            __DSB();

            // Get Status
            *status = *(__IO uint8_t *)NAND_DEVICE;

            // Return to read mode
            *(__IO uint8_t *)((uint32_t)(NAND_DEVICE | CMD_AREA)) = NAND_CMD_AREA_A;
            __DSB();

            // Reset NAND state
            hnand->State = HAL_NAND_STATE_READY;
        }
        else if (hnand->State == HAL_NAND_STATE_BUSY)
        {
            return_value = RET_NOT_AVAILABLE;
        }
        else
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

#endif /* STM32H7 */
