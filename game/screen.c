#include "game.h"

void 
screen_pokeCopperList(frame_buffer_t frameBuffer, uint16_t volatile* copperPtr, uint16_t screenWidthBytes)
{
  /* poke bitplane pointers into copper list */
  uint32_t bitplanesPtr = (uint32_t)frameBuffer;

  for (int16_t i = 0; i < SCREEN_BIT_DEPTH; i++) {
    copperPtr[1] = (uint16_t)bitplanesPtr;
    copperPtr[3] = (uint16_t)(((uint32_t)bitplanesPtr)>>16);
    bitplanesPtr = bitplanesPtr + screenWidthBytes;
    copperPtr = copperPtr + 4;
  }
}


void 
screen_setup(uint16_t volatile* copperPtr)
{
  volatile uint16_t scratch;

  custom->dmacon = (DMAF_BLITTER|DMAF_COPPER|DMAF_RASTER|DMAF_MASTER);  
  /* set up playfield */
  
  custom->diwstrt = (RASTER_Y_START<<8)|RASTER_X_START;
  custom->diwstop = ((GAME_RASTER_Y_STOP-256)<<8)|(RASTER_X_STOP-256);
  custom->ddfstop = (RASTER_X_START/2-SCREEN_RES)+(8*((SCREEN_WIDTH/16)-1));
  custom->bplcon0 = (SCREEN_BIT_DEPTH<<12)|0x200;

  /* install copper list, then enable dma and selected interrupts */
  custom->cop1lc = (uint32_t)copperPtr;
  scratch = custom->copjmp1;
  USE(scratch);

  custom->dmacon = (DMAF_BLITTER|DMAF_SETCLR|DMAF_COPPER|DMAF_RASTER|DMAF_MASTER);
}

