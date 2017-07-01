#include "game.h"


enum {
  LEVEL2_WAVE1_1, 
};

level_enemy_config_t level2_configs[] = {
  [LEVEL2_WAVE1_1] = {
    .x = -64,
    .y = 85,
    .animId = OBJECT_ANIM_ENEMY_LEVEL2_1_STAND_RIGHT,
    .attackConfig = enemy_attackConfig1,
    .attackWait = ENEMY_ATTACK_WAIT_TICS,
    .enemyCount = 0,
    .intelligence = 0
  }
};

/*
static object_t* level1_door;
static int16_t
level1_addPhoneBooth(uint16_t argument)
{
  __USE(argument);
  thing_add(OBJECT_ID_PHONEBOOTH, OBJECT_ANIM_PHONEBOOTH, OBJECT_ANIM_PHONEBOOTH_BROKEN, OBJECT_ANIM_PHONEBOOTH_JUNK1, game_cameraX+argument, 80);
  return 1;
}

*/
/*
static int16_t
level2_processEnemyConfig(uint16_t argument)
{
  level_enemy_config_t* ptr = &level2_configs[argument];
  if (ptr->enemyCount >= 0) {
    if (ptr->enemyCount == enemy_count) {
      enemy_add(ptr->animId, game_cameraX + ptr->x, ptr->y, ptr->attackConfig, ptr->attackWait, ptr->intelligence);
      return 1;
    }
    return 0;
  }
  return 1;
  }*/

/*
static int16_t
level2_scroll(uint16_t argument)
{
  if (enemy_count == 0) {
    conductor_setScrollTarget(argument);
    return 1;
  }

  return 0;
}
*/

static int16_t
level2_addSixPack(uint16_t argument)
{
  int16_t y = 120;
  object_t* ptr = thing_add(OBJECT_ID_SIXPACK, OBJECT_ANIM_SIXPACK1, OBJECT_ANIM_SIXPACK1, 0, game_cameraX+argument, y, 0);
  object_set_z(ptr, object_y(ptr)+32);
  ptr = thing_add(OBJECT_ID_SIXPACK, OBJECT_ANIM_SIXPACK2, OBJECT_ANIM_SIXPACK2, 0, game_cameraX+argument, y+16, 0);
  object_set_z(ptr, object_y(ptr)+16);
  thing_add(OBJECT_ID_SIXPACK, OBJECT_ANIM_SIXPACK3, OBJECT_ANIM_SIXPACK3, 0, game_cameraX+argument, 120+32, 2);  
  return 1;
}

static int16_t
level2_addTableAndChairs(uint16_t argument)
{
  thing_add(OBJECT_ID_TABLE, OBJECT_ANIM_TABLE, OBJECT_ANIM_TABLE, 0, game_cameraX+argument, 180, 0);
  thing_add(OBJECT_ID_CHAIR, OBJECT_ANIM_CHAIR1, OBJECT_ANIM_CHAIR1, 0, game_cameraX+argument+45, 180, 0);
  thing_add(OBJECT_ID_CHAIR, OBJECT_ANIM_CHAIR2, OBJECT_ANIM_CHAIR2, 0, game_cameraX+argument-15, 180, 0);  
  return 1;
}

int16_t
level2_start(uint16_t argument)
{
  __USE(argument);
  level2_addSixPack(50);
  level2_addTableAndChairs(100);
  //if (1) {
  // level1_doAddMotorbike();
  //}
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


conductor_instruction_t level2_instructions[] = {
  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, 0, level2_start},

  {CONDUCTOR_INSTRUCTION_CAMERAX, 0, 0, level2_pause},      

  {CONDUCTOR_INSTRUCTION_END}
};
