#ifndef __TILE_H
#define __TILE_H

#define BACKGROUND_TILE(x,y)   (*(&level.tileAddresses[0][0] + ((((y)>>4)<<4) + ((x)>>4))))
//#define BACKGROUND_TILE(x,y) (*(&backgroundTiles[0][0] + ((((y/TILE_HEIGHT)*TILE_WIDTH)) + (x/TILE_WIDTH))))

void
tile_init(void);
#ifdef GAME_TRIPLE_BUFFER
void
tile_renderScreen(frame_buffer_t onScreen, frame_buffer_t offScreen, frame_buffer_t backScreen);
#else
void 
tile_renderScreen(frame_buffer_t onScreen, frame_buffer_t offScreen);
#endif

#endif
