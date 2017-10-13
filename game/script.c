#include "game.h"

#ifdef SCRIPTING
__EXTERNAL uint16_t script_port = 0;

void
script_process(void)
{
  if (script_port > 0 && script_port <= 8) {
    hw_joystickPos = script_port;
  } else if (script_port > 8 && script_port <= 255) {
    if (script_port == 10) {
      keyboard_code = keyboard_lastCode = KEYBOARD_CODE_RETURN;
      keyboard_key = script_port;
    } else {
      keyboard_code = 0; // todo: correct code
      keyboard_key = script_port;
    }
  }

  script_port = 0;
}
#endif
