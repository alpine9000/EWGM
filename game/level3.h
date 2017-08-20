#pragma once

extern conductor_instruction_t level3_instructions[];

uint16_t
level3_playerIntelligence(uint16_t deltaT, object_t* ptr, fighter_data_t* data);
