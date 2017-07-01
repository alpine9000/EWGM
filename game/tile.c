#include "game.h"

#define MAX_INVALID_TILES 24

void
tile_init(void)
{

}

void
#ifdef GAME_TRIPLE_BUFFER
tile_renderScreen(frame_buffer_t onScreen, frame_buffer_t offScreen, frame_buffer_t backScreen)
#else
tile_renderScreen(frame_buffer_t onScreen, frame_buffer_t offScreen)
#endif
{
  for (int16_t x = 0; x < FRAME_BUFFER_WIDTH-TILE_HEIGHT; x += TILE_WIDTH) {
    for (int16_t y = 0; y < PLAYAREA_HEIGHT; y += TILE_HEIGHT) {
      uint16_t offset = levelFast.tileAddresses[x>>4][y>>4];
      gfx_renderTile(offScreen, x, y, levelChip.tileBitplanes+offset);
      gfx_renderTile(onScreen, x, y, levelChip.tileBitplanes+offset);
#ifdef GAME_TRIPLE_BUFFER
      gfx_renderTile(backScreen, x, y, levelChip.tileBitplanes+offset);
#endif
    }
  }
}
