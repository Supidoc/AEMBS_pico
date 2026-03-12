/*
 * Copyright (c) 2025-2026, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "blinky_rp_sdk.h"
#include "platform.h"
#if PL_CONFIG_USE_SDK_BLINKY && PL_CONFIG_USE_SDK_BLINKY_MCULIB != 1

#endif

#if PL_CONFIG_USE_SDK_BLINKY_MCULIB == 1
#include "McuLED.h"
#include "McuGPIO.h"
#include "McuWait.h"
#endif

#include "platform.h"

#if PL_CONFIG_USE_SDK_BLINKY && PL_CONFIG_USE_SDK_BLINKY_MCULIB != 1
static void delay(void)
{
    for (volatile int i = 0; i < 5000000; i++)
    {
        __asm("nop");
    }
}
#endif

void blinky_sdk_rp(void)
{
#if PL_CONFIG_USE_SDK_BLINKY && PL_CONFIG_USE_SDK_BLINKY_MCULIB != 1

    const unsigned int LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    for (;;)
    {
        gpio_put(LED_PIN, 0);
        delay();
        gpio_put(LED_PIN, 1);
        delay();
    }

#endif
#if PL_CONFIG_USE_SDK_BLINKY_MCULIB == 1

    McuGPIO_Config_t gpioConfig;
    McuGPIO_Handle_t gpioPin;

    McuLED_Config_t ledConfig;
    McuLED_Handle_t ledPin;

    McuGPIO_GetDefaultConfig(&gpioConfig);

    gpioConfig.isInput = true;
    gpioConfig.hw.pin  = 11;
    gpioConfig.hw.pull = McuGPIO_PULL_UP;

    gpioPin = McuGPIO_InitGPIO(&gpioConfig);

    if (gpioPin == NULL)
    {
        for (;;)
            ;
    }

    McuLED_GetDefaultConfig(&ledConfig);

    ledConfig.isOnInit    = false;
    ledConfig.hw.pin      = PICO_DEFAULT_LED_PIN;
    ledConfig.isLowActive = false;
    ledConfig.isOnInit    = false;

    ledPin = McuLED_InitLed(&ledConfig);

    if (ledPin == NULL)
    {
        for (;;)
            ;
    }

    bool buttonState = false;
    for (;;)
    {
        buttonState = McuGPIO_GetValue(gpioPin);
        McuLED_Set(ledPin, buttonState);
        McuWait_Waitms(10);
    }

#endif
}
