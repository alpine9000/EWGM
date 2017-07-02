#ifndef __ENEMY_H
#define __ENEMY_H

#define ENEMY_WIDTH                 22
#define ENEMY_ATTACK_WAIT_TICS      25
#define ENEMY_INTERCEPT_Y_THRESHOLD 2
#define ENEMY_INTERCEPT_X_RANGE     1
#define ENEMY_WALKABOUT_TICS        50 // if an enemy collides but cannot attack, go for a walk for this many units
#define ENEMY_ATTACK_DURATION_TICS  20
#define ENEMY_INITIAL_HEALTH        100

#define ENEMY_DEFAULT_NUM_ATTACKS   2
#define ENEMY_DEFAULT_SPEED         1

#define ENEMY_ENEMY_LEVEL2_3_RUN_ANIM_SPEED 4
#define ENEMY_ENEMY_LEVEL2_3_ATTACK_TICS_PER_FRAME 12

#define ENEMY_BOSS_RUN_ANIM_SPEED 4
//#define ENEMY_BOSS_NUM_ATTACK_FRAMES 3
#define ENEMY_BOSS_ATTACK_TICS_PER_FRAME 8
#define ENEMY_BOSS_ATTACK_DAMMAGE   20
#define ENEMY_BOSS_ATTACK_RANGE     20

#define ENEMY_MOTORBIKE_RIDE_ANIM_SPEED 8
// the following are temporary and will be refactored
#define ENEMY_ATTACK_DAMMAGE        5


extern uint16_t enemy_count;

#ifdef DEBUG
extern uint16_t enemy_pause;
#endif

extern fighter_attack_config_t enemy_attackConfig1[];
extern fighter_attack_config_t enemy_attackConfig2[];
extern fighter_attack_config_t enemy_bossAttackConfig[];
extern fighter_attack_config_t enemy_motorbikeAttackConfig[];

typedef struct {
  fighter_attack_config_t* attackConfig;
  uint16_t attackWait;
  uint16_t attackRangeY;
  uint16_t postAttackInvincibleTics;
  uint16_t speed;
  uint16_t numAttacks;
  uint16_t (*intelligence)(uint16_t deltaT, object_t* ptr, fighter_data_t* data);
} enemy_config_t;

void
enemy_init(void);

object_t*
//enemy_add(uint16_t animId, uint16_t x, uint16_t y, fighter_attack_config_t* attackConfig, uint16_t attackWait, uint16_t (*intelligence)(uint16_t deltaT, object_t* ptr, fighter_data_t* data));
enemy_add(uint16_t animId, uint16_t x, uint16_t y, enemy_config_t* config);

uint16_t
enemy_intelligence(uint16_t deltaT, object_t* ptr, fighter_data_t* data);

object_t*
enemy_closestPlayer(object_t* ptr);
#endif
