/*
 * Copyright (c) 2025-2026, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "platform.h"
#if PL_CONFIG_USE_SDK_BLINKY
#include "blinky_rp_sdk.h"
#endif

#if PL_CONFIG_USE_BM_LOOP
#include "bmLoop.h"
#endif

void App_Run(void)
{
  PL_Init(); /* initialize platform and drivers */
#if PL_CONFIG_USE_SDK_BLINKY
  blinky_sdk_rp();
#endif

#if PL_CONFIG_USE_BM_LOOP
  BML_loop();
#endif
  PL_Deinit(); /* deinitialize the platform */
}
