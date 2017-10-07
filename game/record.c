#include "game.h"

#ifdef GAME_RECORDING

__EXTERNAL record_t* record_ptr = (record_t*)&levelFast.recordData;
__EXTERNAL record_state_t record_state;

record_state_t
record_getState(void)
{
  return levelFast.record->state;
}


void
record_setState(record_state_t state)
{
  record_state = state;
  levelFast.record->size = sizeof(record_t);
  levelFast.record->frame = 0;
  levelFast.record->state = state;
  levelFast.record->index = 0;
  levelFast.record->lastJoystickPos = 0xff;
  levelFast.record->lastJoystickButton = 0xff;
  levelFast.record->lastJoystick2Pos = 0xff;
  levelFast.record->lastJoystick2Button = 0xff;
  levelFast.record->lastKey = 0xffff;
  levelFast.record->joystickPos = 0;
  levelFast.record->joystickButton = 0;
  levelFast.record->joystick2Pos = 0;
  levelFast.record->joystick2Button = 0;
}


void
record_process(void)
{
#ifdef GAME_RECORDING
  if (levelFast.record->state == RECORD_RECORD &&
      (levelFast.record->lastJoystickPos != hw_joystickPos ||
       levelFast.record->lastJoystickButton != hw_joystickButton ||
       levelFast.record->lastJoystick2Pos != hw_joystick2Pos ||
       levelFast.record->lastJoystick2Button != hw_joystick2Button ||
       levelFast.record->lastKey != keyboard_key)) {
    if (levelFast.record->index < RECORD_MAX_RECORD) {
      levelFast.record->buffer[levelFast.record->index].joystickPos = hw_joystickPos;
      levelFast.record->buffer[levelFast.record->index].joystickButton = hw_joystickButton;
      levelFast.record->buffer[levelFast.record->index].joystick2Pos = hw_joystick2Pos;
      levelFast.record->buffer[levelFast.record->index].joystick2Button = hw_joystick2Button;
      levelFast.record->buffer[levelFast.record->index].key = keyboard_key;
      levelFast.record->buffer[levelFast.record->index].frame = levelFast.record->frame;
      levelFast.record->lastJoystickPos = hw_joystickPos;
      levelFast.record->lastJoystickButton = hw_joystickButton;
      levelFast.record->lastJoystick2Pos = hw_joystick2Pos;
      levelFast.record->lastJoystick2Button = hw_joystick2Button;
      levelFast.record->lastKey = keyboard_key;
      levelFast.record->index++;
    }
#ifdef DEBUG
    else {
      PANIC("RECORD_OVERRUN");
    }
#endif
  } else if (levelFast.record->state == RECORD_PLAYBACK) {
#ifndef DEBUG
    if (game_fire()) {
      keyboard_key = 'Q';
    }
#endif
    if (levelFast.record->index < RECORD_MAX_RECORD) {
      if (levelFast.record->buffer[levelFast.record->index].frame == levelFast.record->frame) {
	levelFast.record->joystickPos = levelFast.record->buffer[levelFast.record->index].joystickPos;
	levelFast.record->joystickButton = levelFast.record->buffer[levelFast.record->index].joystickButton;
	levelFast.record->joystick2Pos = levelFast.record->buffer[levelFast.record->index].joystick2Pos;
	levelFast.record->joystick2Button = levelFast.record->buffer[levelFast.record->index].joystick2Button;
	keyboard_key = levelFast.record->buffer[levelFast.record->index].key;
	levelFast.record->index++;
      }
      hw_joystickPos = levelFast.record->joystickPos;
      hw_joystickButton = levelFast.record->joystickButton;
      hw_joystick2Pos = levelFast.record->joystick2Pos;
      hw_joystick2Button = levelFast.record->joystick2Button;
    }

#ifdef DEBUG
    else {
      PANIC("RECORD_OVERRUN");
    }
#endif
  }


  levelFast.record->frame++;
#endif
}

#endif
