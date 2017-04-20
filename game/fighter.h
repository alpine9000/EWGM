#ifndef __FIGHTER_H
#define __FIGHTER_H

#define FIGHTER_MAX_FIGHTERS         8
#define FIGHTER_LONG_PUNCH_RANGE     13
#define FIGHTER_SHORT_PUNCH_RANGE    8
//#define FIGHTER_Y_ATTACK_THRESHOLD   6
#define FIGHTER_ENEMY_Y_ATTACK_RANGE 6
#define FIGHTER_SPAWN_FLASH_COUNT_TICS 5
#define FIGHTER_SPAWN_FLASH_DURATION_TICS 100
#define FIGHTER_HIT_FLASH_COUNT_TICS 5
#define FIGHTER_HIT_FLASH_DURATION_TICS 75

typedef struct fighter_data {
  struct fighter_data* prev;
  struct fighter_data* next;  
  uint16_t attackCount;
  uint16_t attackHitAnimTic;
  uint16_t attackChecked;
  uint16_t attackType;
  uint16_t numAttacks;
  int16_t attack_py;
  uint16_t (*intelligence)(uint16_t deltaT, object_t* ptr, struct fighter_data* data);
  int16_t walkAbout; // todo: move to enemy data  
  int16_t health;
  uint16_t buttonReleased;
  uint16_t attackQueued;
  uint16_t attackDurationTics;
  uint16_t attackDammage;
  int16_t flashCount;
  int16_t flashDurationTics;
  int16_t enemyAttackWait;
  int16_t enemyAttackWaitTics;  
  uint16_t* attackRange;
  uint16_t attackRangeY;
  int16_t postAttackInvincibleTics;
  int16_t postAttackCount;
  int16_t speed;
} fighter_data_t;


void
fighter_init(void);

object_t*
fighter_add(uint16_t id, uint16_t animId, int16_t x, int16_t y, uint16_t initialHealth, uint16_t attackDammage, uint16_t (*intelligence)(uint16_t deltaT, object_t* ptr, struct fighter_data* data));

int16_t
fighter_collision(int16_t deltaT, object_t* a, object_collision_t* collision, uint16_t thresholdx, uint16_t thresholdy);

void
fighter_update(uint16_t deltaT, object_t* ptr);

#endif
