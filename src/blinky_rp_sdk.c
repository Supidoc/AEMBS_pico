/*
 * Copyright (c) 2025-2026, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "blinky_rp_sdk.h"
#include "hardware/gpio.h"

#include "platform.h"

static void delay(void) {
  for(volatile int i=0; i<5000000; i++) {
      __asm("nop");
  }
}

void blinky_sdk_rp(void) {
    const unsigned int LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    for(;;) {
        gpio_put(LED_PIN, 0);
        delay();
        gpio_put(LED_PIN, 1);
        delay();
    }
}
