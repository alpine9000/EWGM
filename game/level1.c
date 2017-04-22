#include "game.h"

#define LEVEL1_ENEMY_BOSS_START_Y (7+56)
#define LEVEL1_ENEMY_BOSS_START_X (GAME_WORLD_WIDTH-96)

typedef struct {
  int16_t x;
  int16_t y;
  uint16_t attackDammage;
  uint16_t attackWait;
  uint16_t attackDuration;
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
};

level_enemy_config_t level1_configs[] = {
  [LEVEL1_WAVE1_1] = {
    .x = -64,
    .y = 85,
    .animId = OBJECT_ANIM_PLAYER1_STAND_RIGHT,
    .attackDammage = ENEMY_ATTACK_DAMMAGE,
    .attackWait = ENEMY_ATTACK_WAIT_TICS,
    .attackDuration = ENEMY_ATTACK_DURATION_TICS,
    .enemyCount = 0,
    .intelligence = 0
  },
  [LEVEL1_WAVE1_2] = {
    .x = SCREEN_WIDTH+64,
    .y = 85,
    .attackDammage = ENEMY_ATTACK_DAMMAGE,
    .attackWait = ENEMY_ATTACK_WAIT_TICS,
    .animId = OBJECT_ANIM_PLAYER1_STAND_RIGHT,    
    .attackDuration = ENEMY_ATTACK_DURATION_TICS,
    .enemyCount = 1,
    .intelligence = 0   
  },
  [LEVEL1_WAVE1_3] = {
    .x = SCREEN_WIDTH,
    .y = 185,
    .attackDammage = ENEMY_ATTACK_DAMMAGE,
    .animId = OBJECT_ANIM_PLAYER1_STAND_RIGHT,    
    .attackWait = ENEMY_ATTACK_WAIT_TICS,
    .attackDuration = ENEMY_ATTACK_DURATION_TICS,
    .enemyCount = 1,
    .intelligence = 0
  },

  [LEVEL1_WAVE2_1] = {
    .x = -64,
    .y = 85,
    .attackDammage = ENEMY_ATTACK_DAMMAGE,
    .animId = OBJECT_ANIM_PLAYER1_STAND_RIGHT,    
    .attackWait = ENEMY_ATTACK_WAIT_TICS,
    .attackDuration = ENEMY_ATTACK_DURATION_TICS,
    .enemyCount = 0,
    .intelligence = 0
  },
  [LEVEL1_WAVE2_2] = {
    .x = SCREEN_WIDTH,
    .y = 200,
    .animId = OBJECT_ANIM_PLAYER1_STAND_RIGHT,
    .attackDammage = ENEMY_ATTACK_DAMMAGE,    
    .attackWait = ENEMY_ATTACK_WAIT_TICS,
    .attackDuration = ENEMY_ATTACK_DURATION_TICS,
    .enemyCount = 1,
    .intelligence = 0
  },

  //=======================
  [LEVEL1_WAVE3_1] = {
    .x = SCREEN_WIDTH,
    .y = 150,
    .animId = OBJECT_ANIM_PLAYER1_STAND_RIGHT,
    .attackDammage = ENEMY_ATTACK_DAMMAGE,    
    .attackWait = ENEMY_ATTACK_WAIT_TICS/2,
    .attackDuration = ENEMY_ATTACK_DURATION_TICS,
    .enemyCount = 0,
    .intelligence = 0
  },
  [LEVEL1_WAVE3_2] = {
    .x = -48,
    .y = 150,
    .animId = OBJECT_ANIM_PLAYER1_STAND_RIGHT,
    .attackDammage = ENEMY_ATTACK_DAMMAGE,    
    .attackWait = ENEMY_ATTACK_WAIT_TICS/2,
    .attackDuration = ENEMY_ATTACK_DURATION_TICS,
    .enemyCount = 1,
    .intelligence = 0
  },
  [LEVEL1_WAVE3_3] = {
    .x = -48,
    .y = 75,
    .animId = OBJECT_ANIM_PLAYER1_STAND_RIGHT,
    .attackDammage = ENEMY_ATTACK_DAMMAGE,    
    .attackWait = ENEMY_ATTACK_WAIT_TICS/2,
    .attackDuration = ENEMY_ATTACK_DURATION_TICS,
    .enemyCount = 2,
    .intelligence = 0
  },
  [LEVEL1_WAVE3_4] = {
    .x = -48,
    .y = 150,
    .animId = OBJECT_ANIM_PLAYER1_STAND_RIGHT,
    .attackDammage = ENEMY_ATTACK_DAMMAGE,    
    .attackWait = ENEMY_ATTACK_WAIT_TICS/2,
    .attackDuration = ENEMY_ATTACK_DURATION_TICS,
    .enemyCount = 2,
    .intelligence = 0
  },
  [LEVEL1_WAVE3_5] = {
    .x = SCREEN_WIDTH+32,
    .y = 75,
    .animId = OBJECT_ANIM_PLAYER1_STAND_RIGHT,
    .attackDammage = ENEMY_ATTACK_DAMMAGE,
    .attackWait = ENEMY_ATTACK_WAIT_TICS/2,
    .attackDuration = ENEMY_ATTACK_DURATION_TICS,
    .enemyCount = 3,
    .intelligence = 0
  },
  [LEVEL1_WAVE3_6] = {
    .x = SCREEN_WIDTH,
    .y = 75,
    .animId = OBJECT_ANIM_PLAYER1_STAND_RIGHT,
    .attackDammage = ENEMY_ATTACK_DAMMAGE,    
    .attackWait = ENEMY_ATTACK_WAIT_TICS/2,
    .attackDuration = ENEMY_ATTACK_DURATION_TICS,
    .enemyCount = 0,
    .intelligence = 0
  },
  //=======================
  [LEVEL1_WAVE4_1] = {
    .x = SCREEN_WIDTH,
    .y = 150,
    .animId = OBJECT_ANIM_PLAYER4_STAND_RIGHT,
    .attackDammage = ENEMY_ATTACK_DAMMAGE*2,    
    .attackWait = 0,
    .attackDuration = ENEMY_ATTACK_DURATION_TICS,
    .enemyCount = 0,
    .intelligence = 0
  },
  [LEVEL1_WAVE4_2] = {
    .x = -48,
    .y = 150,
    .animId = OBJECT_ANIM_PLAYER1_STAND_RIGHT,
    .attackDammage = ENEMY_ATTACK_DAMMAGE,    
    .attackWait = ENEMY_ATTACK_WAIT_TICS/2,
    .attackDuration = ENEMY_ATTACK_DURATION_TICS,
    .enemyCount = 1,
    .intelligence = 0
  },  
};
  
static object_t* level1_door;
static int16_t
level1_addPhoneBooth(uint16_t argument)
{
  USE(argument);
  thing_add(OBJECT_ID_PHONEBOOTH, OBJECT_ANIM_PHONEBOOTH, OBJECT_ANIM_PHONEBOOTH_BROKEN, OBJECT_ANIM_PHONEBOOTH_JUNK1, game_cameraX+argument, 80);
  return 1;
}



#if 1
static int16_t
level1_addPostbox(uint16_t argument)
{
  thing_add(OBJECT_ID_POSTBOX, OBJECT_ANIM_POSTBOX, OBJECT_ANIM_POSTBOX_BROKEN, OBJECT_ANIM_POSTBOX_JUNK1, game_cameraX+argument, 80);
  return 1;
}
#endif

static int16_t
level1_processEnemyConfig(uint16_t argument)
{
  level_enemy_config_t* ptr = &level1_configs[argument];
  if (ptr->enemyCount >= 0) {
    if (ptr->enemyCount == enemy_count) {
      enemy_add(ptr->animId, game_cameraX + ptr->x, ptr->y, ptr->attackDammage, ptr->attackWait, ptr->attackDuration, ptr->intelligence);
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
  if (object_y(ptr) == LEVEL1_ENEMY_BOSS_START_Y && object_x(ptr) > GAME_WORLD_WIDTH-144) {
    ptr->velocity.x = -1;
  } else if (object_y(ptr) < GAME_PAVEMENT_START) {
    ptr->velocity.x = 0;
    ptr->velocity.y = 1;
  } else {
    return enemy_intelligence(deltaT, ptr, data);
  }
  
  return 0;
}


static void
level1_addDoorEnemy(void)
{
  enemy_add(OBJECT_ANIM_PLAYER1_STAND_RIGHT, LEVEL1_ENEMY_BOSS_START_X, LEVEL1_ENEMY_BOSS_START_Y, ENEMY_ATTACK_DAMMAGE, ENEMY_ATTACK_WAIT_TICS, ENEMY_ATTACK_DURATION_TICS, level1_doorIntelligence);    
}


static uint16_t level1_boss_attack_range[] = {
  [OBJECT_PUNCH_LEFT1] = 20,
  [OBJECT_PUNCH_LEFT2] = 20,
  [OBJECT_PUNCH_RIGHT1] = 20,
  [OBJECT_PUNCH_RIGHT2] = 20,
};


static void
level1_doAddBoss(uint16_t x)
{
  uint16_t y = LEVEL1_ENEMY_BOSS_START_Y;  
  
  object_t* ptr =  fighter_add(OBJECT_ID_ENEMY, OBJECT_ANIM_BOSS_STAND_RIGHT, x, y, ENEMY_INITIAL_HEALTH, ENEMY_ATTACK_DAMMAGE, level1_doorIntelligence);
  fighter_data_t* data = (fighter_data_t*)ptr->data;
  data->attackRange = level1_boss_attack_range;
  data->attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE;
  data->attackDurationTics = PLAYER_ATTACK_DURATION_TICS-2;
  //  data->widthOffset = (OBJECT_WIDTH-ENEMY_WIDTH)/2;
  ptr->widthOffset = (OBJECT_WIDTH-ENEMY_WIDTH)/2;
  ptr->width = OBJECT_WIDTH;
  data->enemyAttackWait = 0;//ENEMY_BOSS_ATTACK_TICS_PER_FRAME*ENEMY_BOSS_NUM_ATTACK_FRAMES;
  data->attackDammage = 20;
  data->enemyAttackWaitTics = 0;//data->enemyAttackWait;
  data->attackHitAnimTic = ENEMY_BOSS_ATTACK_TICS_PER_FRAME;
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
  USE(argument);
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

  music_toggle();
  
  alarm_add(50, level1_song3);
  alarm_add(100, level1_addDoorEnemy);
  alarm_add(200, level1_addDoorEnemy);
  alarm_add(300, level1_addBoss);
  alarm_add(400, level1_removeDoor);  
  return 1;
}


int16_t
level1_start(uint16_t argument)
{
  USE(argument);
  level1_addPostbox(50);
  //  level1_addPhoneBooth(50);
  //  level1_doAddBoss(50);
  return 1;
}

#ifdef DEBUG
int16_t
level1_pause(uint16_t argument)
{
  USE(argument);
  return 0;
}
#endif


conductor_instruction_t level1_instructions[] = {
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, 0, level1_start},

  //   {CONDUCTOR_INSTRUCTION_CAMERAX, 0, 0, level1_pause},      
#if 0
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL1_WAVE3_1, level1_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL1_WAVE3_2, level1_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL1_WAVE3_3, level1_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL1_WAVE3_4, level1_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL1_WAVE3_5, level1_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, LEVEL1_WAVE3_6, level1_processEnemyConfig},
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, 0, level1_pause},        
  #endif
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
  
  
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, (SCREEN_WIDTH*4), level1_scroll},  
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*3)+80, SCREEN_WIDTH, level1_addPostbox},      
  {CONDUCTOR_INSTRUCTION_CAMERAX, (SCREEN_WIDTH*4)-1, 0, level1_wave3},
  {CONDUCTOR_INSTRUCTION_END}
};
