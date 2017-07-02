#include "game.h"

#define LEVEL1_ENEMY_BOSS_START_Y (7+56)
#define LEVEL1_ENEMY_BOSS_START_X (GAME_WORLD_WIDTH-96)

static uint16_t
level1_doorIntelligence(uint16_t deltaT, object_t* ptr, fighter_data_t* data);

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

enum {
  LEVEL1_EASY_ENEMY = 0,
  LEVEL1_MEDIUM_ENEMY = 1,
  LEVEL1_MEDIUM_STRONG_ENEMY = 2,
  LEVEL1_MEDIUM_FAST_ENEMY = 3,  
  LEVEL1_HARD_ENEMY = 4,
  LEVEL1_BOSS = 5,
  LEVEL1_DOORMAN = 6
};

enemy_config_t level1_enemy_configs[] = {
  [LEVEL1_EASY_ENEMY] = {
    .attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE,    
    .attackConfig = enemy_attackConfig1,
    .attackWait = ENEMY_ATTACK_WAIT_TICS,
    .postAttackInvincibleTics = 0,
    .numAttacks = ENEMY_DEFAULT_NUM_ATTACKS,
    .speed = ENEMY_DEFAULT_SPEED,
    .intelligence = 0
  },
  [LEVEL1_MEDIUM_ENEMY] = {
    .attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE,    
    .attackConfig = enemy_attackConfig1,    
    .attackWait = ENEMY_ATTACK_WAIT_TICS/2,
    .postAttackInvincibleTics = 0,
    .numAttacks = ENEMY_DEFAULT_NUM_ATTACKS,
    .speed = ENEMY_DEFAULT_SPEED,
    .intelligence = 0
  },
  [LEVEL1_MEDIUM_STRONG_ENEMY] = {
    .attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE,    
    .attackConfig = enemy_attackConfig2,    
    .attackWait = ENEMY_ATTACK_WAIT_TICS/2,
    .postAttackInvincibleTics = 0,
    .numAttacks = ENEMY_DEFAULT_NUM_ATTACKS,
    .speed = ENEMY_DEFAULT_SPEED,
    .intelligence = 0
  },
  [LEVEL1_MEDIUM_FAST_ENEMY] = {
    .attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE,    
    .attackConfig = enemy_attackConfig1,    
    .attackWait = 0,
    .postAttackInvincibleTics = 0,
    .numAttacks = ENEMY_DEFAULT_NUM_ATTACKS,
    .speed = ENEMY_DEFAULT_SPEED,
    .intelligence = 0
  },
  [LEVEL1_HARD_ENEMY] = {
    .attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE,    
    .attackConfig = enemy_attackConfig2,    
    .attackWait = 0,
    .postAttackInvincibleTics = 0,
    .numAttacks = ENEMY_DEFAULT_NUM_ATTACKS,
    .speed = ENEMY_DEFAULT_SPEED,
  },
  [LEVEL1_BOSS] = {
    .attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE*2,
    .attackConfig = enemy_bossAttackConfig,
    .attackWait = 0,
    .postAttackInvincibleTics = 50,
    .numAttacks = 1,
    .speed = 2,
    .intelligence = level1_doorIntelligence
  },
  [LEVEL1_DOORMAN] = {
    .attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE,    
    .attackConfig = enemy_attackConfig1,    
    .attackWait = ENEMY_ATTACK_WAIT_TICS,
    .postAttackInvincibleTics = 0,
    .numAttacks = ENEMY_DEFAULT_NUM_ATTACKS,
    .speed = ENEMY_DEFAULT_SPEED,
    .intelligence = level1_doorIntelligence
  }
};


level_enemy_config_t level1_configs[] = {
  [LEVEL1_WAVE1_1] = {
    .x = -64,
    .y = 85,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_1_STAND_RIGHT,
    .config = &level1_enemy_configs[LEVEL1_EASY_ENEMY],
    .enemyCount = 0,    
  },
  [LEVEL1_WAVE1_2] = {
    .x = SCREEN_WIDTH+64,
    .y = 85,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_2_STAND_RIGHT,    
    .config = &level1_enemy_configs[LEVEL1_EASY_ENEMY],
    .enemyCount = 1,    
  },
  [LEVEL1_WAVE1_3] = {
    .x = SCREEN_WIDTH,
    .y = 185,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_1_STAND_RIGHT,    
    .config = &level1_enemy_configs[LEVEL1_EASY_ENEMY],
    .enemyCount = 1,    
  },

  //======================
  [LEVEL1_WAVE2_1] = {
    .x = -64,
    .y = 85,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_1_STAND_RIGHT,    
    .config = &level1_enemy_configs[LEVEL1_EASY_ENEMY],
    .enemyCount = 0,    
  },
  [LEVEL1_WAVE2_2] = {
    .x = SCREEN_WIDTH,
    .y = 200,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_2_STAND_RIGHT,
    .config = &level1_enemy_configs[LEVEL1_EASY_ENEMY],
    .enemyCount = 1,    
  },

  //=======================
  [LEVEL1_WAVE3_1] = {
    .x = SCREEN_WIDTH,
    .y = 150,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_1_STAND_RIGHT,
    .config = &level1_enemy_configs[LEVEL1_MEDIUM_ENEMY],
    .enemyCount = 0,    
  },
  [LEVEL1_WAVE3_2] = {
    .x = -48,
    .y = 150,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_2_STAND_RIGHT,
    .config = &level1_enemy_configs[LEVEL1_MEDIUM_ENEMY],
    .enemyCount = 1,    
  },
  [LEVEL1_WAVE3_3] = {
    .x = -48,
    .y = 75,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_1_STAND_RIGHT,
    .config = &level1_enemy_configs[LEVEL1_MEDIUM_ENEMY],
    .enemyCount = 2,    
  },
  [LEVEL1_WAVE3_4] = {
    .x = -48,
    .y = 150,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_2_STAND_RIGHT,
    .config = &level1_enemy_configs[LEVEL1_MEDIUM_ENEMY],
    .enemyCount = 2,    
  },
  [LEVEL1_WAVE3_5] = {
    .x = SCREEN_WIDTH+32,
    .y = 75,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_1_STAND_RIGHT,
    .config = &level1_enemy_configs[LEVEL1_MEDIUM_ENEMY],
    .enemyCount = 3,    
  },
  [LEVEL1_WAVE3_6] = {
    .x = SCREEN_WIDTH,
    .y = 75,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_2_STAND_RIGHT,
    .config = &level1_enemy_configs[LEVEL1_MEDIUM_ENEMY],
    .enemyCount = 0,    
  },

  //=======================
  [LEVEL1_WAVE4_1] = {
    .x = SCREEN_WIDTH,
    .y = 150,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_2_STAND_RIGHT,
    .config = &level1_enemy_configs[LEVEL1_HARD_ENEMY],
    .enemyCount = 0,    
  },
  [LEVEL1_WAVE4_2] = {
    .x = -48,
    .y = 150,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_1_STAND_RIGHT,
    .config = &level1_enemy_configs[LEVEL1_MEDIUM_STRONG_ENEMY],
    .enemyCount = 1,    
  },

  //=======================
  [LEVEL1_WAVE5_1] = {
    .x = SCREEN_WIDTH,
    .y = 100,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_1_STAND_RIGHT,
    .config = &level1_enemy_configs[LEVEL1_MEDIUM_FAST_ENEMY],
    .enemyCount = 0,    
  },
  [LEVEL1_WAVE5_2] = {
    .x = -48,
    .y = 120,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_2_STAND_RIGHT,
    .config = &level1_enemy_configs[LEVEL1_MEDIUM_STRONG_ENEMY],
    .enemyCount = 1,    
  },
  [LEVEL1_WAVE5_3] = {
    .x = SCREEN_WIDTH,
    .y = 160,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_1_STAND_RIGHT,
    .config = &level1_enemy_configs[LEVEL1_MEDIUM_FAST_ENEMY],            
    .enemyCount = 2,
  },
  [LEVEL1_WAVE5_4] = {
    .x = -48,
    .y = 180,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_2_STAND_RIGHT,
    .config = &level1_enemy_configs[LEVEL1_MEDIUM_STRONG_ENEMY],            
    .enemyCount = 3,
  },
  [LEVEL1_WAVE5_5] = {
    .x = SCREEN_WIDTH,
    .y = 200,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_1_STAND_RIGHT,
    .config = &level1_enemy_configs[LEVEL1_MEDIUM_FAST_ENEMY],                
    .enemyCount = 3,
  },
  [LEVEL1_WAVE5_6] = {
    .x = -48,
    .y = 100,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_2_STAND_RIGHT,
    .config = &level1_enemy_configs[LEVEL1_MEDIUM_STRONG_ENEMY],
    .enemyCount = 4,
  },
  
  //================================
  [LEVEL1_WAVE6_1] = {
    .x = SCREEN_WIDTH,
    .y = 88,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_2_STAND_RIGHT,
    .config = &level1_enemy_configs[LEVEL1_MEDIUM_STRONG_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL1_WAVE6_2] = {
    .x = -48,
    .y = 200,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_1_STAND_RIGHT,
    .config = &level1_enemy_configs[LEVEL1_MEDIUM_STRONG_ENEMY],    
    .enemyCount = 1,
  },

  [LEVEL1_WAVE6_3] = {
    .x = SCREEN_WIDTH,
    .y = 200,
    .animId = OBJECT_ANIM_ENEMY_LEVEL1_2_STAND_RIGHT,
    .config = &level1_enemy_configs[LEVEL1_MEDIUM_STRONG_ENEMY],    
    .enemyCount = 2,
  },  
};


static object_t* level1_door;
static int16_t
level1_addPhoneBooth(uint16_t argument)
{
  __USE(argument);
  thing_add(OBJECT_ID_PHONEBOOTH, OBJECT_ANIM_PHONEBOOTH, OBJECT_ANIM_PHONEBOOTH_BROKEN, OBJECT_ANIM_PHONEBOOTH_JUNK1, game_cameraX+argument, 80, 2);
  return 1;
}


static int16_t
level1_addPostbox(uint16_t argument)
{
  thing_add(OBJECT_ID_POSTBOX, OBJECT_ANIM_POSTBOX, OBJECT_ANIM_POSTBOX_BROKEN, OBJECT_ANIM_POSTBOX_JUNK1, game_cameraX+argument, 80, 2);
  return 1;
}


static int16_t
level1_processEnemyConfig(uint16_t argument)
{
  level_enemy_config_t* ptr = &level1_configs[argument];
  if (ptr->enemyCount >= 0) {
    if (ptr->enemyCount == enemy_count) {
      enemy_add(ptr->animId, game_cameraX + ptr->x, ptr->y, ptr->config);      
      return 1;
    }
    return 0;
  }
  return 1;
}


static void
level1_song3(void* data)
{
  __USE(data);
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
level1_addDoorEnemy(void* data)
{
  __USE(data);
  
  enemy_add(OBJECT_ANIM_ENEMY_LEVEL1_1_STAND_RIGHT, LEVEL1_ENEMY_BOSS_START_X, LEVEL1_ENEMY_BOSS_START_Y, &level1_enemy_configs[LEVEL1_DOORMAN]);    
}


static void
level1_addDoorEnemy2(void* data)
{
  __USE(data);
  
  enemy_add(OBJECT_ANIM_ENEMY_LEVEL1_2_STAND_RIGHT, LEVEL1_ENEMY_BOSS_START_X, LEVEL1_ENEMY_BOSS_START_Y, &level1_enemy_configs[LEVEL1_DOORMAN]);
}


static void
level1_doAddMotorbike(void)
{
  int16_t x = -160;
  int16_t y = 150;

  motorbike_add(x, y);
}


static void
level1_doAddBoss(uint16_t x)
{
  uint16_t y = LEVEL1_ENEMY_BOSS_START_Y; 

  enemy_add(OBJECT_ANIM_BOSS_STAND_RIGHT, x, y, &level1_enemy_configs[LEVEL1_BOSS]);
}


static void
level1_addBoss(void* data)
{
  __USE(data);
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
level1_removeDoor(void* data)
{
  __USE(data);
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
    alarm_add(50, level1_song3, 0);    
  }
  
  alarm_add(100, level1_addDoorEnemy, 0);
  alarm_add(200, level1_addDoorEnemy2, 0);
  alarm_add(300, level1_addBoss, 0);
  alarm_add(400, level1_removeDoor, 0);  
  return 1;
}


int16_t
level1_start(uint16_t argument)
{
  __USE(argument);
  level1_addPostbox(50);
  if (0) {
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
