#ifndef __ENEMY_H
#define __ENEMY_H

#define ENEMY_WIDTH                 22
#define ENEMY_ATTACK_WAIT_TICS      25
#define ENEMY_INTERCEPT_Y_THRESHOLD 1
#define ENEMY_INTERCEPT_X_RANGE     1
#define ENEMY_WALKABOUT_TICS        50 // if an enemy collides but cannot attack, go for a walk for this many units
#define ENEMY_ATTACK_DURATION_TICS  20
#define ENEMY_INITIAL_HEALTH        100

#define ENEMY_BOSS_RUN_ANIM_SPEED 4
#define ENEMY_BOSS_NUM_ATTACK_FRAMES 3
#define ENEMY_BOSS_ATTACK_TICS_PER_FRAME 8

// the following are temporary and will be refactored
#define ENEMY_ATTACK_DAMMAGE        5

extern uint16_t enemy_count;

#ifdef DEBUG
uint16_t enemy_pause;
#endif

void
enemy_init(void);

void
enemy_add(uint16_t x, uint16_t y, uint16_t attackWait, uint16_t attackDuration, uint16_t (*intelligence)(uint16_t deltaT, object_t* ptr, fighter_data_t* data));

uint16_t
enemy_intelligence(uint16_t deltaT, object_t* ptr, fighter_data_t* data);
#endif
