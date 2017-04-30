#pragma once

#define PALETTE_FADE_IN_SIZE (65*32)

extern uint16_t palette_background;

void 
palette_black(void);
void
palette_fadeTo(uint16_t* palette, uint16_t colors, uint16_t colorFrom);
void
palette_fade(uint16_t* from, uint16_t* tp, uint16_t colors, uint16_t steps);
#if 0
void
palette_fadeFrom(uint16_t* palette, uint16_t colors, uint16_t colorTo);
#else
void
palette_fadeFrom(uint16_t* palette, uint16_t colors, uint16_t color, uint16_t steps);
#endif

