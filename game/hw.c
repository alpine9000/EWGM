#include "game.h"

#ifdef GAME_BLITTER_WAIT_DEBUG

void _hw_debugWaitBlitter(void)
{
  custom->color[0] = 0x0f0;
  while (((volatile struct Custom*)CUSTOM)->dmaconr & 1<<14);
  custom->color[0] = palette_background;
}

#endif
