#include "game.h"

static void
fighter_attack(object_t* ptr, int16_t dx)
{
  fighter_data_t* data = (fighter_data_t*)ptr->data;
  sound_queueSound(SOUND_KILL);
  object_set_z(ptr, object_y(ptr));
  data->attack_py = object_py(ptr);
  ptr->velocity.y = -4*OBJECT_PHYSICS_FACTOR;
  ptr->velocity.x = dx;
  ptr->state = OBJECT_STATE_HIT;
  if (dx >= 0) {
    object_setAction(ptr, OBJECT_HIT_LEFT);
  } else {
    object_setAction(ptr, OBJECT_HIT_RIGHT);
  }
}

void
fighter_updatePositionUnderAttack(uint16_t deltaT, object_t* ptr, fighter_data_t* data)
{
  if (object_py(ptr) > data->attack_py) {
    object_set_py(ptr, data->attack_py);
    ptr->velocity.y = 0;
    ptr->velocity.x = 0;
    ptr->state = OBJECT_STATE_ALIVE;
  } else {
    ptr->velocity.y += deltaT;
  }
}


void
fighter_update(uint16_t deltaT, object_t* ptr)
{
  fighter_data_t* data = (fighter_data_t*)ptr->data;
  uint16_t attack = (ptr->state == OBJECT_STATE_ALIVE) ? data->intelligence(ptr, data) : 0;
    
  if (attack && data->punchCount == 0) {
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
    if (object_collision(ptr, &collision, OBJECT_HIT_DISTANCE, 6)) {
      if (ptr->anim->facing == FACING_RIGHT && collision.right) {
	fighter_attack(collision.right, 1);
      } else if (ptr->anim->facing == FACING_LEFT && collision.left) {
	fighter_attack(collision.left,-1);
      }
    }
  } else if (data->punchCount) {
    if (data->punchCount >= deltaT) {
      data->punchCount-=deltaT;
    } else {
      data->punchCount = 0;
    }    
  } else {
    if (ptr->state == OBJECT_STATE_HIT) {
      fighter_updatePositionUnderAttack(deltaT, ptr, data);
      object_updatePosition(deltaT, ptr);
    } else {
      object_collision_t collision;
      if (object_collision(ptr, &collision, 20, 1)) {
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
      
      if (object_x(ptr) - game_cameraX < -TILE_WIDTH && ptr->velocity.x < 0) {
	ptr->velocity.x = 0;    
      }
      
      object_updatePosition(deltaT, ptr);
    }
  }
}


object_t*
fighter_add(uint16_t animId, int16_t x, int16_t y, fighter_data_t* data, uint16_t (*intelligence)(object_t* ptr, struct fighter_data* data))
{
  data->intelligence = intelligence;
  data->punchType = 0;
  data->punchCount = 0;
  data->walkAbout = 0;
  return object_add(x, y, 0, animId, fighter_update, data);
}
