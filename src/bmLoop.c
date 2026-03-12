/*
 * Copyright (c) 2025-2026, Dominik Grob
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "McuShellCdcDevice.h"
#include "McuWait.h"
#include "bmLoopConfig.h"
#include "leds.h"
#include "stdint.h"

void BML_loop(void) {
  static uint16_t counter = 0;
  for (;;) {
    counter++;
    if (counter >=
        BML_LED_DELAY_MS / McuShellCdcDevice_CONFIG_PROCESS_WAIT_TIME_MS) {
      Leds_Neg(LEDS_BLUE);
      McuShellCdcDevice_WriteStr("Hello World!\r\n");
      McuShellCdcDevice_Flush();

      counter = 0;
    }
    McuShellCdcDevice_Process();
    McuWait_Waitms(McuShellCdcDevice_CONFIG_PROCESS_WAIT_TIME_MS);
  }
}

void BML_init(void) {}

void BML_deinit(void) {}
