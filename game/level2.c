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

#define LEVEL2_BASEBALL_ATTACK_DURATION_TICS (LEVEL2_3_ENEMY_ATTACK_TICS_PER_FRAME*3)
#define LEVEL2_BOSS_ATTACK_RANGE         (SCREEN_WIDTH)
#define LEVEL2_BOSS_ATTACK_DURATION_TICS (LEVEL2_BOSS_ATTACK_TICS_PER_FRAME*4)

static fighter_attack_config_t level2_boss_attackConfig[] = {
  [OBJECT_PUNCH_LEFT1] = {
    .rangeX = LEVEL2_BOSS_ATTACK_RANGE,
    .dammage = 0,
    .durationTics = LEVEL2_BOSS_ATTACK_DURATION_TICS,
    .hitAnimTic = LEVEL2_BOSS_ATTACK_TICS_PER_FRAME*2,
    .vx = 0,
    .vy = 0,
    .jump = 0
  },
  [OBJECT_PUNCH_RIGHT1] =  {
    .rangeX = LEVEL2_BOSS_ATTACK_RANGE,
    .dammage = 0,
    .durationTics = LEVEL2_BOSS_ATTACK_DURATION_TICS,
    .hitAnimTic = LEVEL2_BOSS_ATTACK_TICS_PER_FRAME*2,
    .vx = 0,
    .vy = 0,
    .jump = 0
  }
};

static enemy_config_t level2_boss_config = {
  .attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE,
  .attackConfig = level2_boss_attackConfig,
  .attackWait = 0,//ENEMY_ATTACK_WAIT_TICS,
  .postAttackInvincibleTics = 0,
  .numAttacks = 1,
  .randomDistanceMask = 0x0,
  .randomFrequencyMask = 0xffff,
  .speedX = 1,
  .speedY = 2,
  .intelligence = gunfighter_intelligence
};


static fighter_attack_config_t level2_attackConfig1[] = {
  [OBJECT_PUNCH_LEFT1] = {
    .rangeX = FIGHTER_LONG_PUNCH_RANGE,
    .dammage = ENEMY_DEFAULT_ATTACK_DAMMAGE,
    .durationTics = ENEMY_DEFAULT_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0
  },
  [OBJECT_PUNCH_LEFT2] =  {
    .rangeX = FIGHTER_SHORT_PUNCH_RANGE,
    .dammage = ENEMY_DEFAULT_ATTACK_DAMMAGE,
    .durationTics = ENEMY_DEFAULT_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0
  },
  [OBJECT_PUNCH_RIGHT1] =  {
    .rangeX = FIGHTER_LONG_PUNCH_RANGE,
    .dammage = ENEMY_DEFAULT_ATTACK_DAMMAGE,
    .durationTics = ENEMY_DEFAULT_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0
  },
  [OBJECT_PUNCH_RIGHT2] =  {
    .rangeX = FIGHTER_SHORT_PUNCH_RANGE,
    .dammage = ENEMY_DEFAULT_ATTACK_DAMMAGE,
    .durationTics = ENEMY_DEFAULT_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0
  }
};

static fighter_attack_config_t level2_baseballAttackConfig[] = {
[OBJECT_PUNCH_LEFT1] = {
    .rangeX = 16,
    .dammage = ENEMY_DEFAULT_ATTACK_DAMMAGE,
    .durationTics = LEVEL2_BASEBALL_ATTACK_DURATION_TICS,
    .hitAnimTic = LEVEL2_3_ENEMY_ATTACK_TICS_PER_FRAME,
    .vx = 0,
    .vy = 0,
    .jump = 0
  },

  [OBJECT_PUNCH_RIGHT1] =  {
    .rangeX = 16,
    .dammage = ENEMY_DEFAULT_ATTACK_DAMMAGE,
    .durationTics = LEVEL2_BASEBALL_ATTACK_DURATION_TICS,
    .hitAnimTic = LEVEL2_3_ENEMY_ATTACK_TICS_PER_FRAME,
    .vx = 0,
    .vy = 0,
    .jump = 0
  },

};

static enemy_config_t level2_enemy_configs[] = {
  [LEVEL2_EASY_ENEMY] = {
    .attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE,
    .attackConfig = level2_attackConfig1,
    .attackWait = ENEMY_DEFAULT_ATTACK_WAIT_TICS,
    .postAttackInvincibleTics = 0,
    .numAttacks = ENEMY_DEFAULT_NUM_ATTACKS,
    .speedX = ENEMY_DEFAULT_SPEED,
    .speedY = ENEMY_DEFAULT_SPEED,
    .intelligence = 0
  },
  [LEVEL2_BASEBALL_ENEMY] = {
    .attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE+2,
    .attackConfig = level2_baseballAttackConfig,
    .attackWait = 15,
    .postAttackInvincibleTics = 10,
    .numAttacks = 1,
    .speedX = ENEMY_DEFAULT_SPEED,
    .speedY = ENEMY_DEFAULT_SPEED,
    .intelligence = 0
  }
};

static level_enemy_config_t level2_configs[] = {
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

static gunfighter_config_t level2_gunfighterConfig = {
  .animId = OBJECT_ANIM_LEVEL2_BOSS_STAND_RIGHT,
  .bulletAnimId = OBJECT_ANIM_BULLET_RIGHT,
  .bulletSpeed = 16,
  .bulletHeight = 24,
  .bulletDammage = 0,
  .bulletXOffsetLeft = 0,
  .bulletXOffsetRight = OBJECT_WIDTH,
};


static int16_t
level2_processEnemyConfig(uint16_t argument)
{
  level_enemy_config_t* ptr = &level2_configs[argument];
  if (ptr->enemyCount >= 0) {
    if (ptr->enemyCount <= enemy_count) {
      enemy_add(ptr->animId, 0, game_cameraX + ptr->x, ptr->y, ptr->config);
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
  object_t* ptr = thing_add(OBJECT_ID_SIXPACK, OBJECT_ANIM_SIXPACK1, OBJECT_ANIM_SIXPACK1, 0, game_cameraX+argument, y, 0, 0);
  object_set_z(ptr, object_y(ptr)+32);
  ptr = thing_add(OBJECT_ID_SIXPACK, OBJECT_ANIM_SIXPACK2, OBJECT_ANIM_SIXPACK2, 0, game_cameraX+argument, y+16, 0, 0);
  object_set_z(ptr, object_y(ptr)+16);
  thing_add(OBJECT_ID_SIXPACK, OBJECT_ANIM_SIXPACK3, OBJECT_ANIM_SIXPACK3, 0, game_cameraX+argument, y+32, 2, 0);
  return 1;
}


static int16_t
level2_addTableAndChairs(uint16_t x, int16_t y)
{
  object_t* table = thing_add(OBJECT_ID_TABLE, OBJECT_ANIM_TABLE, OBJECT_ANIM_TABLE, 0, game_cameraX+x, y, 0, 0);
  thing_add(OBJECT_ID_CHAIR, OBJECT_ANIM_CHAIR1, OBJECT_ANIM_CHAIR1, 0, game_cameraX+x+45, y, 1, THING_BONUS_TYPE_POINTS);
  thing_add(OBJECT_ID_CHAIR, OBJECT_ANIM_CHAIR2, OBJECT_ANIM_CHAIR2, 0, game_cameraX+x-15, y, 0, 0);

  object_t* beer = thing_addJunk(table, OBJECT_ANIM_BONUS_BEER, 0, -20, 1, THING_BONUS_TYPE_POINTS);
  //  object_set_py(beer, object_py(table)+(0*OBJECT_PHYSICS_FACTOR));
  object_set_z(beer, object_z(table)+1);
  thing_data(beer)->platform = table;
  return 1;
}

static int16_t
level2_start(__UNUSED uint16_t argument)
{
  level2_addSixPack(50);
  level2_addTableAndChairs(100, 180);
  level2_addTableAndChairs(220, 180);
  return 1;
}


//#ifdef DEBUG
int16_t
level2_pause(__UNUSED uint16_t argument)
{
  return 0;
}
//#endif

int16_t level2_bossSequenceTargetX;

static void
level2_restoreIntelligence(__UNUSED void* data)
{
  if (game_player1) {
    game_player1->collisionsEnabled = 1;
    fighter_data(game_player1)->intelligence = player_intelligence;
  }
  if (game_player2) {
    game_player2->collisionsEnabled = 1;
    fighter_data(game_player2)->intelligence = player_intelligence;
  }
}


static void
level2_addBoss(__UNUSED void* data)
{
  alarm_add(100, level2_restoreIntelligence, 0);
  gunfighter_add(&level2_gunfighterConfig, &level2_boss_config, level2_bossSequenceTargetX+SCREEN_WIDTH, 150);
}

static void
level2_playersReady(__UNUSED void* data)
{
  if (game_player1) {
    game_player1->collisionsEnabled = 1;
    fighter_data(game_player1)->intelligence = fighter_nullIntelligence;
  }
  if (game_player2) {
    game_player2->collisionsEnabled = 1;
    fighter_data(game_player2)->intelligence = fighter_nullIntelligence;
  }

  alarm_add(0, level2_addBoss, 0);
}


uint16_t
level2_nullIntelligence(__UNUSED uint16_t deltaT, object_t* ptr, __UNUSED fighter_data_t* data)
{
  ptr->collisionsEnabled = 0;
  if (object_x(ptr) < level2_bossSequenceTargetX) {
    ptr->velocity.x = 1;
  } else if (object_x(ptr) > level2_bossSequenceTargetX) {
    ptr->velocity.x = -1;
  } else {
    ptr->velocity.x = 0;
  }

  if (ptr == game_player1) {
    if (object_y(ptr) > 80) {
      ptr->velocity.y = -1;
    } else if (object_y(ptr) < 80) {
      ptr->velocity.y = 1;
    } else {
      ptr->velocity.y = 0;
    }
  } else {
    if (object_y(ptr) > 180) {
      ptr->velocity.y = -1;
    } else if (object_y(ptr) < 180) {
      ptr->velocity.y = 1;
    } else {
      ptr->velocity.y = 0;
    }
  }

  int16_t ready = 1;
  if (game_player1) {
    if (object_y(game_player1) != 80 || object_x(game_player1) != level2_bossSequenceTargetX) {
      ready = 0;
    }
  }

  if (game_player2) {
    if (object_y(game_player2) != 180 || object_x(game_player2) != level2_bossSequenceTargetX) {
      ready = 0;
    }
  }

  if (ready) {
    level2_playersReady(0);
  }
  return 0;
}

static int16_t
level2_bossSequence(uint16_t argument)
{
  level2_bossSequenceTargetX = argument + (SCREEN_WIDTH/2) + 32;

  if (game_player1) {
    fighter_data(game_player1)->intelligence = level2_nullIntelligence;
    game_player1->velocity.x = 0;
    game_player1->velocity.y = 0;
  }

  if (game_player2) {
    fighter_data(game_player2)->intelligence = level2_nullIntelligence;
    game_player2->velocity.x = 0;
    game_player2->velocity.y = 0;
  }

  game_requestCameraX(argument+(SCREEN_WIDTH/2)+20);
  return 1;
}


#if 0
static int16_t
level2_addBoss(uint16_t argument)
{
  if (0)

  level2_addSixPack(argument+50);
  level2_addTableAndChairs(argument+100, 180);
  level2_addTableAndChairs(argument+220, 180);

  return 1;
}
#endif

conductor_instruction_t level2_instructions[] = {
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, 0, level2_start},


  //  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL2_WAVE1_1, level2_processEnemyConfig},
  //  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL2_WAVE1_2, level2_processEnemyConfig},
  //  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL2_WAVE1_3, level2_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, SCREEN_WIDTH*2, level_scroll},

  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH*2, SCREEN_WIDTH*2, level2_bossSequence},

  //{CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*2)+(SCREEN_WIDTH/2), (SCREEN_WIDTH*2)+(SCREEN_WIDTH/2), level2_addBoss},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, 0, level2_pause},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL2_WAVE1_3, level2_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_END, 0, 0, 0}
};
