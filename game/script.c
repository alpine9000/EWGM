#include "game.h"

#ifdef DEBUG
__EXTERNAL uint16_t script_port = 0;
uint32_t script_breakpoint = 0xffffffff;

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

  } else if (script_port & 0x8000) {
    script_breakpoint = script_port & 0x7fff;    
  }

  script_port = 0;
}
#endif
