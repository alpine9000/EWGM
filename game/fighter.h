#ifndef __FIGHTER_H
#define __FIGHTER_H

#define FIGHTER_MAX_FIGHTERS              8
#define FIGHTER_LONG_PUNCH_RANGE          13
#define FIGHTER_SHORT_PUNCH_RANGE         8
//#define FIGHTER_Y_ATTACK_THRESHOLD      6
#define FIGHTER_ENEMY_Y_ATTACK_RANGE      6
#define FIGHTER_SPAWN_FLASH_COUNT_TICS    5
#define FIGHTER_SPAWN_FLASH_DURATION_TICS 100
#define FIGHTER_HIT_FLASH_COUNT_TICS      5
#define FIGHTER_HIT_FLASH_DURATION_TICS   75
#define FIGHTER_DATA_MAGIC_NUMBER         0xDEADBEEF

#define fighter_data(x) ((fighter_data_t*)(_object_data(x, OBJECT_DATA_TYPE_FIGHTER)))

typedef struct {
  uint16_t hitAnimTic;
  uint16_t durationTics;
  uint16_t dammage;
  uint16_t rangeX;
  uint16_t vx;
  uint16_t vy;
  uint16_t jump;
} fighter_attack_config_t;


typedef struct fighter_data {
#ifdef DEBUG
  uint32_t magicNumber;
#endif
  struct fighter_data* prev;
  struct fighter_data* next;
  uint16_t attackCount;
  uint16_t attackChecked;
  uint16_t attackType;
  int16_t attackRangeY;
  uint16_t numAttacks;
  uint16_t (*intelligence)(uint16_t deltaT, object_t* ptr, struct fighter_data* data);
  int16_t walkAbout;
  uint16_t randomFrequencyMask;
  uint16_t randomDistanceMask;
  int16_t health;
  uint16_t buttonReleased;
  uint16_t attackQueued;
  uint16_t attackJump;
  int16_t attackJumpY;
  int16_t flashCount;
  int16_t flashDurationTics;
  int16_t enemyAttackWait;
  int16_t enemyAttackWaitTics;
  int16_t postAttackInvincibleTics;
  int16_t postAttackCount;
  int16_t speedX;
  int16_t speedY;
  fighter_attack_config_t* attackConfig;
  void (*killEnemyCallback)(object_t* me, object_t* victim);
  void (*hitEnemyCallback)(object_t* me, object_t* victim);
  void (*dieCallback)(object_t* me);
} fighter_data_t;


void
fighter_init(void);

object_t*
fighter_add(uint16_t id, uint16_t attributes, uint16_t animId, int16_t x, int16_t y, uint16_t initialHealth, fighter_attack_config_t* attackConfig, uint16_t (*intelligence)(uint16_t deltaT, object_t* ptr, struct fighter_data* data));

void
fighter_update(uint16_t deltaT, object_t* ptr);

uint16_t
fighter_nullIntelligence(uint16_t deltaT, object_t* ptr, fighter_data_t* data);

void
fighter_updateSprite(object_t* ptr);

void
fighter_die(object_t* ptr);

object_t*
fighter_attackCollision(object_t* a, int16_t thresholdx, uint16_t thresholdy);

void
fighter_dieCallback(object_t* me);
#endif
