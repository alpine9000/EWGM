#include "game.h"

static int16_t fighter_count;
static fighter_data_t* fighter_freeList;
static __section(random_c) fighter_data_t fighter_buffer[FIGHTER_MAX_FIGHTERS];

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

  return entry;
}


//static
void
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


static int16_t
fighter_attackCollision(object_t* a, object_collision_t* collision, uint16_t thresholdx, uint16_t thresholdy)
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
    if (b->collidable && b != a) {
      int16_t b_y = ((object_y(b)));

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
fighter_attack(object_t* attacker, object_t* ptr, uint16_t dammage, int16_t dx)
{
  if (ptr->class == OBJECT_CLASS_THING) {
    thing_attack(ptr, dx); 
    return;
  }
  
  if (object_get_state(ptr) == OBJECT_STATE_HIT) {
    return;
  }
  
  fighter_data_t* data = (fighter_data_t*)ptr->data;  
  if (data->postAttackCount > 0) {
    return;
  }

#ifdef GAME_STARS
  star_add(ptr, dx);
#endif
  
  object_set_z(ptr, object_y(ptr));
  data->attack_py = object_py(ptr);

  if (ptr->id != OBJECT_ID_ENEMY && attacker->id != OBJECT_ID_ENEMY) {
    data->health -= (dammage/4);
  } else {
    data->health -= dammage;
  }
  if (data->health <= 0) {
    data->health = 0;
    switch (attacker->id) {
    case OBJECT_ID_PLAYER1:
      game_player1Score += 1000;
      sound_queueSound(SOUND_DIE01);
      break;
    case OBJECT_ID_PLAYER2:
      game_player2Score += 1000;
      sound_queueSound(SOUND_DIE02);
      break;
    default:
      sound_queueSound(SOUND_DIE03);
      break;
    }
    ptr->velocity.y = -8*OBJECT_PHYSICS_FACTOR;
    ptr->velocity.x = dx*2;
  } else {
    switch (attacker->id) {
    case OBJECT_ID_PLAYER1:    
      sound_queueSound(SOUND_TERENCE_PUNCH01);
      break;
    case OBJECT_ID_PLAYER2:          
      sound_queueSound(SOUND_BUD_PUNCH01);
      break;
    default:
      sound_queueSound(SOUND_ENEMY_PUNCH01);
      
      break;
    }
    ptr->velocity.y = -4*OBJECT_PHYSICS_FACTOR;
    ptr->velocity.x = dx;
  }

  switch(ptr->id) {
  case OBJECT_ID_PLAYER1:        
    game_updatePlayerHealth(GAME_PLAYER1_HEALTH_SCOREBOARD_X, ((fighter_data_t*)game_player1->data)->health);    
    break;
  case OBJECT_ID_PLAYER2:        
    game_updatePlayerHealth(GAME_PLAYER2_HEALTH_SCOREBOARD_X, ((fighter_data_t*)game_player2->data)->health);    
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
  if (object_py(ptr) >= data->attack_py && ptr->velocity.y > 0) {
    object_set_py_no_checks(ptr, data->attack_py);
    ptr->velocity.y = 0;
    ptr->velocity.x = 0;    
    if (data->health <= 0) {
      object_set_state(ptr, OBJECT_STATE_FLASHING);
      data->flashCount = FIGHTER_HIT_FLASH_COUNT_TICS;
      data->flashDurationTics = FIGHTER_HIT_FLASH_DURATION_TICS;
    } else {
      object_set_state(ptr, OBJECT_STATE_ALIVE);
      //      data->attackQueued = 1;
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
  
  if (!data->attackChecked && attackConfig->hitAnimTic == ptr->frameCounter) {
    object_collision_t collision;
    if (fighter_attackCollision(ptr, &collision, attackConfig->rangeX, data->attackRangeY)) {
      if (ptr->anim->facing == FACING_RIGHT && collision.right) {
	fighter_attack(ptr, collision.right, attackConfig->dammage, 1);
      } else if (ptr->anim->facing == FACING_LEFT && collision.left) {
	fighter_attack(ptr, collision.left, attackConfig->dammage, -1);
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


static void
fighter_updateSprite(object_t* ptr)
{
  //  if (object_get_state(ptr) == OBJECT_STATE_ALIVE) {
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
      //    }
  }
}


void
fighter_die(object_t* ptr)
{
  object_set_state(ptr, OBJECT_STATE_REMOVED);
  switch (ptr->id) {
  case OBJECT_ID_ENEMY:
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
    break;	  
  case OBJECT_ID_PLAYER1:
    game_player1 = 0;
    game_scoreBoardPlayerText(OBJECT_ID_PLAYER1, I18N_GAME_OVER);
    if (!game_player2) {
      game_setGameOver();	    
    } else if (game_loopControl == GAME_LOOP_CONTROL_DEATHMATCH) {
      game_setGameComplete();
    }
    break;
  case OBJECT_ID_PLAYER2:
    game_player2 = 0;
    if (!game_player1) {
      game_setGameOver();	    
    } else if (game_loopControl == GAME_LOOP_CONTROL_DEATHMATCH) {      
      game_setGameComplete();
    }	  
    game_scoreBoardPlayerText(OBJECT_ID_PLAYER2, I18N_GAME_OVER);	  	  
    break;
  }
}

uint16_t
fighter_nullIntelligence(uint16_t deltaT, object_t* ptr, fighter_data_t* data)
{
  USE(deltaT);
  USE(ptr);
  USE(data);
  return 0;
}

void
fighter_update(uint16_t deltaT, object_t* ptr)
{
  fighter_data_t* data = (fighter_data_t*)ptr->data;

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

  //  ptr->velocity.ix = 0;      
  
  if (data->attackQueued && data->attackCount == 0 && object_get_state(ptr) == OBJECT_STATE_ALIVE) {
    data->buttonReleased = 0;
    data->attackQueued = 0;
    fighter_doAttack(ptr, data);
  } else if (data->attackCount) {
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
      if (ptr->collidable && (ptr->velocity.x || ptr->velocity.y)) {
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


NOINLINE object_t*
fighter_add(uint16_t id, uint16_t animId, int16_t x, int16_t y, uint16_t initialHealth,  fighter_attack_config_t* attackConfig, uint16_t (*intelligence)(uint16_t deltaT, object_t* ptr, struct fighter_data* data))
{
  fighter_data_t* data = fighter_getFree();
  data->buttonReleased = 0;

  if (id == OBJECT_ID_ENEMY && game_difficulty == GAME_DIFFICULTY_EASY) {
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
#ifdef ENEMY_RUNAWAY
  data->lastState = OBJECT_STATE_ALIVE;
#endif
  return object_add(id, OBJECT_CLASS_FIGHTER, x, y, 0, animId, fighter_update, data, fighter_addFree);
}
