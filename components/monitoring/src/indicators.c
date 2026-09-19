/**
 * @file    indicators.c
 * @author  Merlin Kooshmanian
 * @brief   Header file for system indicators (such as LED) handling
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

/******************************* Include Files *******************************/

#include "monitoring/indicators.h"
#include "drivers/peripherals.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

#ifdef CONFIG_HAS_INVERTED_SYSLED_LOGIC
#define SYSLED_ON  GPIO_PIN_RESET /**< Define GPIO line state for which the LED is ON */
#define SYSLED_OFF GPIO_PIN_SET   /**< Define GPIO line state for which the LED is OFF */
#else
#define SYSLED_ON  GPIO_PIN_SET   /**< Define GPIO line state for which the LED is ON */
#define SYSLED_OFF GPIO_PIN_RESET /**< Define GPIO line state for which the LED is OFF */
#endif

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/**
 * @var     led_status_conf
 * @brief   Status LED configuration definition
 */
extern const gpioConf_t led_status_conf;

/**
 * @var     led_status_inst
 * @brief   Status LED instance definition
 */
extern gpioInst_t led_status_inst;

/**
 * @var     led_error_conf
 * @brief   Error LED configuration definition
 */
extern const gpioConf_t led_error_conf;

/**
 * @var     led_error_inst
 * @brief   Error LED instance definition
 */
extern gpioInst_t led_error_inst;

/*************************** Functions Definitions ***************************/

/**
 * @copydoc InitIndicators
 */
void InitIndicators(void)
{
    returnCode_t return_value;

    // First initialises LED Status
    return_value = GpioOpen(&led_status_inst, &led_status_conf);
    if (return_value == RET_SUCCESSFUL)
    {
        return_value = GpioWrite(&led_status_inst, SYSLED_OFF);
        if (return_value != RET_SUCCESSFUL)
        {
            KernelPanic();
        }
    }
    else
    {
        KernelPanic();
    }

    // Then initialises LED Error
    if (return_value == RET_SUCCESSFUL)
    {
        return_value = GpioOpen(&led_error_inst, &led_error_conf);
        if (return_value == RET_SUCCESSFUL)
        {
            if (GpioWrite(&led_error_inst, SYSLED_OFF) != RET_SUCCESSFUL)
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

/**
 * @copydoc LEDStatToggle
 */
void LEDStatToggle(void)
{
    (void)GpioIoctl(&led_status_inst, IOCTL_GPIO_TOGGLE, NULL, 0u);
}

/**
 * @copydoc LEDErrorOn
 */
void LEDErrorOn(void)
{
    (void)GpioWrite(&led_status_inst, SYSLED_OFF);
    (void)GpioWrite(&led_error_inst, SYSLED_ON);
}
