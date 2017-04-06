#include "game.h"

player_data_t player_data[2];

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

static void
player_updatePlayer(object_t* ptr)
{
  uint16_t punch = 0;
  
  player_data_t* data = &player_data[(int)ptr->data];
  
  if (ptr->data == 0) {
    player_processJoystick(ptr, hw_joystickPos);
    if (hw_joystickButton != hw_lastJoystickButton) {
      punch = hw_joystickButton & 0x1;
    }
  } else {
    player_processJoystick(ptr, hw_joystick2Pos);
    if (hw_joystick2Button != hw_lastJoystick2Button) {
      punch = hw_joystick2Button & 0x1;
    }    
  }

  object_updatePlayer(ptr, punch, data);
#if 0  
  if (ptr->x-game_cameraX > 256) {
    game_requestCameraX(game_cameraX+(SCREEN_WIDTH/3));
  }
#endif
}

object_t*
player_init(uint32_t id, uint16_t animId, int16_t x)
{
  player_data_t* data = &player_data[id];
  data->punchType = 0;
  data->punchCount = 0;
  return object_add(x, 100, 0, animId, player_updatePlayer, (void*)id);
}
