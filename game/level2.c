#include "game.h"

enum {
  LEVEL2_WAVE1_1,
  LEVEL2_WAVE1_2,
  LEVEL2_WAVE1_3,
  LEVEL2_WAVE1_4,

  LEVEL2_WAVE2_1,
  LEVEL2_WAVE2_2,
  LEVEL2_WAVE2_3,
  LEVEL2_WAVE2_4,

  LEVEL2_WAVE3_1,
  LEVEL2_WAVE3_2,
  LEVEL2_WAVE3_3,
  LEVEL2_WAVE3_4,

  LEVEL2_WAVE4_1,
  LEVEL2_WAVE4_2,
  LEVEL2_WAVE4_3,
  LEVEL2_WAVE4_4,

  LEVEL2_WAVE5_1,
  LEVEL2_WAVE5_2,
  LEVEL2_WAVE5_3,
  LEVEL2_WAVE5_4,

  LEVEL2_WAVE6_1,
  LEVEL2_WAVE6_2,
  LEVEL2_WAVE6_3,
  LEVEL2_WAVE6_4,

  LEVEL2_WAVE7_1,
  LEVEL2_WAVE7_2,
  LEVEL2_WAVE7_3,
  LEVEL2_WAVE7_4,
};

enum {
  LEVEL2_EASY_ENEMY = 0,
  LEVEL2_MEDIUM_ENEMY = 1,
  LEVEL2_HARD_ENEMY = 2,
  LEVEL2_BASEBALL_ENEMY = 3,
};

#define LEVEL2_RANDOM_DISTANCE_MASK    0x7f //0x3f
#define LEVEL2_RANDOM_FREQUENCY_MASK   0x7 //0x1f

#define LEVEL2_EASY_ATTACK_WAIT_TICS     15
#define LEVEL2_MEDIUM_ATTACK_WAIT_TICS   10
#define LEVEL2_HARD_ATTACK_WAIT_TICS     0

#define LEVEL2_BASEBALL_ATTACK_DURATION_TICS (LEVEL2_3_ENEMY_ATTACK_TICS_PER_FRAME*3)
#define LEVEL2_BOSS_ATTACK_RANGE         (SCREEN_WIDTH+32)
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
  .attackWait = 0,
  .postAttackInvincibleTics = 10,
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

static fighter_attack_config_t level2_attackConfig2[] = {
  [OBJECT_PUNCH_LEFT1] = {
    .rangeX = FIGHTER_LONG_PUNCH_RANGE,
    .dammage = ENEMY_DEFAULT_ATTACK_DAMMAGE*2,
    .durationTics = ENEMY_DEFAULT_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0
  },
  [OBJECT_PUNCH_LEFT2] =  {
    .rangeX = FIGHTER_SHORT_PUNCH_RANGE,
    .dammage = ENEMY_DEFAULT_ATTACK_DAMMAGE*2,
    .durationTics = ENEMY_DEFAULT_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0
  },
  [OBJECT_PUNCH_RIGHT1] =  {
    .rangeX = FIGHTER_LONG_PUNCH_RANGE,
    .dammage = ENEMY_DEFAULT_ATTACK_DAMMAGE*2,
    .durationTics = ENEMY_DEFAULT_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0
  },
  [OBJECT_PUNCH_RIGHT2] =  {
    .rangeX = FIGHTER_SHORT_PUNCH_RANGE,
    .dammage = ENEMY_DEFAULT_ATTACK_DAMMAGE*2,
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
    .attackWait = LEVEL2_EASY_ATTACK_WAIT_TICS,
    .randomDistanceMask = LEVEL2_RANDOM_DISTANCE_MASK,
    .randomFrequencyMask = LEVEL2_RANDOM_FREQUENCY_MASK,
    .postAttackInvincibleTics = 0,
    .numAttacks = ENEMY_DEFAULT_NUM_ATTACKS,
    .speedX = ENEMY_DEFAULT_SPEED,
    .speedY = ENEMY_DEFAULT_SPEED,
    .intelligence = 0
  },
  [LEVEL2_MEDIUM_ENEMY] = {
    .attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE,
    .attackConfig = level2_attackConfig2,
    .attackWait = LEVEL2_MEDIUM_ATTACK_WAIT_TICS,
    .randomDistanceMask = LEVEL2_RANDOM_DISTANCE_MASK,
    .randomFrequencyMask = LEVEL2_RANDOM_FREQUENCY_MASK,
    .postAttackInvincibleTics = 0,
    .numAttacks = ENEMY_DEFAULT_NUM_ATTACKS,
    .speedX = ENEMY_DEFAULT_SPEED,
    .speedY = ENEMY_DEFAULT_SPEED,
    .intelligence = 0
  },
  [LEVEL2_HARD_ENEMY] = {
    .attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE,
    .attackConfig = level2_attackConfig2,
    .attackWait = LEVEL2_HARD_ATTACK_WAIT_TICS,
    .randomDistanceMask = LEVEL2_RANDOM_DISTANCE_MASK,
    .randomFrequencyMask = 0xffff,
    .postAttackInvincibleTics = 5,
    .numAttacks = ENEMY_DEFAULT_NUM_ATTACKS,
    .speedX = ENEMY_DEFAULT_SPEED*2,
    .speedY = ENEMY_DEFAULT_SPEED,
    .intelligence = 0
  },
  [LEVEL2_BASEBALL_ENEMY] = {
    .attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE+2,
    .attackConfig = level2_baseballAttackConfig,
    .attackWait = 15,
    .randomDistanceMask = LEVEL2_RANDOM_DISTANCE_MASK,
    .randomFrequencyMask = 0xffff,
    .postAttackInvincibleTics = 10,
    .numAttacks = 1,
    .speedX = ENEMY_DEFAULT_SPEED,
    .speedY = ENEMY_DEFAULT_SPEED,
    .intelligence = 0
  }
};

static level_enemy_config_t level2_configs[] = {
  [LEVEL2_WAVE1_1] = {
    .x = -48,
    .y = GAME_PAVEMENT_START+100,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_1_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_EASY_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL2_WAVE1_2] = {
    .x = SCREEN_WIDTH+48,
    .y = GAME_PAVEMENT_START+80,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_2_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_EASY_ENEMY],
    .enemyCount = 1,
  },
  [LEVEL2_WAVE1_3] = {
    .x = SCREEN_WIDTH+48,
    .y = GAME_PAVEMENT_START+60,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_1_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_EASY_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL2_WAVE1_4] = {
    .x = -48,
    .y = GAME_PAVEMENT_START+180,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_2_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_EASY_ENEMY],
    .enemyCount = 1,
  },
  /*======================*/
  [LEVEL2_WAVE2_1] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+80,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_1_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_MEDIUM_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL2_WAVE2_2] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+180,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_2_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_MEDIUM_ENEMY],
    .enemyCount = 1,
  },
  [LEVEL2_WAVE2_3] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+32,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_2_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_MEDIUM_ENEMY],
    .enemyCount = 2,
  },
  [LEVEL2_WAVE2_4] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+132,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_3_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_BASEBALL_ENEMY],
    .enemyCount = 0,
  },
  /*======================*/

  [LEVEL2_WAVE3_1] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+80,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_1_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_MEDIUM_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL2_WAVE3_2] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+180,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_3_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_BASEBALL_ENEMY],
    .enemyCount = 1,
  },
  [LEVEL2_WAVE3_3] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+32,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_2_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_MEDIUM_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL2_WAVE3_4] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+132,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_3_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_BASEBALL_ENEMY],
    .enemyCount = 1,
  },

  /*======================*/

  [LEVEL2_WAVE4_1] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+80,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_1_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_MEDIUM_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL2_WAVE4_2] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+180,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_3_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_BASEBALL_ENEMY],
    .enemyCount = 1,
  },
  [LEVEL2_WAVE4_3] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+32,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_2_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_MEDIUM_ENEMY],
    .enemyCount = 2,
  },
  [LEVEL2_WAVE4_4] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+132,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_3_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_BASEBALL_ENEMY],
    .enemyCount = 3,
  },

/*======================*/

  [LEVEL2_WAVE5_1] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+80,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_1_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_MEDIUM_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL2_WAVE5_2] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+180,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_3_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_BASEBALL_ENEMY],
    .enemyCount = 1,
  },
  [LEVEL2_WAVE5_3] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+32,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_2_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_MEDIUM_ENEMY],
    .enemyCount = 2,
  },
  [LEVEL2_WAVE5_4] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+132,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_3_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_BASEBALL_ENEMY],
    .enemyCount = 3,
  },

  /*======================*/

  [LEVEL2_WAVE6_1] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+80,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_1_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_HARD_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL2_WAVE6_2] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+180,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_2_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_HARD_ENEMY],
    .enemyCount = 1,
  },
  [LEVEL2_WAVE6_3] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+32,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_1_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_HARD_ENEMY],
    .enemyCount = 2,
  },
  [LEVEL2_WAVE6_4] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+132,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_2_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_HARD_ENEMY],
    .enemyCount = 3,
  } ,

  /*======================*/

  [LEVEL2_WAVE7_1] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+80,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_3_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_BASEBALL_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL2_WAVE7_2] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+180,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_3_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_BASEBALL_ENEMY],
    .enemyCount = 1,
  },
  [LEVEL2_WAVE7_3] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+32,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_3_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_BASEBALL_ENEMY],
    .enemyCount = 2,
  },
  [LEVEL2_WAVE7_4] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+132,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_3_STAND_RIGHT,
    .config = &level2_enemy_configs[LEVEL2_BASEBALL_ENEMY],
    .enemyCount = 3,
  } ,
};

static
gunfighter_config_t level2_gunfighterConfig = {
  .animId = OBJECT_ANIM_LEVEL2_BOSS_STAND_RIGHT,
  .bulletAnimId = OBJECT_ANIM_BULLET_RIGHT,
  .bulletSpeed = 16,
  .bulletHeight = 24,
  .bulletDammage = 20,
  .bulletXOffsetLeft = 0,
  .bulletXOffsetRight = OBJECT_WIDTH,
};


static int16_t
level2_processEnemyConfig(uint16_t argument)
{
  level_enemy_config_t* ptr = &level2_configs[argument];
  if (ptr->enemyCount >= 0) {
    if (enemy_count <= ptr->enemyCount) {
      enemy_add(ptr->animId, 0, game_cameraX + ptr->x, ptr->y, ptr->config);
      return 1;
    }
    return 0;
  }
  return 0;
}


static int16_t
level2_addSixPack(uint16_t argument)
{
  int16_t y = GAME_PAVEMENT_START-28;
  object_t* bottom = thing_add(OBJECT_ID_SIXPACK, OBJECT_ANIM_SIXPACK3, OBJECT_ANIM_SIXPACK3, 0, argument, y+32, 2, 0);

  object_t* middle = thing_add(OBJECT_ID_SIXPACK, OBJECT_ANIM_SIXPACK2, OBJECT_ANIM_SIXPACK2, 0, argument, y+16, 0, 0);
  object_set_z(middle, object_z(bottom)+1);
  thing_data(middle)->platform = bottom;

  object_t* top = thing_add(OBJECT_ID_SIXPACK, OBJECT_ANIM_SIXPACK1, OBJECT_ANIM_SIXPACK1, 0, argument, y, 0, 0);
  object_set_z(top, object_y(bottom)+2);
  thing_data(top)->platform = bottom;


  return 1;
}


static int16_t
level2_addSixPack2(uint16_t argument)
{
  int16_t y = GAME_PAVEMENT_START-28;
  object_t* bottom = thing_add(OBJECT_ID_SIXPACK, OBJECT_ANIM_SIXPACK3, OBJECT_ANIM_SIXPACK3, 0, argument, y+32, 0, 0);

  object_t* middle = thing_add(OBJECT_ID_SIXPACK, OBJECT_ANIM_SIXPACK2, OBJECT_ANIM_SIXPACK2, 0, argument, y+16, 0, 0);
  object_set_z(middle, object_z(bottom)+1);
  thing_data(middle)->platform = bottom;

  return 1;
}


static int16_t
level2_addTableAndChairs(uint16_t x, int16_t y, int16_t beerX, int16_t bonusType)
{
  object_t* table = thing_add(OBJECT_ID_TABLE, OBJECT_ANIM_TABLE, OBJECT_ANIM_TABLE, 0, x+15, y, 0, 0);
  thing_add(OBJECT_ID_CHAIR, OBJECT_ANIM_CHAIR1, OBJECT_ANIM_CHAIR1, 0, x+45+15, y, 1, THING_BONUS_TYPE_HEALTH);
  thing_add(OBJECT_ID_CHAIR, OBJECT_ANIM_CHAIR2, OBJECT_ANIM_CHAIR2, 0, x, y, 0, 0);

  object_t* beer = thing_addJunk(table, OBJECT_ANIM_BONUS_BEER, 0, beerX, -20, 1, bonusType);
  //  object_set_py(beer, object_py(table)+(0*OBJECT_PHYSICS_FACTOR));
  object_set_z(beer, object_z(table)+1);
  thing_data(beer)->platform = table;
  return 1;
}

uint16_t level2_bossSequenceMaxX;

//#ifdef DEBUG
int16_t
level2_pause(__UNUSED uint16_t argument)
{
  return 0;
}
//#endif

static void
level2_restoreIntelligence(__UNUSED void* data)
{
  if (game_player1) {
    game_player1->collisionsEnabled = 1;
    fighter_data(game_player1)->intelligence = player_intelligence;
    object_set_state(game_player1, OBJECT_STATE_ALIVE);
    game_player1->visible = 1;
    game_player1->velocity.x = 0;
    game_player1->velocity.y = 0;
  }
  if (game_player2) {
    game_player2->collisionsEnabled = 1;
    fighter_data(game_player2)->intelligence = player_intelligence;
    object_set_state(game_player2, OBJECT_STATE_ALIVE);
    game_player2->visible = 1;
    game_player2->velocity.x = 0;
    game_player2->velocity.y = 0;
  }
}


uint16_t
level2_playerIntelligence(uint16_t deltaT, object_t* ptr, fighter_data_t* data)
{
  uint16_t attack = player_intelligence(deltaT, ptr, data);

  if (object_x(ptr) >= level2_bossSequenceMaxX) {
    ptr->velocity.x = 0;
  }

  if (game_player1 && game_player2) {
    if (object_x(game_player1) >= level2_bossSequenceMaxX &&
	object_x(game_player2) >= level2_bossSequenceMaxX ) {
      game_requestCameraX(SCREEN_WIDTH*4);
    }
  } else {
    if (object_x(ptr) >= level2_bossSequenceMaxX) {
      game_requestCameraX(SCREEN_WIDTH*4);
    }
  }

  return attack;
}

static int16_t
level2_start(__UNUSED uint16_t argument)
{
  level2_bossSequenceMaxX = SCREEN_WIDTH*4;

  if (game_player1) {
    fighter_data(game_player1)->intelligence = level2_playerIntelligence;
  }

  if (game_player2) {
    fighter_data(game_player2)->intelligence = level2_playerIntelligence;
  }

  return 1;
}


static int16_t
level2_addTableAndChairs_1(uint16_t argument)
{
  level2_addTableAndChairs(argument, 180, 0, THING_BONUS_TYPE_HEALTH);
  return 1;
}

static int16_t
level2_addTableAndChairs_2(uint16_t argument)
{
  level2_addTableAndChairs(argument, 120, 20, THING_BONUS_TYPE_HEALTH);
  return 1;
}

static int16_t
level2_addTableAndChairs_3(uint16_t argument)
{
  level2_addTableAndChairs(argument, 180, 40, THING_BONUS_TYPE_HEALTH);
  return 1;
}


static int16_t
level2_bossSequence(__UNUSED uint16_t argument)
{
  music_play(MUSIC_BOSS_1);
  alarm_add(75, level2_restoreIntelligence, 0);
  object_t* gf = gunfighter_add(&level2_gunfighterConfig, &level2_boss_config, SCREEN_WIDTH*5, 150);
  fighter_data(gf)->health *= 4;

  return 1;
}

static int16_t
level_addBossChairs(uint16_t argument)
{
  //    return 1;

  thing_add(OBJECT_ID_CHAIR, OBJECT_ANIM_CHAIR1, OBJECT_ANIM_CHAIR1, 0, argument+45, 100, 0, 0);
  thing_add(OBJECT_ID_CHAIR, OBJECT_ANIM_CHAIR2, OBJECT_ANIM_CHAIR2, 0, argument+85, 200, 0, 0);
  thing_add(OBJECT_ID_CHAIR, OBJECT_ANIM_CHAIR1, OBJECT_ANIM_CHAIR1, 0, argument+145, 120, 0, 0);
  thing_add(OBJECT_ID_CHAIR, OBJECT_ANIM_CHAIR2, OBJECT_ANIM_CHAIR2, 0, argument+185, 160, 0, 0);
  thing_add(OBJECT_ID_CHAIR, OBJECT_ANIM_CHAIR1, OBJECT_ANIM_CHAIR1, 0, argument+225, 90, 0, 0);
  thing_add(OBJECT_ID_CHAIR, OBJECT_ANIM_CHAIR2, OBJECT_ANIM_CHAIR2, 0, argument+245, 120, 0, 0);
  thing_add(OBJECT_ID_CHAIR, OBJECT_ANIM_CHAIR2, OBJECT_ANIM_CHAIR2, 0, argument+100, 180, 0, 0);
  return 1;
}

/*
  00 11 22 33 44
  *
*/

conductor_instruction_t level2_instructions[] = {
  /* type, cameraX, argument, callback */
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, 0, level2_start},
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, 32, level2_addTableAndChairs_1},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL2_WAVE1_1, level2_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL2_WAVE1_2, level2_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL2_WAVE1_3, level2_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL2_WAVE1_4, level2_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, SCREEN_WIDTH/2, level_scroll},
  {CONDUCTOR_INSTRUCTION_CAMERAX, 32, SCREEN_WIDTH+32, level2_addTableAndChairs_2},

  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH/2, LEVEL2_WAVE2_1, level2_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH/2, LEVEL2_WAVE2_2, level2_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH/2, LEVEL2_WAVE2_3, level2_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH/2, LEVEL2_WAVE2_4, level2_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, SCREEN_WIDTH, level_scroll},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH, LEVEL2_WAVE3_1, level2_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH, LEVEL2_WAVE3_2, level2_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH, LEVEL2_WAVE3_3, level2_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH, LEVEL2_WAVE3_4, level2_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, SCREEN_WIDTH+(SCREEN_WIDTH/2), level_scroll},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH+32, (SCREEN_WIDTH*2)+32, level2_addTableAndChairs_3},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH+(SCREEN_WIDTH/2), LEVEL2_WAVE4_1, level2_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH+(SCREEN_WIDTH/2), LEVEL2_WAVE4_2, level2_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH+(SCREEN_WIDTH/2), LEVEL2_WAVE4_3, level2_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH+(SCREEN_WIDTH/2), LEVEL2_WAVE4_4, level2_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, SCREEN_WIDTH*2, level_scroll},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH*2, LEVEL2_WAVE5_1, level2_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH*2, LEVEL2_WAVE5_2, level2_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH*2, LEVEL2_WAVE5_3, level2_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH*2, LEVEL2_WAVE5_4, level2_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, (SCREEN_WIDTH*2)+(SCREEN_WIDTH/2), level_scroll},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*2)+64, (SCREEN_WIDTH*3)+64, level2_addSixPack},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*2)+128, (SCREEN_WIDTH*3)+128, level2_addSixPack2},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*2)+(SCREEN_WIDTH/2), LEVEL2_WAVE6_1, level2_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*2)+(SCREEN_WIDTH/2), LEVEL2_WAVE6_2, level2_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*2)+(SCREEN_WIDTH/2), LEVEL2_WAVE6_3, level2_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*2)+(SCREEN_WIDTH/2), LEVEL2_WAVE6_4, level2_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, (SCREEN_WIDTH*3), level_scroll},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*3), LEVEL2_WAVE7_1, level2_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*3), LEVEL2_WAVE7_2, level2_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*3), LEVEL2_WAVE7_3, level2_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*3), LEVEL2_WAVE7_4, level2_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, SCREEN_WIDTH*4, level_scroll},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*3)+1, SCREEN_WIDTH*4, level_addBossChairs},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*4)-1, 0, level2_bossSequence},

  {CONDUCTOR_INSTRUCTION_END, 0, 0, 0},
};
