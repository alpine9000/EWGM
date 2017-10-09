#pragma once

#define LEVEL4_3_ENEMY_RUN_ANIM_SPEED        4
#define LEVEL4_3_ENEMY_ATTACK_TICS_PER_FRAME 12
#define LEVEL4_1_BOSS_RUN_ANIM_SPEED           4
#define LEVEL4_2_BOSS_RUN_ANIM_SPEED           4
#define LEVEL4_BOSS_ATTACK_TICS_PER_FRAME    12


extern conductor_instruction_t level4_instructions[];

uint16_t
level4_playerIntelligence(uint16_t deltaT, object_t* ptr, fighter_data_t* data);
