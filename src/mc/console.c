/**
 * @file    console.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for Console functions
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

/******************************* Include Files *******************************/

#include "mc/console.h"
#include "time/time.h"
#include "core/tasks.h"
#include "fdir/fdir.h"
#include "fs/fs.h"
#include "drv/peripherals.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

#if !defined(CONFIG_CONSOLE_NONE)
static void ConsoleLock(void);
static void ConsoleUnlock(void);

static const char *StringOfInt(int value, bool sign);
static const char *StringOfHex(int value, bool uppercase);
static const char *StringOfOctal(int value);

static const char *StringOfTimestamp(void);
static void ConsoleSpecificInit(void);
static void CheckConsoleSize(void);

static void ConsolePrintString(const char *str);
static void ConsolePrintChar(char c);
static void ConsoleSync(void);
#endif

/*************************** Variables Definitions ***************************/

#if !defined(CONFIG_CONSOLE_NONE)
static consoleStatus_t console_status = CONSOLE_NOT_INITIALISED;
static mutexHandle_t console_mutex    = { 0 };
static bool console_mutex_initialised = false;
#endif /* !CONFIG_CONSOLE_NONE */

/*************************** Functions Definitions ***************************/

/**
 * @fn          InitConsole(void)
 * @brief       Initialise the console
 * @return      Nothing
 */
void InitConsole(void)
{
#if !defined(CONFIG_CONSOLE_NONE)
    // Then do the specific init depending on the console mode
    ConsoleSpecificInit();

    // Finally declare the console initialised
    console_status = CONSOLE_INITIALISED;
#endif
}

/**
 * @fn      CreateConsoleMutexes(void)
 * @brief   Function that allows to postpone mutex initilisation when other mutexes will be initialised.
 */
void CreateConsoleMutexes(void)
{
#if !defined(CONFIG_CONSOLE_NONE)
    // Initialise mutex for the filesystem
    console_mutex = xSemaphoreCreateMutex();
    if (console_mutex == NULL)
    {
        KernelPanic();
    }
    else
    {
        console_mutex_initialised = true;
    }
#endif
}

/**
 * @fn          ConsolePrint(const char *fmt, ...)
 * @brief       This function prints a string to the console following the
 *              format passed as first argument and the values as other
 *              arguments.
 * @param[in]   fmt The format string
 * @param[in]   ... The argument to replace in the format
 * @return      Nothing
 *
 * The types are checked compile time due to GCC attribute making this function
 * safe to use for the ARMv7 ABI.
 *
 * The currently supported formats are:
 *  - %u -> print usigned int
 *  - %d -> print signed int
 *  - %i -> print signed int
 *  - %x -> print lowercase hex
 *  - %X -> print uppercase hex
 *  - %o -> print octal int
 *  - %s -> print string
 *  - %c -> print char
 */
extern ATTR_CHECK_FORMAT void ConsolePrint(const char *fmt, ...)
{
#if !defined(CONFIG_CONSOLE_NONE)
    // Print only if the console is initialised
    if (console_status == CONSOLE_INITIALISED)
    {
        // uint32_t line_index = 0u;
        // uint32_t i          = 0u;

        // First lock the console
        ConsoleLock();

        // Then Check the console size
        CheckConsoleSize();

        // Print the timestamp
        ConsolePrintString(StringOfTimestamp());

        // Get the first arg after fmt from the function stack
        const uint32_t *argp = (const uint32_t *)(&fmt + 1);

        // Parse the format string
        while (*fmt)
        {
            if (*fmt == '%')
            {
                // If format is %l_ then print a long number
                if (*(fmt + 1) == 'l')
                {
                    fmt++;
                }

                // In case it is a format, match the char after %
                switch (*(fmt + 1))
                {
                    // If format is %u then print an usigned int
                    case 'u' :
                        ConsolePrintString(StringOfInt(*argp++, false));
                        break;
                    // If format is %d or %i then print a signed int
                    case 'd' :
                    case 'i' :
                        ConsolePrintString(StringOfInt(*argp++, true));
                        break;
                    // If format is %x then print a lowercase hex number
                    case 'x' :
                        ConsolePrintString(StringOfHex(*argp++, false));
                        break;
                    // If format is %X then print an uppercase hex number
                    case 'X' :
                        ConsolePrintString(StringOfHex(*argp++, true));
                        break;
                    // If format is %o then print an octal number
                    case 'o' :
                        ConsolePrintString(StringOfOctal(*argp++));
                        break;
                    // If format is %s then print a string
                    case 's' :
                        ConsolePrintString((const char *)*argp++); // cppcheck-suppress misra-c2012-11.4; Required to get variadic args
                        break;
                    // If format is %c then print a char
                    case 'c' :
                        ConsolePrintChar((char)*argp++);
                        break;
                    // Else it is not a valid format so just print the char
                    default :
                        ConsolePrintChar(*(fmt + 1));
                        break;
                }
                fmt += 2;
            }
            else
            {
                // Else the current string position is not a format so we just print the format string content
                ConsolePrintChar(*fmt++);
            }
        }

        // // Check if the last character is not '\n'
        // if ((i > 0u) && (msg[i - 1u] != '\n'))
        // {
        //     ConsolePrintChar('\n'); // Add a newline if not already present
        // }

        // Synchronise console
        ConsoleSync();

        // Finally unlock the console
        ConsoleUnlock();
    }
#else
    (void)(fmt);
#endif /* CONFIG_CONSOLE_NONE */
}

#if !defined(CONFIG_CONSOLE_NONE)
/**
 * @fn      ConsoleLock(void)
 * @brief   Lock the Console with a mutex
 * @return  Nothing
 */
static void ConsoleLock(void)
{
    BaseType_t mutex_status;

    // Lock the mutex if it has been initialised
    if (console_mutex_initialised)
    {
        mutex_status = xSemaphoreTake(console_mutex, portMAX_DELAY);
        if (mutex_status != pdTRUE)
        {
            KernelPanic();
        }
    }
}

/**
 * @fn      ConsoleUnlock(void)
 * @brief   Unlock the Console (which has been locked with a mutex)
 * @return  Nothing
 */
static void ConsoleUnlock(void)
{
    BaseType_t mutex_status;

    // Unlock the mutex if it has been initialised
    if (console_mutex_initialised)
    {
        mutex_status = xSemaphoreGive(console_mutex);
        if (mutex_status != pdTRUE)
        {
            KernelPanic();
        }
    }
}

/**
 * @fn StringOfInt
 * @brief This function converts an int to a string
 * @param[in] value The int to convert
 * @param[in] sign If the integer is signed
 * @return A string representing the int given as input
 */
static const char *StringOfInt(int value, bool sign)
{
    static char result[12];
    bool neg       = (value < 0);
    unsigned int u = 0u;
    if (sign)
    {
        u = neg ? -value : value;
    }
    else
    {
        u = (unsigned int)value;
    }
    int i = 0;

    // Get the digits
    do
    {
        result[i] = (u % 10u) + '0';
        i++;
        u /= 10;
    } while (u > 0u);

    // Add '-' if number is negative
    if (neg)
    {
        result[i] = '-';
        i++;
    }

    // End the string with '\0'
    result[i] = '\0';

    // Revers the string
    for (int j = 0; j < (i / 2); ++j)
    {
        char tmp          = result[j];
        result[j]         = result[i - j - 1];
        result[i - j - 1] = tmp;
    }

    return result;
}

/**
 * @fn StringOfHex
 * @brief This function converts an int to a hexadecimal string
 * @param[in] value The int to convert
 * @param[in] uppercase If the hex output uses upercase
 * @return A string representing the int in hexadecimal format
 */
static const char *StringOfHex(int value, bool uppercase)
{
    static char result[11]; // "0x" + 8 hex digits + '\0' = 11
    unsigned int u = (unsigned int)value;
    int i          = 0;

    // Get the hex digits (in reverse order)
    do
    {
        unsigned int digit  = u % 16u;
        char a              = uppercase ? 'A' : 'a';
        result[i++]         = (digit < 10u) ? (digit + (unsigned int)'0') : (digit - 10u + (unsigned int)a);
        u                  /= 16u;
    } while (u > 0u);

    // Add the "0x" prefix
    result[i] = 'x';
    i++;
    result[i] = '0';
    i++;

    // End the string
    result[i] = '\0';

    // Reverse the string
    for (int j = 0; j < (i / 2); ++j)
    {
        char tmp          = result[j];
        result[j]         = result[i - j - 1];
        result[i - j - 1] = tmp;
    }

    return result;
}

/**
 * @fn StringOfOctal
 * @brief This function converts an int to an octal string
 * @param[in] value The int to convert
 * @return A string representing the int in octal format
 */
static const char *StringOfOctal(int value)
{
    static char result[14]; // "0" + up to 11 octal digits for 32-bit + '\0'
    unsigned int u = (unsigned int)value;
    int i          = 0;

    // Get the octal digits (in reverse order)
    do
    {
        result[i] = (u % 8u) + '0';
        i++;
        u /= 8u;
    } while (u > 0u);

    // Add the "0" prefix
    result[i] = '0';
    i++;

    // End the string
    result[i] = '\0';

    // Reverse the string
    for (int j = 0; j < (i / 2); ++j)
    {
        char tmp          = result[j];
        result[j]         = result[i - j - 1];
        result[i - j - 1] = tmp;
    }

    return result;
}

/**
 * @fn          StringOfTimestamp(void)
 * @brief       Function that prints the header of each line
 * @return      The timestamp formated as a string
 *
 * Currently the header is the CUC time
 */
static const char *StringOfTimestamp(void)
{
    // Init a buffer for the timestamp
    static char timestamp_buffer[33u] = { 0 };

    // First get time and task no
    time_t time   = GetTime();
    taskNo_t task = GetCurrentTask();

    // Start header
    timestamp_buffer[0] = '[';

    // Print timestamp
    for (uint32_t i = 1u; i <= (2u * sizeof(time_t)); i++)
    {
        // Compute position of the 4 bits that will be printed
        uint32_t shift = 4u * ((2u * sizeof(time_t)) - i);

        // Extract the current hex digit by shifting and masking
        uint8_t hex_digit = (uint8_t)((time >> shift) & 0x000000000000000Fllu);

        // Convert to character and store in buffer
        if (hex_digit < 10u)
        {
            timestamp_buffer[i] = '0' + hex_digit;
        }
        else
        {
            timestamp_buffer[i] = 'a' + (hex_digit - 10u);
        }
    }

    // Print task no
    uint32_t index          = 1u + (2u * sizeof(time_t));
    timestamp_buffer[index] = ',';
    index++;
    timestamp_buffer[index] = '#';
    index++;

    // Convert task number to string and append to buffer
    const char *task_str = StringOfInt(task, false);
    for (int j = 0; task_str[j] != '\0'; j++)
    {
        timestamp_buffer[index] = task_str[j];
        index++;
    }

    // Print header end
    timestamp_buffer[index] = ']';
    index++;
    timestamp_buffer[index] = ':';
    index++;
    timestamp_buffer[index] = ' ';
    index++;
    timestamp_buffer[index] = '\0';

    return timestamp_buffer;
}
#endif /* CONFIG_CONSOLE_NONE */

/************************ File Based Console Functions ***********************/

#if defined(CONFIG_CONSOLE_FILE)

#if !defined(CONFIG_FS_ENABLED)
#error "File console cannot be chosen when file system is disabled"
#endif

#define LOG_DIRECTORY_PATH           "logs"                                /**< Log directory path */
#define CONSOLE_FILE_PATH            "logs/console.log"                    /**< Console file path */
#define CONSOLE_FILE_ACCESS_MODE     (FA_OPEN_ALWAYS | FA_WRITE | FA_READ) /**< Console file acess mode */
#define OLD_CONSOLE_FILE_PATH        "logs/old_console.log"                /**< Old console file path */
#define OLD_CONSOLE_FILE_ACCESS_MODE (FA_OPEN_ALWAYS | FA_WRITE | FA_READ) /**< Old console file acess mode */

/**
 * @var     console_file
 * @brief   Console file
 */
static FIL console_file = { 0 };

/**
 * @var     old_console_file
 * @brief   Old console file
 */
static FIL old_console_file = { 0 };

/**
 * @fn          ConsoleSpecificInit
 * @brief       Initialisation specific to the console type choosed
 * @return      Nothing
 */
static void ConsoleSpecificInit(void)
{
    // First create "logs" directory
    FRESULT res = f_mkdir(LOG_DIRECTORY_PATH);
    if ((res != FR_OK) && (res != FR_EXIST))
    {
        KernelPanic();
    }

    // Then open files
    if (f_open(&console_file, CONSOLE_FILE_PATH, CONSOLE_FILE_ACCESS_MODE) != FR_OK)
    {
        KernelPanic();
    }
    if (f_open(&old_console_file, OLD_CONSOLE_FILE_PATH, OLD_CONSOLE_FILE_ACCESS_MODE) != FR_OK)
    {
        KernelPanic();
    }

    // Put file pointer at the end of the console file
    if (f_lseek(&console_file, f_size(&console_file)) != FR_OK)
    {
        KernelPanic();
    }
}

/**
 * @fn          CheckConsoleSize
 * @brief       Check the console file size update the file if it reaches the maximum size
 * @return      Nothing
 *
 * If reach the maximum size, the content is saved in
 * the console_old.log and a new console.log is opened.
 */
static void CheckConsoleSize(void)
{
    // If the maximum size is reached, then transfer the logs to console_old.log
    if (f_size(&console_file) > ((uint32_t)(CONFIG_CONSOLE_FILE_SIZE) * 1024u))
    {
        // First close the files in order to avoid issues when renaming and deleting files
        if (f_close(&console_file) != FR_OK)
        {
            KernelPanic();
        }
        if (f_close(&old_console_file) != FR_OK)
        {
            KernelPanic();
        }

        // Remove the old console file (we keep only one old file)
        if (f_unlink(OLD_CONSOLE_FILE_PATH) != FR_OK)
        {
            KernelPanic();
        }

        // Then rename the file
        if (f_rename(CONSOLE_FILE_PATH, OLD_CONSOLE_FILE_PATH) != FR_OK)
        {
            KernelPanic();
        }

        // Then we can open the console files again
        if (f_open(&console_file, CONSOLE_FILE_PATH, CONSOLE_FILE_ACCESS_MODE) != FR_OK)
        {
            KernelPanic();
        }
        if (f_open(&old_console_file, OLD_CONSOLE_FILE_PATH, OLD_CONSOLE_FILE_ACCESS_MODE) != FR_OK)
        {
            KernelPanic();
        }
    }
}

/**
 * @fn          ConsolePrintString(const char *str)
 * @brief       Function used to print a string
 * @param[in]   str String that will be printed
 * @return      Nothing
 */
static void ConsolePrintString(const char *str)
{
    // Calls ConsolePrintChar for each char of the string until the null terminator is reached
    while (*str != '\0')
    {
        ConsolePrintChar(*str++);
    }
}

/**
 * @fn          ConsolePrintChar(char c)
 * @brief       Function used to print a character
 * @param[in]   c Character that will be printed
 * @return      Nothing
 */
static void ConsolePrintChar(char c)
{
    UINT nb_writen;
    if ((f_write(&console_file, &c, 1u, &nb_writen) != FR_OK) || (nb_writen != 1u))
    {
        KernelPanic();
    }
}

/**
 * @fn          ConsoleSync(void)
 * @brief       Allow to flush data onto the file system if CONFIG_CONSOLE_FILE used
 * @return      Nothing
 */
static void ConsoleSync(void)
{
    if (f_sync(&console_file) != FR_OK)
    {
        KernelPanic();
    }
}

#endif /* CONFIG_CONSOLE_FILE */

/************************ UART Based Console Functions ***********************/

#if defined(CONFIG_CONSOLE_UART)

/**
 * @var     uart_print_conf
 * @brief   Print UART configuration definition
 */
extern const uartConf_t uart_print_conf;

/**
 * @var     uart_print_inst
 * @brief   Print UART instance definition
 */
extern uartInst_t uart_print_inst;

/**
 * @fn          ConsoleSpecificInit
 * @brief       Initialisation specific to the console type choosed
 * @return      Nothing
 */
static void ConsoleSpecificInit(void)
{
    if (UartOpen(&uart_print_inst, &uart_print_conf) != RET_SUCCESSFUL)
    {
        KernelPanic();
    }
}

/**
 * @fn          CheckConsoleSize
 * @brief       Check the console file size update the file if it reaches the maximum size
 * @return      Nothing
 *
 * Does nothing for this console type
 */
static void CheckConsoleSize(void)
{
    // Nothing to do
}

/**
 * @fn          ConsolePrintString(const char *str)
 * @brief       Function used to print a string
 * @param[in]   str String that will be printed
 * @return      Nothing
 */
static void ConsolePrintString(const char *str)
{
    // Calls ConsolePrintChar for each char of the string until the null terminator is reached
    while (*str != '\0')
    {
        ConsolePrintChar(*str++);
    }
}

/**
 * @fn          ConsolePrintChar(char c)
 * @brief       Function used to print a character
 * @param[in]   c Character that will be printed
 * @return      Nothing
 */
static void ConsolePrintChar(char c)
{
    (void)UartWrite(&uart_print_inst, (data_t)&c, sizeof(char));
}

/**
 * @fn          ConsoleSync(void)
 * @brief       Allow to flush data onto the file system if CONFIG_CONSOLE_FILE used
 * @return      Nothing
 *
 * Does nothing for this console type
 */
static void ConsoleSync(void)
{
    // Nothing to do
}

#endif /* CONFIG_CONSOLE_UART */

/************************ UART Based Console Functions ***********************/

#if defined(CONFIG_CONSOLE_ITM)

/**
 * @fn          ConsoleSpecificInit
 * @brief       Initialisation specific to the console type choosed
 * @return      Nothing
 */
static void ConsoleSpecificInit(void)
{
    // Nothing to do
}

/**
 * @fn          CheckConsoleSize
 * @brief       Check the console file size update the file if it reaches the maximum size
 * @return      Nothing
 *
 * Does nothing for this console type
 */
static void CheckConsoleSize(void)
{
    // Nothing to do
}

/**
 * @fn          ConsolePrintString(const char *str)
 * @brief       Function used to print a string
 * @param[in]   str String that will be printed
 * @return      Nothing
 */
static void ConsolePrintString(const char *str)
{
    // Calls ConsolePrintChar for each char of the string until the null terminator is reached
    while (*str != '\0')
    {
        ConsolePrintChar(*str++);
    }
}

/**
 * @fn          ConsolePrintChar(char c)
 * @brief       Function used to print a character
 * @param[in]   c Character that will be printed
 * @return      Nothing
 */
static void ConsolePrintChar(char c)
{
    (void)ITM_SendChar(c);
}

/**
 * @fn          ConsoleSync(void)
 * @brief       Allow to flush data onto the file system if CONFIG_CONSOLE_FILE used
 * @return      Nothing
 *
 * Does nothing for this console type
 */
static void ConsoleSync(void)
{
    // Nothing to do
}

#endif /* CONFIG_CONSOLE_ITM */

/****************** Circular Buffer Based Console Functions ******************/

#if defined(CONFIG_CONSOLE_CIRCULAR_BUFFER)

/**
 * @var     g_circular_buffer
 * @brief   Circular buffer for console printing
 */
uint8_t g_circular_buffer[CONFIG_CIRCULAR_BUFFER_SIZE * 1024u] __attribute__((aligned(32))) = { 0 };

/**
 * @fn          ConsoleSpecificInit
 * @brief       Initialisation specific to the console type choosed
 * @return      Nothing
 *
 * Does nothing for this console type
 */
static void ConsoleSpecificInit(void)
{
    // Nothing to do
}

/**
 * @fn          CheckConsoleSize
 * @brief       Check the console file size update the file if it reaches the maximum size
 * @return      Nothing
 *
 * Does nothing for this console type
 */
static void CheckConsoleSize(void)
{
    // Nothing to do
}

/**
 * @fn          ConsolePrintString(const char *str)
 * @brief       Function used to print a string
 * @param[in]   str String that will be printed
 * @return      Nothing
 */
static void ConsolePrintString(const char *str)
{
    // Calls ConsolePrintChar for each char of the string until the null terminator is reached
    while (*str != '\0')
    {
        ConsolePrintChar(*str++);
    }
}

/**
 * @fn          ConsolePrintChar(char c)
 * @brief       Function used to print a character
 * @param[in]   c Character that will be printed
 * @return      Nothing
 */
static void ConsolePrintChar(char c)
{
    // Variable declaration
    static uint32_t circular_buffer_index = 0u;

    // Check if the pointer reach the end of the circular buffer
    if (circular_buffer_index == ((uint32_t)CONFIG_CIRCULAR_BUFFER_SIZE * 1024u))
    {
        circular_buffer_index = 0u;
    }
    g_circular_buffer[circular_buffer_index] = c;
    circular_buffer_index++;
}

/**
 * @fn          ConsoleSync(void)
 * @brief       Allow to flush data onto the file system if CONFIG_CONSOLE_FILE used
 * @return      Nothing
 *
 * Does nothing for this console type
 */
static void ConsoleSync(void)
{
    // Nothing to do
}

#endif /* CONFIG_CONSOLE_CIRCULAR_BUFFER */
