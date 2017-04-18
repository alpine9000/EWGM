#ifndef LEVEL_H
#define LEVEL_H

#define LEVEL_NUM_LEVELS 1

typedef struct {
  uint16_t tileAddresses[MAP_TILE_WIDTH][MAP_TILE_HEIGHT];
  uint8_t spriteBitplanes[SPRITE_SHEET_WIDTH_BYTES*SCREEN_BIT_DEPTH*SPRITE_SHEET_HEIGHT];

#ifndef GAME_ONE_BITPLANE_SPRITE_MASK
  uint8_t spriteMask[SPRITE_SHEET_WIDTH_BYTES*SCREEN_BIT_DEPTH*SPRITE_SHEET_HEIGHT];
#else
  uint8_t spriteMask[SPRITE_SHEET_WIDTH_BYTES*SPRITE_SHEET_HEIGHT];
#endif

  uint8_t tileBitplanes[TILE_SHEET_WIDTH_BYTES*SCREEN_BIT_DEPTH*TILE_SHEET_HEIGHT];
  uint16_t palette[32];
  uint16_t clouds;
  uint16_t moduleIndex;
  conductor_instruction_t* instructions;
#ifdef GAME_RECORDING
  uint8_t recordData[sizeof(record_t)];
  record_t* record;
#endif
  void (*initFunctor)(void);
  void (*effectFunctor)(frame_buffer_t fb);
} level_t DISK_SECTOR_ALIGN;

extern level_t level;

void
level_load(uint16_t index);

#endif
