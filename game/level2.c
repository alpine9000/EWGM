#include "game.h"


enum {
  LEVEL2_WAVE1_1,
  LEVEL2_WAVE1_2,
  LEVEL2_WAVE1_3,
};

enum {
  LEVEL2_EASY_ENEMY = 0,
  LEVEL2_BASEBALL_ENEMY = 1,
};

#define LEVEL2_BASEBALL_ATTACK_DURATION_TICS (ENEMY_ENEMY_LEVEL2_3_ATTACK_TICS_PER_FRAME*3)
#define LEVEL2_BOSS_ATTACK_RANGE         (SCREEN_WIDTH/2)
#define LEVEL2_BOSS_ATTACK_DURATION_TICS (ENEMY_LEVEL2_BOSS_ATTACK_TICS_PER_FRAME*3)

static fighter_attack_config_t level2_boss_attackConfig[] = {
  [OBJECT_PUNCH_LEFT1] = {
    .rangeX = LEVEL2_BOSS_ATTACK_RANGE,
    .dammage = 0,
    .durationTics = LEVEL2_BOSS_ATTACK_DURATION_TICS,
    .hitAnimTic = ENEMY_LEVEL2_BOSS_ATTACK_TICS_PER_FRAME*2,
    .vx = 0,
    .vy = 0,
    .jump = 0
  },
  [OBJECT_PUNCH_RIGHT1] =  {
    .rangeX = LEVEL2_BOSS_ATTACK_RANGE,
    .dammage = 0,
    .durationTics = LEVEL2_BOSS_ATTACK_DURATION_TICS,
    .hitAnimTic = ENEMY_LEVEL2_BOSS_ATTACK_TICS_PER_FRAME*2,
    .vx = 0,
    .vy = 0,
    .jump = 0    
  }
};

static enemy_config_t level2_boss_config = {
  .attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE,
  .attackConfig = level2_boss_attackConfig,
  .attackWait = ENEMY_ATTACK_WAIT_TICS,
  .postAttackInvincibleTics = 0,
  .numAttacks = 1,
  .speed = 8,
  .intelligence = gunfighter_intelligence
};


fighter_attack_config_t level2_baseballAttackConfig[] = {
[OBJECT_PUNCH_LEFT1] = {
    .rangeX = 16,
    .dammage = ENEMY_ATTACK_DAMMAGE,
    .durationTics = LEVEL2_BASEBALL_ATTACK_DURATION_TICS,
    .hitAnimTic = ENEMY_ENEMY_LEVEL2_3_ATTACK_TICS_PER_FRAME,
    .vx = 0,
    .vy = 0,
    .jump = 0
  },

  [OBJECT_PUNCH_RIGHT1] =  {
    .rangeX = 16,
    .dammage = ENEMY_ATTACK_DAMMAGE,
    .durationTics = LEVEL2_BASEBALL_ATTACK_DURATION_TICS,
    .hitAnimTic = ENEMY_ENEMY_LEVEL2_3_ATTACK_TICS_PER_FRAME,
    .vx = 0,
    .vy = 0,
    .jump = 0    
  },

};
enemy_config_t level2_enemy_configs[] = {
  [LEVEL2_EASY_ENEMY] = {
    .attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE,    
    .attackConfig = enemy_attackConfig1,
    .attackWait = ENEMY_ATTACK_WAIT_TICS,
    .postAttackInvincibleTics = 0,
    .numAttacks = ENEMY_DEFAULT_NUM_ATTACKS,
    .speed = ENEMY_DEFAULT_SPEED,
    .intelligence = 0
  },
  [LEVEL2_BASEBALL_ENEMY] = {
    .attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE+2,
    .attackConfig = level2_baseballAttackConfig,
    .attackWait = 15,
    .postAttackInvincibleTics = 10,
    .numAttacks = 1,
    .speed = ENEMY_DEFAULT_SPEED,
    .intelligence = 0
  }
};

level_enemy_config_t level2_configs[] = {
  [LEVEL2_WAVE1_1] = {
    .x = -64,
    .y = 185,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_1_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_EASY_ENEMY],
    .enemyCount = 0,    
  },
  [LEVEL2_WAVE1_2] = {
    .x = -34,
    .y = 105,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_2_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_EASY_ENEMY],
    .enemyCount = 0,    
  },
  [LEVEL2_WAVE1_3] = {
    .x = SCREEN_WIDTH+64,
    .y = GAME_PAVEMENT_START+2,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_3_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_BASEBALL_ENEMY],
    .enemyCount = 0,    
  },    
};

static int16_t
level2_processEnemyConfig(uint16_t argument)
{
  level_enemy_config_t* ptr = &level2_configs[argument];
  if (ptr->enemyCount >= 0) {
    if (ptr->enemyCount <= enemy_count) {
      enemy_add(ptr->animId, game_cameraX + ptr->x, ptr->y, ptr->config);      
      return 1;
    }
    return 0;
  }
  return 1;
}


static int16_t
level2_addSixPack(uint16_t argument)
{
  int16_t y = GAME_PAVEMENT_START-28;
  object_t* ptr = thing_add(OBJECT_ID_SIXPACK, OBJECT_ANIM_SIXPACK1, OBJECT_ANIM_SIXPACK1, 0, game_cameraX+argument, y, 0);
  object_set_z(ptr, object_y(ptr)+32);
  ptr = thing_add(OBJECT_ID_SIXPACK, OBJECT_ANIM_SIXPACK2, OBJECT_ANIM_SIXPACK2, 0, game_cameraX+argument, y+16, 0);
  object_set_z(ptr, object_y(ptr)+16);
  thing_add(OBJECT_ID_SIXPACK, OBJECT_ANIM_SIXPACK3, OBJECT_ANIM_SIXPACK3, 0, game_cameraX+argument, y+32, 2);  
  return 1;
}


static int16_t
level2_addTableAndChairs(uint16_t x, int16_t y)
{
  thing_add(OBJECT_ID_TABLE, OBJECT_ANIM_TABLE, OBJECT_ANIM_TABLE, 0, game_cameraX+x, y, 0);
  thing_add(OBJECT_ID_CHAIR, OBJECT_ANIM_CHAIR1, OBJECT_ANIM_CHAIR1, 0, game_cameraX+x+45, y, 0);
  thing_add(OBJECT_ID_CHAIR, OBJECT_ANIM_CHAIR2, OBJECT_ANIM_CHAIR2, 0, game_cameraX+x-15, y, 0);

  return 1;
}

int16_t
level2_start(uint16_t argument)
{
  __USE(argument);
  level2_addSixPack(50);
  level2_addTableAndChairs(100, 180);
  level2_addTableAndChairs(220, 180);
  return 1;
}


//#ifdef DEBUG
int16_t
level2_pause(uint16_t argument)
{
  __USE(argument);
  return 0;
}
//#endif

gunfighter_config_t leve2_gunfighterConfig = {
  .animId = OBJECT_ANIM_LEVEL2_BOSS_STAND_RIGHT,
  .bulletAnimId = OBJECT_ANIM_BULLET_RIGHT,
  .bulletSpeed = 8,
  .bulletHeight = 24,
  .bulletDammage = 0,
  .bulletXOffsetLeft = 0,
  .bulletXOffsetRight = OBJECT_WIDTH,  
};

int16_t
level2_addBoss(uint16_t argument)
{  
  gunfighter_add(&leve2_gunfighterConfig, &level2_boss_config, argument+SCREEN_WIDTH*2, 150);
  level2_addSixPack(argument+50);
  level2_addTableAndChairs(argument+100, 180);
  level2_addTableAndChairs(argument+220, 180);
  
  return 1;
}

conductor_instruction_t level2_instructions[] = {
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, 0, level2_start},


  //  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL2_WAVE1_1, level2_processEnemyConfig},
  //  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL2_WAVE1_2, level2_processEnemyConfig},
  //  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL2_WAVE1_3, level2_processEnemyConfig},    
  
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, SCREEN_WIDTH*2, level_scroll},

  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH, SCREEN_WIDTH, level2_addBoss},  

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, 0, level2_pause},    

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL2_WAVE1_3, level2_processEnemyConfig},      

  {CONDUCTOR_INSTRUCTION_END}
};
