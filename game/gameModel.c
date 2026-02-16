/*
 * Copyright (c) 2023, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "game.h"
#include "gameModel.h"
#include "gameView.h"
#include "gameController.h"
#include "McuUtility.h"
#include <string.h>
#include <assert.h>
#include "sprite.h"
#include "shipa.h"
#include "asteroid0.h"
#include "asteroid1.h"
#include "asteroid2.h"
#include "boom0.h"
#include "boom1.h"

typedef struct GameModel_t {
  GameState_e state;
  Sprite_t *player; /* sprite for the player */
#if GAME_CONFIG_HAS_MISSILES
  Sprite_t *missiles; /* the list of missiles */
#endif
#if GAME_CONFIG_HAS_ASTEROIDS
  Sprite_t *asteroids; /* the list of asteroids */
#endif
#if GAME_CONFIG_HAS_SCORE
  int32_t score; /* game score */
  uint32_t timeMs; /* gaming time in milli-seconds */
#endif
} GameModel_t;

static GameModel_t game; /* game data */

GameState_e Game_GetState(void) {
  return game.state;
}

void Game_SetState(GameState_e state) {
  game.state = state;
}

#if GAME_CONFIG_HAS_SCORE
int32_t Game_GetScore(void) {
  int32_t score;

  score = game.score + (game.timeMs/1000)*GAME_SCORE_SECOND_COUNT;
  if (score<0) {
    score = 0; /* if there is an underflow */
  }
  return score;
}

void Game_AddScore(int32_t score) {
  game.score += score;
}

void Game_SetScore(int32_t score) {
  game.score = score;
}
#endif

bool Game_CheckIfGameLost(void) {
  return game.player==NULL;
}

bool Game_CheckIfGameWon(void) {
  return false;
}

#if GAME_CONFIG_HAS_SCORE
void Game_IncPlayTimeMs(uint32_t ms) {
  game.timeMs += ms;
}
#endif

void Game_PlayerAction(Game_PlayerAction_e action, Game_Player_Speed_e speed) {
  int x, y;

  switch (action) {
    case Game_Player_Move_Up:
    case Game_Player_Move_Down:
    case Game_Player_Move_Left:
    case Game_Player_Move_Right:
    switch (action) {
      case Game_Player_Move_Up:    x=0;  y=-1; break;
      case Game_Player_Move_Down:  x=0;  y=1; break;
      case Game_Player_Move_Left:  x=-1; y=0; break;
      case Game_Player_Move_Right: x=1;  y=0; break;
      default: x = y = 0; break;
    } /* switch */
    if (speed==Game_Player_Speed_Fast) {
      x = 2*x;
      y = 2*y;
    } else if (speed==Game_Player_Speed_Fastest) {
      x = 4*x;
      y = 4*y;
    }
    Sprite_MoveLimitedDisplay(game.player, x, y);
    break;

#if GAME_CONFIG_HAS_MISSILES
    case Game_Player_Fire:
        x = 1;
        y = 0;
      Game_NewMissileFromSprite(game.player, x, y);
      #if GAME_CONFIG_HAS_SCORE
      game.score += GAME_SCORE_USED_MISSILE; /* missile costs one point */
      #endif
      break;
#endif
    default:
      break;
  } /* switch */
}

void Game_FreeSprites(void) {
  int cntr = 0;
  Sprite_t **sprites;

  do {
    sprites = Game_IteratorGetSpriteLists(&cntr);
    if (sprites!=NULL) {
      Sprite_DeleteList(sprites);
    }
  } while(cntr!=0);
}

#if GAME_CONFIG_HAS_MISSILES
static PIMAGE NewMissilePImage(void) {
  PIMAGE missileImage;
  McuGDisplaySSD1306_PixelDim w, h;

#if GAME_CONFIG_KIND==GAME_CONFIG_KIND_ASTEROIDS
  w = 3; h = 1;
#elif GAME_CONFIG_KIND==GAME_CONFIG_KIND_INVADER
  w = 1; h = 3;
#else
  #error "game?"
#endif
  missileImage = Sprite_NewEmptyPImage(w, h);
  memset((void*)missileImage->pixmap, 0xff, missileImage->size); /* initialize all pixels 1 */
  return missileImage;
}

void Game_NewMissileFromSprite(Sprite_t *sprite, int8_t xMove, int8_t yMove) {
  Sprite_t *missile;
  PIMAGE missileImage;
  McuGDisplaySSD1306_PixelDim x, y; /* new coordinates */
  McuGDisplaySSD1306_PixelDim sw, sh; /* sprite width and height */
  McuGDisplaySSD1306_PixelDim mw, mh; /* missile width and height */

  if (sprite!=NULL) {
    missileImage = NewMissilePImage();
    mw = missileImage->width;
    mh = missileImage->height;
    sw = sprite->images->image->width;
    sh = sprite->images->image->height;
    if (yMove>0 && xMove==0) { /* move missile down from sprite */
      if (sw>mw) { /* center x in the middle of the sprite */
        x = sprite->posX + (sw-mw)/2;
      } else {
        x = sprite->posX - (mw-sw)/2;
      }
      y = sprite->posY + sh; /* set y below the sprite */
    } else if (yMove<0 && xMove==0) { /* move missile up from sprite */
      if (sw>mw) { /* center x in the middle of the sprite */
        x = sprite->posX + (sw-mw)/2;
      } else {
        x = sprite->posX - (mw-sw)/2;
      }
      y = sprite->posY - mh; /* set y on top of the sprite */
    } else if (yMove==0 && xMove>0) { /* move missile right from sprite */
      x = sprite->posX + sw;
      if (sh>mh) {
        y = sprite->posY + (sh-mh)/2;
      } else {
        y = sprite->posY - (mh-sh)/2;
      }
    } else if (yMove==0 && xMove<0) { /* move missile left from sprite */
      x = sprite->posX - mw;
      if (sh>mh) {
        y = sprite->posY + (sh-mh)/2;
      } else {
        y = sprite->posY - (mh-sh)/2;
      }
    } else { /* should not get here */
      assert(false);
    }
    missile = Sprite_New(Sprite_NewImageFromRAM(missileImage), x, y);
    missile->move.x = xMove;
    missile->move.y = yMove;
    Sprite_AddToList(&game.missiles, missile);
  }
}
#endif

#if GAME_CONFIG_HAS_ASTEROIDS
bool Game_NewRandomAsteroid(void) {
  int idx;
  SpriteImage_t *images; /* image list */
  Sprite_t *sprite;
  PIMAGE asteroidImage;
  McuGDisplaySSD1306_PixelDim x, y;

  idx = McuUtility_random(0, 1000); /* get a random number */
  if (idx<50) { /* to reduce probability */
    /* pick random asteroid sprite image */
    if ((idx%2)==0) {
      asteroidImage = Asteroid0_GetImage();
    } else if ((idx%2)==1) {
      asteroidImage = Asteroid1_GetImage();
    } else {
      asteroidImage = Asteroid2_GetImage();
    }
    images = Sprite_NewImageFromROM(asteroidImage);
    x = McuGDisplaySSD1306_GetWidth()-1; /* starts at the first pixel of the right border */
    y = McuUtility_random(0, McuGDisplaySSD1306_GetHeight()-1-asteroidImage->height); /* fully inside display */
    sprite = Sprite_New(images, x, y);
    sprite->move.x = -1;
    sprite->move.y = 0;
    Sprite_AddToList(&game.asteroids, sprite);
    return true; /* something has changed */
  }
  return false; /* nothing has */
}
#endif

bool Game_CheckCollisions(void) {
  bool changed = false;
  const PIMAGE boom0 = Boom0_GetImage();
  const PIMAGE boom1 = Boom1_GetImage();
  int score;

  /* check if we have hit something: */
#if GAME_CONFIG_HAS_MISSILES
  changed |= Sprite_CheckCollision(&game.missiles, true, &game.player, 0, NULL, boom0, boom1); /* check if missile hit the player */
#if GAME_CONFIG_HAS_SCORE
  score = GAME_SCORE_MISSILE_HIT_MISSILE;
#else
  score = 0;
#endif
  changed |= Sprite_CheckCollision(&game.missiles, true, &game.missiles, score, NULL, boom0, boom1); /* check if we have missile-missile collisions */
#endif
#if GAME_CONFIG_HAS_MISSILES && GAME_CONFIG_HAS_ASTEROIDS
#if GAME_CONFIG_HAS_SCORE
  score = GAME_SCORE_HIT_ASTEROID;
#else
  score = 0;
#endif
  changed |= Sprite_CheckCollision(&game.missiles, true, &game.asteroids, score, NULL, boom0, boom1); /* check if we hit asteroids */
#endif
#if GAME_CONFIG_HAS_ASTEROIDS
  changed |= Sprite_CheckCollision(&game.asteroids, false, &game.player, 0, NULL, boom0, boom1); /* check if we the player touches an asteroid */
#endif
  return changed;
}

bool Game_AgeAndCheckIfAlive(void) {
  bool changed = false;
  int cntr = 0;
  Sprite_t **sprites;

  do {
    sprites = Game_IteratorGetSpriteLists(&cntr);
    if (sprites!=NULL) {
      changed |= Sprite_AgeAndCheckIfAlive(sprites);
    }
  } while(cntr!=0);
  return changed;
}

#if GAME_CONFIG_HAS_ASTEROIDS
bool Game_CheckAndRemoveAsteroids(void) {
  /* check if asteroids are getting outside of screen */
  Sprite_t *s, *next;
  bool updateView = false;

  s = game.asteroids;
  while (s!=NULL) {
    if (s->posX <= -((int)s->images->image->width)) { /* out of display area: remove */
      #if GAME_CONFIG_HAS_SCORE
      game.score += GAME_SCORE_MISSED_ASTEROID; /* missed asteroid */
      #endif
      next = s->next;
      Sprite_DeleteFromList(&game.asteroids, s);
      updateView |= true;
      s = next;
    } else {
      s = s->next;
    }
  }
  return updateView;
}
#endif

#if GAME_CONFIG_HAS_MISSILES
bool Game_CheckAndRemoveMissiles(void) {
  /* check if missiles are getting outside of screen */
  Sprite_t *s, *next;
  bool updateView = false;

  s = game.missiles;
  while (s!=NULL) {
#if GAME_CONFIG_KIND==GAME_CONFIG_KIND_ASTEROIDS /* left and right border */
    if (   s->posX <= -((int)s->images->image->width) /* outside left border */
        || s->posX >= McuGDisplaySSD1306_GetWidth() /* outside right border */
        )
#elif GAME_CONFIG_KIND==GAME_CONFIG_KIND_INVADER /* top and bottom border */
  if (   s->posY <= -((int)s->images->image->height) /* outside top border */
      || s->posY >= McuGDisplaySSD1306_GetHeight() /* outside lower border */
      )
#else
  #error "game?"
#endif
    { /* out of display area: remove */
      next = s->next;
      Sprite_DeleteFromList(&game.missiles, s);
      updateView |= true;
      s = next;
    } else {
      s = s->next;
    }
  }
  return updateView;
}
#endif

Sprite_t **Game_IteratorGetSpriteLists(int *cntr) {
  Sprite_t **sprites;

  switch(*cntr) {
    case 0: sprites = &game.player;     (*cntr)++; break;
#if GAME_CONFIG_HAS_MISSILES
    case 1: sprites = &game.missiles;  (*cntr)++; break;
#endif
#if GAME_CONFIG_HAS_ASTEROIDS
    case 2: sprites = &game.asteroids; (*cntr)++; break;
#endif
    default: sprites = NULL; (*cntr) = 0; /* end of list */ break;
  }
  return sprites;
}

bool Game_ModelHasMovingSprites(void) {
  bool moving = false;
  int cntr = 0;
  Sprite_t **sprites;

  do {
    sprites = Game_IteratorGetSpriteLists(&cntr);
    if (sprites!=NULL) {
      moving |= Sprite_HasMovingSpriteInList(*sprites);
    }
    if (moving) {
      return true;
    }
  } while(cntr!=0);
  return false;
}

bool Game_ModelHasAnimatedSprites(void) {
  bool moving = false;
  int cntr = 0;
  Sprite_t **sprites;

  do {
    sprites = Game_IteratorGetSpriteLists(&cntr);
    if (sprites!=NULL) {
      moving |= Sprite_HasAnimatedSpriteInList(*sprites);
    }
    if (moving) {
      return true;
    }
  } while(cntr!=0);
  return false;
}

static void CreatePlayer(void) {
  SpriteImage_t *images; /* image list */
  PIMAGE player;
  McuGDisplaySSD1306_PixelDim x, y;

  player = ShipA_GetImage();
  images = Sprite_NewImageFromROM(player);
  x = 0;
  y = (McuGDisplaySSD1306_GetHeight() - player->height)/2;
  game.player = Sprite_New(images, x, y);
}

void Game_New(void) {
#if GAME_CONFIG_HAS_SCORE
  game.score = 0; /* reset score */
  game.timeMs = 0; /* reset time */
#endif
  CreatePlayer();
}

void Game_ModelDeinit(void) {
}

void Game_ModelInit(void) {
}
