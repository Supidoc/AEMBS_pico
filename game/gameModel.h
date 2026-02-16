/*
 * Copyright (c) 2022, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef GAMEMODEL_H_
#define GAMEMODEL_H_

#include <stdint.h>
#include <stdbool.h>
#include "game_config.h"
#include "McuGDisplaySSD1306.h"
#include "sprite.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum GameState_e {
  GAME_STATE_INTRO,
  GAME_STATE_RUNNING,
} GameState_e;

GameState_e Game_GetState(void);
void Game_SetState(GameState_e state);

#if GAME_CONFIG_HAS_SCORE
  void Game_AddScore(int32_t score);
  int32_t Game_GetScore(void);
  void Game_SetScore(int32_t score);

  void Game_LoadHighScore(void);
  int32_t Game_GetHighScore(void);
  void Game_SetHighScore(int32_t score);

  void Game_IncPlayTimeMs(uint32_t ms);
#endif

bool Game_CheckCollisions(void);
bool Game_AgeAndCheckIfAlive(void);

#if GAME_CONFIG_HAS_MISSILES
  bool Game_CheckAndRemoveMissiles(void);
  void Game_NewMissileFromSprite(Sprite_t *sprite, int8_t xMove, int8_t yMove);
#endif

#if GAME_CONFIG_HAS_ASTEROIDS
  bool Game_NewRandomAsteroid(void);
  bool Game_CheckAndRemoveAsteroids(void);
#endif

bool Game_CheckIfGameLost(void);
bool Game_CheckIfGameWon(void);

/*!
 * \brief Iterator to get all the sprite lists in the model (e.g. for painting)
 * \param cntr Iterator counter: Initialize with zero. Iterator will increase the value. If value returned is zero, list end has been reached.
 * \return Address of the List of Sprites, might be NULL
 */
Sprite_t **Game_IteratorGetSpriteLists(int *cntr);

typedef enum Game_Player_Speed_e {
  Game_Player_Speed_Normal,
  Game_Player_Speed_Fast,
  Game_Player_Speed_Fastest
} Game_Player_Speed_e;

typedef enum Game_PlayerAction_e {
  Game_Player_Move_Up,
  Game_Player_Move_Down,
  Game_Player_Move_Left,
  Game_Player_Move_Right,
#if GAME_CONFIG_HAS_MISSILES
  Game_Player_Fire,
#endif
} Game_PlayerAction_e;

/*!
 * \brief Perform a player action
 * \param action Action to be performed
 * \param speed Speed of action
 */
void Game_PlayerAction(Game_PlayerAction_e action, Game_Player_Speed_e speed);

/*!
 * \brief Free and release all game sprites
 */
void Game_FreeSprites(void);

/*!
 * \brief Decides if the model has moving sprites.
 * \return true if any sprites in the model are moving, false otherwise.
*/
bool Game_ModelHasMovingSprites(void);

/*!
 * \brief Decides if the model has animated sprites.
 * \return true if any sprites in the model are moving, false otherwise.
*/
bool Game_ModelHasAnimatedSprites(void);

/*!
 * \brief Create a new game and initialize model data structure.
 */
void Game_New(void);

/*!
 * \brief Model module de-initialization.
 */
void Game_ModelDeinit(void);

/*!
 * \brief Model module initialization.
 */
void Game_ModelInit(void);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* GAMEMODEL_H_ */
