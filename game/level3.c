#include "game.h"

enum {
  LEVEL3_WAVE1_1,
  LEVEL3_WAVE1_2,
  LEVEL3_WAVE1_3,
  LEVEL3_WAVE1_4,

  LEVEL3_WAVE2_1,
  LEVEL3_WAVE2_2,
  LEVEL3_WAVE2_3,
  LEVEL3_WAVE2_4,

  LEVEL3_WAVE3_1,
  LEVEL3_WAVE3_2,
  LEVEL3_WAVE3_3,
  LEVEL3_WAVE3_4,

  LEVEL3_WAVE4_1,
  LEVEL3_WAVE4_2,
  LEVEL3_WAVE4_3,
  LEVEL3_WAVE4_4,

  LEVEL3_WAVE5_1,
  LEVEL3_WAVE5_2,
  LEVEL3_WAVE5_3,
  LEVEL3_WAVE5_4,

  LEVEL3_WAVE6_1,
  LEVEL3_WAVE6_2,
  LEVEL3_WAVE6_3,
  LEVEL3_WAVE6_4,

  LEVEL3_WAVE7_1,
  LEVEL3_WAVE7_2,
  LEVEL3_WAVE7_3,
  LEVEL3_WAVE7_4,
};

enum {
  LEVEL3_EASY_ENEMY = 0,
  LEVEL3_MEDIUM_ENEMY = 1,
  LEVEL3_HARD_ENEMY = 2
};

#define LEVEL3_RANDOM_DISTANCE_MASK    0x7f //0x3f
#define LEVEL3_RANDOM_FREQUENCY_MASK   0x7 //0x1f
#define LEVEL3_HARD_RANDOM_DISTANCE_MASK  0xf
#define LEVEL3_HARD_RANDOM_FREQUENCY_MASK 0x1f

#define LEVEL3_EASY_ATTACK_WAIT_TICS     15
#define LEVEL3_MEDIUM_ATTACK_WAIT_TICS   10
#define LEVEL3_HARD_ATTACK_WAIT_TICS     5

//#define LEVEL3_BOSS_ATTACK_RANGE         (SCREEN_WIDTH+32)
//#define LEVEL3_BOSS_ATTACK_DURATION_TICS (LEVEL3_BOSS_ATTACK_TICS_PER_FRAME*4)

/*static fighter_attack_config_t level3_boss_attackConfig[] = {
  [OBJECT_PUNCH_LEFT1] = {
    .rangeX = LEVEL3_BOSS_ATTACK_RANGE,
    .dammage = 0,
    .durationTics = LEVEL3_BOSS_ATTACK_DURATION_TICS,
    .hitAnimTic = LEVEL3_BOSS_ATTACK_TICS_PER_FRAME*2,
    .vx = 0,
    .vy = 0,
    .jump = 0
  },
  [OBJECT_PUNCH_RIGHT1] =  {
    .rangeX = LEVEL3_BOSS_ATTACK_RANGE,
    .dammage = 0,
    .durationTics = LEVEL3_BOSS_ATTACK_DURATION_TICS,
    .hitAnimTic = LEVEL3_BOSS_ATTACK_TICS_PER_FRAME*2,
    .vx = 0,
    .vy = 0,
    .jump = 0
  }
};

static enemy_config_t level3_boss_config = {
  .attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE,
  .attackConfig = level3_boss_attackConfig,
  .attackWait = 0,
  .postAttackInvincibleTics = 10,
  .numAttacks = 1,
  .randomDistanceMask = 0x0,
  .randomFrequencyMask = 0xffff,
  .speedX = 1,
  .speedY = 2,
  .intelligence = gunfighter_intelligence
};
*/

static fighter_attack_config_t level3_attackConfig1[] = {
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

static fighter_attack_config_t level3_attackConfig2[] = {
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

static enemy_config_t level3_enemy_configs[] = {
  [LEVEL3_EASY_ENEMY] = {
    .attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE,
    .attackConfig = level3_attackConfig1,
    .attackWait = LEVEL3_EASY_ATTACK_WAIT_TICS,
    .randomDistanceMask = LEVEL3_RANDOM_DISTANCE_MASK,
    .randomFrequencyMask = LEVEL3_RANDOM_FREQUENCY_MASK,
    .postAttackInvincibleTics = 0,
    .numAttacks = ENEMY_DEFAULT_NUM_ATTACKS,
    .speedX = ENEMY_DEFAULT_SPEED,
    .speedY = ENEMY_DEFAULT_SPEED,
    .intelligence = 0
  },
  [LEVEL3_MEDIUM_ENEMY] = {
    .attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE,
    .attackConfig = level3_attackConfig2,
    .attackWait = LEVEL3_MEDIUM_ATTACK_WAIT_TICS,
    .randomDistanceMask = LEVEL3_RANDOM_DISTANCE_MASK,
    .randomFrequencyMask = LEVEL3_RANDOM_FREQUENCY_MASK,
    .postAttackInvincibleTics = 0,
    .numAttacks = ENEMY_DEFAULT_NUM_ATTACKS,
    .speedX = ENEMY_DEFAULT_SPEED,
    .speedY = ENEMY_DEFAULT_SPEED,
    .intelligence = 0
  },
  [LEVEL3_HARD_ENEMY] = {
    .attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE,
    .attackConfig = level3_attackConfig2,
    .attackWait = LEVEL3_HARD_ATTACK_WAIT_TICS,
    .randomDistanceMask = LEVEL3_HARD_RANDOM_DISTANCE_MASK,
    .randomFrequencyMask = LEVEL3_HARD_RANDOM_FREQUENCY_MASK,
    .postAttackInvincibleTics = 2,
    .numAttacks = ENEMY_DEFAULT_NUM_ATTACKS,
    .speedX = ENEMY_DEFAULT_SPEED*2,
    .speedY = ENEMY_DEFAULT_SPEED,
    .intelligence = 0
  }
};

static level_enemy_config_t level3_configs[] = {
  [LEVEL3_WAVE1_1] = {
    .x = -48,
    .y = GAME_PAVEMENT_START+100,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_1_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_EASY_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL3_WAVE1_2] = {
    .x = SCREEN_WIDTH+48,
    .y = GAME_PAVEMENT_START+80,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_2_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_EASY_ENEMY],
    .enemyCount = 1,
  },
  [LEVEL3_WAVE1_3] = {
    .x = SCREEN_WIDTH+48,
    .y = GAME_PAVEMENT_START+60,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_1_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_EASY_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL3_WAVE1_4] = {
    .x = -48,
    .y = GAME_PAVEMENT_START+180,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_2_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_EASY_ENEMY],
    .enemyCount = 1,
  },
  /*======================*/
  [LEVEL3_WAVE2_1] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+80,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_1_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_MEDIUM_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL3_WAVE2_2] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+180,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_2_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_MEDIUM_ENEMY],
    .enemyCount = 1,
  },
  [LEVEL3_WAVE2_3] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+32,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_2_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_MEDIUM_ENEMY],
    .enemyCount = 2,
  },
  [LEVEL3_WAVE2_4] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+132,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_3_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_MEDIUM_ENEMY],
    .enemyCount = 0,
  },
  /*======================*/

  [LEVEL3_WAVE3_1] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+80,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_1_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_MEDIUM_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL3_WAVE3_2] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+180,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_3_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_MEDIUM_ENEMY],
    .enemyCount = 1,
  },
  [LEVEL3_WAVE3_3] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+32,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_2_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_MEDIUM_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL3_WAVE3_4] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+132,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_3_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_MEDIUM_ENEMY],
    .enemyCount = 1,
  },

  /*======================*/

  [LEVEL3_WAVE4_1] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+80,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_1_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_MEDIUM_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL3_WAVE4_2] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+180,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_3_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_MEDIUM_ENEMY],
    .enemyCount = 1,
  },
  [LEVEL3_WAVE4_3] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+32,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_2_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_MEDIUM_ENEMY],
    .enemyCount = 2,
  },
  [LEVEL3_WAVE4_4] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+132,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_3_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_MEDIUM_ENEMY],
    .enemyCount = 3,
  },

/*======================*/

  [LEVEL3_WAVE5_1] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+80,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_1_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_MEDIUM_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL3_WAVE5_2] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+180,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_3_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_MEDIUM_ENEMY],
    .enemyCount = 1,
  },
  [LEVEL3_WAVE5_3] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+32,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_2_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_MEDIUM_ENEMY],
    .enemyCount = 2,
  },
  [LEVEL3_WAVE5_4] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+132,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_3_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_MEDIUM_ENEMY],
    .enemyCount = 3,
  },

  /*======================*/

  [LEVEL3_WAVE6_1] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+80,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_1_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_HARD_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL3_WAVE6_2] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+180,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_2_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_HARD_ENEMY],
    .enemyCount = 1,
  },
  [LEVEL3_WAVE6_3] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+32,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_1_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_HARD_ENEMY],
    .enemyCount = 2,
  },
  [LEVEL3_WAVE6_4] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+132,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_2_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_HARD_ENEMY],
    .enemyCount = 3,
  } ,

  /*======================*/

  [LEVEL3_WAVE7_1] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+80,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_3_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_MEDIUM_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL3_WAVE7_2] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+180,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_3_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_MEDIUM_ENEMY],
    .enemyCount = 1,
  },
  [LEVEL3_WAVE7_3] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+32,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_3_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_MEDIUM_ENEMY],
    .enemyCount = 2,
  },
  [LEVEL3_WAVE7_4] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+132,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_3_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_MEDIUM_ENEMY],
    .enemyCount = 3,
  } ,
};

/*
static
gunfighter_config_t level3_gunfighterConfig = {
  .animId = OBJECT_ANIM_LEVEL3_BOSS_STAND_RIGHT,
  .bulletAnimId = OBJECT_ANIM_BULLET_RIGHT,
  .bulletSpeed = 16,
  .bulletHeight = 24,
  .bulletDammage = 20,
  .bulletXOffsetLeft = 0,
  .bulletXOffsetRight = OBJECT_WIDTH,
};
*/

static int16_t
level3_processEnemyConfig(uint16_t argument)
{
  level_enemy_config_t* ptr = &level3_configs[argument];
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
level3_addSixPack(uint16_t argument)
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
level3_addSixPack2(uint16_t argument)
{
  int16_t y = GAME_PAVEMENT_START-28;
  object_t* bottom = thing_add(OBJECT_ID_SIXPACK, OBJECT_ANIM_SIXPACK3, OBJECT_ANIM_SIXPACK3, 0, argument, y+32, 0, 0);

  object_t* middle = thing_add(OBJECT_ID_SIXPACK, OBJECT_ANIM_SIXPACK2, OBJECT_ANIM_SIXPACK2, 0, argument, y+16, 0, 0);
  object_set_z(middle, object_z(bottom)+1);
  thing_data(middle)->platform = bottom;

  return 1;
}


static int16_t
level3_addLockers(uint16_t x, int16_t y)
{
  thing_add(OBJECT_ID_LOCKER, OBJECT_ANIM_LOCKER1, OBJECT_ANIM_LOCKER1_BROKEN, OBJECT_ANIM_POSTBOX_JUNK1, x, y, 0, 0)->attributes |= OBJECT_ATTRIBUTE_IMMOVABLE;
  thing_add(OBJECT_ID_LOCKER, OBJECT_ANIM_LOCKER2, OBJECT_ANIM_LOCKER2_BROKEN, OBJECT_ANIM_POSTBOX_JUNK1, x+16, y, 1, THING_BONUS_TYPE_HEALTH)->attributes |= OBJECT_ATTRIBUTE_IMMOVABLE;
  thing_add(OBJECT_ID_LOCKER, OBJECT_ANIM_LOCKER3, OBJECT_ANIM_LOCKER3_BROKEN, OBJECT_ANIM_POSTBOX_JUNK1, x+32, y, 0, 0)->attributes |= OBJECT_ATTRIBUTE_IMMOVABLE;
  return 1;
}
/*
uint16_t level3_bossSequenceMaxX;
*/

//#ifdef DEBUG
int16_t
level3_pause(__UNUSED uint16_t argument)
{
  return 0;
}
//#endif
/*
static void
level3_restoreIntelligence(__UNUSED void* data)
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
level3_playerIntelligence(uint16_t deltaT, object_t* ptr, fighter_data_t* data)
{
  uint16_t attack = player_intelligence(deltaT, ptr, data);

  if (object_x(ptr) >= level3_bossSequenceMaxX) {
    ptr->velocity.x = 0;
  }

  if (game_player1 && game_player2) {
    if (object_x(game_player1) >= level3_bossSequenceMaxX &&
	object_x(game_player2) >= level3_bossSequenceMaxX ) {
      game_requestCameraX(SCREEN_WIDTH*4);
    }
  } else {
    if (object_x(ptr) >= level3_bossSequenceMaxX) {
      game_requestCameraX(SCREEN_WIDTH*4);
    }
  }

  return attack;
}
*/
static int16_t
level3_start(__UNUSED uint16_t argument)
{
  //level3_bossSequenceMaxX = SCREEN_WIDTH*4;

  return 1;
}


static int16_t
level3_addLockers_1(uint16_t argument)
{
  level3_addLockers(argument, GAME_PAVEMENT_START+8);
  return 1;
}
/*
static int16_t
level3_addLockers_2(uint16_t argument)
{
  level3_addLockers(argument, 120, 20, THING_BONUS_TYPE_HEALTH);
  return 1;
}

static int16_t
level3_addLockers_3(uint16_t argument)
{
  level3_addLockers(argument, 180, 40, THING_BONUS_TYPE_HEALTH);
  return 1;
}


static int16_t
level3_bossSequence(__UNUSED uint16_t argument)
{
  music_play(MUSIC_BOSS_1);
  alarm_add(75, level3_restoreIntelligence, 0);
  object_t* gf = gunfighter_add(&level3_gunfighterConfig, &level3_boss_config, SCREEN_WIDTH*5, 150);
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
*/
/*
  00 11 22 33 44
  *
*/

conductor_instruction_t level3_instructions[] = {
  /* type, cameraX, argument, callback */
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, 0, level3_start},
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, 32, level3_addLockers_1},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL3_WAVE1_1, level3_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL3_WAVE1_2, level3_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL3_WAVE1_3, level3_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL3_WAVE1_4, level3_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, SCREEN_WIDTH/2, level_scroll},
  //  {CONDUCTOR_INSTRUCTION_CAMERAX, 32, SCREEN_WIDTH+32, level3_addLockers_2},

  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH/2, LEVEL3_WAVE2_1, level3_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH/2, LEVEL3_WAVE2_2, level3_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH/2, LEVEL3_WAVE2_3, level3_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH/2, LEVEL3_WAVE2_4, level3_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, SCREEN_WIDTH, level_scroll},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH, LEVEL3_WAVE3_1, level3_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH, LEVEL3_WAVE3_2, level3_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH, LEVEL3_WAVE3_3, level3_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH, LEVEL3_WAVE3_4, level3_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, SCREEN_WIDTH+(SCREEN_WIDTH/2), level_scroll},
  //  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH+32, (SCREEN_WIDTH*2)+32, level3_addLockers_3},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH+(SCREEN_WIDTH/2), LEVEL3_WAVE4_1, level3_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH+(SCREEN_WIDTH/2), LEVEL3_WAVE4_2, level3_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH+(SCREEN_WIDTH/2), LEVEL3_WAVE4_3, level3_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH+(SCREEN_WIDTH/2), LEVEL3_WAVE4_4, level3_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, SCREEN_WIDTH*2, level_scroll},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH*2, LEVEL3_WAVE5_1, level3_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH*2, LEVEL3_WAVE5_2, level3_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH*2, LEVEL3_WAVE5_3, level3_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH*2, LEVEL3_WAVE5_4, level3_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, (SCREEN_WIDTH*2)+(SCREEN_WIDTH/2), level_scroll},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*2)+64, (SCREEN_WIDTH*3)+64, level3_addSixPack},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*2)+128, (SCREEN_WIDTH*3)+128, level3_addSixPack2},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*2)+(SCREEN_WIDTH/2), LEVEL3_WAVE6_1, level3_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*2)+(SCREEN_WIDTH/2), LEVEL3_WAVE6_2, level3_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*2)+(SCREEN_WIDTH/2), LEVEL3_WAVE6_3, level3_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*2)+(SCREEN_WIDTH/2), LEVEL3_WAVE6_4, level3_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, (SCREEN_WIDTH*3), level_scroll},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*3), LEVEL3_WAVE7_1, level3_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*3), LEVEL3_WAVE7_2, level3_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*3), LEVEL3_WAVE7_3, level3_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*3), LEVEL3_WAVE7_4, level3_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, SCREEN_WIDTH*4, level_scroll},
  //  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*3)+1, SCREEN_WIDTH*4, level_addBossChairs},
  //  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*4)-1, 0, level3_bossSequence},

  {CONDUCTOR_INSTRUCTION_END, 0, 0, 0},
};
