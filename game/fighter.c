#include "game.h"

static int16_t fighter_count;
static fighter_data_t* fighter_freeList;
static __section(random_c) fighter_data_t fighter_buffer[FIGHTER_MAX_FIGHTERS];

static uint16_t figher_attack_range[] = {
  [OBJECT_PUNCH_LEFT1] = FIGHTER_LONG_PUNCH_RANGE,
  [OBJECT_PUNCH_LEFT2] = FIGHTER_SHORT_PUNCH_RANGE,
  [OBJECT_PUNCH_RIGHT1] = FIGHTER_LONG_PUNCH_RANGE,
  [OBJECT_PUNCH_RIGHT2] = FIGHTER_SHORT_PUNCH_RANGE,    
};

static fighter_data_t*
fighter_getFree(void)
{
  fighter_count++;

#ifdef DEBUG
  if (fighter_count > FIGHTER_MAX_FIGHTERS) {
    PANIC("fighter_getFree: empty list");
  }
#endif
  
  
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

#if 0
int16_t
fighter_collision(object_t* a, object_collision_t* collision, uint16_t thresholdx, uint16_t thresholdy)
{
  int16_t _collision = 0;
  object_t* b = object_activeList;
  
  collision->up = collision->down = collision->left = collision->right = 0;

  while (b) {
    if (b != a && b->state == OBJECT_STATE_ALIVE) {
      int16_t a_x = ((object_px(a) + a->velocity.x) / OBJECT_PHYSICS_FACTOR) + a->image->dx + (OBJECT_WIDTH>>2);
      int16_t b_x = ((object_px(b) + b->velocity.x) / OBJECT_PHYSICS_FACTOR) + a->image->dx + (OBJECT_WIDTH>>2);
      int16_t a_y = ((object_py(a) + a->velocity.y) / OBJECT_PHYSICS_FACTOR);
      int16_t b_y = ((object_py(b) + b->velocity.y) / OBJECT_PHYSICS_FACTOR);      
      
      if (abs(a_x - b_x) <= thresholdx && abs(a_y - b_y) <= thresholdy) {
	if (b_y >= a_y) {
	  collision->up = b;
	} else if (b_y < a_y) {
	  collision->down = b;
	}
	if (b_x >= a_x) {
	  collision->right = b;
	} else if (b_x < a_x) {
	  collision->left = b;
	}      
	_collision = 1;
      }
    }
    b = b->next;
  }
  
  return _collision;
}
#else

/*
   ************ 
+----------------+
*/
int16_t
fighter_collision(int16_t deltaT, object_t* a, object_collision_t* collision, uint16_t thresholdx, uint16_t thresholdy)
{
  USE(deltaT);
  int16_t vx = a->velocity.x;
  int16_t vy = a->velocity.y;

  switch (deltaT) {
  case 0:
  case 1:
    break;
  case 2:
    vx *= 2;
    vy *= 2;
    break;
  default:

    break;
  }
  

  int16_t _collision = 0;
  fighter_data_t* a_data = a->data;  
  object_t* b = object_activeList;
  
  collision->up = collision->down = collision->left = collision->right = 0;

  while (b) {
    fighter_data_t* b_data = b->data;      
    if (b != a && b->state == OBJECT_STATE_ALIVE) {

      int16_t a_widthOffset = a_data->widthOffset;
      int16_t b_widthOffset = b_data->widthOffset;
      int16_t a_x1 = (((object_px(a) + vx) / OBJECT_PHYSICS_FACTOR) + a_widthOffset)-thresholdx;
      int16_t a_x2 = (((object_px(a) + vx) / OBJECT_PHYSICS_FACTOR) + (OBJECT_WIDTH - a_widthOffset)) + thresholdx;
      int16_t b_x1 = ((object_px(b)) / OBJECT_PHYSICS_FACTOR) + b_widthOffset;
      int16_t b_x2 = ((object_px(b)) / OBJECT_PHYSICS_FACTOR) + (OBJECT_WIDTH - b_widthOffset);
      int16_t a_y = ((object_py(a) + vy) / OBJECT_PHYSICS_FACTOR);
      int16_t b_y = ((object_py(b)) / OBJECT_PHYSICS_FACTOR);      
      
      if (abs(a_y - b_y) <= thresholdy && a_x1 < b_x2 && a_x2 > b_x1) {
	if (b_y >= a_y) {
	  collision->up = b;
	} else if (b_y < a_y) {
	  collision->down = b;
	}
	if (b_x1 >= a_x1) {
	  collision->right = b;
	} else if (b_x1 < a_x1) {
	  collision->left = b;
	}      
	_collision = 1;
      }
    }
    b = b->next;
  }
  
  return _collision;
}
#endif


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
fighter_doAttack(int16_t deltaT, object_t* ptr, fighter_data_t* data)
{
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
  if (fighter_collision(deltaT, ptr, &collision, figher_attack_range[ptr->actionId], FIGHTER_Y_ATTACK_THRESHOLD)) {
    if (ptr->anim->facing == FACING_RIGHT && collision.right) {
      fighter_attack(collision.right, data->attackDammage, 1);
    } else if (ptr->anim->facing == FACING_LEFT && collision.left) {
      fighter_attack(collision.left, data->attackDammage, -1);
    }
  }
}

void
fighter_update(uint16_t deltaT, object_t* ptr)
{
  fighter_data_t* data = (fighter_data_t*)ptr->data;
  uint16_t attack = (ptr->state == OBJECT_STATE_ALIVE) ? data->intelligence(deltaT, ptr, data) : 0;

  if (!data->attackQueued && attack) {
    data->attackQueued = 1;
  }
  
  if (data->punchCount >= deltaT) {
    data->punchCount-=deltaT;
  } else {
    data->punchCount = 0;
  }      
  
  if (data->attackQueued && data->punchCount == 0 && ptr->state == OBJECT_STATE_ALIVE) {
    data->buttonReleased = 0;
    data->attackQueued = 0;
    fighter_doAttack(deltaT, ptr, data);
  } else if (data->punchCount) {

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
	switch (ptr->id) {
	case 0:
	  enemy_count--;
	  if (enemy_count == 0) {
	    game_phase += SCREEN_WIDTH;
	    if (game_phase < WORLD_WIDTH) {
	      hand_show();
	    }
	  }
	  break;	  
	case 1:
	  game_player1 = 0;
	  break;
	case 2:
	  game_player2 = 0;
	  break;
	}
      }
    } else {
      object_collision_t collision;
      if (fighter_collision(deltaT, ptr, &collision, 0, 1)) {
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
fighter_add(uint16_t id, uint16_t animId, int16_t x, int16_t y, uint16_t initialHealth,  uint16_t attackDammage, uint16_t (*intelligence)(uint16_t deltaT, object_t* ptr, struct fighter_data* data))
{
  fighter_data_t* data = fighter_getFree();
  data->buttonReleased = 0;
  data->attackQueued = 0;
  data->intelligence = intelligence;
  data->punchType = 0;
  data->punchCount = 0;
  data->walkAbout = 0;
  data->health = initialHealth;
  data->attackDammage = attackDammage;
  return object_add(id, x, y, 0, animId, fighter_update, data);
}

void
fighter_remove(object_t* ptr)
{
  USE(ptr);
  object_free(ptr);
  fighter_addFree(ptr->data);
}
