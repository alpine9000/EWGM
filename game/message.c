#include "game.h"

#if TRACKLOADER==1
uint16_t message_loadingAnimation = 0;
static char* message_message;
static frame_buffer_t message_fb;
static int16_t message_loadingState;
#endif
uint16_t message_on = 0;
uint16_t message_textColor = 0xfff;

typedef struct {
  uint16_t bpl1[SCREEN_BIT_DEPTH*2*2];
  uint16_t color1[2];
  uint16_t wait[2];
  uint16_t color2[2];
  uint16_t end[2];
} message_copper_t;

static  __SECTION_DATA_C  message_copper_t message_copper  = {
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

  .color1 = { COLOR01, 0x000},
  .wait = { ((RASTER_Y_START+PLAYAREA_HEIGHT-1)<<8)|0xe1, 0xfffe},
  .color2 = {COLOR01, 0x000},

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


__NOINLINE void
message_fadeIn(void)
{
  for (uint16_t c = 0x0; c != message_textColor; c+=0x111) {
    message_copper.color1[1] = c;
    hw_waitVerticalBlank();
  }
  message_copper.color1[1] = message_textColor;
}

__NOINLINE void
message_screenOn(char* message)
{
#if TRACKLOADER==1
  message_message =  message;
#endif

  if (game_messageBuffer != game_offScreenBuffer) {
    game_switchFrameBuffers();
  }

#if TRACKLOADER==1
  message_loadingAnimation = 0;
  hw_interruptsGameInit();
#endif
  custom->dmacon = DMAF_SPRITE;

  if (message_on) {
    hw_waitVerticalBlank();
    for (uint16_t c = message_textColor; c != 0; c-=0x111) {
      message_copper.color1[1] = c;
      hw_waitVerticalBlank();
    }
    message_copper.color1[1] = 0x000;
    palette_black();
    gfx_fillRectSmallScreen(game_messageBuffer, 0, 0, SCREEN_WIDTH, PLAYAREA_HEIGHT, 0);
    text_drawMaskedText8Blitter(game_messageBuffer, message, (SCREEN_WIDTH/2)-(strlen(message)*4), (SCREEN_HEIGHT/2)+4);
    hw_waitBlitter();
    custom->bltafwm = 0xffff;

    for (uint16_t c = 0x0; c != message_textColor; c+=0x111) {
      message_copper.color1[1] = c;
      hw_waitVerticalBlank();
    }
    message_copper.color1[1] = message_textColor;
    custom->dmacon = (DMAF_SETCLR|DMAF_COPPER|DMAF_RASTER);
    return;
  }

  hw_waitBlitter();
  hw_waitVerticalBlank();
  message_copper.color1[1] = 0x000;
  palette_black();
  hw_waitVerticalBlank();

  //  volatile uint16_t scratch;

  hw_waitBlitter();
  custom->dmacon = (DMAF_COPPER|DMAF_RASTER);

  gfx_fillRectSmallScreen(game_messageBuffer, 0, 0, SCREEN_WIDTH, PLAYAREA_HEIGHT, 0);

#ifdef DEBUG
  game_checkCanary();
#endif

  text_drawMaskedText8Blitter(game_messageBuffer, message, (SCREEN_WIDTH/2)-(strlen(message)*4), (SCREEN_HEIGHT/2)+4);

  hw_waitBlitter();
  custom->bltafwm = 0xffff;

  /* set up playfield */

  custom->diwstrt = (RASTER_Y_START<<8)|RASTER_X_START;
  custom->diwstop = ((SCREEN_RASTER_Y_STOP-256)<<8)|(RASTER_X_STOP-256);

  custom->ddfstrt = (RASTER_X_START/2-SCREEN_RES);
  custom->ddfstop = (RASTER_X_START/2-SCREEN_RES)+(8*((SCREEN_WIDTH/16)-1));
  custom->bplcon0 = (SCREEN_BIT_DEPTH<<12)|0x200;
  custom->bpl1mod = (SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH)-SCREEN_WIDTH_BYTES;
  custom->bpl2mod = (SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH)-SCREEN_WIDTH_BYTES;

  message_pokeCopperList(game_messageBuffer);
  // in case previous copper list changed color registers
  palette_black();
  /* install copper list, then enable dma and selected interrupts */
  uint16_t volatile* copperPtr = (uint16_t*)&message_copper;
  custom->cop1lc = (uint32_t)copperPtr;
  //  scratch = custom->copjmp1;

  custom->dmacon = (DMAF_SETCLR|DMAF_COPPER|DMAF_RASTER);

  hw_waitVerticalBlank();

  //custom->color[1] = 0xfff;
  message_fadeIn();

  message_on = 1;
}

__NOINLINE void
message_screenOff(void)
{
#if TRACKLOADER==1
  message_loadingAnimation = 0;
  hw_interruptsGameInit();
#endif
  hw_waitBlitter();
  hw_waitVerticalBlank();

  if (message_on) {
    message_copper.color1[1] = 0x000;
    palette_black();
    hw_waitVerticalBlank();
    gfx_fillRectSmallScreen(game_messageBuffer, 0, 0, SCREEN_WIDTH, PLAYAREA_HEIGHT, 0);
    hw_waitBlitter();
    hw_waitVerticalBlank();
    screen_setup((uint16_t*)&copper);
    hw_waitVerticalBlank();
    message_on = 0;
  }
}

__NOINLINE void
#if TRACKLOADER==1
message_loading(char* message)
#else
message_loading(__UNUSED char* message)
#endif
{

#if TRACKLOADER==1
  message_screenOn(message);
#else
  message_screenOn(" ");
#endif

  hw_waitBlitter();
  hw_waitVerticalBlank();
#if TRACKLOADER==1
  message_fb = game_messageBuffer;
  message_loadingAnimation = 1;
  message_loadingState = 0;
  hw_interruptsInit();
#endif
}

__NOINLINE uint16_t
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


#if TRACKLOADER==1
__EXTERNAL void
message_loadingAnimate(void)
{

  if (!message_loadingAnimation) {
    return;
  }

  int16_t x;

  switch (message_loadingState) {
  case 80:
    x = (SCREEN_WIDTH/2)+(strlen(message_message)*4);
    gfx_fillRectSmallScreen(message_fb, x, (SCREEN_HEIGHT/2)+4, 3*9, 9, 0);
    message_loadingState = 0;
    break;
  case 60:
    x = (SCREEN_WIDTH/2)+(strlen(message_message)*4);
    text_drawMaskedText8Blitter(message_fb, "...", x, (SCREEN_HEIGHT/2)+4);
    break;
  case 40:
    x = (SCREEN_WIDTH/2)+(strlen(message_message)*4);
    text_drawMaskedText8Blitter(message_fb, "..", x, (SCREEN_HEIGHT/2)+4);
    break;
  case 20:
    x = (SCREEN_WIDTH/2)+(strlen(message_message)*4);
    text_drawMaskedText8Blitter(message_fb, ".", x, (SCREEN_HEIGHT/2)+4);
    break;
  }

  message_loadingState++;
}
#endif
