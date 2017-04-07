#include "game.h"

#define PLAYER_ATTACK_DURATION_FRAMES 10


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
  
  if (data->id == 0) {
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
  object_t* ptr = fighter_add(id, animId, x, 100, 100, 20, player_intelligence);
  fighter_data_t* data = (fighter_data_t*)ptr->data;
  data->attackDurationFrames = PLAYER_ATTACK_DURATION_FRAMES;
  return ptr;
}
