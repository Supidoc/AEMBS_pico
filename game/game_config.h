/*
 * Copyright (c) 2023, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef GAME_CONFIG_H_
#define GAME_CONFIG_H_

#define GAME_CONFIG_HAS_ASTEROIDS       (0)
#define GAME_CONFIG_HAS_MISSILES        (0)

/* game score and settings */
#define GAME_CONFIG_HAS_SCORE           (0)
#if GAME_CONFIG_HAS_SCORE
  #define GAME_SCORE_MISSED_ASTEROID      (-5) /* costs of asteroid going out of screen */
  #define GAME_SCORE_HIT_ASTEROID         (10) /* score for hitting an asteroid */
  #define GAME_SCORE_USED_MISSILE         (-1) /* cost of using missile */
  #define GAME_SCORE_MISSILE_HIT_MISSILE  (5)  /* if a missile hits a missile */
  #define GAME_SCORE_SECOND_COUNT         (1) /* score for time of game: the longer the better */
#endif

#endif /* GAME_CONFIG_H_ */
