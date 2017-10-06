#pragma once

#define LEVEL3_BOSS_RUN_ANIM_SPEED           4
#define LEVEL3_BOSS_ATTACK_TICS_PER_FRAME    4//8

extern conductor_instruction_t level3_instructions[];

uint16_t
level3_playerIntelligence(uint16_t deltaT, object_t* ptr, fighter_data_t* data);
