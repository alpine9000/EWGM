#include "game.h"

uint16_t enemy_count;
#ifdef DEBUG
uint16_t enemy_pause;
#endif
  
object_t*
enemy_closestPlayer(object_t* ptr)
{
  if (game_player2 == 0) {
    if (game_player1) {
      return game_player1;
    } else {
      return 0;
    }
  } else if (game_player1 == 0) {
    return game_player2;
  } 

  if (object_get_state(game_player1) == OBJECT_STATE_REMOVED &&
      object_get_state(game_player2) != OBJECT_STATE_REMOVED) {
    return game_player2;
  } else  if (object_get_state(game_player2) == OBJECT_STATE_REMOVED &&
	      object_get_state(game_player1) != OBJECT_STATE_REMOVED) {
    return game_player1;
  } else if (object_get_state(game_player2) == OBJECT_STATE_REMOVED &&
	     object_get_state(game_player1) == OBJECT_STATE_REMOVED) {
    return 0;
  }

  int32_t player1Diff = abs(object_x(ptr)-object_x(game_player1)) + abs(object_y(ptr)-object_y(game_player1));
  int32_t player2Diff = abs(object_x(ptr)-object_x(game_player2)) + abs(object_y(ptr)-object_y(game_player2));

  if (player1Diff < player2Diff) {
    return game_player1;
  } else {
    return game_player2;
  }
}

#if 0

static int16_t
enemy_strikingDistanceX(object_t* a, object_t* b)
{
  int16_t a_x = object_x(a) + (a->image->w>>2);
  int16_t b_x = object_x(b) + (b->image->w>>2);

  if (abs(a_x - b_x) > OBJECT_HIT_DISTANCE) {
    return (object_x(a) > object_x(b)) ? 1 : -1;
  }
  
  return 0;
}

#else

static int16_t
enemy_strikingDistanceX(object_t* a, object_t* b)
{
  fighter_data_t* b_data = b->data;    
  uint16_t thresholdx;
  if (b_data->numAttacks == 1) {
    thresholdx = b_data->attackConfig[OBJECT_PUNCH_LEFT1].rangeX;
  } else {
    thresholdx = b_data->attackConfig[OBJECT_PUNCH_LEFT2].rangeX;
  }

  int16_t a_widthOffset = a->widthOffset;
  int16_t b_widthOffset = b->widthOffset;  
  int16_t a_x1 = (((object_px(a)) / OBJECT_PHYSICS_FACTOR) + a_widthOffset);
  int16_t a_x2 = (((object_px(a)) / OBJECT_PHYSICS_FACTOR) + (OBJECT_WIDTH - a_widthOffset));
  int16_t b_x1 = (((object_px(b)) / OBJECT_PHYSICS_FACTOR) + b_widthOffset) - thresholdx;
  int16_t b_x2 = (((object_px(b)) / OBJECT_PHYSICS_FACTOR) + (OBJECT_WIDTH - b_widthOffset)) + thresholdx;
  
  if (a_x1 < b_x2 && a_x2 > b_x1) {
    return 0;
  } else {
    return (object_x(a) > object_x(b)) ? b_data->speed : -b_data->speed;
  }
        
  return 0;
}

#endif


uint16_t
enemy_intelligence(uint16_t deltaT, object_t* ptr, fighter_data_t* data)
{
  uint16_t attack = 0;
  
  if (object_get_state(ptr) != OBJECT_STATE_ALIVE) {
#ifdef ENEMY_RUNAWAY
    data->lastState = object_get_state(ptr);
#endif
    return 0;
  }
  
#ifdef DEBUG
  if (enemy_pause) {
    ptr->velocity.x = 0;
    ptr->velocity.y = 0;
    return 0;    
  }
#endif  
  
  if (data->walkAbout > 0) {
    data->walkAbout-=deltaT;
    if (object_y(ptr) >= PLAYAREA_HEIGHT-1) {
      ptr->velocity.y = 0;
    }
  } else {
    uint32_t rand = random();
    object_t* player = enemy_closestPlayer(ptr);
    if (!player) {
      ptr->velocity.x = 0;
      ptr->velocity.y = 0;
      return 0;
    }
    object_collision_t collision;

    if (object_x(ptr)-game_cameraX <= 0) {
      data->walkAbout = rand & 0x7f; //ENEMY_WALKABOUT_TICS;
      ptr->velocity.x = 1;
    } else if (object_x(ptr)-game_cameraX >= SCREEN_WIDTH) {
      data->walkAbout = rand & 0x7f; //ENEMY_WALKABOUT_TICS;
      ptr->velocity.x = -1;      
    } else {
      ptr->velocity.x = enemy_strikingDistanceX(player, ptr);
      if (abs(object_y(ptr)-object_y(player)) <= data->attackRangeY) {
	if (ptr->velocity.x == 0) {
	  if (data->enemyAttackWait <= 0) {
	    data->enemyAttackWait = data->enemyAttackWaitTics;
	    attack = 1;
	  } else {
	    data->enemyAttackWait-=deltaT;
	  }
	} 
	ptr->velocity.y = 0;
      }
      else if (object_y(ptr) < object_y(player)) {
	ptr->velocity.y = data->speed;
	if (((rand >> 8) & 0x3) == 0) {
	  ptr->velocity.x = 0;
	  data->walkAbout = rand & 0x7f;
	}	
      } else if (object_y(ptr) > object_y(player)) {
	ptr->velocity.y = -data->speed;
	if (((rand >> 8) & 0xff) == 0) {
	  ptr->velocity.x = 0;
	  data->walkAbout = 10;
	}		
      }

      if (!attack) {
	if ((object_collision(deltaT, ptr, &collision, ENEMY_INTERCEPT_X_RANGE, ENEMY_INTERCEPT_Y_THRESHOLD*2))) {
	  switch ((rand >> 8) & 0x7) {
	  case 0:
	    attack = 1;
	    break;
	  case 1:
	    ptr->velocity.x = 0;
	    ptr->velocity.y = 0;	
	    break;	
	  case 2:
	    ptr->velocity.x = -1;
	    ptr->velocity.y = 0;
	    break;
	  case 3:
	    ptr->velocity.x = 1;
	    ptr->velocity.y = 0;
	    break;	
	  case 4:
	    ptr->velocity.x = -1;
	    ptr->velocity.y = -1;
	    break;	
	  case 5:
	    ptr->velocity.x = -1;
	    ptr->velocity.y = 1;	
	    break;
	  case 6:
	    ptr->velocity.x = 1;
	    ptr->velocity.y = 1;
	    break;
	  case 7:
	    ptr->velocity.x = 1;
	    ptr->velocity.y = -1;	      		
	    break;
	  }
	  data->walkAbout = rand & 0x7f;//ENEMY_WALKABOUT_TICS;
	}
      }
    }
  }

  
  return attack;
}


object_t* __NOINLINE
//enemy_add(uint16_t animId, uint16_t x, uint16_t y, fighter_attack_config_t* attackConfig, uint16_t attackWait, uint16_t (*intelligence)(uint16_t deltaT, object_t* ptr, fighter_data_t* data))
enemy_add(uint16_t animId, uint16_t x, uint16_t y, enemy_config_t* config)
{
  uint16_t (*intelligence)(uint16_t deltaT, object_t* ptr, fighter_data_t* data)  = config->intelligence;
  if (intelligence == 0) {
    intelligence = enemy_intelligence;
  }
  object_t* ptr =  fighter_add(OBJECT_ID_ENEMY, animId, x, y, ENEMY_INITIAL_HEALTH, config->attackConfig, intelligence);
  fighter_data_t* data = (fighter_data_t*)ptr->data;
  ptr->widthOffset = (OBJECT_WIDTH-ENEMY_WIDTH)/2;
  ptr->width = OBJECT_WIDTH;

  if (game_difficulty == GAME_DIFFICULTY_EASY) {
    data->enemyAttackWaitTics = ENEMY_ATTACK_WAIT_TICS;
    data->enemyAttackWait = ENEMY_ATTACK_WAIT_TICS;  
  } else {
    data->enemyAttackWaitTics = config->attackWait;
    data->enemyAttackWait = config->attackWait;
  }
  
  data->numAttacks = config->numAttacks;
  data->attackRangeY = config->attackRangeY;
  data->postAttackInvincibleTics = config->postAttackInvincibleTics;
    
  enemy_count++;
  return ptr;
}

void
enemy_init(void)
{
  enemy_count = 0;
#ifdef DEBUG
  enemy_pause = 0;
#endif
}


fighter_attack_config_t enemy_attackConfig1[] = {
  [OBJECT_PUNCH_LEFT1] = {
    .rangeX = FIGHTER_LONG_PUNCH_RANGE,
    .dammage = ENEMY_ATTACK_DAMMAGE,
    .durationTics = ENEMY_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0
  },
  [OBJECT_PUNCH_LEFT2] =  {
    .rangeX = FIGHTER_SHORT_PUNCH_RANGE,
    .dammage = ENEMY_ATTACK_DAMMAGE,
    .durationTics = ENEMY_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0    
  },
  [OBJECT_PUNCH_RIGHT1] =  {
    .rangeX = FIGHTER_LONG_PUNCH_RANGE,
    .dammage = ENEMY_ATTACK_DAMMAGE,
    .durationTics = ENEMY_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0    
  },
  [OBJECT_PUNCH_RIGHT2] =  {
    .rangeX = FIGHTER_SHORT_PUNCH_RANGE,
    .dammage = ENEMY_ATTACK_DAMMAGE,
    .durationTics = ENEMY_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0    
  }
};


fighter_attack_config_t enemy_attackConfig2[] = {
  [OBJECT_PUNCH_LEFT1] = {
    .rangeX = FIGHTER_LONG_PUNCH_RANGE,
    .dammage = ENEMY_ATTACK_DAMMAGE*2,
    .durationTics = ENEMY_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0
  },
  [OBJECT_PUNCH_LEFT2] =  {
    .rangeX = FIGHTER_SHORT_PUNCH_RANGE,
    .dammage = ENEMY_ATTACK_DAMMAGE*2,
    .durationTics = ENEMY_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0    
  },
  [OBJECT_PUNCH_RIGHT1] =  {
    .rangeX = FIGHTER_LONG_PUNCH_RANGE,
    .dammage = ENEMY_ATTACK_DAMMAGE*2,
    .durationTics = ENEMY_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0    
  },
  [OBJECT_PUNCH_RIGHT2] =  {
    .rangeX = FIGHTER_SHORT_PUNCH_RANGE,
    .dammage = ENEMY_ATTACK_DAMMAGE*2,
    .durationTics = ENEMY_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0    
  }
};


fighter_attack_config_t enemy_bossAttackConfig[] = {
  [OBJECT_PUNCH_LEFT1] = {
    .rangeX = ENEMY_BOSS_ATTACK_RANGE,
    .dammage = ENEMY_BOSS_ATTACK_DAMMAGE,
    .durationTics = ENEMY_ATTACK_DURATION_TICS*2,
    .hitAnimTic = ENEMY_BOSS_ATTACK_TICS_PER_FRAME,
    .vx = 0,
    .vy = 0,
    .jump = 0
  },

  [OBJECT_PUNCH_RIGHT1] =  {
    .rangeX = ENEMY_BOSS_ATTACK_RANGE,
    .dammage = ENEMY_BOSS_ATTACK_DAMMAGE,
    .durationTics = ENEMY_ATTACK_DURATION_TICS*2,
    .hitAnimTic = ENEMY_BOSS_ATTACK_TICS_PER_FRAME,
    .vx = 0,
    .vy = 0,
    .jump = 0    
  },

};



