#ifndef __PLAYER_H
#define __PLAYER_H

#define PLAYER_PLAYER1_WIDTH        22
#define PLAYER_PLAYER2_WIDTH        26
#define PLAYER_ATTACK_DURATION_TICS 10
#define PLAYER_SPEED_X              2
#define PLAYER_SPEED_Y              1
#define PLAYER_INITIAL_HEALTH       100

#define PLAYER_ATTACK_DAMMAGE_EASY  100
#define PLAYER_ATTACK_DAMMAGE_HARD  20

object_t*
player_init(uint16_t id, uint16_t animId, int16_t x, int16_t health);

uint16_t
player_intelligence(uint16_t deltaT, object_t* ptr, fighter_data_t* data);

uint16_t
player_select(void);
#endif
