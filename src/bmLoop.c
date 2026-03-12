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

void BML_loop(void)
{
  static uint32_t counter = 0;
  for (;;)
  {
    counter++;
    if (counter % (BML_LED_DELAY_MS /
                   McuShellCdcDevice_CONFIG_PROCESS_WAIT_TIME_MS) ==
        0)
    {
      Leds_Neg(LEDS_BLUE);
    }
    if (counter % (BML_MSG_DELAY_MS /
                   McuShellCdcDevice_CONFIG_PROCESS_WAIT_TIME_MS) ==
        0)
    {
      McuShellCdcDevice_WriteStr("Hello World!\r\n");
      McuShellCdcDevice_Flush();
    }

    if (counter >= BML_MSG_DELAY_MS * BML_LED_DELAY_MS /
                       McuShellCdcDevice_CONFIG_PROCESS_WAIT_TIME_MS)
    {
      counter = 0;
    }
    McuShellCdcDevice_Process();
    McuWait_Waitms(McuShellCdcDevice_CONFIG_PROCESS_WAIT_TIME_MS);
  }
}

void BML_init(void) {}

void BML_deinit(void) {}
