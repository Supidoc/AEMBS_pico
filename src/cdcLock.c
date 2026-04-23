/*
 * Copyright (c) 2025-2026, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "platform.h"
#include "cdcLock.h"
#include "McuRTOS.h"

static SemaphoreHandle_t mutex = NULL;

void CdcLock_Take(void)
{
  if (mutex != NULL)
  {
    if (xSemaphoreTake(mutex, portMAX_DELAY) != pdTRUE)
    {
      for (;;)
      {
      }
    }
  }
  else
  {
    for (;;)
    {
    }
  }
}

void CdcLock_Give(void)
{
  if (mutex != NULL)
  {
    xSemaphoreGive(mutex);
  }
  else
  {
    for (;;)
    {
    }
  }
}

void CdcLock_Deinit(void)
{
  if (mutex != NULL)
  {
    vSemaphoreDelete(mutex);
    mutex = NULL;
  }
}

void CdcLock_Init(void)
{
  if (mutex == NULL)
  {
    mutex = xSemaphoreCreateRecursiveMutex();
  }

  if (mutex == NULL)
  {
    for (;;)
    {
    }
  }

  vQueueAddToRegistry(mutex, "CdcLockMutex");
}
