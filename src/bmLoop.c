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
#include "buttons.h"
#include "stdio.h"

#if BML_PROCESS_BUTTONS
void ProcessButtons(void);
void GenerateButtonMessage(bool currentState, bool previousState, const char *buttonName);
#endif

#if BML_PROCESS_BUTTONS
static uint32_t BML_buttonPressedBits = 0;
#endif

void BML_loop(void)
{
#if BML_ENABLE_LED
  static uint32_t ledCounter = 0;
#endif
#if BML_ENABLE_MSG
  static uint32_t msgCounter = 0;
#endif
  for (;;)
  {
#if BML_ENABLE_LED
    ledCounter++;
    if (ledCounter >= BML_LED_DELAY_MS / McuShellCdcDevice_CONFIG_PROCESS_WAIT_TIME_MS)
    {
      Leds_Neg(LEDS_BLUE);
      ledCounter = 0;
    }
#endif
#if BML_ENABLE_MSG
    if (msgCounter >= BML_MSG_DELAY_MS / McuShellCdcDevice_CONFIG_PROCESS_WAIT_TIME_MS)
    {
      McuShellCdcDevice_WriteStr("Hello World!\r\n");
      McuShellCdcDevice_Flush();
      msgCounter = 0;
    }
#endif
#if BML_PROCESS_BUTTONS
    ProcessButtons();
#endif
    McuShellCdcDevice_Process();
    McuWait_Waitms(McuShellCdcDevice_CONFIG_PROCESS_WAIT_TIME_MS);
  }
}

void BML_init(void) {}

void BML_deinit(void) {}

#if BML_PROCESS_BUTTONS
void ProcessButtons(void)
{
  static bool            previousState[BUTTONS_NOF_BUTTONS]  = {0};
  static const Buttons_e buttonBitsEnum[BUTTONS_NOF_BUTTONS] = {BUTTONS_BIT_NAV_UP, BUTTONS_BIT_NAV_DOWN, BUTTONS_BIT_NAV_LEFT,
                                                                BUTTONS_BIT_NAV_RIGHT, BUTTONS_BIT_NAV_CENTER};
  static const char     *buttonNames[BUTTONS_NOF_BUTTONS]    = {"NAV_UP", "NAV_DOWN", "NAV_LEFT", "NAV_RIGHT", "NAV_CENTER"};

  for (size_t i = 0; i < BUTTONS_NOF_BUTTONS; i++)
  {
    bool currentState = BML_buttonPressedBits & buttonBitsEnum[i];
    BML_buttonPressedBits &= ~buttonBitsEnum[i]; /* clear the bit after processing */
    if (currentState)
    {
      char msg[64];
      snprintf(msg, sizeof(msg), "Button %s pressed\r\n", buttonNames[i]);
      McuShellCdcDevice_WriteStr(msg);
    }

    previousState[i] = currentState;
  }

  McuShellCdcDevice_Flush();
}

void GenerateButtonMessage(bool currentState, bool previousState, const char *buttonName)
{

  if (currentState && !previousState)
  {
    char msg[64];
    snprintf(msg, sizeof(msg), "Button %s pressed\r\n", buttonName);
    McuShellCdcDevice_WriteStr(msg);
    McuShellCdcDevice_Flush();
  }
  else if (!currentState && previousState)
  {
    char msg[64];
    snprintf(msg, sizeof(msg), "Button %s released\r\n", buttonName);
    McuShellCdcDevice_WriteStr(msg);
    McuShellCdcDevice_Flush();
  }
}

void BML_OnISRButtonPressed(uint32_t buttonBits) { BML_buttonPressedBits = buttonBits; }
#endif
