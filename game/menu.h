#ifndef __MENU_H
#define __MENU_H

#define MENU_SCREEN_WIDTH 320
#define MENU_SCREEN_WIDTH_BYTES (MENU_SCREEN_WIDTH/8)

typedef enum {
  MENU_COMMAND_PLAY = 0,
  MENU_COMMAND_EXIT,
  MENU_COMMAND_REPLAY,
  MENU_COMMAND_RECORD,
  MENU_COMMAND_MENU,
  MENU_COMMAND_LEVEL,
  MENU_COMMAND_NUM_PLAYERS
} menu_command_t;

typedef enum {
  MENU_MODE_INACTIVE = 0,
  MENU_MODE_MENU = 1,
  MENU_MODE_HISCORES = 2
} menu_mode_t;

menu_command_t
menu_loop(menu_mode_t mode);

#endif
