#include "game.h"

static int16_t fighter_count;
static fighter_data_t* fighter_freeList;
static __SECTION_RANDOM fighter_data_t fighter_buffer[FIGHTER_MAX_FIGHTERS];

/*static uint16_t figher_attack_range[] = {
  [OBJECT_PUNCH_LEFT1] = FIGHTER_LONG_PUNCH_RANGE,
  [OBJECT_PUNCH_LEFT2] = FIGHTER_SHORT_PUNCH_RANGE,
  [OBJECT_PUNCH_RIGHT1] = FIGHTER_LONG_PUNCH_RANGE,
  [OBJECT_PUNCH_RIGHT2] = FIGHTER_SHORT_PUNCH_RANGE,
  [OBJECT_KICK_RIGHT] = FIGHTER_LONG_PUNCH_RANGE,
  [OBJECT_KICK_LEFT] = FIGHTER_LONG_PUNCH_RANGE,  
  };*/

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
#ifdef DEBUG
  entry->magicNumber = FIGHTER_DATA_MAGIC_NUMBER;
#endif
  return entry;
}


static void
fighter_addFree(void* data)
{
  fighter_data_t* ptr = data;
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



int16_t
fighter_attackCollision(object_t* a, object_collision_t* collision, int16_t thresholdx, uint16_t thresholdy)
{
  int16_t _collision = 0;
  object_t* b = object_activeList;
  
  collision->left = collision->right = 0;

#ifdef DEBUG
  if (!game_collisions) {
    return 0;
  }
#endif

  int16_t a_y = object_z(a);
  int16_t a_x1 = (((object_x(a))) + a->widthOffset)-thresholdx;
  int16_t a_x2 = (((object_x(a))) + (a->width - a->widthOffset)) + thresholdx;
  
  while (b) {
    if (b->collisionsEnabled && b != a) {
      int16_t b_y = ((object_z(b)));

      if (abs(a_y - b_y) <= thresholdy) {
	int16_t b_x1 = ((object_x(b))) + b->widthOffset;
	int16_t b_x2 = ((object_x(b))) + (b->width - b->widthOffset);
	
	if (a_x1 < b_x2 && a_x2 > b_x1) {		  
	  if (b_x1 >= a_x1) {
	    collision->right = b;
	  } else if (b_x1 < a_x1) {
	    collision->left = b;
	  }
	  _collision = 1;
	}
      }
    }
    b = b->next;
  }
  
  return _collision;
}



static void
fighter_killEnemyCallback(object_t* me, object_t* victim)
{
  __USE(me);
  __USE(victim);
  sound_queueSound(SOUND_DIE03);
}


static void
fighter_hitEnemyCallback(object_t* me, object_t* victim)
{
  __USE(me);
  __USE(victim);
  sound_queueSound(SOUND_ENEMY_PUNCH01);      
}


static void
fighter_attack(object_t* attacker, object_t* ptr, uint16_t dammage, int16_t dx)
{
  fighter_data_t* data = fighter_data(ptr);  
  fighter_data_t* attackerData = fighter_data(attacker);
  
  if (data->postAttackCount > 0) {
    return;
  }

#ifdef GAME_STARS
  star_add(ptr, dx);
#endif
  
  object_set_z(ptr, object_y(ptr));
     
  if (ptr->attributes & OBJECT_ATTRIBUTE_PLAYER && attacker->attributes & OBJECT_ATTRIBUTE_PLAYER) {
   data->health -= (dammage/4);
  } else {
    data->health -= dammage;
  }
  
  if (data->health <= 0) {
    data->health = 0;    
    attackerData->killEnemyCallback(attacker, ptr);
    ptr->velocity.y = -8*OBJECT_PHYSICS_FACTOR;
    ptr->velocity.x = dx*2;
  } else {
    attackerData->hitEnemyCallback(attacker, ptr);
    ptr->velocity.y = -4*OBJECT_PHYSICS_FACTOR;
    ptr->velocity.x = dx;
  }

  switch (ptr->id) {
  case OBJECT_ID_PLAYER1:
    game_updatePlayer1Health(GAME_PLAYER1_HEALTH_SCOREBOARD_X, fighter_data(game_player1)->health);
    break;
  case OBJECT_ID_PLAYER2:
    game_updatePlayer2Health(GAME_PLAYER2_HEALTH_SCOREBOARD_X, fighter_data(game_player2)->health);
    break;
  }
  
  object_set_state(ptr, OBJECT_STATE_HIT);
  if (dx >= 0) {
    object_setAction(ptr, OBJECT_HIT_LEFT);
  } else {
    object_setAction(ptr, OBJECT_HIT_RIGHT);
  }
}


void
fighter_updatePositionUnderAttack(uint16_t deltaT, object_t* ptr, fighter_data_t* data)
{
  if (object_y(ptr) >= object_z(ptr) && ptr->velocity.y > 0) {
    object_set_py_no_checks(ptr, object_z(ptr)*OBJECT_PHYSICS_FACTOR);
    ptr->velocity.y = 0;
    ptr->velocity.x = 0;    
    if (data->health <= 0) {
      object_set_state(ptr, OBJECT_STATE_FLASHING);
      data->flashCount = FIGHTER_HIT_FLASH_COUNT_TICS;
      data->flashDurationTics = FIGHTER_HIT_FLASH_DURATION_TICS;
    } else {
      object_set_state(ptr, OBJECT_STATE_ALIVE);
      data->postAttackCount = data->postAttackInvincibleTics;
    }
  } else {
    ptr->velocity.y += deltaT;
  }
}


static void
fighter_checkAttack(object_t* ptr, fighter_data_t* data)
{
  fighter_attack_config_t* attackConfig = &data->attackConfig[ptr->actionId];
  
  if (!data->attackChecked && attackConfig->hitAnimTic == ptr->frameCounter && attackConfig->dammage > 0) {
    object_collision_t collision;
    if (fighter_attackCollision(ptr, &collision, attackConfig->rangeX, data->attackRangeY)) {


      if (ptr->anim->facing == FACING_RIGHT && collision.right) {
	if (object_get_state(collision.right) == OBJECT_STATE_ALIVE) {
	  collision.right->hit.attacker = ptr;
	  collision.right->hit.dammage = attackConfig->dammage;
	  collision.right->hit.dx = 1;
	  object_set_state(collision.right, OBJECT_STATE_ABOUT_TO_BE_HIT);
	}
      } else if (ptr->anim->facing == FACING_LEFT && collision.left) {
	if (object_get_state(collision.left) == OBJECT_STATE_ALIVE) {
	  collision.left->hit.attacker = ptr;
	  collision.left->hit.dammage = attackConfig->dammage;
	  collision.left->hit.dx = -1;
	  object_set_state(collision.left, OBJECT_STATE_ABOUT_TO_BE_HIT);
	}
      }
      
    } 
    data->attackChecked = 1;
  }
}


static void
fighter_doAttack(object_t* ptr, fighter_data_t* data)
{

  int16_t attackId;
  
  if (ptr->anim->facing == FACING_RIGHT) {
    attackId = OBJECT_PUNCH_RIGHT1 + data->attackType;
  } else {
    attackId = OBJECT_PUNCH_LEFT1 + data->attackType;
  }

  fighter_attack_config_t* attackConfig = &data->attackConfig[attackId];  
  ptr->velocity.x = attackConfig->vx;
  ptr->velocity.y = attackConfig->vy;
  data->attackCount = attackConfig->durationTics;
  data->attackJump = attackConfig->jump;  
  data->attackJumpY = object_py(ptr);
  
  object_setAction(ptr, attackId);

  data->attackChecked = 0;
  
  data->attackType++;
  if (data->attackType >= data->numAttacks) {
    data->attackType = 0;
  }
  fighter_checkAttack(ptr, data);
}


void
fighter_updateSprite(object_t* ptr)
{
  object_set_z(ptr, object_y(ptr));
  if (ptr->velocity.dx || ptr->velocity.dy) {
    if (ptr->velocity.dx > 0) {
      object_setAction(ptr, OBJECT_RUN_RIGHT);
    } else if (ptr->velocity.dx < 0) {
      object_setAction(ptr, OBJECT_RUN_LEFT);
    } else {
      if (ptr->anim->facing == FACING_RIGHT) {
	object_setAction(ptr, OBJECT_RUN_RIGHT);
      } else {
	object_setAction(ptr, OBJECT_RUN_LEFT);
      }
    }
  } else {
    if (ptr->velocity.ix > 0) {
      object_setAction(ptr, OBJECT_STAND_RIGHT);
    } else if (ptr->velocity.ix < 0) {
      object_setAction(ptr, OBJECT_STAND_LEFT);
    } else {
      if (ptr->anim->facing == FACING_RIGHT) {
	object_setAction(ptr, OBJECT_STAND_RIGHT);
      } else {
	object_setAction(ptr, OBJECT_STAND_LEFT);
      }
    }
  }
}

void
fighter_dieCallback(object_t* me)
{
  __USE(me);
  enemy_count--;
  if (enemy_count == 0) {
    if (conductor_complete()) {
      if (game_numPlayers == 2 && game_player1 && game_player2) {
	game_loopControl = GAME_LOOP_CONTROL_DISPLAY_DEATHMATCH;
      } else {
	game_setGameComplete();
      }
    }
  }
}

void
fighter_die(object_t* ptr)
{
  object_set_state(ptr, OBJECT_STATE_REMOVED);
  fighter_data(ptr)->dieCallback(ptr);  
}

uint16_t
fighter_nullIntelligence(uint16_t deltaT, object_t* ptr, fighter_data_t* data)
{
  __USE(deltaT);
  __USE(ptr);
  __USE(data);
  return 0;
}

void
fighter_update(uint16_t deltaT, object_t* ptr)
{
  fighter_data_t* data = fighter_data(ptr);

  uint16_t attack = data->intelligence(deltaT, ptr, data);

  if (!data->attackQueued && attack) {
    data->attackQueued = 1;
  }
  
  if (data->attackCount >= deltaT) {
    data->attackCount-=deltaT;
  } else {
    data->attackCount = 0;
  }

  if (data->postAttackCount > 0) {
    data->postAttackCount--;
  }

  if (data->attackQueued && data->attackCount == 0 && object_get_state(ptr) == OBJECT_STATE_ALIVE) {
    data->buttonReleased = 0;
    data->attackQueued = 0;
    fighter_doAttack(ptr, data);
  } else if (data->attackCount) {
    /* for non immediate attacks */
    fighter_checkAttack(ptr, data);
    if (data->attackJump) {
      if (object_py(ptr) >= data->attackJumpY) {
	object_set_py(ptr, data->attackJumpY);
      } 
      object_updatePosition(deltaT, ptr);
      if (object_py(ptr) >= data->attackJumpY) {
	data->attackCount = 0;
      } else {
	ptr->velocity.y += deltaT;
      }
    }
  } else {
    if (object_get_state(ptr) == OBJECT_STATE_ABOUT_TO_BE_HIT) {
      fighter_attack(ptr->hit.attacker, ptr, ptr->hit.dammage, ptr->hit.dx);
    }
    
    if (object_get_state(ptr) == OBJECT_STATE_HIT) {
      fighter_updatePositionUnderAttack(deltaT, ptr, data);
      object_updatePositionNoChecks(deltaT, ptr);
    } else if (object_get_state(ptr) == OBJECT_STATE_FLASHING && data->health <= 0) {
      if (data->flashCount <= 0) {
	ptr->visible = !ptr->visible;
	data->flashCount = FIGHTER_HIT_FLASH_COUNT_TICS;
      }
      data->flashCount -= deltaT;
      data->flashDurationTics -= deltaT;
      if (data->flashDurationTics <= 0) {
	fighter_die(ptr);
      }
    } else {
      if (object_get_state(ptr) == OBJECT_STATE_FLASHING) {
	if (data->flashCount <= 0) {
	  ptr->visible = !ptr->visible;
	  data->flashCount = FIGHTER_SPAWN_FLASH_COUNT_TICS;
	}
	data->flashCount-=deltaT;
	data->flashDurationTics -= deltaT;

	if (data->flashDurationTics <= 0) {
	  object_set_state(ptr, OBJECT_STATE_ALIVE);
	  ptr->visible = 1;	  
	}
      }
      ptr->velocity.ix = 0;      
      if (ptr->collisionsEnabled && (ptr->velocity.x || ptr->velocity.y)) {
	object_collision_t collision;	
	if (object_collision(deltaT, ptr, &collision, 0, 2)) {
	  if (ptr->velocity.x > 0 && collision.right) {	  
	    ptr->velocity.ix = 1;
	    ptr->velocity.x = 0;
	  }
	  if (ptr->velocity.x < 0 && collision.left) {
	    ptr->velocity.ix = -1;	  
	    ptr->velocity.x = 0;
	  }
	  if (ptr->velocity.y > 0 && collision.up) {
	    ptr->velocity.y = 0;
	  }
	  if (ptr->velocity.y < 0 && collision.down) {
	    ptr->velocity.y = 0;
	  }    
	}
      }
      
      if (object_x(ptr) - game_cameraX < -TILE_WIDTH && ptr->velocity.x < 0) {
	ptr->velocity.x = 0;
      }
      
      object_updatePosition(deltaT, ptr);
      fighter_updateSprite(ptr);      
    }
  }
}


__NOINLINE object_t*
fighter_add(uint16_t id, uint16_t attributes, uint16_t animId, int16_t x, int16_t y, uint16_t initialHealth,  fighter_attack_config_t* attackConfig, uint16_t (*intelligence)(uint16_t deltaT, object_t* ptr, struct fighter_data* data))
{
  fighter_data_t* data = fighter_getFree();
  data->buttonReleased = 0;

  if (!(attributes & (OBJECT_ATTRIBUTE_PLAYER|OBJECT_ATTRIBUTE_DONT_OVERRIDE_CONFIG)) && game_difficulty == GAME_DIFFICULTY_EASY) {
    data->attackConfig = enemy_attackConfig1;
  } else {
    data->attackConfig = attackConfig;
  }
  data->attackQueued = 0;
  data->intelligence = intelligence;
  data->attackType = 0;
  data->numAttacks = 0;  
  data->attackCount = 0;
  data->walkAbout = 0;
  data->health = initialHealth;
  data->attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE;
  data->postAttackInvincibleTics = 0;
  data->postAttackCount = 0;
  data->speed = 1;
  data->killEnemyCallback = fighter_killEnemyCallback;
  data->hitEnemyCallback = fighter_hitEnemyCallback;
  data->dieCallback = fighter_dieCallback;
  object_t* ptr = object_add(id,attributes|OBJECT_ATTRIBUTE_COLLIDABLE, x, y, 0, animId, fighter_update, OBJECT_DATA_TYPE_FIGHTER, data, fighter_addFree);
  return ptr;
}
