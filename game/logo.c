#include "game.h" 

#define LOGO_BIT_DEPTH 4

extern void palette_menuInstall(void);
extern frame_buffer_t logo_frameBuffer;

typedef struct {
  uint16_t bpl1[SCREEN_BIT_DEPTH*2*2];
  uint16_t end[2];
} logo_copper_t;

static uint16_t logoPalette[1<<LOGO_BIT_DEPTH] = {
  #include "out/palette_logo.h"
};

static uint32_t logo_startFrame;

static  __section(data_c) logo_copper_t logo_copper  = {
  .bpl1 = {
    BPL1PTL,0x0000,
    BPL1PTH,0x0000,
    BPL2PTL,0x0000,
    BPL2PTH,0x0000,
    BPL3PTL,0x0000,
    BPL3PTH,0x0000,
    BPL4PTL,0x0000,
    BPL4PTH,0x0000,
  },
  .end = {0xFFFF, 0xFFFE}
};


static void 
logo_pokeCopperList(frame_buffer_t frameBuffer)
{
  uint16_t volatile* copperPtr = (uint16_t*)&logo_copper;
  /* poke bitplane pointers into copper list */
  uint32_t bitplanesPtr = (uint32_t)frameBuffer;

  for (int16_t i = 0; i < LOGO_BIT_DEPTH; i++) {
    copperPtr[1] = (uint16_t)bitplanesPtr;
    copperPtr[3] = (uint16_t)(((uint32_t)bitplanesPtr)>>16);
    bitplanesPtr = bitplanesPtr + (MENU_SCREEN_WIDTH_BYTES);
    copperPtr = copperPtr + 4;
  }
}


void
logo_display(void)
{
  volatile uint16_t scratch;

  logo_startFrame = hw_verticalBlankCount;

  disk_loadData((void*)game_onScreenBuffer, (void*)logo_frameBuffer, SCREEN_WIDTH_BYTES*SCREEN_HEIGHT*LOGO_BIT_DEPTH);

  hw_waitVerticalBlank();
  custom->dmacon = DMAF_RASTER|DMAF_SPRITE;

  palette_black();

  uint16_t volatile* copperPtr = (uint16_t*)&logo_copper;

  custom->dmacon = (DMAF_BLITTER|DMAF_SETCLR|DMAF_MASTER);

  /* set up playfield */
  
  custom->diwstrt = (RASTER_Y_START<<8)|RASTER_X_START;
  custom->diwstop = ((SCREEN_RASTER_Y_STOP-256)<<8)|(RASTER_X_STOP-256);
  custom->ddfstrt = (RASTER_X_START/2-SCREEN_RES);
  custom->ddfstop = (RASTER_X_START/2-SCREEN_RES)+(8*((MENU_SCREEN_WIDTH/16)-1));
  custom->bplcon0 = (LOGO_BIT_DEPTH<<12)|0x200;
  custom->bplcon1 = 0;
  custom->bpl1mod = (MENU_SCREEN_WIDTH_BYTES*LOGO_BIT_DEPTH)-MENU_SCREEN_WIDTH_BYTES;
  custom->bpl2mod = (MENU_SCREEN_WIDTH_BYTES*LOGO_BIT_DEPTH)-MENU_SCREEN_WIDTH_BYTES;

  /* install copper list, then enable dma and selected interrupts */
  custom->cop1lc = (uint32_t)copperPtr;
  scratch = custom->copjmp1;

  USE(scratch);

  logo_pokeCopperList(game_onScreenBuffer);

  hw_waitVerticalBlank();

  custom->dmacon = (DMAF_BLITTER|DMAF_SETCLR|DMAF_COPPER|DMAF_RASTER|DMAF_MASTER);

  palette_fadeTo(logoPalette, 1<<LOGO_BIT_DEPTH, 0);
  
#if 0
  hw_waitVerticalBlank();
  custom->dmacon = DMAF_RASTER;
  palette_black();

  menu_mode = MENU_MODE_INACTIVE;

  return command;
#endif
}

void
logo_clear(void)
{
  while (hw_verticalBlankCount < logo_startFrame+200) {
    hw_waitVerticalBlank();
  }
  
  palette_fadeFrom(logoPalette, 16, 0);
}
