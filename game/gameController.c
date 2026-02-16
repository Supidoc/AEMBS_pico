/*
 * Copyright (c) 2023-2025, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "platform.h"
#include "game.h"
#include "McuRTOS.h"
#include "McuLog.h"
#include "leds.h"
#include "McuSSD1306.h"
#include "McuGDisplaySSD1306.h"
#include "gameView.h"
#include "McuFontDisplay.h"
#include "McuGFont.h"
#include "McuUtility.h"
#include "sprite.h"
#include "gameModel.h"
#include "gameView.h"

static Game_event_t game_event;
static bool new_event = false;

void Game_SendEvent(Game_event_t *event) {
  game_event = *event; /* struct copy */
  new_event = true;
}

static bool Game_GetEvent(Game_event_t *pEvent) {
  if (new_event) {
    new_event = false; /* reset flag */
    *pEvent = game_event; /* struct copy */
    return true;
  }
  return false; /* no new event */
}

void Game_OnNewRandomSeed(int seed) {
  Game_event_t event;

  event.kind = Game_Event_Kind_RandomSeed;
  event.u.seed.val = seed;
  Game_SendEvent(&event);
}

void Game_OnButtonEvent(Buttons_e button, McuDbnc_EventKinds kind) {
  Game_event_t event;

  event.kind = Game_Event_Kind_Button;
  event.u.btn.button = button;
  event.u.btn.kind = kind;
  Game_SendEvent(&event);
}

static uint8_t PrintStatus(McuShell_ConstStdIOType *io) {
  McuShell_SendStatusStr((const unsigned char*)"game", (const unsigned char*)"Game module status\r\n", io->stdOut);
#if GAME_CONFIG_HAS_SCORE
  unsigned char buf[32];
  buf[0] = '\0';
  McuUtility_strcatNum32u(buf,sizeof(buf), Game_GetScore());
  McuUtility_strcat(buf,sizeof(buf), (unsigned char*)"\r\n");
  McuShell_SendStatusStr((const unsigned char*)"  score", buf, io->stdOut);
#endif
  return ERR_OK;
}

static uint8_t PrintHelp(McuShell_ConstStdIOType *io) {
  McuShell_SendHelpStr((unsigned char*)"game", (const unsigned char*)"Group of game commands\r\n", io->stdOut);
  McuShell_SendHelpStr((unsigned char*)"  help|status", (const unsigned char*)"Print help or status information\r\n", io->stdOut);
  return ERR_OK;
}

uint8_t Game_ParseCommand(const uint8_t *cmd, bool *handled, McuShell_ConstStdIOType *io) {
  if (McuUtility_strcmp((char*)cmd, McuShell_CMD_HELP)==0 || McuUtility_strcmp((char*)cmd, "game help")==0) {
    *handled = TRUE;
    return PrintHelp(io);
  } else if ((McuUtility_strcmp((char*)cmd, McuShell_CMD_STATUS)==0) || (McuUtility_strcmp((char*)cmd, "game status")==0)) {
    *handled = TRUE;
    return PrintStatus(io);
  }
  return ERR_OK;
}

static void delay_ms(uint32_t ms) {
  vTaskDelay(pdMS_TO_TICKS(ms));
}

static void gameTask(void *pv) {
  Game_event_t event;
#if GAME_CONFIG_HAS_MISSILES
  TickType_t lastMissileTickCount=0, currTickCount;
#endif
#if GAME_CONFIG_HAS_SCORE
  TickType_t prevTicks=0, currTicks, ms;
#endif
  bool update;
  bool hasEvent;

  /* initialize display */
  McuSSD1306_Init();
  McuGDisplaySSD1306_Init();

  Game_New();
  Game_ShowScreen(GAME_SCREEN_INTRO);
  delay_ms(2000);
  for(;;) {
    Game_ShowScreen(GAME_SCREEN_GAMEPLAY);
    update = false;
    hasEvent = Game_GetEvent(&event);
    if (Game_CheckIfGameWon()) { /* have we won the game? */
      event.kind = Game_Event_Kind_Game_Won;
      hasEvent = true;
    } else if (Game_CheckIfGameLost()) { /* game over? */
      event.kind = Game_Event_Kind_Game_Lost;
      hasEvent = true;
    }
  #if GAME_CONFIG_HAS_SCORE
    currTicks = xTaskGetTickCount();
    if (prevTicks!=0) { /* only if not at the start of the game */
      ms = pdTICKS_TO_MS(currTicks-prevTicks);
      Game_IncPlayTimeMs(ms);
    }
    prevTicks = currTicks;
  #endif
  #if GAME_CONFIG_HAS_ASTEROIDS
    update |= Game_NewRandomAsteroid();
  #endif
    if (hasEvent) {
      switch(event.kind) {
        case Game_Event_Kind_RandomSeed:
          McuUtility_randomSetSeed(event.u.seed.val);
          break;
        case Game_Event_Kind_Game_Lost:
          Game_ShowScreen(GAME_SCREEN_GAME_LOST);
          for(;;) {
            delay_ms(1000);
          }
          break;
        case Game_Event_Kind_Game_Won:
          Game_ShowScreen(GAME_SCREEN_GAME_WON);
          for(;;) {
            delay_ms(1000);
          }
          break;

        case Game_Event_Kind_Update_View:
          update |= true;
          break;

        case Game_Event_Kind_Button:
          update |= true;
          switch(event.u.btn.button) {
            case BUTTONS_NAV_CENTER:
          #if GAME_CONFIG_HAS_MISSILES
              if (event.u.btn.kind==MCUDBNC_EVENT_PRESSED) {
                  currTickCount = xTaskGetTickCount();
                  if (currTickCount-lastMissileTickCount > pdMS_TO_TICKS(750)) { /* limit shooting cadence  */
                    lastMissileTickCount = currTickCount;
                    Game_PlayerAction(Game_Player_Fire, Game_Player_Speed_Normal);
                  }
              }
          #endif
              break;
            case BUTTONS_NAV_UP:
              if (event.u.btn.kind==MCUDBNC_EVENT_PRESSED) {
                Game_PlayerAction(Game_Player_Move_Up, Game_Player_Speed_Normal);
              } else if (event.u.btn.kind==MCUDBNC_EVENT_PRESSED_REPEAT) {
                Game_PlayerAction(Game_Player_Move_Up, Game_Player_Speed_Fast);
              } else if (event.u.btn.kind==MCUDBNC_EVENT_LONG_PRESSED_REPEAT) {
                Game_PlayerAction(Game_Player_Move_Up, Game_Player_Speed_Fastest);
              }
              break;
            case BUTTONS_NAV_DOWN:
              if (event.u.btn.kind==MCUDBNC_EVENT_PRESSED) {
                Game_PlayerAction(Game_Player_Move_Down, Game_Player_Speed_Normal);
              } else if (event.u.btn.kind==MCUDBNC_EVENT_PRESSED_REPEAT) {
                Game_PlayerAction(Game_Player_Move_Down, Game_Player_Speed_Fast);
              } else if (event.u.btn.kind==MCUDBNC_EVENT_LONG_PRESSED_REPEAT) {
                Game_PlayerAction(Game_Player_Move_Down, Game_Player_Speed_Fastest);
              }
              break;
            case BUTTONS_NAV_LEFT:
              if (event.u.btn.kind==MCUDBNC_EVENT_PRESSED) {
                Game_PlayerAction(Game_Player_Move_Left, Game_Player_Speed_Normal);
              } else if (event.u.btn.kind==MCUDBNC_EVENT_PRESSED_REPEAT) {
                Game_PlayerAction(Game_Player_Move_Left, Game_Player_Speed_Fast);
              } else if (event.u.btn.kind==MCUDBNC_EVENT_LONG_PRESSED_REPEAT) {
                Game_PlayerAction(Game_Player_Move_Left, Game_Player_Speed_Fastest);
              }
              break;
            case BUTTONS_NAV_RIGHT:
              if (event.u.btn.kind==MCUDBNC_EVENT_PRESSED) {
                Game_PlayerAction(Game_Player_Move_Right, Game_Player_Speed_Normal);
              } else if (event.u.btn.kind==MCUDBNC_EVENT_PRESSED_REPEAT) {
                Game_PlayerAction(Game_Player_Move_Right, Game_Player_Speed_Fast);
              } else if (event.u.btn.kind==MCUDBNC_EVENT_LONG_PRESSED_REPEAT) {
                Game_PlayerAction(Game_Player_Move_Right, Game_Player_Speed_Fastest);
              }
              break;
            default:
              break;
          } /* switch button */
          break;

        default:
          break;
      } /* switch */
    } /* if has event */
    if (Game_ModelHasMovingSprites() || Game_ModelHasAnimatedSprites()) {
      /* update view and check collisions/etc */
      update |= Game_AgeAndCheckIfAlive();
      update |= Game_CheckCollisions();
      #if GAME_CONFIG_HAS_MISSILES
      update |= Game_CheckAndRemoveMissiles();
      #endif
      #if GAME_CONFIG_HAS_ASTEROIDS 
      update |= Game_CheckAndRemoveAsteroids();
      #endif
      if (update) {
        Game_ShowScreen(GAME_SCREEN_GAMEPLAY);
      }
    }
    Leds_Neg(LEDS_GREEN);
    delay_ms(100);
  } /* for */
}

void Game_ControllerDeinit(void) {
}

void Game_ControllerInit(void) {
  BaseType_t res;

  res = xTaskCreate(gameTask, "gameTask", 600/sizeof(StackType_t), NULL, tskIDLE_PRIORITY+1, NULL);
  if (res!=pdPASS) {
    /* error! */
    for(;;) {}
  }
  Game_ViewInit();
  Game_ModelInit();
}
