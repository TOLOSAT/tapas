/**
 * @file    info.c
 * @author  Merlin Kooshmanian
 * @brief   Source defining system information handling
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

/******************************* Include Files *******************************/

#include "mc/info.h"
#include "fdir/context.h"
#include "mc/console.h"

/***************************** Macros Definitions ****************************/

/**
 * @def     STR
 * @brief   Convert a macro to a string
 */
#define STR(x)  #x // cppcheck-suppress misra-c2012-20.10; Exception: not used for critical code, just for pretty printing purpose

#define VERSION "v" STR(MAJOR) "." STR(MINOR) "." STR(PATCH) /**< Version string */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/**
 * @var     g_system_info
 * @brief   Constante struct containing system information
 */
const sytemInfo_t g_system_info = {
    .name       = SYSTEM_NAME,
    .version    = { .major = MAJOR, .minor = MINOR, .patch = PATCH },
    .build_date = __DATE__,
    .build_time = __TIME__,
    .target     = BOARD,
};

/**
 * @var     g_program_name
 * @brief   Constant that holds program name
 */
const char *g_program_name = PROGRAM_NAME;

/*************************** Functions Definitions ***************************/

/**
 * @fn      PrintSystemInfo(void)
 * @brief   Function that prints all the system information onto the console
 * @return  Nothing
 */
void PrintSystemInfo(void)
{
    context_t context = { 0 };

    ConsolePrint("===============================================\n");
    ConsolePrint("Welcome on %s\n", PROGRAM_NAME);
    ConsolePrint("System : %s v%d.%d.%d\n", SYSTEM_NAME, MAJOR, MINOR, PATCH);
    ConsolePrint("Built on %s at %s, for target %s\n", __DATE__, __TIME__, BOARD);

    if (ReadContext(&context) == RET_SUCCESSFUL)
    {
        ConsolePrint("-----------------------------------------------\n");
        ConsolePrint("System context :\n");
        if (context.state == SOFTWARE_STATE_NOMINAL)
        {
            ConsolePrint("  State : Nominal\n");
        }
        else if (context.state == SOFTWARE_STATE_SAFE)
        {
            ConsolePrint("  State : Safe\n");
        }
        else
        {
            ConsolePrint("  State : Unknown\n");
        }
        ConsolePrint("  Safe software ID : %d\n", context.safe_software_id);
        ConsolePrint("  Nominal software ID : %d\n", context.nominal_software_id);
        ConsolePrint("  Boot count : %ld\n", context.boot);
        ConsolePrint("  Critical error count : %ld\n", context.critical_error);

        ConsolePrint("  CFSR : %lx\n", context.cfsr);
        ConsolePrint("  HFSR : %lx\n", context.hfsr);
        ConsolePrint("  R0 : %lx\n", context.registers.r[0]);
        ConsolePrint("  R1 : %lx\n", context.registers.r[1]);
        ConsolePrint("  R2 : %lx\n", context.registers.r[2]);
        ConsolePrint("  R3 : %lx\n", context.registers.r[3]);
        ConsolePrint("  R12 : %lx\n", context.registers.r12);
        ConsolePrint("  xPSR : %lx\n", context.registers.xpsr);
        ConsolePrint("  LR : %lx\n", context.registers.lr);
        ConsolePrint("  PC : %lx\n", context.registers.pc);

        ConsolePrint("  Call stack :\n");
        for (uint32_t i = 0; i < context.call_stack.calls_nb; i++)
        {
            ConsolePrint("    %lx", context.call_stack.calls[i].function);
            ConsolePrint("+%lx\n", context.call_stack.calls[i].offset);
        }
    }
    ConsolePrint("===============================================\n");
}