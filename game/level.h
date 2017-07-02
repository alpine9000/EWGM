#ifndef LEVEL_H
#define LEVEL_H

#define LEVEL_NUM_LEVELS 3

typedef struct {
  uint16_t tileAddresses[MAP_TILE_WIDTH][16];
  uint16_t palette[32];
  uint16_t greyPalette[32];
  uint16_t moduleIndex;
  conductor_instruction_t* instructions;
#ifdef GAME_RECORDING
  uint8_t recordData[sizeof(record_t)];
  record_t* record;
#endif
  __attribute__ ((aligned (4))) uint32_t end; // needed for packer
} level_fast_t;

typedef struct {
  uint8_t spriteBitplanes[SPRITE_SHEET_WIDTH_BYTES*SCREEN_BIT_DEPTH*SPRITE_SHEET_HEIGHT];

#ifndef GAME_ONE_BITPLANE_SPRITE_MASK
  uint8_t spriteMask[SPRITE_SHEET_WIDTH_BYTES*SCREEN_BIT_DEPTH*SPRITE_SHEET_HEIGHT];
#else
  uint8_t spriteMask[SPRITE_SHEET_WIDTH_BYTES*SPRITE_SHEET_HEIGHT];
#endif

  uint8_t tileBitplanes[TILE_SHEET_WIDTH_BYTES*SCREEN_BIT_DEPTH*TILE_SHEET_HEIGHT];
  __attribute__ ((aligned (4))) uint32_t end; // needed for packer
} level_chip_t;

extern level_chip_t levelChip;
extern level_fast_t levelFast;

typedef struct {
  uint16_t animId;
  int16_t x;
  int16_t y;
  int16_t enemyCount;
  enemy_config_t* config;
} level_enemy_config_t;
  

void
level_load(uint16_t index);

#endif
