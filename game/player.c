#include "game.h"

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
  if (ptr->data == 0) {
    player_processJoystick(ptr, hw_joystickPos);
  } else {
    player_processJoystick(ptr, hw_joystick2Pos);
  }

  object_movement_t movement;
  if (object_collision2(ptr, &movement)) {
    if (ptr->velocity.x > 0 && !movement.right) {
      ptr->velocity.x = 0;
    }
    if (ptr->velocity.x < 0 && !movement.left) {
      ptr->velocity.x = 0;
    }
    if (ptr->velocity.y > 0 && !movement.up) {
      ptr->velocity.y = 0;
    }
    if (ptr->velocity.y < 0 && !movement.down) {
      ptr->velocity.y = 0;
    }    
  }

  if (ptr->x - game_cameraX < -TILE_WIDTH && ptr->velocity.x < 0) {
    ptr->velocity.x = 0;    
  }
  
  object_updateObject(ptr);

  
#if 0
  static int buttonDown = 1;
  if (!buttonDown && JOYSTICK_BUTTON_DOWN/* && ptr->x-game_cameraX > 256*/) {
    game_requestCameraX(game_cameraX+(SCREEN_WIDTH/3));
  }
  buttonDown = JOYSTICK_BUTTON_DOWN;
#else
  if (ptr->x-game_cameraX > 256) {
    game_requestCameraX(game_cameraX+(SCREEN_WIDTH/3));
  }
#endif

}

object_t*
player_init(uint32_t id, uint16_t animId, int16_t x)
{
  return object_add(x, 100, 0, animId, player_updatePlayer, (void*)id);
}
