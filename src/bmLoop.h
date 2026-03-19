
/*
 * Copyright (c) 2025-2026, Dominik Grob
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __BMLOOP_H__
#define __BMLOOP_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "bmLoopConfig.h"
#include "stdint.h"

  void BML_loop(void);

  void BML_init(void);
  void BML_deinit(void);

#if BML_PROCESS_BUTTONS
  void BML_OnISRButtonPressed(uint32_t buttonBits);
#endif
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __BMLOOP_H__ */
