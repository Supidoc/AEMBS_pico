/*
 * Copyright (c) 2023-2025, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef GAMECONTROLLER_H_
#define GAMECONTROLLER_H_

#include "McuShell.h"
#include "buttons.h"
#include "McuDebounce.h"
#include "game_config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum Game_event_kind_e {
  Game_Event_Kind_None,         /*!< initialization value */
  Game_Event_Kind_Update_View,  /*!< request to update view */
  Game_Event_Kind_Game_Lost,    /*!< game is lost */
  Game_Event_Kind_Game_Won,     /*!< winning the game */
  Game_Event_Kind_Button,       /*!< button pressed */
  Game_Event_Kind_RandomSeed,   /*!< new random seed value */
} Game_event_kind_e;
  
typedef struct Game_event_t {
Game_event_kind_e kind; /* kind of event, data is inside union */
union u_ {
  struct { /* for Game_Event_Kind_Button */
    Buttons_e button;
    McuDbnc_EventKinds kind;
  } btn;
  struct { /* for Game_Event_Kind_RandomSeed */
    int val;
  } seed;
} u;
} Game_event_t;

/*!
 * \brief Send an event to the game controller
 * \param event Event to be sent
 */
void Game_SendEvent(Game_event_t *event);

/*!
 * \brief Event handler for the game, dealing with button events. Events called from debouncing.
 * \param button Button, for which the event applies
 * \param kind Kind of button event
 */
void Game_OnButtonEvent(Buttons_e button, McuDbnc_EventKinds kind);

/*!
 * \brief Event handler in the game to receive a new random seed value.
 * \param seed New random seed value
 */
void Game_OnNewRandomSeed(int seed);

/*!
 * \brief Command line handler
 * \param cmd String to command to be parsed
 * \param handled Returns if command has been handled
 * \param io I/O channel
 * \return Error code, ERR_OK if everything is ok
 */
uint8_t Game_ParseCommand(const uint8_t *cmd, bool *handled, McuShell_ConstStdIOType *io);

/*!
 * \brief Module initialization
 */
void Game_ControllerInit(void);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* GAMECONTROLLER_H_ */
