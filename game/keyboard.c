#include "game.h"

extern uint16_t keyboard_code;
extern char keyboard_keymap[];
uint16_t keyboard_key;
uint16_t keyboard_lastKey = 0;
uint16_t keyboard_lastCode = 0;

void
keyboard_read(void)
{
  char c = 0;

  if (keyboard_code != keyboard_lastCode) {
    if (keyboard_code <= 0x47) {
      c = keyboard_keymap[keyboard_code];
    }
  }

  keyboard_lastCode = keyboard_code;

  keyboard_lastKey = keyboard_key;
  keyboard_key = c;
}
