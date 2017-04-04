#ifndef __PALETTE_H
#define __PALETTE_H

#define PALETTE_FADE_IN_SIZE (65*32)

extern uint16_t palette_background;

void 
palette_black(void);
void
palette_fadeIn(uint16_t* fadeInFadeTable, uint16_t colors);
uint16_t
palette_getColor(uint16_t* fadeInFadeTable, uint16_t index);
#endif
