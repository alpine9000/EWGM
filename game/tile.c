#include "game.h"

#define MAX_INVALID_TILES 24

void
tile_init(void)
{

}

void 
tile_renderScreen(frame_buffer_t onScreen, frame_buffer_t offScreen)
{
  for (int16_t x = 0; x < FRAME_BUFFER_WIDTH-TILE_HEIGHT; x += TILE_WIDTH) {
    for (int16_t y = 0; y < PLAYAREA_HEIGHT; y += TILE_HEIGHT) {
      uint16_t offset = level.tileAddresses[x>>4][y>>4];
      gfx_renderTile(offScreen, x, y, level.tileBitplanes+offset);
      gfx_renderTile(onScreen, x, y, level.tileBitplanes+offset);
    }
  }
}
