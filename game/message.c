#include "game.h"


static uint16_t message_on = 0;

typedef struct {
  uint16_t bpl1[SCREEN_BIT_DEPTH*2*2];
  uint16_t end[2];
} message_copper_t;

static  __section(data_c)  message_copper_t message_copper  = {
  .bpl1 = {
    BPL1PTL,0x0000,
    BPL1PTH,0x0000,
    BPL2PTL,0x0000,
    BPL2PTH,0x0000,
    BPL3PTL,0x0000,
    BPL3PTH,0x0000,
    BPL4PTL,0x0000,
    BPL4PTH,0x0000,
    BPL5PTL,0x0000,
    BPL5PTH,0x0000,
  },
  .end = {0xFFFF, 0xFFFE}
};

static void 
message_pokeCopperList(frame_buffer_t frameBuffer)
{
  uint16_t volatile* copperPtr = (uint16_t*)&message_copper;
  /* poke bitplane pointers into copper list */
  uint32_t bitplanesPtr = (uint32_t)frameBuffer;

  for (int16_t i = 0; i < SCREEN_BIT_DEPTH; i++) {
    copperPtr[1] = (uint16_t)bitplanesPtr;
    copperPtr[3] = (uint16_t)(((uint32_t)bitplanesPtr)>>16);
    bitplanesPtr = bitplanesPtr + (SCREEN_WIDTH_BYTES);
    copperPtr = copperPtr + 4;
  }
}


void
message_screenOn(char* message)
{
  if (message_on) {
    gfx_fillRectSmallScreen(game_offScreenBuffer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    text_drawMaskedText8Blitter(game_offScreenBuffer, message, (SCREEN_WIDTH/2)-(strlen(message)*4), (SCREEN_HEIGHT/2)+4);
    hw_waitBlitter();
    custom->bltafwm = 0xffff;
    return;
  }

  hw_waitBlitter();
  hw_waitVerticalBlank();
  palette_black();
  hw_waitVerticalBlank();  

  volatile uint16_t scratch;

  hw_waitBlitter();  
  custom->dmacon = (DMAF_COPPER|DMAF_RASTER|DMAF_MASTER);

  gfx_fillRectSmallScreen(game_offScreenBuffer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  text_drawMaskedText8Blitter(game_offScreenBuffer, message, (SCREEN_WIDTH/2)-(strlen(message)*4), (SCREEN_HEIGHT/2)+4);

  hw_waitBlitter();
  custom->bltafwm = 0xffff;
  custom->dmacon = (DMAF_BLITTER|DMAF_COPPER|DMAF_RASTER|DMAF_MASTER);  

  /* set up playfield */
  
  custom->diwstrt = (RASTER_Y_START<<8)|RASTER_X_START;
  custom->diwstop = ((SCREEN_RASTER_Y_STOP-256)<<8)|(RASTER_X_STOP-256);

  custom->ddfstrt = (RASTER_X_START/2-SCREEN_RES);
  custom->ddfstop = (RASTER_X_START/2-SCREEN_RES)+(8*((SCREEN_WIDTH/16)-1));
  custom->bplcon0 = (SCREEN_BIT_DEPTH<<12)|0x200;
  custom->bpl1mod = (SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH)-SCREEN_WIDTH_BYTES;
  custom->bpl2mod = (SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH)-SCREEN_WIDTH_BYTES;

  message_pokeCopperList(game_offScreenBuffer);  
  /* install copper list, then enable dma and selected interrupts */
  uint16_t volatile* copperPtr = (uint16_t*)&message_copper;  
  custom->cop1lc = (uint32_t)copperPtr;
  //  scratch = custom->copjmp1;
  USE(scratch);

  custom->dmacon = (DMAF_BLITTER|DMAF_SETCLR|DMAF_COPPER|DMAF_RASTER|DMAF_MASTER);

  hw_waitVerticalBlank();

  custom->color[1] = 0xfff;

  message_on = 1;
}

void
message_screenOff(void)
{
  hw_waitBlitter();
  hw_waitVerticalBlank();
  
  if (message_on) {
    palette_black();
    hw_waitVerticalBlank();    
    gfx_fillRectSmallScreen(game_onScreenBuffer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    hw_waitBlitter();    
    hw_waitVerticalBlank();
    screen_setup((uint16_t*)&copper);
    hw_waitVerticalBlank();    
    message_on = 0;
  }
}

void
message_loading(char* message)
{
#if TRACKLOADER==1
  message_screenOn(message);
#else
  USE(message);
  palette_black();
#endif

  hw_waitBlitter();
  hw_waitVerticalBlank();  
}

uint16_t
message_ask(char* message)
{
  uint16_t result = 0;
  message_screenOn(message);
  for (;;) {
    hw_waitVerticalBlank();
    keyboard_read();
    hw_readJoystick();

    if (keyboard_key == 'Y') {
      result = 1;
      break;
    }

    if (keyboard_key == 'N' || game_fire()) {
      break;
    }
  }

  message_screenOff();
  return result;
}

