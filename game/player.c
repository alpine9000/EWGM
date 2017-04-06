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

  if (ptr->state == OBJECT_STATE_HIT) {
    object_updateObject(ptr);
    return;
  }
  
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

  if (punch && data->punchCount == 0) {
    ptr->velocity.x = 0;
    ptr->velocity.y = 0;
    if (ptr->anim->facing == FACING_RIGHT) {
      object_setAction(ptr, OBJECT_PUNCH_RIGHT1 + data->punchType);
    } else {
      object_setAction(ptr, OBJECT_PUNCH_LEFT1 + data->punchType);
    }
    data->punchCount = 10;
    data->punchType = !data->punchType;
    object_collision_t collision;
    if (object_collision2(ptr, &collision, 32, 6)) {
      if (ptr->anim->facing == FACING_RIGHT && collision.right) {
	object_hit(collision.right, 1);
      } else if (ptr->anim->facing == FACING_LEFT && collision.left) {
	object_hit(collision.left, -1);
      }
    }
  } else if (data->punchCount) {
    data->punchCount--;
  } else {
    object_collision_t collision;
    if (object_collision2(ptr, &collision, 20, 1)) {
      if (ptr->velocity.x > 0 && collision.right) {
	ptr->velocity.x = 0;
      }
      if (ptr->velocity.x < 0 && collision.left) {
	ptr->velocity.x = 0;
      }
      if (ptr->velocity.y > 0 && collision.up) {
	ptr->velocity.y = 0;
      }
      if (ptr->velocity.y < 0 && collision.down) {
	ptr->velocity.y = 0;
      }    
    }
    
    if (ptr->x - game_cameraX < -TILE_WIDTH && ptr->velocity.x < 0) {
      ptr->velocity.x = 0;    
    }
    
    object_updateObject(ptr);
  }

    
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
  player_data_t* data = &player_data[id];
  data->punchType = 0;
  data->punchCount = 0;
  return object_add(x, 100, 0, animId, player_updatePlayer, (void*)id);
}
