/*
 * Copyright (c) 2023-2025, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "game.h"

void Game_Init(void) {
  Game_ModelInit();
  Game_ViewInit();
  Game_ControllerInit();
}
