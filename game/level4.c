#include "game.h"

enum {
  LEVEL4_WAVE1_1,
  LEVEL4_WAVE1_2,
  LEVEL4_WAVE1_3,
  LEVEL4_WAVE1_4,

  LEVEL4_WAVE2_1,
  LEVEL4_WAVE2_2,
  LEVEL4_WAVE2_3,
  LEVEL4_WAVE2_4,

  LEVEL4_WAVE3_1,
  LEVEL4_WAVE3_2,
  LEVEL4_WAVE3_3,
  LEVEL4_WAVE3_4,

  LEVEL4_WAVE4_1,
  LEVEL4_WAVE4_2,
  LEVEL4_WAVE4_3,
  LEVEL4_WAVE4_4,

  LEVEL4_WAVE5_1,
  LEVEL4_WAVE5_2,
  LEVEL4_WAVE5_3,
  LEVEL4_WAVE5_4,

  LEVEL4_WAVE6_1,
  LEVEL4_WAVE6_2,
  LEVEL4_WAVE6_3,
  LEVEL4_WAVE6_4,

  LEVEL4_WAVE7_1,
  LEVEL4_WAVE7_2,
  LEVEL4_WAVE7_3,
  LEVEL4_WAVE7_4,

  LEVEL4_WAVE8_1,
  LEVEL4_WAVE8_2,
  LEVEL4_WAVE8_3,
  LEVEL4_WAVE8_4,
  LEVEL4_WAVE8_5,
  LEVEL4_WAVE8_6,
  LEVEL4_WAVE8_7,
  LEVEL4_WAVE8_8,


  LEVEL4_WAVE9_1,
  LEVEL4_WAVE9_2,
  LEVEL4_WAVE9_3,
  LEVEL4_WAVE9_4,

  LEVEL4_BOSS_1,
  LEVEL4_BOSS_2,

};

enum {
  LEVEL4_EASY_ENEMY = 0,
  LEVEL4_MEDIUM_ENEMY = 1,
  LEVEL4_HARD_ENEMY = 2,
  LEVEL4_HARD_FAST_ENEMY = 3,
  LEVEL4_LEMMING_ENEMY = 4,
  LEVEL4_BOSS,
};

#define LEVEL4_RANDOM_DISTANCE_MASK    0x7f //0x3f
#define LEVEL4_RANDOM_FREQUENCY_MASK   0x7 //0x1f
#define LEVEL4_HARD_RANDOM_DISTANCE_MASK  0xf
#define LEVEL4_HARD_RANDOM_FREQUENCY_MASK 0x1f

#define LEVEL4_EASY_MIN_ATTACK_WAIT_TICS     5
#define LEVEL4_EASY_MAX_ATTACK_WAIT_TICS     15

#define LEVEL4_MEDIUM_MIN_ATTACK_WAIT_TICS   5
#define LEVEL4_MEDIUM_MAX_ATTACK_WAIT_TICS   10

#define LEVEL4_HARD_MIN_ATTACK_WAIT_TICS     5
#define LEVEL4_HARD_MAX_ATTACK_WAIT_TICS     5

#define LEVEL4_BOSS_MIN_ATTACK_WAIT_TICS     25
#define LEVEL4_BOSS_MAX_ATTACK_WAIT_TICS     25

#define LEVEL4_BOSS_ATTACK_DAMMAGE       20
#define LEVEL4_BOSS_ATTACK_DURATION_TICS (LEVEL4_BOSS_ATTACK_TICS_PER_FRAME*3)
#define LEVEL4_BOSS_ATTACK_RANGE         15

static fighter_attack_config_t level4_bossAttackConfig[] = {
  [OBJECT_PUNCH_LEFT1] = {
    .rangeX = LEVEL4_BOSS_ATTACK_RANGE,
    .dammage = LEVEL4_BOSS_ATTACK_DAMMAGE,
    .durationTics = LEVEL4_BOSS_ATTACK_DURATION_TICS,
    .hitAnimTic = LEVEL4_BOSS_ATTACK_TICS_PER_FRAME,
    .vx = 0,
    .vy = 0,
    .jump = 0
  },

  [OBJECT_PUNCH_RIGHT1] =  {
    .rangeX = LEVEL4_BOSS_ATTACK_RANGE,
    .dammage = LEVEL4_BOSS_ATTACK_DAMMAGE,
    .durationTics = LEVEL4_BOSS_ATTACK_DURATION_TICS,
    .hitAnimTic = LEVEL4_BOSS_ATTACK_TICS_PER_FRAME,
    .vx = 0,
    .vy = 0,
    .jump = 0
  },
};


static fighter_attack_config_t level4_attackConfig1[] = {
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

static fighter_attack_config_t level4_attackConfig2[] = {
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


static enemy_config_t level4_enemy_configs[] = {
  [LEVEL4_EASY_ENEMY] = {
    .attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE,
    .attackConfig = level4_attackConfig1,
    .minAttackWait = LEVEL4_EASY_MIN_ATTACK_WAIT_TICS,
    .maxAttackWait = LEVEL4_EASY_MAX_ATTACK_WAIT_TICS,
    .randomDistanceMask = LEVEL4_RANDOM_DISTANCE_MASK,
    .randomFrequencyMask = LEVEL4_RANDOM_FREQUENCY_MASK,
    .postAttackInvincibleTics = 0,
    .numAttacks = ENEMY_DEFAULT_NUM_ATTACKS,
    .speedX = ENEMY_DEFAULT_SPEED,
    .speedY = ENEMY_DEFAULT_SPEED,
    .intelligence = 0
  },
  [LEVEL4_MEDIUM_ENEMY] = {
    .attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE,
    .attackConfig = level4_attackConfig2,
    .minAttackWait = LEVEL4_MEDIUM_MIN_ATTACK_WAIT_TICS,
    .maxAttackWait = LEVEL4_MEDIUM_MAX_ATTACK_WAIT_TICS,
    .randomDistanceMask = LEVEL4_RANDOM_DISTANCE_MASK,
    .randomFrequencyMask = LEVEL4_RANDOM_FREQUENCY_MASK,
    .postAttackInvincibleTics = 0,
    .numAttacks = ENEMY_DEFAULT_NUM_ATTACKS,
    .speedX = ENEMY_DEFAULT_SPEED,
    .speedY = ENEMY_DEFAULT_SPEED,
    .intelligence = 0
  },
  [LEVEL4_HARD_ENEMY] = {
    .attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE,
    .attackConfig = level4_attackConfig2,
    .minAttackWait = LEVEL4_HARD_MIN_ATTACK_WAIT_TICS,
    .maxAttackWait = LEVEL4_HARD_MAX_ATTACK_WAIT_TICS,
    .randomDistanceMask = LEVEL4_HARD_RANDOM_DISTANCE_MASK,
    .randomFrequencyMask = LEVEL4_HARD_RANDOM_FREQUENCY_MASK,
    .postAttackInvincibleTics = 2,
    .numAttacks = ENEMY_DEFAULT_NUM_ATTACKS,
    .speedX = ENEMY_DEFAULT_SPEED*2,
    .speedY = ENEMY_DEFAULT_SPEED,
    .intelligence = 0
  },
  [LEVEL4_HARD_FAST_ENEMY] = {
    .attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE,
    .attackConfig = level4_attackConfig2,
    .minAttackWait = LEVEL4_HARD_MIN_ATTACK_WAIT_TICS,
    .maxAttackWait = LEVEL4_HARD_MAX_ATTACK_WAIT_TICS,
    .randomDistanceMask = LEVEL4_HARD_RANDOM_DISTANCE_MASK,
    .randomFrequencyMask = LEVEL4_HARD_RANDOM_FREQUENCY_MASK,
    .postAttackInvincibleTics = 2,
    .numAttacks = ENEMY_DEFAULT_NUM_ATTACKS,
    .speedX = ENEMY_DEFAULT_SPEED*2,
    .speedY = ENEMY_DEFAULT_SPEED*2,
    .intelligence = 0
  },
  [LEVEL4_LEMMING_ENEMY] = {
    .attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE,
    .attackConfig = level4_attackConfig2,
    .minAttackWait = 100,
    .maxAttackWait = 100,
    .randomDistanceMask = LEVEL4_HARD_RANDOM_DISTANCE_MASK,
    .randomFrequencyMask = LEVEL4_HARD_RANDOM_FREQUENCY_MASK,
    .postAttackInvincibleTics = 0,
    .numAttacks = ENEMY_DEFAULT_NUM_ATTACKS,
    .speedX = ENEMY_DEFAULT_SPEED*2,
    .speedY = ENEMY_DEFAULT_SPEED*2,
    .initialHealth = 2,
    .intelligence = 0
  },
  [LEVEL4_BOSS] = {
    .attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE*2,
    .attackConfig = level4_bossAttackConfig,
    .minAttackWait = LEVEL4_BOSS_MIN_ATTACK_WAIT_TICS,
    .maxAttackWait = LEVEL4_BOSS_MAX_ATTACK_WAIT_TICS,
    .postAttackInvincibleTics = 50,
    .randomDistanceMask = LEVEL4_RANDOM_DISTANCE_MASK,
    .randomFrequencyMask = 0xffff,//LEVEL4_RANDOM_FREQUENCY_MASK,
    .numAttacks = 1,
    .speedX = 2,
    .speedY = 2,
    .intelligence = 0
  },
};

static level_enemy_config_t level4_configs[] = {
  [LEVEL4_BOSS_1] =  {
    .x = -48,
    .y = GAME_PAVEMENT_START+100,
    .animId = OBJECT_ANIM_LEVEL4_1_BOSS_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_BOSS],
    .enemyCount = 0,
  },
  [LEVEL4_BOSS_2] =  {
    .x = SCREEN_WIDTH+48,
    .y = GAME_PAVEMENT_START+100,
    .animId = OBJECT_ANIM_LEVEL4_2_BOSS_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_BOSS],
    .enemyCount = 1,
  },
  [LEVEL4_WAVE1_1] = {
    .x = -48,
    .y = GAME_PAVEMENT_START+100,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_1_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_EASY_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL4_WAVE1_2] = {
    .x = SCREEN_WIDTH+48,
    .y = GAME_PAVEMENT_START+80,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_2_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_EASY_ENEMY],
    .enemyCount = 1,
  },
  [LEVEL4_WAVE1_3] = {
    .x = SCREEN_WIDTH+48,
    .y = GAME_PAVEMENT_START+60,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_1_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_EASY_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL4_WAVE1_4] = {
    .x = -48,
    .y = GAME_PAVEMENT_START+180,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_2_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_EASY_ENEMY],
    .enemyCount = 1,
  },
  /*======================*/
  [LEVEL4_WAVE2_1] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+80,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_1_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_MEDIUM_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL4_WAVE2_2] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+180,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_2_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_MEDIUM_ENEMY],
    .enemyCount = 1,
  },
  [LEVEL4_WAVE2_3] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+32,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_2_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_MEDIUM_ENEMY],
    .enemyCount = 2,
  },
  [LEVEL4_WAVE2_4] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+132,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_3_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_HARD_FAST_ENEMY],
    .enemyCount = 0,
  },
  /*======================*/

  [LEVEL4_WAVE3_1] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+80,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_1_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_MEDIUM_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL4_WAVE3_2] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+180,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_3_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_HARD_FAST_ENEMY],
    .enemyCount = 1,
  },
  [LEVEL4_WAVE3_3] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+32,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_2_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_MEDIUM_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL4_WAVE3_4] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+132,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_3_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_HARD_FAST_ENEMY],
    .enemyCount = 1,
  },

  /*======================*/

  [LEVEL4_WAVE4_1] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+80,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_1_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_MEDIUM_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL4_WAVE4_2] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+180,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_3_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_HARD_FAST_ENEMY],
    .enemyCount = 1,
  },
  [LEVEL4_WAVE4_3] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+32,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_2_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_MEDIUM_ENEMY],
    .enemyCount = 2,
  },
  [LEVEL4_WAVE4_4] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+132,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_3_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_HARD_FAST_ENEMY],
    .enemyCount = 3,
  },

/*======================*/

  [LEVEL4_WAVE5_1] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+80,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_1_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_MEDIUM_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL4_WAVE5_2] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+180,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_3_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_HARD_FAST_ENEMY],
    .enemyCount = 1,
  },
  [LEVEL4_WAVE5_3] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+32,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_2_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_MEDIUM_ENEMY],
    .enemyCount = 2,
  },
  [LEVEL4_WAVE5_4] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+132,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_3_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_HARD_FAST_ENEMY],
    .enemyCount = 3,
  },

  /*======================*/

  [LEVEL4_WAVE6_1] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+80,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_1_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_HARD_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL4_WAVE6_2] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+180,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_2_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_HARD_ENEMY],
    .enemyCount = 1,
  },
  [LEVEL4_WAVE6_3] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+32,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_1_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_HARD_ENEMY],
    .enemyCount = 2,
  },
  [LEVEL4_WAVE6_4] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+132,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_2_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_HARD_ENEMY],
    .enemyCount = 3,
  } ,

  /*======================*/

  [LEVEL4_WAVE7_1] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+80,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_3_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_HARD_FAST_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL4_WAVE7_2] = {
    .x = -34,
    .y = GAME_PAVEMENT_START+180,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_3_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_HARD_FAST_ENEMY],
    .enemyCount = 1,
  },
  [LEVEL4_WAVE7_3] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+32,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_3_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_HARD_FAST_ENEMY],
    .enemyCount = 2,
  },
  [LEVEL4_WAVE7_4] = {
    .x = SCREEN_WIDTH+34,
    .y = GAME_PAVEMENT_START+132,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_3_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_HARD_FAST_ENEMY],
    .enemyCount = 3,
  } ,

  /*========================*/
  [LEVEL4_WAVE8_1] = {
    .x = SCREEN_WIDTH+32,
    .y = GAME_PAVEMENT_START+50,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_3_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_LEMMING_ENEMY],
    .enemyCount = 0,
  },
  [LEVEL4_WAVE8_2] = {
    .x = SCREEN_WIDTH+64,
    .y = GAME_PAVEMENT_START+50,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_3_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_LEMMING_ENEMY],
    .enemyCount = 1,
  },
  [LEVEL4_WAVE8_3] = {
    .x = SCREEN_WIDTH+96,
    .y = GAME_PAVEMENT_START+50,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_3_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_LEMMING_ENEMY],
    .enemyCount = 2,
  },
  [LEVEL4_WAVE8_4] = {
    .x = SCREEN_WIDTH+128,
    .y = GAME_PAVEMENT_START+50,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_3_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_LEMMING_ENEMY],
    .enemyCount = 3,
  },
  [LEVEL4_WAVE8_5] = {
    .x = SCREEN_WIDTH+160,
    .y = GAME_PAVEMENT_START+50,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_3_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_LEMMING_ENEMY],
    .enemyCount = 4,
  },
  [LEVEL4_WAVE8_6] = {
    .x = SCREEN_WIDTH+192,
    .y = GAME_PAVEMENT_START+50,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_3_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_LEMMING_ENEMY],
    .enemyCount = 5,
  },
    [LEVEL4_WAVE8_7] = {
    .x = SCREEN_WIDTH+224,
    .y = GAME_PAVEMENT_START+50,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_3_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_LEMMING_ENEMY],
    .enemyCount = 6,
  },
  [LEVEL4_WAVE8_8] = {
    .x = SCREEN_WIDTH+256,
    .y = GAME_PAVEMENT_START+50,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_3_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_LEMMING_ENEMY],
    .enemyCount = 7,
  },

  [LEVEL4_WAVE9_1] = {
    .x = -32,
    .y = GAME_PAVEMENT_START+50,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_3_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_LEMMING_ENEMY],
    .enemyCount = 4,
  },
  [LEVEL4_WAVE9_2] = {
    .x = -64,
    .y = GAME_PAVEMENT_START+50,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_3_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_LEMMING_ENEMY],
    .enemyCount = 5,
  },
    [LEVEL4_WAVE9_3] = {
    .x = -96,
    .y = GAME_PAVEMENT_START+50,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_3_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_LEMMING_ENEMY],
    .enemyCount = 6,
  },
  [LEVEL4_WAVE9_4] = {
    .x = -128,
    .y = GAME_PAVEMENT_START+50,
    .animId = OBJECT_ANIM_ENEMY_LEVEL4_3_STAND_RIGHT,
    .config = &level4_enemy_configs[LEVEL4_LEMMING_ENEMY],
    .enemyCount = 7,
  },
};


static int16_t
level4_processEnemyConfig(uint16_t argument)
{
  level_enemy_config_t* ptr = &level4_configs[argument];
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
level4_addSixPack(uint16_t argument)
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
level4_addSixPack2(uint16_t argument)
{
  int16_t y = GAME_PAVEMENT_START-28;
  object_t* bottom = thing_add(OBJECT_ID_SIXPACK, OBJECT_ANIM_SIXPACK3, OBJECT_ANIM_SIXPACK3, 0, argument, y+32, 0, 0);

  object_t* middle = thing_add(OBJECT_ID_SIXPACK, OBJECT_ANIM_SIXPACK2, OBJECT_ANIM_SIXPACK2, 0, argument, y+16, 0, 0);
  object_set_z(middle, object_z(bottom)+1);
  thing_data(middle)->platform = bottom;

  return 1;
}


static int16_t
level4_addTableAndChairs(uint16_t x, int16_t y, int16_t beerX, int16_t bonusType)
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


#ifdef DEBUG
int16_t
level4_pause(__UNUSED uint16_t argument)
{
  return 0;
}
#endif


static int16_t
level4_addTableAndChairs_1(__UNUSED uint16_t argument)
{
  //  level4_addTableAndChairs(argument, 180, 0, THING_BONUS_TYPE_HEALTH);


  baloon_add(100, 100);
  baloon_add(60, 120);
  baloon_add(120, 80);
  baloon_add(60, 130);
  return 1;
}


static int16_t
level4_addTableAndChairs_2(uint16_t argument)
{
  level4_addTableAndChairs(argument, 120, 20, THING_BONUS_TYPE_HEALTH);
  return 1;
}


static int16_t
level4_addTableAndChairs_3(uint16_t argument)
{
  level4_addTableAndChairs(argument, 180, 40, THING_BONUS_TYPE_HEALTH);
  return 1;
}


static int16_t
level4_bossSequence(__UNUSED uint16_t argument)
{
  music_play(MUSIC_BOSS_1);
  //  object_t* gf = gunfighter_add(&level4_gunfighterConfig, &level4_boss_config, SCREEN_WIDTH*5, 150);
  //  fighter_data(gf)->health *= 4;

  return 1;
}


conductor_instruction_t level4_instructions[] = {
  /* type, cameraX, argument, callback */
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, 32, level4_addTableAndChairs_1},

  //  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL4_BOSS_1, level4_processEnemyConfig},
  //  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL4_BOSS_2, level4_processEnemyConfig},

  //  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, 0, level3_pause},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL4_WAVE1_1, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL4_WAVE1_2, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL4_WAVE1_3, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL4_WAVE1_4, level4_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, SCREEN_WIDTH/2, level_scroll},
  {CONDUCTOR_INSTRUCTION_CAMERAX, 32, SCREEN_WIDTH+32, level4_addTableAndChairs_2},

  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH/2, LEVEL4_WAVE2_1, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH/2, LEVEL4_WAVE2_2, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH/2, LEVEL4_WAVE2_3, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH/2, LEVEL4_WAVE2_4, level4_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, SCREEN_WIDTH, level_scroll},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH, LEVEL4_WAVE3_1, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH, LEVEL4_WAVE3_2, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH, LEVEL4_WAVE3_3, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH, LEVEL4_WAVE3_4, level4_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, SCREEN_WIDTH+(SCREEN_WIDTH/2), level_scroll},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH+32, (SCREEN_WIDTH*2)+32, level4_addTableAndChairs_3},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH+(SCREEN_WIDTH/2), LEVEL4_WAVE4_1, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH+(SCREEN_WIDTH/2), LEVEL4_WAVE4_2, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH+(SCREEN_WIDTH/2), LEVEL4_WAVE4_3, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH+(SCREEN_WIDTH/2), LEVEL4_WAVE4_4, level4_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, SCREEN_WIDTH*2, level_scroll},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH*2, LEVEL4_WAVE5_1, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH*2, LEVEL4_WAVE5_2, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH*2, LEVEL4_WAVE5_3, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, SCREEN_WIDTH*2, LEVEL4_WAVE5_4, level4_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, (SCREEN_WIDTH*2)+(SCREEN_WIDTH/2), level_scroll},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*2)+64, (SCREEN_WIDTH*3)+64, level4_addSixPack},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*2)+128, (SCREEN_WIDTH*3)+128, level4_addSixPack2},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*2)+(SCREEN_WIDTH/2), LEVEL4_WAVE6_1, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*2)+(SCREEN_WIDTH/2), LEVEL4_WAVE6_2, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*2)+(SCREEN_WIDTH/2), LEVEL4_WAVE6_3, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*2)+(SCREEN_WIDTH/2), LEVEL4_WAVE6_4, level4_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, (SCREEN_WIDTH*3), level_scroll},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*3), LEVEL4_WAVE7_1, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*3), LEVEL4_WAVE7_2, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*3), LEVEL4_WAVE7_3, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*3), LEVEL4_WAVE7_4, level4_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, (SCREEN_WIDTH*3)+(SCREEN_WIDTH/2), level_scroll},
  //  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*3)+32, (SCREEN_WIDTH*4)+32, level4_addTableAndChairs_3},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*3)+(SCREEN_WIDTH/2), LEVEL4_WAVE8_1, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*3)+(SCREEN_WIDTH/2), LEVEL4_WAVE8_2, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*3)+(SCREEN_WIDTH/2), LEVEL4_WAVE8_3, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*3)+(SCREEN_WIDTH/2), LEVEL4_WAVE8_4, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*3)+(SCREEN_WIDTH/2), LEVEL4_WAVE8_5, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*3)+(SCREEN_WIDTH/2), LEVEL4_WAVE8_6, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*3)+(SCREEN_WIDTH/2), LEVEL4_WAVE8_7, level4_processEnemyConfig},
  //  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*3)+(SCREEN_WIDTH/2), LEVEL4_WAVE8_8, level4_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, (SCREEN_WIDTH*4), level_scroll},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*4), LEVEL4_WAVE8_1, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*4), LEVEL4_WAVE8_2, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*4), LEVEL4_WAVE8_3, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*4), LEVEL4_WAVE8_4, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*4), LEVEL4_WAVE9_1, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*4), LEVEL4_WAVE9_2, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*4), LEVEL4_WAVE9_3, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*4), LEVEL4_WAVE9_4, level4_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, (SCREEN_WIDTH*4)+(SCREEN_WIDTH/2), level_scroll},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*4)+32, (SCREEN_WIDTH*5)+32, level4_addTableAndChairs_3},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*4)+(SCREEN_WIDTH/2), LEVEL4_WAVE4_1, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*4)+(SCREEN_WIDTH/2), LEVEL4_WAVE4_2, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*4)+(SCREEN_WIDTH/2), LEVEL4_WAVE4_3, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*4)+(SCREEN_WIDTH/2), LEVEL4_WAVE4_4, level4_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, (SCREEN_WIDTH*5), level_scroll},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*5), LEVEL4_WAVE7_1, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*5), LEVEL4_WAVE7_2, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*5), LEVEL4_WAVE7_3, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*5), LEVEL4_WAVE7_4, level4_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, (SCREEN_WIDTH*5)+(SCREEN_WIDTH/2), level_scroll},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*5)+32, (SCREEN_WIDTH*6)+32, level4_addTableAndChairs_3},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*5)+(SCREEN_WIDTH/2), LEVEL4_WAVE4_1, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*5)+(SCREEN_WIDTH/2), LEVEL4_WAVE4_2, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*5)+(SCREEN_WIDTH/2), LEVEL4_WAVE4_3, level4_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*5)+(SCREEN_WIDTH/2), LEVEL4_WAVE4_4, level4_processEnemyConfig},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, (SCREEN_WIDTH*6)-1, level_scroll},
  //  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, 0, level4_pause},
  //  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*3)+1, SCREEN_WIDTH*4, level4_addBossChairs},
   {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*4)-1, 0, level4_bossSequence},


  {CONDUCTOR_INSTRUCTION_END, 0, 0, 0},
};
