/*
 * Copyright (c) 2023-2025, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef GAME_H_
#define GAME_H_

#include "game_config.h"
#include "gameModel.h"
#include "gameView.h"
#include "gameController.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Module initialization
 */
void Game_Deinit(void);

/*!
 * \brief Initializes the game, included the model, view and controller.
 */
void Game_Init(void);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* GAME_H_ */
