#ifndef __PLAYER_H
#define __PLAYER_H

#define PLAYER_PLAYER1_WIDTH        22
#define PLAYER_PLAYER2_WIDTH        26
#define PLAYER_ATTACK_DURATION_TICS 10
#define PLAYER_SPEED_X              2
#define PLAYER_SPEED_Y              1
#define PLAYER_INITIAL_HEALTH       100

// the following are temporary and will be refactored
#define PLAYER_ATTACK_DAMMAGE 20


object_t*
player_init(uint16_t id, uint16_t animId, int16_t x);

#endif
