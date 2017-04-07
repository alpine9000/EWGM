#include "game.h"

#define FIGHTER_MAX_FIGHTERS 10

static int16_t fighter_count;
static fighter_data_t* fighter_freeList;
static __section(random_c) fighter_data_t fighter_buffer[FIGHTER_MAX_FIGHTERS];


static fighter_data_t*
fighter_getFree(void)
{
  fighter_data_t* entry = fighter_freeList;
  fighter_freeList = fighter_freeList->next;
  if (fighter_freeList) {
    fighter_freeList->prev = 0;
  }
  return entry;
}


//static
void
fighter_addFree(fighter_data_t* ptr)
{
  fighter_count--;

  if (fighter_freeList == 0) {
    fighter_freeList = ptr;
    ptr->next = 0;
    ptr->prev = 0;
  } else {
    ptr->next = fighter_freeList;
    ptr->next->prev = ptr;
    ptr->prev = 0;
    fighter_freeList = ptr;
  }
}


void
fighter_init(void)
{
  fighter_count = 0;
  fighter_freeList = &fighter_buffer[0];
  fighter_freeList->prev = 0;
  fighter_data_t* ptr = fighter_freeList;
  for (int16_t i = 1; i < FIGHTER_MAX_FIGHTERS; i++) {
      ptr->next = &fighter_buffer[i];
      ptr->next->prev = ptr;
      ptr = ptr->next;
  }
  ptr->next = 0;
}

static void
fighter_attack(object_t* ptr, uint16_t dammage, int16_t dx)
{
  fighter_data_t* data = (fighter_data_t*)ptr->data;
  sound_queueSound(SOUND_KILL);
  object_set_z(ptr, object_y(ptr));
  data->attack_py = object_py(ptr);
  data->health -= dammage;
  if (data->health <= 0) {
    ptr->velocity.y = -8*OBJECT_PHYSICS_FACTOR;
    ptr->velocity.x = dx*2;
  } else {
    ptr->velocity.y = -4*OBJECT_PHYSICS_FACTOR;
    ptr->velocity.x = dx;
  }
  
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
  if (object_py(ptr) >= data->attack_py && ptr->velocity.y > 0) {
    object_set_py(ptr, data->attack_py);
    ptr->velocity.y = 0;
    ptr->velocity.x = 0;    
    if (data->health <= 0) {
      ptr->state = OBJECT_STATE_FLASHING;
      data->flashCount = 7;
      data->flashFrames = 75;
    } else {
      ptr->state = OBJECT_STATE_ALIVE;
    }
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
    data->punchCount = data->attackDurationFrames;
    data->punchType = !data->punchType;
    object_collision_t collision;
    if (object_collision(ptr, &collision, OBJECT_HIT_DISTANCE, 6)) {
      if (ptr->anim->facing == FACING_RIGHT && collision.right) {
	fighter_attack(collision.right, data->attackDammage, 1);
      } else if (ptr->anim->facing == FACING_LEFT && collision.left) {
	fighter_attack(collision.left, data->attackDammage, -1);
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
    } else if (ptr->state == OBJECT_STATE_FLASHING) {
      if (data->flashCount <= 0) {
	ptr->visible = !ptr->visible;
	data->flashCount = 10;
      }
      data->flashCount-=deltaT;
      data->flashFrames -= deltaT;
      if (data->flashFrames <= 0) {
	ptr->state = OBJECT_STATE_REMOVED;
      }
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
fighter_add(uint16_t id, uint16_t animId, int16_t x, int16_t y, uint16_t initialHealth,  uint16_t attackDammage, uint16_t (*intelligence)(object_t* ptr, struct fighter_data* data))
{
  fighter_data_t* data = fighter_getFree();
  data->id = id;
  data->intelligence = intelligence;
  data->punchType = 0;
  data->punchCount = 0;
  data->walkAbout = 0;
  data->health = initialHealth;
  data->attackDammage = attackDammage;
  return object_add(x, y, 0, animId, fighter_update, data);
}
