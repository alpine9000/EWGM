#include "game.h"

fighter_data_t player_data[2];

static void
player_processJoystick(object_t * ptr, uint8_t joystickPos)
{
  switch (joystickPos) {
  case JOYSTICK_POS_IDLE:
    ptr->velocity.x = 0;
    ptr->velocity.y = 0;
    break;
  case JOYSTICK_POS_LEFT:
    ptr->velocity.x = -2*OBJECT_PHYSICS_FACTOR;
    break;
  case JOYSTICK_POS_RIGHT:
    ptr->velocity.x = 2*OBJECT_PHYSICS_FACTOR;
    break;
  case JOYSTICK_POS_UP:
    ptr->velocity.y = -1*OBJECT_PHYSICS_FACTOR;
    break;
  case JOYSTICK_POS_DOWN:
    ptr->velocity.y = 1*OBJECT_PHYSICS_FACTOR;
    break;    
  case JOYSTICK_POS_UPRIGHT:
    ptr->velocity.y = -1*OBJECT_PHYSICS_FACTOR;
    ptr->velocity.x = 2*OBJECT_PHYSICS_FACTOR;    
    break;
  case JOYSTICK_POS_UPLEFT:
    ptr->velocity.y = -1*OBJECT_PHYSICS_FACTOR;
    ptr->velocity.x = -2*OBJECT_PHYSICS_FACTOR;
    break;
  case JOYSTICK_POS_DOWNRIGHT:
    ptr->velocity.y = 1*OBJECT_PHYSICS_FACTOR;
    ptr->velocity.x = 2*OBJECT_PHYSICS_FACTOR;    
    break;
  case JOYSTICK_POS_DOWNLEFT:
    ptr->velocity.y = 1*OBJECT_PHYSICS_FACTOR;
    ptr->velocity.x = -2*OBJECT_PHYSICS_FACTOR;
    break;
  }
}


uint16_t
player_intelligence(object_t* ptr, fighter_data_t* data)
{
  USE(data);
  uint16_t attack = 0;
  
  if (ptr->data == &player_data[0]) {
    player_processJoystick(ptr, hw_joystickPos);
    if (hw_joystickButton != hw_lastJoystickButton) {
      attack = hw_joystickButton & 0x1;
    }
  } else {
    player_processJoystick(ptr, hw_joystick2Pos);
    if (hw_joystick2Button != hw_lastJoystick2Button) {
      attack = hw_joystick2Button & 0x1;
    }    
  }

  return attack;
}


object_t*
player_init(uint32_t id, uint16_t animId, int16_t x)
{
  fighter_data_t* data = &player_data[id];
  return fighter_add(animId, x, 100, data, player_intelligence);
}
