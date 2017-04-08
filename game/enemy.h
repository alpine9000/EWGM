#ifndef __ENEMY_H
#define __ENEMY_H

#define ENEMY_ATTACK_WAIT_FRAMES 25

void
enemy_init(object_t* player1, object_t* player2);

extern uint16_t enemy_count;

#endif
