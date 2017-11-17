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

#define LEVEL3_EASY_MIN_ATTACK_WAIT_TICS     10
#define LEVEL3_EASY_MAX_ATTACK_WAIT_TICS     20

#define LEVEL3_MEDIUM_MIN_ATTACK_WAIT_TICS   5
#define LEVEL3_MEDIUM_MAX_ATTACK_WAIT_TICS   15

#define LEVEL3_HARD_MIN_ATTACK_WAIT_TICS     5
#define LEVEL3_HARD_MAX_ATTACK_WAIT_TICS     10

#define LEVEL3_BOSS_ATTACK_RANGE         11
#define LEVEL3_BOSS_ATTACK_DURATION_TICS 10

static fighter_attack_config_t level3_boss_attackConfig[] = {
  [OBJECT_PUNCH_LEFT1] = {
    .rangeX = LEVEL3_BOSS_ATTACK_RANGE,
    .dammage = ENEMY_DEFAULT_ATTACK_DAMMAGE,
    .durationTics = LEVEL3_BOSS_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0
  },
  [OBJECT_PUNCH_LEFT2] =  {
    .rangeX = LEVEL3_BOSS_ATTACK_RANGE,
    .dammage = ENEMY_DEFAULT_ATTACK_DAMMAGE,
    .durationTics = LEVEL3_BOSS_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0
  },
  [OBJECT_PUNCH_RIGHT1] =  {
    .rangeX = LEVEL3_BOSS_ATTACK_RANGE,
    .dammage = ENEMY_DEFAULT_ATTACK_DAMMAGE,
    .durationTics = LEVEL3_BOSS_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0
  },
  [OBJECT_PUNCH_RIGHT2] =  {
    .rangeX = LEVEL3_BOSS_ATTACK_RANGE,
    .dammage = ENEMY_DEFAULT_ATTACK_DAMMAGE,
    .durationTics = LEVEL3_BOSS_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0
  }
};

static enemy_config_t level3_boss_config = {
  .attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE,
  .attackConfig = level3_boss_attackConfig,
  .minAttackWait = 5,
  .postAttackInvincibleTics = 25,
  .numAttacks = 2,
  .randomDistanceMask = 0x0,
  .randomFrequencyMask = 0xffff,
  .speedX = 2,
  .speedY = 2,
  .intelligence = 0
};


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
    .minAttackWait = LEVEL3_EASY_MIN_ATTACK_WAIT_TICS,
    .maxAttackWait = LEVEL3_EASY_MAX_ATTACK_WAIT_TICS,
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
    .minAttackWait = LEVEL3_MEDIUM_MIN_ATTACK_WAIT_TICS,
    .maxAttackWait = LEVEL3_MEDIUM_MAX_ATTACK_WAIT_TICS,
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
    .minAttackWait = LEVEL3_HARD_MIN_ATTACK_WAIT_TICS,
    .maxAttackWait = LEVEL3_HARD_MAX_ATTACK_WAIT_TICS,
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
    .y = GAME_PAVEMENT_START+100,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_2_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_EASY_ENEMY],
    .enemyCount = 1,
  },
  [LEVEL3_WAVE1_3] = {
    .x = -64,
    .y = GAME_PAVEMENT_START+60,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_1_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_EASY_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL3_WAVE1_4] = {
    .x = -48,
    .y = GAME_PAVEMENT_START+200,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_2_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_EASY_ENEMY],
    .enemyCount = 1,
  },
  /*======================*/
  [LEVEL3_WAVE2_1] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+20,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_1_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_MEDIUM_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL3_WAVE2_2] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+120,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_2_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_MEDIUM_ENEMY],
    .enemyCount = 1,
  },
  [LEVEL3_WAVE2_3] = {
    .x = SCREEN_WIDTH+64,
    .y = GAME_PAVEMENT_START+32,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_2_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_MEDIUM_ENEMY],
    .enemyCount = 2,
  },
  [LEVEL3_WAVE2_4] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+180,
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
    .x = -64,
    .y = GAME_PAVEMENT_START+32,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_2_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_MEDIUM_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL3_WAVE3_4] = {
    .x = SCREEN_WIDTH+64,
    .y = GAME_PAVEMENT_START+100,
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
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+20,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_1_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_MEDIUM_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL3_WAVE5_2] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+100,
    .animId = OBJECT_ANIM_ENEMY_LEVEL3_3_STAND_RIGHT,
    .config = &level3_enemy_configs[LEVEL3_MEDIUM_ENEMY],
    .enemyCount = 1,
  },
  [LEVEL3_WAVE5_3] = {
    .x = SCREEN_WIDTH+64,
    .y = GAME_PAVEMENT_START+132,
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
level3_addLockers(uint16_t x, int16_t y)
{
  thing_add(OBJECT_ID_LOCKER, OBJECT_ANIM_LOCKER1, OBJECT_ANIM_LOCKER1_BROKEN, OBJECT_ANIM_POSTBOX_JUNK1, x, y, 0, 0)->attributes |= OBJECT_ATTRIBUTE_IMMOVABLE;
  thing_add(OBJECT_ID_LOCKER, OBJECT_ANIM_LOCKER2, OBJECT_ANIM_LOCKER2_BROKEN, OBJECT_ANIM_POSTBOX_JUNK1, x+16, y, 1, THING_BONUS_TYPE_HEALTH)->attributes |= OBJECT_ATTRIBUTE_IMMOVABLE;
  thing_add(OBJECT_ID_LOCKER, OBJECT_ANIM_LOCKER3, OBJECT_ANIM_LOCKER3_BROKEN, OBJECT_ANIM_POSTBOX_JUNK1, x+32, y, 1, THING_BONUS_TYPE_HEALTH)->attributes |= OBJECT_ATTRIBUTE_IMMOVABLE;
  return 1;
}

//#ifdef DEBUG
int16_t
level3_pause(__UNUSED uint16_t argument)
{
  return 0;
}
//#endif


static uint16_t
level3_punchingBagIntelligence(__UNUSED uint16_t deltaT, __UNUSED object_t* ptr, __UNUSED fighter_data_t* data)
{
  uint16_t attack = 0;
  if (object_get_state(ptr) == OBJECT_STATE_HIT) {
    switch (ptr->actionId) {
    case OBJECT_HIT_LEFT:
      object_setAction(ptr, OBJECT_PUNCH_RIGHT1);
      attack = 1;
      break;
    case OBJECT_HIT_RIGHT:
      object_setAction(ptr, OBJECT_PUNCH_LEFT1);
      attack = 1;
      break;
    }

    object_set_state(ptr, OBJECT_STATE_ALIVE);
  }
  return attack;
}

static fighter_attack_config_t level3_punchingBag_attackConfig[] = {
  [OBJECT_PUNCH_LEFT1] = {
    .rangeX = 10,
    .dammage = 10,
    .durationTics = 10,
    .hitAnimTic = 2,
    .vx = 0,
    .vy = 0,
    .jump = 0
  },
  [OBJECT_PUNCH_RIGHT1] =  {
    .rangeX = 10,
    .dammage = 10,
    .durationTics = 10,
    .hitAnimTic = 2,
    .vx = 0,
    .vy = 0,
    .jump = 0
  }
};

static enemy_config_t level3_punchingBag_config = {
  .attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE,
  .attackConfig = level3_punchingBag_attackConfig,
  .maxAttackWait = 0,
  .minAttackWait = 0,
  .postAttackInvincibleTics = 0,
  .numAttacks = 1,
  .randomDistanceMask = 0,
  .randomFrequencyMask = 0,
  .speedX = 0,
  .speedY = 0,
  .intelligence = level3_punchingBagIntelligence
};

static int16_t
level3_start(__UNUSED uint16_t argument)
{
  //level3_bossSequenceMaxX = SCREEN_WIDTH*4;

  object_t* ptr = enemy_add(OBJECT_ANIM_PUNCHING_BAG_STAND_RIGHT, OBJECT_ATTRIBUTE_IMMOVABLE, 100, 100, &level3_punchingBag_config);
  ptr->widthOffset = 0,
  ptr->width = 16;

  thing_add(OBJECT_ID_POMMEL_HORSE, OBJECT_ANIM_POMMEL_HORSE, OBJECT_ANIM_POMMEL_HORSE, 0, 180, 150, 2, 0);

  return 1;
}


static int16_t
level3_addLockers_1(uint16_t argument)
{
  level3_addLockers(argument, GAME_PAVEMENT_START+8);
  return 1;
}


static int16_t
level3_addLockers_2(uint16_t argument)
{
  level3_addLockers(argument+(SCREEN_WIDTH/4), GAME_PAVEMENT_START+8);

  object_t* ptr = enemy_add(OBJECT_ANIM_PUNCHING_BAG_STAND_RIGHT, OBJECT_ATTRIBUTE_IMMOVABLE, argument, 150, &level3_punchingBag_config);
  ptr->widthOffset = 0,
  ptr->width = 16;
  ptr = enemy_add(OBJECT_ANIM_PUNCHING_BAG_STAND_RIGHT, OBJECT_ATTRIBUTE_IMMOVABLE, argument+18, 150, &level3_punchingBag_config);
  ptr->widthOffset = 0,
  ptr->width = 16;
  ptr = enemy_add(OBJECT_ANIM_PUNCHING_BAG_STAND_RIGHT, OBJECT_ATTRIBUTE_IMMOVABLE, argument+(18*2), 150, &level3_punchingBag_config);
  ptr->widthOffset = 0,
  ptr->width = 16;
  return 1;
}


static int16_t
level3_addLockers_3(uint16_t argument)
{
  level3_addLockers(argument+(SCREEN_WIDTH/4), GAME_PAVEMENT_START+8);

  object_t* ptr = enemy_add(OBJECT_ANIM_PUNCHING_BAG_STAND_RIGHT, OBJECT_ATTRIBUTE_IMMOVABLE, argument, 150, &level3_punchingBag_config);
  ptr->widthOffset = 0,
  ptr->width = 16;

  return 1;
}


static int16_t
level3_addLockers_5(uint16_t argument)
{
  level3_addLockers(argument+(SCREEN_WIDTH/4), GAME_PAVEMENT_START+8);

  object_t* ptr = enemy_add(OBJECT_ANIM_PUNCHING_BAG_STAND_RIGHT, OBJECT_ATTRIBUTE_IMMOVABLE, argument, 150, &level3_punchingBag_config);
  ptr->widthOffset = 0,
  ptr->width = 16;

  ptr = enemy_add(OBJECT_ANIM_PUNCHING_BAG_STAND_RIGHT, OBJECT_ATTRIBUTE_IMMOVABLE, argument+200, 170, &level3_punchingBag_config);
  ptr->widthOffset = 0,
  ptr->width = 16;


  thing_add(OBJECT_ID_POMMEL_HORSE, OBJECT_ANIM_POMMEL_HORSE, OBJECT_ANIM_POMMEL_HORSE, 0, argument+100, 200, 2, 0);

  return 1;
}


static int16_t
level3_addBoss(uint16_t argument)
{
  enemy_add(OBJECT_ANIM_LEVEL3_BOSS_STAND_RIGHT, 0, argument, 100, &level3_boss_config);
  return 1;
}


static int16_t
level3_bossSequence(__UNUSED uint16_t argument)
{
  game_maxHealth();

  music_play(MUSIC_LEVEL_3_BOSS);

  return 1;
}


conductor_instruction_t level3_instructions[] = {
  /* type, cameraX, argument, callback */
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, 0, level3_start},
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, 32, level3_addLockers_1},

  //  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, 0, level3_pause},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL3_WAVE1_1, level3_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL3_WAVE1_2, level3_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL3_WAVE1_3, level3_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL3_WAVE1_4, level3_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, SCREEN_WIDTH/2, level_scroll},
  {CONDUCTOR_INSTRUCTION_CAMERAX, 32, SCREEN_WIDTH+32, level3_addLockers_2},

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
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH+32, (SCREEN_WIDTH*2)+32, level3_addLockers_3},
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
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*2)+32, (SCREEN_WIDTH*3)+32, level3_addLockers_3},
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
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, (SCREEN_WIDTH*4)+32, level3_addLockers_5},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*4)-1, (SCREEN_WIDTH*5)+100, level3_addBoss},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*4)-1, 0, level3_bossSequence},
  //  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*3)+1, SCREEN_WIDTH*4, level_addBossChairs},
  //  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*4)-1, 0, level3_bossSequence},

  {CONDUCTOR_INSTRUCTION_END, 0, 0, 0},
};
