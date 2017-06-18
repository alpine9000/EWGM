#include "game.h"

#define LEVEL1_ENEMY_BOSS_START_Y (7+56)
#define LEVEL1_ENEMY_BOSS_START_X (GAME_WORLD_WIDTH-96)

typedef struct {
  int16_t x;
  int16_t y;
  fighter_attack_config_t* attackConfig;
  uint16_t attackWait;
  int16_t enemyCount;
  uint16_t animId;
  uint16_t (*intelligence)(uint16_t deltaT, object_t* ptr, fighter_data_t* data);
} level_enemy_config_t;

enum {
  LEVEL1_WAVE1_1,
  LEVEL1_WAVE1_2,
  LEVEL1_WAVE1_3,

  LEVEL1_WAVE2_1,
  LEVEL1_WAVE2_2,

  LEVEL1_WAVE3_1,
  LEVEL1_WAVE3_2,
  LEVEL1_WAVE3_3,
  LEVEL1_WAVE3_4,
  LEVEL1_WAVE3_5,
  LEVEL1_WAVE3_6,

  LEVEL1_WAVE4_1,
  LEVEL1_WAVE4_2,  

  LEVEL1_WAVE5_1,
  LEVEL1_WAVE5_2,
  LEVEL1_WAVE5_3,
  LEVEL1_WAVE5_4,
  LEVEL1_WAVE5_5,
  LEVEL1_WAVE5_6,

  LEVEL1_WAVE6_1,
  LEVEL1_WAVE6_2,
  LEVEL1_WAVE6_3,    

  
};

level_enemy_config_t level1_configs[] = {
  [LEVEL1_WAVE1_1] = {
    .x = -64,
    .y = 85,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_1_STAND_RIGHT,
    .attackConfig = enemy_attackConfig1,
    .attackWait = ENEMY_ATTACK_WAIT_TICS,
    .enemyCount = 0,
    .intelligence = 0
  },
  [LEVEL1_WAVE1_2] = {
    .x = SCREEN_WIDTH+64,
    .y = 85,
    .attackConfig = enemy_attackConfig1,    
    .attackWait = ENEMY_ATTACK_WAIT_TICS,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_1_STAND_RIGHT,    
    .enemyCount = 1,
    .intelligence = 0   
  },
  [LEVEL1_WAVE1_3] = {
    .x = SCREEN_WIDTH,
    .y = 185,
    .attackConfig = enemy_attackConfig1,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_1_STAND_RIGHT,    
    .attackWait = ENEMY_ATTACK_WAIT_TICS,
    .enemyCount = 1,
    .intelligence = 0
  },

  //======================
  [LEVEL1_WAVE2_1] = {
    .x = -64,
    .y = 85,
    .attackConfig = enemy_attackConfig1,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_1_STAND_RIGHT,    
    .attackWait = ENEMY_ATTACK_WAIT_TICS,
    .enemyCount = 0,
    .intelligence = 0
  },
  [LEVEL1_WAVE2_2] = {
    .x = SCREEN_WIDTH,
    .y = 200,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_2_STAND_RIGHT,
    .attackConfig = enemy_attackConfig1,    
    .attackWait = ENEMY_ATTACK_WAIT_TICS,
    .enemyCount = 1,
    .intelligence = 0
  },

  //=======================
  [LEVEL1_WAVE3_1] = {
    .x = SCREEN_WIDTH,
    .y = 150,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_1_STAND_RIGHT,
    .attackConfig = enemy_attackConfig1,    
    .attackWait = ENEMY_ATTACK_WAIT_TICS/2,
    .enemyCount = 0,
    .intelligence = 0
  },
  [LEVEL1_WAVE3_2] = {
    .x = -48,
    .y = 150,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_2_STAND_RIGHT,
    .attackConfig = enemy_attackConfig1,    
    .attackWait = ENEMY_ATTACK_WAIT_TICS/2,
    .enemyCount = 1,
    .intelligence = 0
  },
  [LEVEL1_WAVE3_3] = {
    .x = -48,
    .y = 75,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_1_STAND_RIGHT,
    .attackConfig = enemy_attackConfig1,    
    .attackWait = ENEMY_ATTACK_WAIT_TICS/2,
    .enemyCount = 2,
    .intelligence = 0
  },
  [LEVEL1_WAVE3_4] = {
    .x = -48,
    .y = 150,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_2_STAND_RIGHT,
    .attackConfig = enemy_attackConfig1,    
    .attackWait = ENEMY_ATTACK_WAIT_TICS/2,
    .enemyCount = 2,
    .intelligence = 0
  },
  [LEVEL1_WAVE3_5] = {
    .x = SCREEN_WIDTH+32,
    .y = 75,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_1_STAND_RIGHT,
    .attackConfig = enemy_attackConfig1,
    .attackWait = ENEMY_ATTACK_WAIT_TICS/2,
    .enemyCount = 3,
    .intelligence = 0
  },
  [LEVEL1_WAVE3_6] = {
    .x = SCREEN_WIDTH,
    .y = 75,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_2_STAND_RIGHT,
    .attackConfig = enemy_attackConfig1,    
    .attackWait = ENEMY_ATTACK_WAIT_TICS/2,
    .enemyCount = 0,
    .intelligence = 0
  },
  //=======================
  [LEVEL1_WAVE4_1] = {
    .x = SCREEN_WIDTH,
    .y = 150,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_2_STAND_RIGHT,
    .attackConfig = enemy_attackConfig2,    
    .attackWait = 0,
    .enemyCount = 0,
    .intelligence = 0
  },
  [LEVEL1_WAVE4_2] = {
    .x = -48,
    .y = 150,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_1_STAND_RIGHT,
    .attackConfig = enemy_attackConfig2,    
    .attackWait = ENEMY_ATTACK_WAIT_TICS/2,
    .enemyCount = 1,
    .intelligence = 0
  },


  //=======================
  [LEVEL1_WAVE5_1] = {
    .x = SCREEN_WIDTH,
    .y = 100,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_1_STAND_RIGHT,
    .attackConfig = enemy_attackConfig1,    
    .attackWait = 0,
    .enemyCount = 0,
    .intelligence = 0
  },
  [LEVEL1_WAVE5_2] = {
    .x = -48,
    .y = 120,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_2_STAND_RIGHT,
    .attackConfig = enemy_attackConfig2,    
    .attackWait = ENEMY_ATTACK_WAIT_TICS/2,
    .enemyCount = 1,
    .intelligence = 0
  },
  [LEVEL1_WAVE5_3] = {
    .x = SCREEN_WIDTH,
    .y = 160,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_1_STAND_RIGHT,
    .attackConfig = enemy_attackConfig1,    
    .attackWait = 0,
    .enemyCount = 2,
    .intelligence = 0
  },
  [LEVEL1_WAVE5_4] = {
    .x = -48,
    .y = 180,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_2_STAND_RIGHT,
    .attackConfig = enemy_attackConfig2,    
    .attackWait = ENEMY_ATTACK_WAIT_TICS/2,
    .enemyCount = 3,
    .intelligence = 0
  },
  [LEVEL1_WAVE5_5] = {
    .x = SCREEN_WIDTH,
    .y = 200,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_1_STAND_RIGHT,
    .attackConfig = enemy_attackConfig1,
    .attackWait = 0,
    .enemyCount = 3,
    .intelligence = 0
  },
  [LEVEL1_WAVE5_6] = {
    .x = -48,
    .y = 100,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_2_STAND_RIGHT,
    .attackConfig = enemy_attackConfig2,    
    .attackWait = 0,
    .enemyCount = 4,
    .intelligence = 0
  },
  //================================
  [LEVEL1_WAVE6_1] = {
    .x = SCREEN_WIDTH,
    .y = 88,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_2_STAND_RIGHT,
    .attackConfig = enemy_attackConfig2,    
    .attackWait = 0,
    .enemyCount = 0,
    .intelligence = 0
  },
  [LEVEL1_WAVE6_2] = {
    .x = -48,
    .y = 200,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_1_STAND_RIGHT,
    .attackConfig = enemy_attackConfig2,    
    .attackWait = 0,
    .enemyCount = 1,
    .intelligence = 0
  },

  [LEVEL1_WAVE6_3] = {
    .x = SCREEN_WIDTH,
    .y = 200,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_2_STAND_RIGHT,
    .attackConfig = enemy_attackConfig2,    
    .attackWait = 0,
    .enemyCount = 2,
    .intelligence = 0
  },  

};
  
static object_t* level1_door;
static int16_t
level1_addPhoneBooth(uint16_t argument)
{
  __USE(argument);
  thing_add(OBJECT_ID_PHONEBOOTH, OBJECT_ANIM_PHONEBOOTH, OBJECT_ANIM_PHONEBOOTH_BROKEN, OBJECT_ANIM_PHONEBOOTH_JUNK1, game_cameraX+argument, 80);
  return 1;
}


static int16_t
level1_addPostbox(uint16_t argument)
{
  thing_add(OBJECT_ID_POSTBOX, OBJECT_ANIM_POSTBOX, OBJECT_ANIM_POSTBOX_BROKEN, OBJECT_ANIM_POSTBOX_JUNK1, game_cameraX+argument, 80);
  return 1;
}


static int16_t
level1_processEnemyConfig(uint16_t argument)
{
  level_enemy_config_t* ptr = &level1_configs[argument];
  if (ptr->enemyCount >= 0) {
    if (ptr->enemyCount == enemy_count) {
      enemy_add(ptr->animId, game_cameraX + ptr->x, ptr->y, ptr->attackConfig, ptr->attackWait, ptr->intelligence);
      return 1;
    }
    return 0;
  }
  return 1;
}


static void
level1_song3(void)
{
  music_play(3);
  music_toggle();
}


static uint16_t
level1_doorIntelligence(uint16_t deltaT, object_t* ptr, fighter_data_t* data)
{
  if (object_get_state(ptr) != OBJECT_STATE_ALIVE) {
    return 0;
  }
  
  if (object_y(ptr) == LEVEL1_ENEMY_BOSS_START_Y && object_x(ptr) > GAME_WORLD_WIDTH-144) {
    ptr->velocity.x = -1;
  } else if (object_y(ptr) < GAME_PAVEMENT_START) {
    ptr->velocity.x = 0;
    ptr->velocity.y = 1;
  } else {
    return enemy_intelligence(deltaT, ptr, data);
  }

  uint16_t attack = 0;
  object_t* player = enemy_closestPlayer(ptr);
  if (player) {
    if (abs(object_y(ptr)-object_y(player)) <= data->attackRangeY) {
      if (ptr->velocity.x == 0) {
	if (data->enemyAttackWait <= 0) {
	  data->enemyAttackWait = data->enemyAttackWaitTics;
	  attack = 1;
	} else {
	  data->enemyAttackWait-=deltaT;
	}
      }
    }
  }
  
  return attack;
}


static void
level1_addDoorEnemy(void)
{
  enemy_add(OBJECT_ANIM_ENEMY_LEVEL1_1_STAND_RIGHT, LEVEL1_ENEMY_BOSS_START_X, LEVEL1_ENEMY_BOSS_START_Y, enemy_attackConfig1, ENEMY_ATTACK_WAIT_TICS, level1_doorIntelligence);    
}


static void
level1_addDoorEnemy2(void)
{
  enemy_add(OBJECT_ANIM_ENEMY_LEVEL1_2_STAND_RIGHT, LEVEL1_ENEMY_BOSS_START_X, LEVEL1_ENEMY_BOSS_START_Y, enemy_attackConfig1, ENEMY_ATTACK_WAIT_TICS, level1_doorIntelligence);    
}

/*
static uint16_t level1_boss_attack_range[] = {
  [OBJECT_PUNCH_LEFT1] = 20,
  [OBJECT_PUNCH_LEFT2] = 20,
  [OBJECT_PUNCH_RIGHT1] = 20,
  [OBJECT_PUNCH_RIGHT2] = 20,
};
*/

uint16_t motorbike_attack = 1;
fighter_data_t motorbike_data;
static uint16_t
level1_motorbikeIntelligence(uint16_t deltaT, object_t* ptr, fighter_data_t* data)
{
  switch (object_get_state(ptr)) {
  case OBJECT_STATE_ALIVE:
  case OBJECT_STATE_HIT:
    __USE(deltaT);
    __USE(data);
    if (object_screenx(ptr) >= SCREEN_WIDTH+160) {
      object_set_state(ptr, OBJECT_STATE_ALIVE);
      ptr->velocity.x = -8;
      motorbike_attack = 1;    
    } else if (object_screenx(ptr) <= -160) {
      object_set_state(ptr, OBJECT_STATE_ALIVE);    
      ptr->velocity.x = 8;
      motorbike_attack = 1;    
    }
    break;
  }
    
  return 0;
}

object_t*
motorbike_collision(object_t* a)
{
  object_t* b;

#ifdef DEBUG
  if (!game_collisions) {
    return 0;
  }
#endif

  int16_t a_y = object_y(a);  
  int16_t a_x1 = object_x(a) + a->widthOffset;
  int16_t a_x2 = object_x(a) + (a->width - a->widthOffset);
  
  if (game_player1) {
    b = game_player1;
    //if ((b->class == OBJECT_CLASS_FIGHTER) && object_get_state(b) == OBJECT_STATE_ALIVE) {      
      int16_t b_y = object_y(b);
      if (abs(a_y - b_y) <= 5) {
	a_x1 = object_x(a) + a->widthOffset;
	a_x2 = object_x(a) + (a->width - a->widthOffset);	
	int16_t b_x1 = object_x(b) + b->widthOffset;
	int16_t b_x2 = object_x(b) + (b->width - b->widthOffset);
	
	if (a_x1 < b_x2 && a_x2 > b_x1) {		  
	  return b;
	}
      }
    //}
  }

  if (game_player2) {
    b = game_player2;
    //if ((b->class == OBJECT_CLASS_FIGHTER) && object_get_state(b) == OBJECT_STATE_ALIVE) {      
      int16_t b_y = object_y(b);
      if (abs(a_y - b_y) <= 5) {
	int16_t b_x1 = object_x(b) + b->widthOffset;
	int16_t b_x2 = object_x(b) + (b->width - b->widthOffset);
	
	if (a_x1 < b_x2 && a_x2 > b_x1) {		  
	  return b;
	}
      }
    //}
  }
  
  return 0;
}

void
motorbike_update(uint16_t deltaT, object_t* ptr)
{
  fighter_data_t* data = (fighter_data_t*)ptr->data;    
  level1_motorbikeIntelligence(deltaT, ptr, 0);
  object_updatePositionNoChecks(deltaT, ptr);

  switch (object_get_state(ptr)) {
  case OBJECT_STATE_ALIVE:
    fighter_updateSprite(ptr);
    break;    
  case OBJECT_STATE_FLASHING:
    if (data->flashCount <= 0) {
      ptr->visible = !ptr->visible;
      data->flashCount = FIGHTER_HIT_FLASH_COUNT_TICS;
    }
    data->flashCount -= deltaT;
    data->flashDurationTics -= deltaT;
    if (data->flashDurationTics <= 0) {
      fighter_die(ptr);
    } 
    break;
  default:
    return;
  }
  
  object_t* player = motorbike_collision(ptr);
  
  if (player) {
    if (player->actionId >= OBJECT_PUNCH_LEFT1 && player->actionId <= OBJECT_KICK_RIGHT) {
      data->health -= 50;
      star_add(ptr, ptr->velocity.x);      
      if (data->health <= 0) {
	object_set_state(ptr, OBJECT_STATE_FLASHING);
	if (ptr->velocity.x > 0) {
	  object_setAnim(ptr, OBJECT_ANIM_MOTORBIKE_BROKEN_RIGHT);
	} else {
	  object_setAnim(ptr, OBJECT_ANIM_MOTORBIKE_BROKEN_LEFT);
	}
	data->flashCount = FIGHTER_HIT_FLASH_COUNT_TICS;
	data->flashDurationTics = FIGHTER_HIT_FLASH_DURATION_TICS;
	sound_queueSound(SOUND_DIE01);	
      } else {
	object_set_state(ptr, OBJECT_STATE_HIT);
	if (ptr->velocity.x < 0) {
	  object_setAction(ptr, OBJECT_HIT_LEFT);
	} else {
	  object_setAction(ptr, OBJECT_HIT_RIGHT);
	}      
	switch (player->id) {
	case OBJECT_ID_PLAYER1:    
	  sound_queueSound(SOUND_TERENCE_PUNCH01);
	  break;
	case OBJECT_ID_PLAYER2:          
	  sound_queueSound(SOUND_BUD_PUNCH01);
	  break;            
	}
      }
    } else if (motorbike_attack && player) {
      motorbike_attack = 0;
      //fighter_attack(ptr, player, 5, ptr->velocity.x);    
    }
  }  
}

static void
level1_doAddMotorbike(void)
{
  int16_t x = -160;
  uint16_t y = 150;

  object_t* ptr = object_add(OBJECT_ID_ENEMY, OBJECT_CLASS_MOTORBIKE, x, y, 0, OBJECT_ANIM_MOTORBIKE_STAND_RIGHT, motorbike_update, &motorbike_data, 0);
  motorbike_data.postAttackCount = 0;
  motorbike_data.health = 100;
  ptr->width = ptr->image->w;
  ptr->widthOffset = 0;
  enemy_count++;
}


static void
level1_doAddBoss(uint16_t x)
{
  uint16_t y = LEVEL1_ENEMY_BOSS_START_Y; 
  
  object_t* ptr =  fighter_add(OBJECT_ID_ENEMY, OBJECT_ANIM_BOSS_STAND_RIGHT, x, y, ENEMY_INITIAL_HEALTH, enemy_bossAttackConfig, level1_doorIntelligence);
  fighter_data_t* data = (fighter_data_t*)ptr->data;

  // override easy setting
  data->attackConfig = enemy_bossAttackConfig;

  data->attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE*2;
  ptr->widthOffset = (OBJECT_WIDTH-ENEMY_WIDTH)/2;
  ptr->width = OBJECT_WIDTH;
  data->enemyAttackWait = 0;//ENEMY_BOSS_ATTACK_TICS_PER_FRAME*ENEMY_BOSS_NUM_ATTACK_FRAMES;
  data->enemyAttackWaitTics = 0;//data->enemyAttackWait;
  data->postAttackInvincibleTics = 50;
  data->speed = 2;
  data->numAttacks = 1;
  enemy_count++;
}


static void
level1_addBoss(void)
{
  level1_doAddBoss(LEVEL1_ENEMY_BOSS_START_X);
}


static int16_t
level1_scroll(uint16_t argument)
{
  if (enemy_count == 0) {
    conductor_setScrollTarget(argument);
    return 1;
  }

  return 0;
}

static void
level1_removeDoor(void)
{
  object_set_state(level1_door, OBJECT_STATE_REMOVED);
}

static int16_t
level1_wave3(uint16_t argument)
{
  __USE(argument);
  level1_door =  object_add(/*id*/OBJECT_ID_DOOR,
			       /*class*/OBJECT_CLASS_DECORATION,
			       /*x*/LEVEL1_ENEMY_BOSS_START_X,
			       /*y*/64,
			       /*dx*/0,
			       /*anim id*/OBJECT_ANIM_DOOR,
			       /*update*/0,
			       /*data*/0,
			       /*freeData*/0);
  level1_door->tileRender = 1;

  if (music_enabled()) {
    music_toggle();
    alarm_add(50, level1_song3);    
  }
  
  alarm_add(100, level1_addDoorEnemy);
  alarm_add(200, level1_addDoorEnemy2);
  alarm_add(300, level1_addBoss);
  alarm_add(400, level1_removeDoor);  
  return 1;
}


int16_t
level1_start(uint16_t argument)
{
  __USE(argument);
  level1_addPostbox(50);
  if (1) {
    level1_doAddMotorbike();
  }
  return 1;
}

//#ifdef DEBUG
int16_t
level1_pause(uint16_t argument)
{
  __USE(argument);
  return 0;
}
//#endif


conductor_instruction_t level1_instructions[] = {
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, 0, level1_start},

  //  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, 0, level1_pause},      

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL1_WAVE1_1, level1_processEnemyConfig},

  // {CONDUCTOR_INSTRUCTION_END},  

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL1_WAVE1_2, level1_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL1_WAVE1_3, level1_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, SCREEN_WIDTH/2, level1_scroll},

  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH/2, LEVEL1_WAVE2_1, level1_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH/2, LEVEL1_WAVE2_2, level1_processEnemyConfig},  

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, SCREEN_WIDTH, level1_scroll},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH, LEVEL1_WAVE3_1, level1_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH, LEVEL1_WAVE3_2, level1_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH, LEVEL1_WAVE3_3, level1_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH, LEVEL1_WAVE3_4, level1_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH, LEVEL1_WAVE3_5, level1_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH, LEVEL1_WAVE3_6, level1_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, SCREEN_WIDTH+(SCREEN_WIDTH/2), level1_scroll},
  
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, SCREEN_WIDTH+1, level1_addPhoneBooth},    
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH+(SCREEN_WIDTH/2), LEVEL1_WAVE4_1, level1_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH+(SCREEN_WIDTH/2), LEVEL1_WAVE4_2, level1_processEnemyConfig},    

#define WAVE_5_X ((SCREEN_WIDTH*2)+150)
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, WAVE_5_X, level1_scroll},
  {CONDUCTOR_INSTRUCTION_CAMERAX, WAVE_5_X, LEVEL1_WAVE5_1, level1_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, WAVE_5_X, LEVEL1_WAVE5_2, level1_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, WAVE_5_X, LEVEL1_WAVE5_3, level1_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, WAVE_5_X, LEVEL1_WAVE5_4, level1_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, WAVE_5_X, LEVEL1_WAVE5_5, level1_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, WAVE_5_X, LEVEL1_WAVE5_6, level1_processEnemyConfig},  

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, (WAVE_5_X)+(SCREEN_WIDTH/2), level1_scroll},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (WAVE_5_X)+1, SCREEN_WIDTH, level1_addPostbox},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (WAVE_5_X)+(SCREEN_WIDTH/2), LEVEL1_WAVE6_1, level1_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (WAVE_5_X)+(SCREEN_WIDTH/2), LEVEL1_WAVE6_2, level1_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (WAVE_5_X)+(SCREEN_WIDTH/2), LEVEL1_WAVE6_3, level1_processEnemyConfig},    
  
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, (SCREEN_WIDTH*4), level1_scroll},  
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*3)+80, SCREEN_WIDTH, level1_addPostbox},      
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*4)-1, 0, level1_wave3},
  {CONDUCTOR_INSTRUCTION_END}
};
