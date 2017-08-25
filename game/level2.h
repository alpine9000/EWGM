#pragma once

#define LEVEL2_3_ENEMY_RUN_ANIM_SPEED        4
#define LEVEL2_3_ENEMY_ATTACK_TICS_PER_FRAME 12
#define LEVEL2_BOSS_RUN_ANIM_SPEED           4
#define LEVEL2_BOSS_ATTACK_TICS_PER_FRAME    4//8


extern conductor_instruction_t level2_instructions[];

uint16_t
level2_playerIntelligence(uint16_t deltaT, object_t* ptr, fighter_data_t* data);
