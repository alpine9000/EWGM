#pragma once

#ifdef GAME_STARS
void
star_add(object_t* ptr, int16_t dx);

void
star_update(uint16_t deltaT);

void
star_init(void);

extern int16_t star_count;
extern uint16_t sprite_nullhsprite[];
#endif
