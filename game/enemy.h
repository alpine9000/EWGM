#ifndef __ENEMY_H
#define __ENEMY_H

#define ENEMY_WIDTH                         22
#define ENEMY_INTERCEPT_X_RANGE             1
#define ENEMY_INTERCEPT_Y_THRESHOLD         2
#define ENEMY_INITIAL_HEALTH                100

#define ENEMY_DEFAULT_ATTACK_WAIT_TICS      45
#define ENEMY_DEFAULT_ATTACK_DURATION_TICS  20
#define ENEMY_DEFAULT_NUM_ATTACKS           2
#define ENEMY_DEFAULT_SPEED                 1
#define ENEMY_DEFAULT_ATTACK_DAMMAGE        5

extern uint16_t enemy_count;

#ifdef DEBUG
extern uint16_t enemy_pause;
#endif

typedef struct {
  fighter_attack_config_t* attackConfig;
  uint16_t minAttackWait;
  uint16_t maxAttackWait;
  uint16_t attackRangeY;
  uint16_t postAttackInvincibleTics;
  uint16_t randomFrequencyMask;
  uint16_t randomDistanceMask;
  uint16_t speedX;
  uint16_t speedY;
  uint16_t numAttacks;
  uint16_t initialHealth;
  uint16_t (*intelligence)(uint16_t deltaT, object_t* ptr, fighter_data_t* data);
} enemy_config_t;

void
enemy_init(void);

object_t*
enemy_add(uint16_t animId, uint16_t attributes, uint16_t x, uint16_t y, enemy_config_t* config);

uint16_t
enemy_intelligence(uint16_t deltaT, object_t* ptr, fighter_data_t* data);

object_t*
enemy_closestPlayer(object_t* ptr);
#endif
