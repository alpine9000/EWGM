#ifndef __ENEMY_H
#define __ENEMY_H

#define ENEMY_WIDTH                 22
#define ENEMY_ATTACK_WAIT_TICS      25
#define ENEMY_INTERCEPT_Y_THRESHOLD 1
#define ENEMY_INTERCEPT_X_RANGE     1
#define ENEMY_WALKABOUT_TICS        50 // if an enemy collides but cannot attack, go for a walk for this many units
#define ENEMY_ATTACK_DURATION_TICS  20
#define ENEMY_INITIAL_HEALTH        100

// the following are temporary and will be refactored
#define ENEMY_ATTACK_DAMMAGE        5

void
enemy_init(object_t* player1, object_t* player2);

extern uint16_t enemy_count;

#endif
